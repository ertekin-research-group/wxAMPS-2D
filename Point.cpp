#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "Point.h"
#include "constant.h"
#include "Calculation.h"
#include <cmath>
#include "LU.h"
#include "float.h"
const double EgApp=0.5;
double xStDv[MGAUSS+1]={-2.4,-2.0,-1.7,-1.5,-1.3,-1.1,-1.0,-0.9,-0.7,-0.5,-0.3,-0.1,
0.1,0.3,0.5,0.7,0.9,1.0,1.1,1.3,1.5,1.7,2.0,2.4,};
double xProb[MGAUSS]={0.014552596023582942, 0.02181533081036402, 0.022241738510314968, 0.029993283316751995,
0.038865576360772036, 0.022989192985074958, 0.025404871415302011, 0.05790352687631406,
0.066573886502913959, 0.073551039085059999,0.07808358491192402,0.079655674554057976,
0.07808358491192402,0.073551039085059999,0.066573886502913959,0.05790352687631406,
0.025404871415302011,0.022989192985074958,0.038865576360772036,0.029993283316751995,
0.022241738510314968,0.02181533081036402,0.014552596023582942};//gaussian probability
Point::Point()
{
}
Point::Point(int num,ifstream& file,double kT)
{
	InitValue(num);
	int index;//double ni2;
	for(int i=0;i<num;i++)
	{
		file>>index>>x[i]>>step[i]>>eps[i]>>Eg[i]>>affi[i]
		>>Nc[i]>>Nv[i]>>un[i]>>up[i]>>Nd[i]>>Na[i];
		//ni2=(Nc[i]*Nv[i]*exp(-Eg[i]/kT));
		//ni[i]=sqrt(ni2);
		//if(n[i]>p[i])//self correction for n, p
		//{
		//	n[i]=n[i]-p[i];
		//	p[i]=ni2/n[i];
		//}
		//else if(n[i]<p[i])
		//{
		//	p[i]=p[i]-n[i];
		//	n[i]=ni2/p[i];
		//}
		//else
		//{
		//	p[i]=Nc[i]*Nv[i]*exp(-Eg[i]/kT);
		//	n[i]=p[i];
		//}
		//Generation rate initialized
		G[i]=0;	
	}
	
}
Point::Point(MyList &LayerList,double kT)
{
	PointNumber=2;
	layerListP=&LayerList;//store the layers
	double hEdge=0.5, hCenter=20;
	int lyrCnt=LayerList.GetCount();
	int num=0;double L;
	int *grids=new int[lyrCnt];double *q=new double[lyrCnt];
	for(int i=0;i<lyrCnt;i++)
	{
		MyList::compatibility_iterator node;
		node=LayerList.Item(i);
		layer *curLyr=node->GetData();
		curLyr->thickness.ToDouble(&L);//unit um
		curLyr->hEdge.ToDouble(&hEdge);curLyr->hCenter.ToDouble(&hCenter);
		if(0==L)
			return;
		if(hCenter>hEdge)
		{
			q[i]=((L-2*hEdge*1e-3)/(L-2*hCenter*1e-3));
			grids[i]=(2*(int)(log(hCenter/hEdge)/log(q[i])+1));		
		}
		else
		{
			grids[i]=2*int(L*1e3/hCenter/2);
			q[i]=1;
		}
		num+=grids[i]+1;//total number		
	}	
	PointNumber=num;
	InitValue(num);

	int index=0;double start=0;
	for(int rgn=0;rgn<lyrCnt;rgn++)
	{
		MyList::compatibility_iterator node;//show the current layer parameters
		node=LayerList.Item(rgn);
		layer *curLyr=node->GetData();
		curLyr->thickness.ToDouble(&L);//unit um
		if(grids[rgn]>0)
		{
			double *h=new double[grids[rgn]];			
			double ratio;
			x[index]=start*1e-4;//cm
			double LSum=0;
			for(int i=0;i<grids[rgn];i++)
			{
				if(i<grids[rgn]/2)
					h[i]=hEdge*1e-3*pow(q[rgn],i);
				else
					h[i]=h[grids[rgn]-1-i];
				LSum+=h[i];			
			}
			ratio=L/LSum;
			LSum=0;
			for(int i=0;i<grids[rgn];i++)
			{
				h[i]=h[i]*ratio;
				step[index]=h[i]*1e-4;//cm
				LSum+=h[i];
				curLyr->Epson.ToDouble(&eps[index]);curLyr->Eg.ToDouble(&Eg[index]);curLyr->affi.ToDouble(&affi[index]);
				curLyr->Nc.ToDouble(&Nc[index]);curLyr->Nv.ToDouble(&Nv[index]);curLyr->un.ToDouble(&un[index]);
				curLyr->up.ToDouble(&up[index]);curLyr->n.ToDouble(&Nd[index]);curLyr->p.ToDouble(&Na[index]);
				x[++index]=(start+LSum)*1e-4;//cm			
			}
			step[index]=0;
			curLyr->Epson.ToDouble(&eps[index]);curLyr->Eg.ToDouble(&Eg[index]);curLyr->affi.ToDouble(&affi[index]);
			curLyr->Nc.ToDouble(&Nc[index]);curLyr->Nv.ToDouble(&Nv[index]);curLyr->un.ToDouble(&un[index]);
			curLyr->up.ToDouble(&up[index]);curLyr->n.ToDouble(&Nd[index]);curLyr->p.ToDouble(&Na[index]);
			index++;
			start+=L;

			delete[] h;//,step;
		}
		
	}
	delete[] grids;delete[] q;
	for(int i=0;i<num;i++)
	{
		double ni2=(Nc[i]*Nv[i]*exp(-Eg[i]/kT));
		ni[i]=sqrt(ni2);
	
		G[i]=0;R[i]=0;
		Ne[i]=0;Ph[i]=0;Ntd[i]=0;Nta[i]=0;nt[i]=0;pt[i]=0;
		dNd[i]=0;dNa[i]=0;dnt[i]=0;dpt[i]=0;
		Jn[i]=0;Jp[i]=0;
		tunC[i]=0;GammaN[i]=0;GammaP[i]=0;
		tunCp[i]=0;
	}

}

Point::~Point(void)
{
	delete [] step;delete []x;delete []psi;delete []Efn;delete []Efp;
	delete [] psiInit;delete []psiOld;delete []EfnOld;delete []EfpOld;
	delete [] Ec;delete []Ev;
	delete [] eps;delete []Eg;delete []affi;delete []Nc;delete []Nv;delete []un;delete []up;delete []Nd;delete []Na;
	delete []Ne;delete []Ph;delete []Ntd;delete []Nta;delete [] dNd;delete []dNa;
	delete [] ni;// delete []taon; delete []taop;
	delete [] Jn;delete []Jp;
	delete []G;
	delete [] R;delete [] dR;
	delete []nt;delete []pt;delete []dnt;delete[] dpt;
	delete []tunC;
	delete []EleF;delete []GammaN; delete []GammaP;
	delete []nInit;delete []pInit;
	delete []tunCp;
}
void Point::InitValue(int num)
{
	step=new double[num];x=new double[num];
	psiInit=new double[num];
	psiOld=new double[PointNumber];EfnOld=new double[PointNumber];EfpOld=new double[PointNumber];
	psi=new double[num],Efn=new double[num], Efp=new double[num];
	Ec=new double[num], Ev=new double[num];
	eps=new double[num];Eg=new double[num];affi=new double[num];
	Nc=new double[num];Nv=new double[num];un=new double[num];
	up=new double[num];Nd=new double[num];Na=new double[num];
	Ne=new double[num];Ph=new double [num];Ntd=new double[num];Nta=new double[num];
	dNd=new double[num];dNa=new double[num];
	ni=new double[num];
//	taon=new double[num], taop=new double[num];
	Jn=new double[num], Jp=new double[num];
	R=new double[num],G=new double[num];
	dR=new double[num];
	nt=new double[num],pt=new double[num];dnt=new double[num],dpt=new double[num];
	tunC=new double[num];
	EleF=new double[num];GammaN=new double[num];GammaP=new double[num];
	nInit=new double[num];pInit=new double[num];
	tunCp=new double[num];
}

//------------f----------- 
void Point::fpsiInit(double *y, int num,double kT,bool IsWF,double TopWF,double BtmWF,double *f)
{	
	//double b=affi[0]-affi[num-1]-kT*log(n[0]*Nc[num-1]/n[num-1]/Nc[0]);
	double Ef=0;//-kT*log(Nc[num-1]/n[num-1])-affi[num-1];
	DefChargeEqui(y,num,kT);//calculate nt,pt,dntdpsi,dptdpsi
	//double Ne,Ph;	//free electrons and holes, Fermi-Dirac distribution
	Ne[0]=/*GP12((affi[0]-y[0])/kT)**/Nc[0]*exp((affi[0]-y[0])/kT);
	Ph[0]=/*GP12((y[0]-affi[0]-Eg[0])/kT)**/Nv[0]*exp((y[0]-affi[0]-Eg[0])/kT);
	if(!IsWF)
		f[0]=QE*(Nd[0]-Na[0]-Ne[0]+Ph[0]+pt[0]-nt[0]);
	else
		f[0]=y[0]-affi[0]-TopWF;
	for (int i=1;i<num-1;i++)
	{
		if(step[i]!=0&&step[i-1]!=0)
		{
			Ne[i]=Nc[i]*exp((Ef+affi[i]-y[i])/kT);//*GP12((affi[i]-y[i])/kT);
			Ph[i]=Nv[i]*exp((y[i]-affi[i]-Eg[i]-Ef)/kT);//*GP12((y[i]-affi[i]-Eg[i])/kT);
			f[i]=2*eps[i]*EPSON*(y[i-1]/step[i-1]/(step[i]+step[i-1])-y[i]/step[i]/step[i-1]+y[i+1]/step[i]/(step[i]+step[i-1]))
			-QE*(Nd[i]-Na[i]+Ph[i]-Ne[i]+pt[i]-nt[i]);
		}	
		else if(0==step[i])
			f[i]=y[i]-y[i+1];
		else
		{
			f[i]=eps[i]*(y[i+1]-y[i])/step[i]-eps[i-1]*(y[i-1]-y[i-2])/step[i-2];
			f[i]*=step[i-2];
		}		
	}
	Ph[num-1]=Nv[num-1]*exp((y[num-1]-affi[num-1]-Eg[num-1])/kT);//*GP12((y[num-1]-affi[num-1]-Eg[num-1])/kT);
	Ne[num-1]=Nc[num-1]*exp((affi[num-1]-y[num-1])/kT);//*GP12((affi[num-1]-y[num-1])/kT);
	if(!IsWF)
		f[num-1]=QE*(Nd[num-1]-Na[num-1]+Ph[num-1]-Ne[num-1]+pt[num-1]-nt[num-1]);
	else
		f[num-1]=y[num-1]-affi[num-1]-BtmWF;
        
}
//--------------J----------------------
void Point::JacoInit(double *y,int num,double kT,bool IsWF,double *J)
{
//	double b=-kT*log(n[0]*Nc[num-1]/n[num-1]/Nc[0]);
	double Ef=0;//-kT*log(Nc[num-1]/n[num-1])-affi[num-1];
	double dNe,dPh;
	dPh=Nv[0]*exp((y[0]-affi[0]-Eg[0])/kT)/kT;//*GM12((y[0]-affi[0]-Eg[0])/kT);
	dNe=-Nc[0]*exp((affi[0]-y[0])/kT)/kT;//*GM12((affi[0]-y[0])/kT);
	if(!IsWF)
		J[0*num+0]=(dNd[0]-dNa[0]+dPh-dNe+dpt[0]-dnt[0])*QE; 
	else
		J[0]=1;
	
	dPh=Nv[num-1]*exp((y[num-1]-affi[num-1]-Eg[num-1])/kT)/kT;//*GM12((y[num-1]-affi[num-1]-Eg[num-1])/kT);
	dNe=-Nc[num-1]*exp((affi[num-1]-y[num-1])/kT)/kT;//*GM12((affi[num-1]-y[num-1])/kT);
	if(!IsWF)
		J[num*num-1]=(dNd[num-1]-dNa[num-1]+dPh-dNe+dpt[num-1]-dnt[num-1])*QE; 
	else
		J[num*num-1]=1;
	//J[0]=1;J[num*num-1]=1;
	for(int i=1;i<num-1;i++)
	{
		if(step[i]!=0&&step[i-1]!=0)
		{
			J[i*num+i-1]=2*eps[i]*EPSON/step[i-1]/(step[i]+step[i-1]);
			dPh=Nv[i]*exp((y[i]-affi[i]-Eg[i]-Ef)/kT)/kT;//*GM12((y[i]-affi[i]-Eg[i]-Ef)/kT);
			dNe=-Nc[i]*exp((Ef+affi[i]-y[i])/kT)/kT;//*GM12((affi[i]-y[i])/kT);
			J[i*num+i]=-2*eps[i]*EPSON/step[i]/step[i-1]-
				QE*(dPh-dNe+dNd[i]-dNa[i]+dpt[i]-dnt[i]);
			J[i*num+i+1]=2*eps[i]*EPSON/step[i]/(step[i]+step[i-1]);
		}
		else if(0==step[i])
		{
			J[i*num+i]=1;
			J[i*num+i+1]=-1;
		}
		else
		{
			J[i*num+i-2]=eps[i-1];
			J[i*num+i-1]=-eps[i-1];
			J[i*num+i]=-step[i-2]*eps[i]/step[i];
			J[i*num+i+1]=step[i-2]*eps[i]/step[i];
		}
	}       
}

//calculate Efn
bool Point:: IterN(double *y, double*J,double*l,double*u,int num,double kT,double TopSn,double BtmSn,wxProgressDialog *dlg)
{
	double *c=new double[num],  *Efn1=new double[num],*dEfn=new double[num],*p1=new double[num],*n1=new double[num];
	double *f=new double[num];
	
	for(int i=0;i<num*num;i++)
	{
		J[i]=0;
	}
	for(int i=0;i<num;i++)
	{
		p1[i]=/*Ph(Nv[i],(y[i]-affi[i]-Eg[i]-Efp[i])/kT,kT);*/Nv[i]*SafeDExp((y[i]-affi[i]/*+affi[num-1]*/-Eg[i]-Efp[i])/kT);		
		if(step[i]!=0)
		{
			c[i]=QE*un[i]*Nc[i]/step[i]*SafeDExp(-(y[i]-affi[i])/kT)*kT*xDexM1((y[i+1]-affi[i+1]-y[i]+affi[i])/kT);
		}
		else//this point is useless
		{
			c[i]=0;			
		}
	}
	//outEB(x,Efn,c,p1,num);
	//wxString str, title="Function:Calculate Efn\nIteration:";
	double ep=1e-6,tol=1;int cnt=0;bool end=0;//	double range;
	while( tol>ep)
	{
		transfer(Efn,Efn1,num);
		fn(y,Efn1,n1,p1,num,kT,TopSn,BtmSn,c,f);
		Jacn(y,Efn1,n1,p1,num,kT,TopSn,BtmSn,c,J);
		LU2(J,f,l,u,num,dEfn);
		clamp(dEfn,limit,num);
		for(int i=0;i<num;i++)
		{
			Efn[i]=Efn1[i]-dEfn[i];
		}		
		tol=norm(Efn,Efn1,num);
		if(_isnan(tol))
			end=1;
		cnt++;		
		if(cnt>(iterTimes/2))
			end=1;
		if(end)
		{
#ifdef _DEBUG
			wxMessageBox(_("too many iterations in calculating Efn , results may not be correct!"));
#endif
			delete[] c;delete[]Efn1;delete[]dEfn;delete[]p1;delete[] n1;delete[] f;//,J,l,u;
			return 0;
		}
	}
	delete[] c;delete[]Efn1;delete[]dEfn;delete[]p1;delete[] n1;delete[] f;//,J,l,u;
	return 1;
}
void Point:: fn(double *y, double *Efn1, double *n1,double *p1,
		  int num,double kT,double TopSn,double BtmSn,double *c,double *f)
{
	for(int i=0;i<num;i++)
	{		
		n1[i]=/*Ne(Nc[i],(Efn1[i]+affi[i]-y[i])/kT);*/Nc[i]*SafeDExp((Efn1[i]+affi[i]/*-affi[num-1]*/-y[i])/kT);		

		if(step[i]!=0)
			Jn[i]=c[i]*SafeDExp(Efn1[i]/kT)*(exp((Efn1[i+1]-Efn1[i])/kT)-1);
		else if(i!=num-1)
		{//2*QE^2/h=7.74813e-5
			double AT2i=7.74813e-5*kT*pow(Nc[i]/2,2./3);
			//double AT2i1=7.74813e-5*kT*pow(Nc[i+1]/2,2./3);
			if(affi[i]>=affi[i+1])//thermal emission current
				Jn[i]=AT2i*SafeDExp((Efn1[i+1]-y[i+1]+affi[i+1])/kT)-AT2i*SafeDExp((Efn1[i]-y[i]+affi[i+1])/kT);
			else
				Jn[i]=AT2i*SafeDExp((Efn1[i+1]-y[i+1]+affi[i])/kT)-AT2i*SafeDExp((Efn1[i]-y[i]+affi[i])/kT);
		}
		else
			Jn[i]=Jn[i-1];
	}
	Recombination(0,n1,p1,num,kT);//dR=dR/dEfn

	f[0]=Jn[0]-QE*TopSn*(Nc[0]*SafeDExp((Efn1[0]+affi[0]-y[0])/kT)-n0L);//?direction ?defect
	f[0]/=step[0];
	for (int i=1;i<num-1;i++)
	{
		if(step[i]!=0&&step[i-1]!=0)
			f[i]=2*(Jn[i]-Jn[i-1])/(step[i]+step[i-1])-QE*R[i]+QE*G[i];
		else if(0==step[i])
			//f[i]=Efn1[i+1]-Efn1[i];//diffusion model
			f[i]=2*(Jn[i]-Jn[i-1])/step[i-1]-QE*R[i]+QE*G[i];//thermal emission model
		else
			f[i]=2*(Jn[i]-Jn[i-1])/(step[i])-QE*R[i]+QE*G[i];
	}
	f[num-1]=Jn[num-2]+QE*BtmSn*(Nc[num-1]*SafeDExp((Efn1[num-1]-y[num-1]+affi[num-1])/kT)-n0R);
	f[num-1]/=step[num-2];

	
}
void Point:: Jacn(double *y, double *Efn1, double *n1,double *p1,
		  int num,double kT,double TopSn,double BtmSn,double *c,double *J)
{
	double AT2i;
	//double S=1e7;//double * dR=new double[num];
	//for(int i=0;i<num;i++)
	{
		//n1[i]=Ne(Nc[i],(Efn1[i]+affi[i]-y[i])/kT);//Nc[i]*exp((Efn1[i]+affi[i]/*-affi[num-1]*/-y[i])/kT);
		//dR[i]=(tp*ni[i]+tn*p1[i])*(p1[i]+ni[i])*n1[i]/pow((tp*(n1[i]+ni[i])+tn*(p1[i]+ni[i])),2);
		//dR[i]=0;
	}
	J[0*num+0]=-c[0]*SafeDExp(Efn1[0]/kT)/kT-QE*TopSn*(Nc[0]*SafeDExp((Efn1[0]+affi[0]-y[0])/kT))/kT;
	J[0*num+1]=c[0]*SafeDExp(Efn1[1]/kT)/kT;
	J[0*num+0]/=step[0];J[0*num+1]/=step[0];
	for(int i=1;i<num-1;i++)
	{
		if(step[i]!=0&&step[i-1]!=0)
		{
			J[i*num+i-1]=2/(step[i]+step[i-1])/kT*c[i-1]*SafeDExp(Efn1[i-1]/kT);
			J[i*num+i]=-2/(step[i]+step[i-1])/kT*(c[i]+c[i-1])*SafeDExp(Efn1[i]/kT)-QE*dR[i];///kT;
			J[i*num+i+1]=2/(step[i]+step[i-1])/kT*c[i]*SafeDExp(Efn1[i+1]/kT);
		}
		else if(0==step[i])
		{
			//J[i*num+i]=-1;J[i*num+i+1]=1;
			AT2i=7.74813e-5*kT*pow(Nc[i]/2,2./3);
			//double AT2i1=7.74813e-5*kT*pow(Nc[i+1]/2,2./3);
			J[i*num+i-1]=2*c[i-1]*SafeDExp(Efn1[i-1]/kT)/kT/step[i-1];
			if(affi[i]>=affi[i+1])
			{
				J[i*num+i]=-2*c[i-1]*SafeDExp(Efn1[i]/kT)/kT/step[i-1]-2*AT2i*SafeDExp((Efn1[i]-y[i]+affi[i+1])/kT)/kT/step[i-1]-QE*dR[i];
				J[i*num+i+1]=2*AT2i*SafeDExp((Efn1[i+1]-y[i+1]+affi[i+1])/kT)/kT/step[i-1];
			}
			else 
			{
				J[i*num+i]=-2*c[i-1]*SafeDExp(Efn1[i]/kT)/kT/step[i-1]-2*AT2i*SafeDExp((Efn1[i]-y[i]+affi[i])/kT)/kT/step[i-1]-QE*dR[i];
				J[i*num+i+1]=2*AT2i*SafeDExp((Efn1[i+1]-y[i+1]+affi[i])/kT)/kT/step[i-1];
			}
		}
		else 
		{
			//J[i*num+i-2]=2/(step[i]+step[i-2])/kT*c[i-2]*exp(Efn1[i-2]/kT);			
			if(affi[i-1]>=affi[i])
			{
				J[i*num+i-1]=2*AT2i*SafeDExp((Efn1[i-1]-y[i-1]+affi[i])/kT)/kT/step[i];
				J[i*num+i]=-2/(step[i])/kT*c[i]*SafeDExp(Efn1[i]/kT)-2/step[i]/kT*AT2i*SafeDExp((Efn1[i]-y[i]+affi[i])/kT)-QE*dR[i];
			}
			else
			{
				J[i*num+i-1]=2*AT2i*SafeDExp((Efn1[i-1]-y[i-1]+affi[i-1])/kT)/kT/step[i];
				J[i*num+i]=-2/(step[i])/kT*c[i]*SafeDExp(Efn1[i]/kT)-2/step[i]/kT*AT2i*SafeDExp((Efn1[i]-y[i]+affi[i-1])/kT)-QE*dR[i];
			}
			J[i*num+i+1]=2/(step[i])/kT*c[i]*SafeDExp(Efn1[i+1]/kT);
		}
	}
	J[num*num-2]=-c[num-2]*SafeDExp(Efn1[num-2]/kT)/kT;
	J[num*num-1]=c[num-2]*SafeDExp(Efn1[num-1]/kT)/kT+QE*BtmSn*(Nc[num-1]*SafeDExp((Efn1[num-1]-y[num-1]+affi[num-1])/kT))/kT;
	J[num*num-2]/=step[num-2];J[num*num-1]/=step[num-2];

	//delete[] dR;
}
//------------------------calculate Efp--------------------------------


//---------------------------------------------------------------------
bool Point:: IterP(double *y, double*J,double*l,double*u, int num,double kT,double TopSp,double BtmSp,wxProgressDialog *dlg)
{
	double *c=new double[num],  *Efp1=new double[num],*dEfp=new double[num],*n1=new double[num];
	double *f=new double[num];
//	double *J=new double[num*num],*l=new double[num*num],*u=new double[num*num];
	for(int i=0;i<num*num;i++)
	{
		J[i]=0;
	}
	for(int i=0;i<num;i++)
	{
		n1[i]=Nc[i]*SafeDExp((Efn[i]+affi[i]-y[i])/kT);
		if(step[i]!=0)
		{
			//c[i]=QE*up[i]*Nv[i]*exp((/*affi[num-1]*/-affi[i]-Eg[i])/kT)*(y[i+1]-y[i])/step[i]/(exp(-y[i+1]/kT)-exp(-y[i]/kT));		
			c[i]=QE*up[i]*Nv[i]/step[i]*SafeDExp((y[i]-affi[i]-Eg[i])/kT)*kT*(-xDexM1((-y[i+1]+affi[i+1]+Eg[i+1]+y[i]-affi[i]-Eg[i])/kT));
		}
		else
			c[i]=0;					
	}
	
	double ep=1e-6,tol=1;int cnt=0;bool end=0;
	ep=precision;
	while( tol>ep)
	{
		transfer(Efp,Efp1,num);
		fp(y,Efp1,n1,num,kT,TopSp,BtmSp,c,f);		
		Jacp(y,Efp1,n1,num,kT,TopSp,BtmSp,c,J);
		LU2(J,f,l,u,num,dEfp);
		clamp(dEfp,limit,num);
		
		for(int i=0;i<num;i++)
		{
			Efp[i]=Efp1[i]-dEfp[i];
		}		
		tol=norm(Efp,Efp1,num);
		if(_isnan(tol))
			end=1;
		cnt++;		
		if(cnt>(iterTimes)/2)
			end=1;
		if(end)
		{
#ifdef _DEBUG
			wxMessageBox(_("too many iterations in calculating Efp , results may be not correct!"));
#endif
			delete[] c;delete[]Efp1;delete[]dEfp;delete[]n1;delete[] f;
			return 0;
		}
	}
	delete[] c;delete[]Efp1;delete[]dEfp;delete[]n1;delete[] f;//delete[] J,l,u;
	return 1;
}
void Point:: fp(double *y, double *Efp1, double *n1,
		  int num,double kT,double TopSp,double BtmSp,double *c,double *f)
{
	double *p1=new double[num];double AT2i, delE;
	
	for(int i=0;i<num;i++)
	{
		p1[i]=Nv[i]*SafeDExp((y[i]-affi[i]/*+affi[num-1]*/-Eg[i]-Efp1[i])/kT);		
	
		if(step[i]!=0)
			Jp[i]=c[i]*SafeDExp(-Efp1[i]/kT)*(SafeDExp((Efp1[i]-Efp1[i+1])/kT)-1);
		else if(i!=num-1)
		{
			if(Nv[i]<Nv[i+1])
				AT2i=7.74813e-5*kT*pow(Nv[i]/2,2./3);
			else
				AT2i=7.74813e-5*kT*pow(Nv[i+1]/2,2./3);
			
			//double AT2i1=7.74813e-5*kT*pow(Nv[i+1]/2,2./3);
			delE=(affi[i+1]+Eg[i+1])-(affi[i]+Eg[i]);
			if(delE>=0)
				Jp[i]=AT2i*SafeDExp((y[i]-affi[i+1]-Eg[i+1]-Efp1[i])/kT)-AT2i*SafeDExp((y[i+1]-affi[i+1]-Eg[i+1]-Efp1[i+1])/kT);//?what if delE<0
			else
				Jp[i]=AT2i*SafeDExp((y[i]-affi[i]-Eg[i]-Efp1[i])/kT)-AT2i*SafeDExp((y[i+1]-affi[i]-Eg[i]-Efp1[i+1])/kT);
			Jp[i]*=1+tunCp[i];
		}
		else
			Jp[i]=Jp[i-1];
	}
	Recombination(1,n1,p1,num,kT);//dR=dR/dEfp
//	outEB(x,n1,p1,R,dR,num);
	f[0]=Jp[0]+QE*TopSp*(Nv[0]*SafeDExp((y[0]-affi[0]/*+affi[num-1]*/-Eg[0]-Efp1[0])/kT)-p0L);//?direction
	f[0]/=step[0];
	for (int i=1;i<num-1;i++)
	{
		if(step[i]!=0&&step[i-1]!=0)
			f[i]=2*(Jp[i]-Jp[i-1])/(step[i]+step[i-1])+QE*R[i]-QE*G[i];
		else if(0==step[i])
			f[i]=2*(Jp[i]-Jp[i-1])/step[i-1]+QE*R[i]-QE*G[i];			
		else
			f[i]=2*(Jp[i]-Jp[i-1])/step[i]+QE*(R[i])-QE*(G[i]);//?recombination model
	}
	f[num-1]=Jp[num-2]-QE*BtmSp*(Nv[num-1]*SafeDExp((y[num-1]-Eg[num-1]-Efp1[num-1]-affi[num-1])/kT)-p0R);
	f[num-1]/=step[num-2];

	delete[] p1;
}
void Point:: Jacp(double *y, double *Efp1, double *n1,
		  int num,double kT,double TopSp,double BtmSp,double *c,double *J)
{
	double delE;
	J[0*num+0]=(c[0]*SafeDExp(-Efp1[0]/kT)-QE*TopSp*Nv[0]*SafeDExp((y[0]-affi[0]-Eg[0]-Efp1[0])/kT))/kT;
	J[0*num+1]=-c[0]*SafeDExp(-Efp1[1]/kT)/kT;
	J[0*num+0]/=step[0];J[0*num+1]/=step[0];
	for(int i=1;i<num-1;i++)
	{
		if(step[i]!=0&&step[i-1]!=0)
		{
			J[i*num+i-1]=-2/(step[i]+step[i-1])/kT*c[i-1]*SafeDExp(-Efp1[i-1]/kT);
			J[i*num+i]=2/(step[i]+step[i-1])/kT*(c[i]+c[i-1])*SafeDExp(-Efp1[i]/kT)+QE*dR[i];///kT;
			J[i*num+i+1]=-2/(step[i]+step[i-1])/kT*c[i]*SafeDExp(-Efp1[i+1]/kT);			
		}
		else if(0==step[i])
		{
			double AT2i;
			if(Nv[i]<Nv[i+1])
				AT2i=7.74813e-5*kT*pow(Nv[i]/2,2./3);
			else
				AT2i=7.74813e-5*kT*pow(Nv[i+1]/2,2./3);
			//double AT2i=7.74813e-5*kT*pow(Nv[i]/2,2./3);
			//double AT2i1=7.74813e-5*kT*pow(Nv[i+1]/2,2./3);
			AT2i*=1+tunCp[i];
			J[i*num+i-1]=-2*c[i-1]*SafeDExp(-Efp1[i-1]/kT)/kT/step[i-1];
			delE=(affi[i+1]+Eg[i+1])-(affi[i]+Eg[i]);
			if(delE>0)
			{
				J[i*num+i]=2*c[i-1]*SafeDExp(-Efp1[i]/kT)/kT/step[i-1]-2*AT2i*SafeDExp((y[i]-affi[i+1]-Eg[i+1]-Efp1[i])/kT)/kT/step[i-1]+QE*dR[i];
				J[i*num+i+1]=2*AT2i*SafeDExp((y[i+1]-affi[i+1]-Eg[i+1]-Efp1[i+1])/kT)/kT/step[i-1];
			}
			else
			{
				J[i*num+i]=(2*c[i-1]*SafeDExp(-Efp1[i]/kT)-2*AT2i*SafeDExp((y[i]-affi[i]-Eg[i]-Efp1[i])/kT))/kT/step[i-1]+QE*dR[i];
				J[i*num+i+1]=2*AT2i*SafeDExp((y[i+1]-affi[i]-Eg[i]-Efp1[i+1])/kT)/kT/step[i-1];
			}
		}
		else
		{
			double AT2i=7.74813e-5*kT*pow(Nv[i]/2,2./3);
			AT2i*=1+tunCp[i-1];
			if(delE>=0)
			{J[i*num+i-1]=2/(step[i])*AT2i*SafeDExp((y[i-1]-affi[i]-Eg[i]-Efp1[i-1])/kT)/kT;
			J[i*num+i]=2/step[i]/kT*(c[i]*SafeDExp(-Efp1[i]/kT)-AT2i*SafeDExp((y[i]-affi[i]-Eg[i]-Efp1[i])/kT))+QE*dR[i];}
			else
			{
				J[i*num+i-1]=2/(step[i])*AT2i*SafeDExp((y[i-1]-affi[i-1]-Eg[i-1]-Efp1[i-1])/kT)/kT;
				J[i*num+i]=2/step[i]/kT*(c[i]*SafeDExp(-Efp1[i]/kT)-AT2i*SafeDExp((y[i]-affi[i-1]-Eg[i-1]-Efp1[i])/kT))+QE*dR[i];
			}
			J[i*num+i+1]=-2/(step[i])*c[i]*SafeDExp(-Efp1[i+1]/kT)/kT;
		}
	}
	J[num*num-2]=c[num-2]*SafeDExp(-Efp1[num-2]/kT)/kT;
	J[num*num-1]=-c[num-2]*SafeDExp(-Efp1[num-1]/kT)/kT+QE*BtmSp*Nv[num-1]*SafeDExp((y[num-1]-affi[num-1]-Eg[num-1]-Efp1[num-1])/kT)/kT;

	J[num*num-2]/=step[num-2];J[num*num-1]/=step[num-2];
	
//	outJ(J,num);
//	delete[] p1;
}
//calculate psi
bool Point:: IterPsi(double v,double *y, double*J,double*l,double*u,int num,double kT,
					 wxProgressDialog *dlg)
{	
	double *y1=new double[num];double *dy=new double[num];
	double *f=new double[num];
	
	for(int i=0;i<num*num;i++)
	{
		J[i]=0;
	}
	//wxString str, title="Function:fpsi\nIteration:";

	double ep=1e-6,tol=1;int cnt=0;		
	ep=precision;
	while( tol>ep)
	{
		transfer(y,y1,num);
		fpsi(v,y1,num,kT,f);
		Jpsi(y1,num,kT,J);
		LU2(J,f,l,u,num,dy);
		clamp(dy,limit,num);
		for(int i=0;i<num;i++)
		{
			y[i]=y1[i]-dy[i];
		}
		tol=norm(y1,y,num);
		
		cnt++;
		/*str.Printf("%d",cnt);str=title+str;
		if(!dlg->Update(cnt%100,str))
			return;*/
//		outEB(x,y,dy,Efn,Efp,num);
		if(cnt>(iterTimes)/2)                                       
		{	
#ifdef _DEBUG
			wxMessageBox(_("too many iterations in calculating psi , results may be not correct!"));
#endif
			delete[] y1;delete[]dy;delete[] f;
			return false;
		}
	}   
	delete[] y1;delete[]dy;delete[] f;// delete[] J;
	return true;
}
void Point::fpsi(double v,double *y,int num,double kT,double *f)
{
	//double * n1=new double[num], *p1=new double[num];
	for(int i=0;i<num;i++)
	{
		Ph[i]=Nv[i]*SafeDExp((y[i]-affi[i]-Eg[i]-Efp[i])/kT);//*GP12((y[i]-affi[i]-Eg[i]-Efp[i])/kT);
		Ne[i]=Nc[i]*SafeDExp((Efn[i]+affi[i]-y[i])/kT);//*GP12((Efn[i]+affi[i]-y[i])/kT);		
	}
	DefChargeNon(Ne,Ph,num,kT);
//	double b=affi[0]-affi[num-1]-kT*log(n[0]*Nc[num-1]/n[num-1]/Nc[0])-v;
	f[0]=y[0]-psi[0];//psiInit[0]+v;
	for (int i=1;i<num-1;i++)
	{
		if(step[i]!=0&&step[i-1]!=0)
			f[i]=2*eps[i]*EPSON*(y[i-1]/step[i-1]/(step[i]+step[i-1])-y[i]/step[i]/step[i-1]+y[i+1]/step[i]/(step[i]+step[i-1]))
			-QE*(Nd[i]-Ne[i]+Ph[i]-Na[i]+pt[i]-nt[i]);
			
		else if(0==step[i])
			f[i]=y[i]-y[i+1];
		else
		{
			f[i]=2*EPSON/(step[i]+step[i-2])*(eps[i]*(y[i+1]-y[i])/step[i]-eps[i-1]*(y[i-1]-y[i-2])/step[i-2])
				-QE*(Nd[i-1]-Ne[i-1]+Ph[i-1]-Na[i-1]+pt[i-1]-nt[i-1])
				-QE*(Nd[i]-Ne[i]+Ph[i]-Na[i]+pt[i]-nt[i]);
			//f[i]*=step[i-2];
		}		
	}
	f[num-1]=y[num-1]-psi[num-1];//-psiInit[num-1];
	//delete []n1; delete []p1;
}
void Point::Jpsi(double *y,int num,double kT,double *J)
{
	double * dn1=new double[num], *dp1=new double[num];
	for(int i=0;i<num;i++)
	{
		dp1[i]=Nv[i]*SafeDExp((y[i]-affi[i]-Eg[i]-Efp[i])/kT)/kT;//*GM12((y[i]-affi[i]-Eg[i]-Efp[i])/kT)/kT;
		dn1[i]=-Nc[i]*SafeDExp((Efn[i]+affi[i]-y[i])/kT)/kT;//*GM12((Efn[i]+affi[i]-y[i]))/kT;		
	}
	//double b=-kT*log(n[0]*Nc[num-1]/n[num-1]/Nc[0]);
	J[0*num+0]=1;  J[num*num-1]=1;
	for(int i=1;i<num-1;i++)
	{
		if(step[i]!=0&&step[i-1]!=0)
		{
			J[i*num+i-1]=2*eps[i]*EPSON/step[i-1]/(step[i]+step[i-1]);
			J[i*num+i]=-2*eps[i]*EPSON/step[i]/step[i-1]-
				QE*(dNd[i]-dNa[i]-dn1[i]+dp1[i]+dpt[i]-dnt[i]);
			J[i*num+i+1]=2*eps[i]*EPSON/step[i]/(step[i]+step[i-1]);
		}
		else if(0==step[i])
		{
			J[i*num+i]=1;
			J[i*num+i+1]=-1;
		}
		else
		{
			J[i*num+i-2]=2/(step[i]+step[i-2])*EPSON*eps[i-1]/step[i-2];
			J[i*num+i-1]=-2/(step[i]+step[i-2])*EPSON*eps[i-1]/step[i-2]-QE*(dNd[i-1]-dNa[i-1]-dn1[i-1]+dp1[i-1]+dpt[i-1]-dnt[i-1]);
			J[i*num+i]=-2/(step[i]+step[i-2])*EPSON*eps[i]/step[i]-QE*(dNd[i]-dNa[i]-dn1[i]+dp1[i]+dpt[i]-dnt[i]);
			J[i*num+i+1]=2/(step[i]+step[i-2])*EPSON*eps[i]/step[i];
		}
	}       
	delete[] dn1; delete []dp1;
}



void Point::DefChargeEqui(double*y,int num, double kT)
{
	int rgn=0;double N, Et,width;int type, dist;	int lyrCnt=layerListP->GetCount();
	MyList::compatibility_iterator node;
	node=layerListP->Item(rgn);	layer *curLyr=node->GetData();	int defCnt=curLyr->defList.GetCount();
	
	for(int i=0;i<num;i++)
	{
		curLyr->n.ToDouble(&N);Nd[i]=N;//initial Nd,Na
		curLyr->p.ToDouble(&N);Na[i]=N;
		Ntd[i]=0;Nta[i]=0;dNd[i]=0;dNa[i]=0;//initial dNd, dNa
		for(int j=0;j<defCnt;j++)
		{			
			defect *curDef=curLyr->defList.Item(j)->GetData();
			type=curDef->type;dist=curDef->dist;
			curDef->den.ToDouble(&N);curDef->el.ToDouble(&Et);curDef->width.ToDouble(&width);
			
			double xNd=0,dxNd=0, xNa=0,dxNa=0;
			switch(type)
			{
			case 0://acceptor-like
				DefAEqui(dist,N,y[i]-affi[i]-Et,width,kT,xNa,dxNa);
				break;
			case 1://donor-like
				DefDEqui(dist,N,-y[i]+affi[i]+Et,width,kT,xNd,dxNd);
				break;
			case 2:
				break;
			}
			Ntd[i]+=xNd;dNd[i]+=dxNd;
			Nta[i]+=xNa;dNa[i]+=dxNa;			
		}		
		Nd[i]+=Ntd[i];Na[i]+=Nta[i];
		//--------------band tails---------------
		if(curLyr->IsBT)
		{
			double Gdo,Ed,Gao,Ea; double Eda, GMgD,GMgA;
			double EgOpt, EgCon,EgVal, EgDif;
			curLyr->EgOpt.ToDouble(&EgOpt);//if wxString==NULL, then ToDouble->0.0
			curLyr->VBTEnergy.ToDouble(&Ed);curLyr->VBTGo.ToDouble(&Gdo);
			curLyr->CBTEnergy.ToDouble(&Ea);curLyr->CBTGo.ToDouble(&Gao);
			curLyr->MGEnergy.ToDouble(&Eda);curLyr->GMGD.ToDouble(&GMgD);curLyr->GMGA.ToDouble(&GMgA);

			//if(Eg[i] <= EgOpt)
				EgDif = 0;	//optical bandgap cannot exceed mobility gap
			/*else
				EgDif = Eg[i] - EgOpt;*/	//assumption: band tail starts from Eg, not EgOpt
			EgVal = EgApp * EgDif;	EgCon = EgDif - EgVal;
			//check for input error
			if(GMgD == 0)				GMgD = 1;
			if(GMgA == 0)				GMgA = 1;
			if(Gdo < GMgD)				Gdo = GMgD;
			if(Gao < GMgA)				Gao = GMgA;
			if(Ed== 0)	//prevent divide by zeros? donor level can't be exactly at band edge
				Ed = 1e-4;
			if(Ea == 0)				Ea = 1e-4;

			double eMeet = Ed * ((Eg[i]-EgDif) + Ea * log(Gdo/Gao)) / (Ed + Ea);//energy level at which tail states meet if extended
			double gMeet = Gdo * exp(-eMeet/Ed);				//density of states at eMeet
			eMeet += EgVal;	//put it in the proper reference
			double ELo, EUp;
			if(curLyr->BgShape)	//"U" shaped distribution
			{
				ELo = Ed * log(Gdo/GMgD);	//distance from opical valence band to lower boundary of midgap states
				EUp = Ea * log(Gao/GMgA);	//" " " conduction " " upper " " " "
				double ELow = EgVal + ELo;		//distance from mobility valence band to lower boundary of midgap states
				double EUpp = EgCon + EUp;		//" " " conduction " " upper " " " "

				if(ELow < (Eg[i] - EUpp))
				{
					if(Eda < ELow)
						Eda = ELow;
					if(Eda > (Eg[i] - EUpp))
						Eda = Eg[i] - EUpp;
				}
				else	//really a "V" shaped distribution
				{
					GMgD = gMeet;
					GMgA = gMeet;
					Eda = eMeet;
					ELo = Ed * log(Gdo / GMgD);
					EUp = Ea * log(Gao / GMgA);
				}
			}
			else	//V shaped
			{
				GMgD = gMeet;
				GMgA = gMeet;
				Eda = eMeet;
				ELo = Ed * log(Gdo / GMgD);
				EUp = Ea * log(Gao / GMgA);
			}
			Traps(y[i]-affi[i],-y[i]+affi[i]+Eg[i],kT,1/kT,Eg[i],EgOpt,
				Gdo,Gao,GMgD,GMgA,Ed,1/Ed,Ea,1/Ea,Eda,
				0,ELo,EUp,pt[i],nt[i],dpt[i],dnt[i]);//not 0k approximation
		}
		//---------------------------------------
		if(0==step[i])
		{
			rgn++;
			if(rgn<lyrCnt)
			{node=layerListP->Item(rgn);	curLyr=node->GetData();	defCnt=curLyr->defList.GetCount();}			
		}
	}
	
}

//double  xStDv[MGAUSS+1]={-2.4,-2.0,-1.7,-1.5,-1.3,-1.1,-1.0,-0.9,-0.7,-0.5,-0.3,-0.1,
//0.1,0.3,0.5,0.7,0.9,1.0,1.1,1.3,1.5,1.7,2.0,2.4,};
//double xProb[MGAUSS]={0.014552596023582942, 0.02181533081036402, 0.022241738510314968, 0.029993283316751995,
//0.038865576360772036, 0.022989192985074958, 0.025404871415302011, 0.05790352687631406,
//0.066573886502913959, 0.073551039085059999,0.07808358491192402,0.079655674554057976,
//0.07808358491192402,0.073551039085059999,0.066573886502913959,0.05790352687631406,
//0.025404871415302011,0.022989192985074958,0.038865576360772036,0.029993283316751995,
//0.022241738510314968,0.02181533081036402,0.014552596023582942};//gaussian probability

void Point::DefDEqui(int dist,double N,double Efd,double w,double kT,double &xNd,double &dxNd)
{
	if(0==w||0==dist)//discrete
	{
		xNd=N*Focc1(1,Efd/kT);
		dxNd=xNd*Focc1(1,-Efd/kT)/kT;
	}
	else if(1==dist)//banded
	{
		xNd=Focc2(1, exp(-w/kT), (Efd+w/2)/kT);
		xNd=N*(1-kT/w*log(xNd));
		dxNd= Focc3(1, exp(-w/kT), (Efd+w/2)/kT);
		dxNd= N /w * dxNd;
	}
	else//gaussian
	{
		double xN,xWidth,xEfd;
		for(int i=0;i<MGAUSS;i++)
		{
			xN = N*xProb[i];	
			xEfd= Efd-w*(xStDv[i]+xStDv[i+1])/2;
			xWidth = w*(xStDv[i+1]-xStDv[i]);

			xNd+=xN*(1-kT/xWidth*log(Focc2(1, exp(-xWidth/kT), (xEfd+xWidth/2)/kT)));
			dxNd+=xN/xWidth*Focc3(1, exp(-xWidth/kT), (xEfd+xWidth/2)/kT);
		}		
	}
}
void Point::DefAEqui(int dist,double N,double Eaf,double w,double kT,double &xNa,double &dxNa)
{
	if(0==w||0==dist)//discrete
	{
		xNa=N*Focc1(1,Eaf/kT);
		dxNa=-xNa*Focc1(1,-Eaf/kT)/kT;
	}
	else if(1==dist)
	{	
		xNa=Focc2(1, exp(-w/kT), (Eaf+w/2)/kT);
		xNa=N*(1-kT/w*log(xNa));
		dxNa= -Focc3(1, exp(-w/kT), (Eaf+w/2)/kT);
		dxNa= N /w * dxNa;
	}
	else//gaussian
	{	
		double xN,xWidth,xEaf;
		for(int i=0;i<MGAUSS;i++)
		{			
			xN = N*xProb[i];	
			xEaf= Eaf+w*(xStDv[i]+xStDv[i+1])/2;
			xWidth = w*(xStDv[i+1]-xStDv[i]);

			xNa+=xN*(1-kT/xWidth*log(Focc2(1, exp(-xWidth/kT), (xEaf+xWidth/2)/kT)));
			dxNa+=-xN/xWidth*Focc3(1, exp(-xWidth/kT), (xEaf+xWidth/2)/kT);				
		}
	}
}
void Point::Recombination(bool dnp,double *n,double *p,int num,double kT)//calculate R and dR
{
	int rgn=0;double N,Et,width,sigP,sigN;int type,dist;
	double En1,Pe1;
	double Vth=sqrt(3.8682e15 * kT);

	layer * curLyr=layerListP->Item(rgn)->GetData();
	int defCnt=curLyr->defList.GetCount();
	double Gdo,Ed,VBTSigN,VBTSigP,Gao,Ea,CBTSigN,CBTSigP; double Eda, GMgD,SigNMGD,SigPMGD,GMgA,SigNMGA,SigPMGA;
	double EgOpt,/*, EgCon,EgVal, EgDif*/ELo,EUp;
	curLyr->EgOpt.ToDouble(&EgOpt);//if wxString==NULL, then ToDouble->0.0
	curLyr->VBTEnergy.ToDouble(&Ed);curLyr->VBTGo.ToDouble(&Gdo);curLyr->VBTSigN.ToDouble(&VBTSigN);curLyr->VBTSigP.ToDouble(&VBTSigP);
	curLyr->CBTEnergy.ToDouble(&Ea);curLyr->CBTGo.ToDouble(&Gao);curLyr->CBTSigN.ToDouble(&CBTSigN);curLyr->CBTSigP.ToDouble(&CBTSigP);
	curLyr->MGEnergy.ToDouble(&Eda);curLyr->GMGD.ToDouble(&GMgD);curLyr->GMGA.ToDouble(&GMgA);
	curLyr->SigNMGD.ToDouble(&SigNMGD);curLyr->SigPMGD.ToDouble(&SigPMGD);
	curLyr->SigNMGA.ToDouble(&SigNMGA);curLyr->SigPMGA.ToDouble(&SigPMGA);
	
	for(int i=0;i<num;i++)
	{
		/*layer * curLyr=layerListP->Item(rgn)->GetData();
		int defCnt=curLyr->defList.GetCount();*/
		R[i]=0;dR[i]=0;//initial
		for(int j=0;j<defCnt;j++)
		{			
			defect *curDef=curLyr->defList.Item(j)->GetData();
			type=curDef->type;dist=curDef->dist;
			curDef->den.ToDouble(&N);curDef->el.ToDouble(&Et);curDef->width.ToDouble(&width);
			curDef->sigN.ToDouble(&sigN);curDef->sigP.ToDouble(&sigP);
			double xR=0,dxR=0;
			
			if(0==width||0==dist)//discrete
			{
				En1 = Nc[i] * exp(-Et/kT);
				Pe1 = Nv[i] * exp((Et-Eg[i])/kT);
				double denom=sigN*(n[i] + /*GP12n0*/ En1) + sigP*(p[i] + /*GP12p0*/  Pe1);
				xR=N*sigN*sigP*Vth*(n[i]*p[i]-ni[i]*ni[i])/denom;
				dxR=N*sigN*sigP*Vth/denom/denom;
				if(0==dnp)//dEfn
					dxR*=(n[i]*p[i]/kT)*denom-(n[i]*p[i]-ni[i]*ni[i])*sigN*n[i]/kT;
				else//dEfp
					dxR*=(-n[i]*p[i]/kT)*denom+(n[i]*p[i]-ni[i]*ni[i])*sigP*p[i]/kT;
			}
			else if(1==dist)//banded
			{
				double E1=Eg[i]-Et-width/2; double E2=Eg[i]-Et+width/2;//Ev as reference
				double gt=N/width;
				calU(gt,E1,E2,Eg[i],Nc[i],Nv[i],
					sigN,sigP,Vth,kT,n[i],p[i],ni[i]*ni[i],
					xR,dxR,dnp);
			}
			else//gaussian
			{
				double xN,xWidth,xEt;
				for(int k=0;k<MGAUSS;k++)
				{			
					xN = N*xProb[k];	
					xEt= Et+width*(xStDv[k]+xStDv[k+1])/2;
					xWidth = width*(xStDv[k+1]-xStDv[k]);
					double E1=Eg[i]-xEt-xWidth/2; double E2=Eg[i]-xEt+xWidth/2;//Ev as reference
					double gt=xN/xWidth; double xGR,dxGR;
					calU(gt,E1,E2,Eg[i],Nc[i],Nv[i],
						sigN,sigP,Vth,kT,n[i],p[i],ni[i]*ni[i],
						xGR,dxGR,dnp);
					xR+=xGR;
					dxR+=dxGR;
				}
			}
			R[i]+=xR;
			dR[i]+=dxR;
		}
//--------------band tails---------------
		if(curLyr->IsBT)
		{
			double RdMd=0,dRd=0,RaMd=0,dRa=0,RdTl=0,dRdTl=0,RaTl=0,dRaTl=0;
			double xR,dxR;
			Eda=Eg[i]-Eda;//Ev as reference
			CheckTail(curLyr->BgShape,Eg[i],EgOpt,Eda,
				Ed,Ea,Gdo,Gao,GMgD,GMgA,ELo,EUp);
			double Sig_N = SigNMGD;			double Sig_P = SigPMGD;
			double E1 =/* EgVal +*/ ELo;	double E2 = Eda;
			double Gt = GMgD;	//donor-like midgap density of states
			if(E1 < E2)	//If lower bound on midgap states is less than the current energy of interest
			{
				calU(Gt,E1,E2,Eg[i],Nc[i],Nv[i],
						Sig_N,Sig_P,Vth,kT,n[i],p[i],ni[i]*ni[i],
						xR,dxR,dnp);
				RdMd=xR;	dRd=dxR;
			}
			
			Sig_N = SigNMGA;
			Sig_P = SigPMGA;
			E1 = Eda;
			E2 = Eg[i]- EUp;
			Gt = GMgA;
			if(E1 < E2)	//current energy is less than top of midgap states
			{
				calU(Gt,E1,E2,Eg[i],Nc[i],Nv[i],
						Sig_N,Sig_P,Vth,kT,n[i],p[i],ni[i]*ni[i],
						xR,dxR,dnp);
				RaMd=xR;	dRa=dxR;
			}

			double EdStep = ELo / NTAILS;	//Elo is lower bound on midgap states (+ UP frpm Ev)
			double EaStep = EUp / NTAILS;	//EUp is upper bound on midgap states (+ DOWN from Ec)
			if(EdStep > 0)
			{
				for(int ie=0; ie<NTAILS; ie++)
				{
					Sig_N = VBTSigN;
					Sig_P = VBTSigP;
					E1 = ie*EdStep;
					E2 = (ie+1)*EdStep;
					Gt = Gdo * Ed * (exp(-E1/Ed) - exp(-E2/Ed)) / EdStep;
					
					calU(Gt,E1,E2,Eg[i],Nc[i],Nv[i],
						Sig_N,Sig_P,Vth,kT,n[i],p[i],ni[i]*ni[i],
						xR,dxR,dnp);
					RdTl+=xR;	dRdTl+=dxR;
				}
			}
			if(EaStep > 0)
			{
				for(int ie=0; ie<NTAILS; ie++)
				{
					Sig_N = CBTSigN;
					Sig_P = CBTSigP;
					E1 = Eg[i] - EUp + ie * EaStep;
					E2 = Eg[i] - EUp + (ie+1) * EaStep;
					Gt = Gao * Ea * (exp((E2-Eg[i])/Ea) - exp((E1-Eg[i])/Ea)) / EaStep;
					calU(Gt,E1,E2,Eg[i],Nc[i],Nv[i],
						Sig_N,Sig_P,Vth,kT,n[i],p[i],ni[i]*ni[i],
						xR,dxR,dnp);
					RaTl+=xR;	dRaTl+=dxR;
				}
			}
			R[i]+=RdTl+RdMd+RaTl+RaMd;
			dR[i]+=dRdTl+dRd+dRaTl+dRa;
		}
//--------------next layer--------------
		if(0==step[i])		
		{	
			rgn++; int lyrCnt=layerListP->GetCount();
			if(rgn<lyrCnt)
			{curLyr=layerListP->Item(rgn)->GetData();
			defCnt=curLyr->defList.GetCount();
			
			curLyr->EgOpt.ToDouble(&EgOpt);//if wxString==NULL, then ToDouble->0.0
			curLyr->VBTEnergy.ToDouble(&Ed);curLyr->VBTGo.ToDouble(&Gdo);curLyr->VBTSigN.ToDouble(&VBTSigN);curLyr->VBTSigP.ToDouble(&VBTSigP);
			curLyr->CBTEnergy.ToDouble(&Ea);curLyr->CBTGo.ToDouble(&Gao);curLyr->CBTSigN.ToDouble(&CBTSigN);curLyr->CBTSigP.ToDouble(&CBTSigP);
			curLyr->MGEnergy.ToDouble(&Eda);curLyr->GMGD.ToDouble(&GMgD);curLyr->GMGA.ToDouble(&GMgA);
			curLyr->SigNMGD.ToDouble(&SigNMGD);curLyr->SigPMGD.ToDouble(&SigPMGD);
			curLyr->SigNMGA.ToDouble(&SigNMGA);curLyr->SigPMGA.ToDouble(&SigPMGA);
			}
		}
	}
}
void Point::DefChargeNon(double *n,double*p,int num,double kT)
{
	int rgn=0;double N,Et,width,sigP,sigN;int type,dist;
	double Vth=sqrt(3.8682e15 * kT);//m*=0.45me
	double En1,Pe1;double Gdo,Ed,VBTSigN,VBTSigP,Gao,Ea,CBTSigN,CBTSigP; double Eda, GMgD,SigNMGD,SigPMGD,GMgA,SigNMGA,SigPMGA;
	double EgOpt,/*, EgCon,EgVal, EgDif*/ELo,EUp;
	layer *curLyr=layerListP->Item(rgn)->GetData();
	int	defCnt=curLyr->defList.GetCount();
	curLyr->EgOpt.ToDouble(&EgOpt);//if wxString==NULL, then ToDouble->0.0
	curLyr->VBTEnergy.ToDouble(&Ed);curLyr->VBTGo.ToDouble(&Gdo);curLyr->VBTSigN.ToDouble(&VBTSigN);curLyr->VBTSigP.ToDouble(&VBTSigP);
	curLyr->CBTEnergy.ToDouble(&Ea);curLyr->CBTGo.ToDouble(&Gao);curLyr->CBTSigN.ToDouble(&CBTSigN);curLyr->CBTSigP.ToDouble(&CBTSigP);
	curLyr->MGEnergy.ToDouble(&Eda);curLyr->GMGD.ToDouble(&GMgD);curLyr->GMGA.ToDouble(&GMgA);
	curLyr->SigNMGD.ToDouble(&SigNMGD);curLyr->SigPMGD.ToDouble(&SigPMGD);
	curLyr->SigNMGA.ToDouble(&SigNMGA);curLyr->SigPMGA.ToDouble(&SigPMGA);
	for(int i=0;i<num;i++)
	{
		layer * curLyr=layerListP->Item(rgn)->GetData();
		int defCnt=curLyr->defList.GetCount();
		curLyr->n.ToDouble(&N);Nd[i]=N;//initial Nd,Na
		curLyr->p.ToDouble(&N);Na[i]=N;
		Ntd[i]=0;Nta[i]=0;dNd[i]=0;dNa[i]=0;//initial dNd, dNa
		for(int j=0;j<defCnt;j++)
		{			
			defect *curDef=curLyr->defList.Item(j)->GetData();
			type=curDef->type;dist=curDef->dist;
			curDef->den.ToDouble(&N);curDef->el.ToDouble(&Et);curDef->width.ToDouble(&width);
			curDef->sigN.ToDouble(&sigN);curDef->sigP.ToDouble(&sigP);
			double xNd=0,dxNd=0, xNa=0,dxNa=0;
			En1 = Nc[i] * exp(-Et/kT);
			Pe1 = Nv[i] * exp((Et-Eg[i])/kT);
			double numer;
			double denom=sigN*(n[i] + /*GP12n0*/ En1) + sigP*(p[i] + /*GP12p0*/  Pe1);
			if(0==width||0==dist)
			{				
				switch(type)
				{
				case 0://acceptor
					numer =  sigN*n[i] + sigP * Pe1;
					xNa=N*numer/denom;
					dxNa=N*(-sigN*n[i]/kT*denom-numer*(-sigN*n[i]+sigP*p[i])/kT)/denom/denom;
					break;
				case 1:
					numer =sigN*En1 + sigP * p[i];
					xNd=N*numer/denom;
					dxNd=N*(sigP*p[i]/kT*denom-numer*(-sigN*n[i]+sigP*p[i])/kT)/denom/denom;
					break;
				case 2:
					break;
				}
			}
			else if(1==dist)
			{
				double E1=Eg[i]-Et-width/2; double E2=Eg[i]-Et+width/2;//Ev as reference
				double gt=N/width;
				double xPt,dxPt;
				calPt(gt,E1,E2,Eg[i],Nc[i],Nv[i],
					sigN,sigP,Vth,kT,n[i],p[i],ni[i]*ni[i],
					xPt,dxPt);
				switch(type)
				{
				case 0://acceptor
					xNa=N-xPt;
					dxNa=-dxPt;
					break;
				case 1:
					xNd=xPt;
					dxNd=dxPt;
					break;
				case 2:
					break;
				}
			}
			else
			{
				double xN,xWidth,xEt;
				for(int k=0;k<MGAUSS;k++)
				{			
					xN = N*xProb[k];	
					xEt= Et+width*(xStDv[k]+xStDv[k+1])/2;
					xWidth = width*(xStDv[k+1]-xStDv[k]);
					double E1=Eg[i]-xEt-xWidth/2; double E2=Eg[i]-xEt+xWidth/2;//Ev as reference
					double gt=xN/xWidth; double xGPt,dxGPt;
					calPt(gt,E1,E2,Eg[i],Nc[i],Nv[i],
						sigN,sigP,Vth,kT,n[i],p[i],ni[i]*ni[i],
						xGPt,dxGPt);
					switch(type)
					{
					case 0://acceptor
						xNa+=xN-xGPt;
						dxNa+=-dxGPt;
						break;
					case 1:
						xNd+=xGPt;
						dxNd+=dxGPt;
						break;
					case 2:
						break;
					}
				}
			}
			Ntd[i]+=xNd;dNd[i]+=dxNd;
			Nta[i]+=xNa;dNa[i]+=dxNa;	
		}
		Nd[i]+=Ntd[i];Na[i]+=Nta[i];
		//------------band tail---------------
		if(curLyr->IsBT)
		{
			pt[i]=0;dpt[i]=0;nt[i]=0;dnt[i]=0;
			double Pt,dPt,PtMd=0,NtMd=0,PtTl=0,NtTl=0;double dPtMd=0,dNtMd=0,dPtTl=0,dNtTl=0;
			Eda=Eg[i]-Eda;//Ev as reference
			CheckTail(curLyr->BgShape,Eg[i],EgOpt,Eda,
				Ed,Ea,Gdo,Gao,GMgD,GMgA,ELo,EUp);
			double Sig_N = SigNMGD;			double Sig_P = SigPMGD;
			double E1 =/* EgVal +*/ ELo;	double E2 = Eda;
			double Gt = GMgD;	//donor-like midgap density of states
			if(E1 < E2)	//If lower bound on midgap states is less than the current energy of interest
			{
				calPt(Gt,E1,E2,Eg[i],Nc[i],Nv[i],
					Sig_N,Sig_P,Vth,kT,n[i],p[i],ni[i]*ni[i],
					Pt,dPt);
				PtMd=Pt;	dPtMd=dPt;
			}

			Sig_N = SigNMGA;
			Sig_P = SigPMGA;
			E1 = Eda;
			E2 = Eg[i]- EUp;
			Gt = GMgA;
			if(E1 < E2)	//current energy is less than top of midgap states
			{
				calPt(Gt,E1,E2,Eg[i],Nc[i],Nv[i],
					Sig_N,Sig_P,Vth,kT,n[i],p[i],ni[i]*ni[i],
					Pt,dPt);
				NtMd=Gt*(E2-E1)-Pt;	dNtMd=-dPt;
			}

			double EdStep = ELo / NTAILS;	//Elo is lower bound on midgap states (+ UP frpm Ev)
			double EaStep = EUp / NTAILS;	//EUp is upper bound on midgap states (+ DOWN from Ec)
			if(EdStep > 0)
			{
				for(int ie=0; ie<NTAILS; ie++)
				{
					Sig_N = VBTSigN;
					Sig_P = VBTSigP;
					E1 = ie*EdStep;
					E2 = (ie+1)*EdStep;
					Gt = Gdo * Ed * (exp(-E1/Ed) - exp(-E2/Ed)) / EdStep;

					calPt(Gt,E1,E2,Eg[i],Nc[i],Nv[i],
						Sig_N,Sig_P,Vth,kT,n[i],p[i],ni[i]*ni[i],
						Pt,dPt);
					PtTl+=Pt;	dPtTl+=dPt;
				}
			}
			if(EaStep > 0)
			{
				for(int ie=0; ie<NTAILS; ie++)
				{
					Sig_N = CBTSigN;
					Sig_P = CBTSigP;
					E1 = Eg[i] - EUp + ie * EaStep;
					E2 = Eg[i] - EUp + (ie+1) * EaStep;
					Gt = Gao * Ea * (exp((E2-Eg[i])/Ea) - exp((E1-Eg[i])/Ea)) / EaStep;
					calPt(Gt,E1,E2,Eg[i],Nc[i],Nv[i],
						Sig_N,Sig_P,Vth,kT,n[i],p[i],ni[i]*ni[i],
						Pt,dPt);
					NtTl+=Gt*(E2-E1)-Pt;	dNtTl+=-dPt;
				}
			}
			pt[i]=PtMd+PtTl;		nt[i]=NtMd+NtTl;
			dpt[i]=dPtMd+dPtTl;		dnt[i]=dNtMd+dNtTl;
		}
		if(0==step[i])	
		{
			rgn++;
			int lyrCnt=layerListP->GetCount();
			if(rgn<lyrCnt)
			{curLyr=layerListP->Item(rgn)->GetData();
			defCnt=curLyr->defList.GetCount();
			
			curLyr->EgOpt.ToDouble(&EgOpt);//if wxString==NULL, then ToDouble->0.0
			curLyr->VBTEnergy.ToDouble(&Ed);curLyr->VBTGo.ToDouble(&Gdo);curLyr->VBTSigN.ToDouble(&VBTSigN);curLyr->VBTSigP.ToDouble(&VBTSigP);
			curLyr->CBTEnergy.ToDouble(&Ea);curLyr->CBTGo.ToDouble(&Gao);curLyr->CBTSigN.ToDouble(&CBTSigN);curLyr->CBTSigP.ToDouble(&CBTSigP);
			curLyr->MGEnergy.ToDouble(&Eda);curLyr->GMGD.ToDouble(&GMgD);curLyr->GMGA.ToDouble(&GMgA);
			curLyr->SigNMGD.ToDouble(&SigNMGD);curLyr->SigPMGD.ToDouble(&SigPMGD);
			curLyr->SigNMGA.ToDouble(&SigNMGA);curLyr->SigPMGA.ToDouble(&SigPMGA);
			}
		}
	}
}
void Point::calcGamma(double *y, double kT)
{
	
}