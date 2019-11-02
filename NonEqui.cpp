#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "Calculation.h"
#include "NonEqui.h"
#include "Constant.h"
#include "LU.h"
#include "Point.h"
#include "mainamps.h"
//#include "resdisplay.h"


result* NonEquiInit(double v, Point* pt,int num,double kT,
				bool IsWF,double TopWF,double TopSn,double TopSp,
				double BtmWF,double BtmSn,double BtmSp,wxProgressDialog *dlg,wxString str,int &prgCnt)
{
	double *psi1= new double[num],*psi2= new double[num];
	double *J=new double[num*num], *l=new double[num*num],*u=new double[num*num];
	double EfInit=0;//-kT*log(pt->Nc[num-1]/pt->Nd[num-1]);
	/*for(int i=0;i<num;i++)
	{
		psi1[i]=pt->psiInit[i]-(1-pt->x[i]/pt->x[num-1])*v;		
		pt->Efn[i]=EfInit-(1-pt->x[i]/pt->x[num-1])*v;
		pt->Efp[i]=EfInit-(1-pt->x[i]/pt->x[num-1])*v;
	}*/
//	wxString str, title="Function:Non Equilibrium\nIteration:";

	double ep=1e-6,tol=1;int cnt=0;
	transfer(pt->psi,psi1,num);
	while( tol>ep)
	{
		cnt++;				
		transfer(psi1,psi2,num);//transfer(Efn1,Efn2,num);transfer(Efp1,Efp2,num);		
		//pt->IterN(psi2,J,l,u,num,kT,TopSn,BtmSn,NULL);
		if(!pt->IterN(psi2,J,l,u,num,kT,TopSn,BtmSn,NULL))
		{
			/*delete[] psi1;delete []psi2;
			delete[] J;delete []l;delete []u;
			return NULL;*/
			//break;
		}
//		outEB(pt->x,pt->psi,psi2,pt->Efn,pt->Efp,num);
		if(!pt->IterP(psi2,J,l,u,num,kT,TopSp,BtmSp,NULL))
		{
			/*delete[] psi1;delete []psi2;
			delete[] J;delete []l;delete []u;
			return NULL;*/
			//break;
		}
		if(!pt->IterPsi(v,psi2,J,l,u,num,kT,NULL))
		{
			/*delete[] psi1;delete []psi2;
			delete[] J;delete []l;delete []u;
			return NULL;*/
			//transfer(psi2,psi1,num);	
			//break;
		}
		tol=norm(psi1,psi2,num);		
		transfer(psi2,psi1,num);			
		/*if(cnt>100)                                       
		{	
			wxMessageBox("too many iterations, failed in converging");
			return NULL;
		}*/
		if(cnt>(pt->iterTimes/15))
			break;
	}
	transfer(psi1,pt->psi,num);
	//----------------postprocess----------------	
//	outEB(pt->x,pt->Jn,pt->Jp,pt->Efn,pt->Efp,num);
	result *curRes=new result;
	curRes->v=v;
	for(int i=0;i<num;i++)
	{
		pt->Ec[i]=pt->psi[i]-pt->affi[i];//+pt->affi[PointNumber-1];
		pt->Ev[i]=pt->Ec[i]-pt->Eg[i];
		curRes->x.push_back(pt->x[i]*1e4);
		curRes->Ec.push_back(pt->Ec[i]);curRes->Ev.push_back(pt->Ev[i]);
		curRes->Efn.push_back(pt->Efn[i]);curRes->Efp.push_back(pt->Efp[i]);
		curRes->n.push_back(pt->Ne[i]);curRes->p.push_back(pt->Ph[i]);//log
		curRes->nt.push_back(pt->nt[i]);curRes->pt.push_back(pt->pt[i]);
		curRes->Ntd.push_back(pt->Ntd[i]);curRes->Nta.push_back(pt->Nta[i]);
		curRes->R.push_back(pt->R[i]);curRes->G.push_back(pt->G[i]);
		curRes->Jn.push_back(pt->Jn[i]);curRes->Jp.push_back(pt->Jp[i]);
	}

	delete[] psi1;delete []psi2;
	delete[] J;delete []l;delete []u;
	return curRes;
}
void ToRes(Point *pt,result *curRes,int num)
{
for(int i=0;i<num;i++)
	{
		pt->Ec[i]=pt->psi[i]-pt->affi[i];//+pt->affi[PointNumber-1];
		pt->Ev[i]=pt->Ec[i]-pt->Eg[i];
		curRes->x.push_back(pt->x[i]*1e4);
		curRes->Ec.push_back(pt->Ec[i]);curRes->Ev.push_back(pt->Ev[i]);
		curRes->Efn.push_back(pt->Efn[i]);curRes->Efp.push_back(pt->Efp[i]);
		curRes->n.push_back(pt->Ne[i]);curRes->p.push_back(pt->Ph[i]);//log
		curRes->nt.push_back(pt->nt[i]);curRes->pt.push_back(pt->pt[i]);
		curRes->Ntd.push_back(pt->Ntd[i]);curRes->Nta.push_back(pt->Nta[i]);
		curRes->R.push_back(pt->R[i]);curRes->G.push_back(pt->G[i]);
		curRes->Jn.push_back(pt->Jn[i]);curRes->Jp.push_back(pt->Jp[i]);
		curRes->EleF.push_back(pt->EleF[i]);
	}
}
result* NonEquiTun(double v, Point* pt,int num,double kT,
				bool IsWF,double TopWF,double TopSn,double TopSp,
				double BtmWF,double BtmSn,double BtmSp,wxProgressDialog *dlg,wxString str,int &prgCnt)
{
	double *psi1= new double[num],*psi2= new double[num];
	double *J=new double[num*num], *l=new double[num*num],*u=new double[num*num];
	double EfInit=0;//-kT*log(pt->Nc[num-1]/pt->Nd[num-1]);
	//for(int i=0;i<num;i++)
	//{
	//	psi1[i]=pt->psiInit[i]-(1-pt->x[i]/pt->x[num-1])*v;		
	//	pt->Efn[i]=EfInit-(1-pt->x[i]/pt->x[num-1])*v;
	//	pt->Efp[i]=EfInit-(1-pt->x[i]/pt->x[num-1])*v;		
	//}
	transfer(pt->psi,psi1,num);
	double ep=1e-6,tol=1;int cnt=0;
	ep=pt->precision;
	while( tol>ep)
	{
		cnt++;	
		transfer(psi1,psi2,num);

		pt->IterNTun(psi2,J,l,u,num,kT,TopSn,BtmSn,NULL);
		//pt->IterN(v,psi2,J,l,u,num,kT,TopSn,BtmSn,NULL);
		//if(!pt->IterP(v,psi2,J,l,u,num,kT,TopSp,BtmSp,NULL))
		if(!pt->IterP(psi2,J,l,u,num,kT,TopSp,BtmSp,NULL))
		{
			delete[] psi1;delete []psi2;
			delete[] J;delete []l;delete []u;
			return NULL;
			//break;
		}	
		if(!pt->IterPsi(v,psi2,J,l,u,num,kT,NULL))
		{
			delete[] psi1;delete []psi2;
			delete[] J;delete []l;delete []u;
			return NULL;
			//transfer(psi2,psi1,num);
			//break;
		}

		pt->calcTunC(psi2,kT);
	//	outEB(pt->x,pt->psi,pt->Efn,pt->Efp,pt->tunC,num);
		tol=norm(psi1,psi2,num);
		transfer(psi2,psi1,num);	

		if(!dlg->Update(prgCnt%100,str))
		{
			//wxMessageBox(_("user interrupt!"));
			dlg->Destroy();
			delete[] psi1;delete []psi2;
			delete[] J;delete []l;delete []u;
			return NULL;			
		}
		if(cnt>150)                                       
		{	
			/*wxString str;
			str.Printf("at v=%.4f",v);str="too many Gummel iterations, failed in converging "+str;
			wxMessageBox(str);			
			delete[] psi1;delete []psi2;
			delete[] J;delete []l;delete []u;
			return NULL;*/
			break;
		}
	}
	transfer(psi1,pt->psi,num);
	//----------------postprocess----------------	
	for(int i=1;i<num-1;i++)
		pt->EleF[i]=(pt->psi[i+1]-pt->psi[i-1])/(pt->step[i]+pt->step[i-1]);	
	pt->EleF[0]=pt->EleF[1];pt->EleF[num-1]=pt->EleF[num-2];
	result *curRes=new result;
	curRes->v=v;
	ToRes(pt,curRes,num);
	//for(int i=0;i<num;i++)
	//{
	//	pt->Ec[i]=pt->psi[i]-pt->affi[i];//+pt->affi[PointNumber-1];
	//	pt->Ev[i]=pt->Ec[i]-pt->Eg[i];
	//	curRes->x.push_back(pt->x[i]*1e4);
	//	curRes->Ec.push_back(pt->Ec[i]);curRes->Ev.push_back(pt->Ev[i]);
	//	curRes->Efn.push_back(pt->Efn[i]);curRes->Efp.push_back(pt->Efp[i]);
	//	curRes->n.push_back(pt->Ne[i]);curRes->p.push_back(pt->Ph[i]);//log
	//	curRes->nt.push_back(pt->nt[i]);curRes->pt.push_back(pt->pt[i]);
	//	curRes->Ntd.push_back(pt->Ntd[i]);curRes->Nta.push_back(pt->Nta[i]);
	//	curRes->R.push_back(pt->R[i]);curRes->G.push_back(pt->G[i]);
	//	curRes->Jn.push_back(pt->Jn[i]);curRes->Jp.push_back(pt->Jp[i]);
	//}

	delete[] psi1;delete []psi2;
	delete[] J;delete []l;delete []u;
	return curRes;
}
//void NonEqui(double v, Point* pt,int num,double kT)
//{
//	double *psi1= new double[num],*psi2= new double[num];
//	double EfInit=-kT*log(pt->pt->Nc[num-1]/pt->pt->Nd[num-1]);
//	for(int i=0;i<num;i++)
//	{
//		psi1[i]=pt->psiInit[i]-(1-pt->x[i]/pt->x[num-1])*v;
//		pt->pt->Efn[i]=EfInit-(1-pt->x[i]/pt->x[num-1])*v;
//		pt->pt->Efp[i]=EfInit-(1-pt->x[i]/pt->x[num-1])*v;
//	}
//	double ep=1e-6,tol=1;int cnt=0;
//	while( tol>ep)
//	{
//		transfer(psi1,psi2,num);//transfer(Efn1,Efn2,num);transfer(Efp1,Efp2,num);		
//		pt->IterN(psi2,num,kT,NULL);//Efn will be changed
//		pt->IterP(psi2,num,kT,NULL);//Efp will be changed
//		pt->IterPsi(v,psi2,num,kT,NULL);//psi2 will be changed
//		tol=norm(psi1,psi2,num);
//
//		transfer(psi2,psi1,num);
//		cnt++;		
//		if(cnt>200)                                       
//		{	
//			wxMessageBox("too many iterations, optimize algorithm parameters");
//			return;
//		}
//	}
//	transfer(psi1,pt->psi,num);
//	delete[] psi1,psi2;
//}
void fpsiInit(Point* pt,double *y, int num,double kT,double *f)
{	
	double b=pt->affi[0]-pt->affi[num-1]-kT*log(pt->Nd[0]*pt->Nc[num-1]/pt->Nd[num-1]/pt->Nc[0]);
	double Ef=-kT*log(pt->Nc[num-1]/pt->Nd[num-1])-pt->affi[num-1];
	f[0]=y[0]-b;
	for (int i=1;i<num-1;i++)
	{
		if(pt->step[i]!=0&&pt->step[i-1]!=0)
			f[i]=2*pt->eps[i]*EPSON*(y[i-1]/pt->step[i-1]/(pt->step[i]+pt->step[i-1])-y[i]/pt->step[i]/pt->step[i-1]+y[i+1]/pt->step[i]/(pt->step[i]+pt->step[i-1]))
			-QE*(pt->Nd[i]-pt->Nc[i]*exp((Ef+pt->affi[i]-y[i])/kT)+pt->Nv[i]*exp((y[i]-pt->affi[i]-pt->Eg[i]-Ef)/kT)-pt->Na[i]);
			
		else if(0==pt->step[i])
			f[i]=y[i]-y[i+1];
		else
		{
			f[i]=pt->eps[i]*(y[i+1]-y[i])/pt->step[i]-pt->eps[i-1]*(y[i-1]-y[i-2])/pt->step[i-2];
			f[i]*=pt->step[i-2];
		}		
	}
	f[num-1]=y[num-1];
        
}
//--------------J----------------------
void JacoInit(Point* pt,double *y,int num,double kT,double *J)
{
	double b=-kT*log(pt->Nd[0]*pt->Nc[num-1]/pt->Nd[num-1]/pt->Nc[0]);
	double Ef=-kT*log(pt->Nc[num-1]/pt->Nd[num-1])-pt->affi[num-1];
	J[0*num+0]=1;  J[num*num-1]=1;

	for(int i=1;i<num-1;i++)
	{
		if(pt->step[i]!=0&&pt->step[i-1]!=0)
		{
			J[i*num+i-1]=2*pt->eps[i]*EPSON/pt->step[i-1]/(pt->step[i]+pt->step[i-1]);
			J[i*num+i]=-2*pt->eps[i]*EPSON/pt->step[i]/pt->step[i-1]-
				QE/kT*(pt->Nc[i]*exp((Ef+pt->affi[i]-y[i])/kT)+pt->Nv[i]*exp((y[i]-pt->affi[i]-pt->Eg[i]-Ef)/kT));
			J[i*num+i+1]=2*pt->eps[i]*EPSON/pt->step[i]/(pt->step[i]+pt->step[i-1]);
		}
		else if(0==pt->step[i])
		{
			J[i*num+i]=1;
			J[i*num+i+1]=-1;
		}
		else
		{
			J[i*num+i-2]=pt->eps[i-1];
			J[i*num+i-1]=-pt->eps[i-1];
			J[i*num+i]=-pt->step[i-2]*pt->eps[i]/pt->step[i];
			J[i*num+i+1]=pt->step[i-2]*pt->eps[i]/pt->step[i];
		}
	}       
}

//calculate Efn
void  IterN(Point* pt,double *y, double*J,double*l,double*u,int num,double kT,wxProgressDialog *dlg)
{
	double *c=new double[num],  *Efn1=new double[num],*dEfn=new double[num],*p1=new double[num];
	double *f=new double[num];
	
	for(int i=0;i<num*num;i++)
	{
		J[i]=0;
	}
	for(int i=0;i<num;i++)
	{
		p1[i]=pt->Nv[i]*exp((y[i]-pt->affi[i]+pt->affi[num-1]-pt->Eg[i]-pt->Efp[i])/kT);		
		if(pt->step[i]!=0)
		{
			c[i]=QE*pt->un[i]*pt->Nc[i]*exp((pt->affi[i]-pt->affi[num-1])/kT)*(y[i+1]-y[i])/pt->step[i]/(exp(y[i+1]/kT)-exp(y[i]/kT));//?pt->affi			
		}
		else//this point is useless
		{
			c[i]=0;			
		}
	}
	//outEB(x,Efn,c,p1,num);
	//wxString str, title="Fupt->Nction:Calculate Efn\nIteration:";
	double ep=1e-6,tol=1;int cnt=0;//	double range;
	while( tol>ep)
	{
		transfer(pt->Efn,Efn1,num);
		fn(pt,y,Efn1,p1,num,kT,c,f);
		Jacn(pt,y,Efn1,p1,num,kT,c,J);
		LU2(J,f,l,u,num,dEfn);
		clamp(dEfn,0.2,num);
		for(int i=0;i<num;i++)
		{
			pt->Efn[i]=Efn1[i]-dEfn[i];
		}		
		tol=norm(pt->Efn,Efn1,num);
		cnt++;
		/*str.Printf("%d",cnt);str=title+str;
		dlg->Update(cnt%100,str);*/
		if(cnt>500)                                       
		{	
			wxMessageBox(_("too many iterations, optimize algorithm parameters"));
			return;
		}
	}
	delete[] c;delete []Efn1;delete []dEfn;delete []p1;delete[] f;//,J,l,u;
}
void  fn(Point* pt,double *y, double *Efn1, double *p1,
		  int num,double kT,double *c,double *f)
{
	double *n1=new double[num];
	double tn=1e-10,tp=1e-10;//this should be an array	
	double S=1e7;
	for(int i=0;i<num;i++)
	{
		n1[i]=pt->Nc[i]*exp((Efn1[i]+pt->affi[i]-pt->affi[num-1]-y[i])/kT);		
		pt->R[i]=(n1[i]*p1[i]-pt->ni[i]*pt->ni[i])/(tp*(n1[i]+pt->ni[i])+tn*(p1[i]+pt->ni[i]));      
		if(pt->step[i]!=0)
			pt->Jn[i]=c[i]*(exp(Efn1[i+1]/kT)-exp(Efn1[i]/kT));
		else 
			pt->Jn[i]=0;//useless
	}

	f[0]=pt->Jn[0]-QE*S*(pt->Nc[0]*exp((Efn1[0]+pt->affi[0]-pt->affi[num-1]-y[0])/kT)-pt->Nd[0]);//?direction
	for (int i=1;i<num-1;i++)
	{
		if(pt->step[i]!=0&&pt->step[i-1]!=0)
			f[i]=2*(pt->Jn[i]-pt->Jn[i-1])/(pt->step[i]+pt->step[i-1])-QE*pt->R[i]+QE*pt->G[i];
		else if(0==pt->step[i])
			f[i]=Efn1[i+1]-Efn1[i];//diffusion model
		else
			f[i]=2*(pt->Jn[i]-pt->Jn[i-2])/(pt->step[i]+pt->step[i-2])-QE*pt->R[i-1]+QE*pt->G[i-1];//?recombination model
	}
	f[num-1]=pt->Jn[num-2]+QE*S*(pt->Nc[num-1]*exp((Efn1[num-1]-y[num-1])/kT)-pt->Nd[num-1]);

	delete[] n1;
}
void  Jacn(Point* pt,double *y, double *Efn1, double *p1,
		  int num,double kT,double *c,double *J)
{
	double tn=1e-10,tp=1e-10;//this should be an array	
	double S=1e7;double * dR=new double[num],*n1=new double[num];
	for(int i=0;i<num;i++)
	{
		n1[i]=pt->Nc[i]*exp((Efn1[i]+pt->affi[i]-pt->affi[num-1]-y[i])/kT);
		dR[i]=(tp*pt->ni[i]+tn*p1[i])*(p1[i]+pt->ni[i])*n1[i]/pow((tp*(n1[i]+pt->ni[i])+tn*(p1[i]+pt->ni[i])),2);
	}
	J[0*num+0]=(-c[0]*exp(Efn1[0]/kT)-QE*S*pt->Nc[0]*exp((Efn1[0]+pt->affi[0]-pt->affi[num-1]-y[0])/kT))/kT;
	J[0*num+1]=c[0]*exp(Efn1[1]/kT)/kT;
	for(int i=1;i<num-1;i++)
	{
		if(pt->step[i]!=0&&pt->step[i-1]!=0)
		{
			J[i*num+i-1]=2/(pt->step[i]+pt->step[i-1])/kT*c[i-1]*exp(Efn1[i-1]/kT);
			J[i*num+i]=-2/(pt->step[i]+pt->step[i-1])/kT*(c[i]+c[i-1])*exp(Efn1[i]/kT)-QE*dR[i]/kT;
			J[i*num+i+1]=2/(pt->step[i]+pt->step[i-1])/kT*c[i]*exp(Efn1[i+1]/kT);
		}
		else if(0==pt->step[i])
		{
			J[i*num+i]=-1;J[i*num+i+1]=1;
		}
		else
		{
			J[i*num+i-2]=2/(pt->step[i]+pt->step[i-2])/kT*c[i-2]*exp(Efn1[i-2]/kT);
			J[i*num+i-1]=-2/(pt->step[i]+pt->step[i-2])/kT*c[i-2]*exp(Efn1[i-1]/kT)-QE*dR[i-1]/kT;
			J[i*num+i]=-2/(pt->step[i]+pt->step[i-2])/kT*c[i]*exp(Efn1[i]/kT);
			J[i*num+i+1]=2/(pt->step[i]+pt->step[i-2])/kT*c[i]*exp(Efn1[i+1]/kT);
		}
	}
	J[num*num-2]=-c[num-2]*exp(Efn1[num-2]/kT)/kT;
	J[num*num-1]=c[num-2]*exp(Efn1[num-1]/kT)/kT+QE*S*pt->Nc[num-1]*exp((Efn1[num-1]-y[num-1])/kT)/kT;

	delete[] dR;
	delete[] n1;
}
//------------------------calculate Efp--------------------------------


//---------------------------------------------------------------------
void  IterP(Point* pt,double *y, double*J,double*l,double*u, int num,double kT,wxProgressDialog *dlg)
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
		n1[i]=pt->Nc[i]*exp((pt->Efn[i]+pt->affi[i]-pt->affi[num-1]-y[i])/kT);
		if(pt->step[i]!=0)
			c[i]=QE*pt->up[i]*pt->Nv[i]*exp((pt->affi[num-1]-pt->affi[i]-pt->Eg[i])/kT)*(y[i+1]-y[i])/pt->step[i]/(exp(-y[i+1]/kT)-exp(-y[i]/kT));		
		else
			c[i]=0;			
		
	}
	
	//wxString str, title="Fupt->Nction:Calculate Efp\nIteration:";
	double ep=1e-6,tol=1;int cnt=0;//	double range;
	while( tol>ep)
	{
		transfer(pt->Efp,Efp1,num);
		fp(pt,y,Efp1,n1,num,kT,c,f);		
		Jacp(pt,y,Efp1,n1,num,kT,c,J);
		LU2(J,f,l,u,num,dEfp);
		//range=Efn1[num-1]-Efn1[0];
		clamp(dEfp,0.2,num);
		for(int i=0;i<num;i++)
		{
			pt->Efp[i]=Efp1[i]-dEfp[i];
		}		
		tol=norm(pt->Efp,Efp1,num);
		cnt++;
		/*str.Printf("%d",cnt);str=title+str;
		dlg->Update(cnt%100,str);*/
		if(cnt>500)                                       
		{	
			wxMessageBox(_("too many iterations, optimize algorithm parameters"));
			return;
		}
	}
	delete[] c;delete []Efp1;delete []dEfp;delete []n1;delete[] f;//delete[] J,l,u;
}
void  fp(Point* pt,double *y, double *Efp1, double *n1,
		  int num,double kT,double *c,double *f)
{
	double *p1=new double[num];
	double tn=1e-10,tp=1e-10;//this should be an array	
	double S=1e7;
	for(int i=0;i<num;i++)
	{
		p1[i]=pt->Nv[i]*exp((y[i]-pt->affi[i]+pt->affi[num-1]-pt->Eg[i]-Efp1[i])/kT);		
		pt->R[i]=(n1[i]*p1[i]-pt->ni[i]*pt->ni[i])/(tp*(n1[i]+pt->ni[i])+tn*(p1[i]+pt->ni[i]));      
		if(pt->step[i]!=0)
			pt->Jp[i]=c[i]*(exp(-Efp1[i+1]/kT)-exp(-Efp1[i]/kT));
		else 
			pt->Jp[i]=0;//useless
	}

	f[0]=pt->Jp[0]+QE*S*(pt->Nv[0]*exp((y[0]-pt->affi[0]+pt->affi[num-1]-pt->Eg[0]-Efp1[0])/kT)-pt->Na[0]);//?direction
	for (int i=1;i<num-1;i++)
	{
		if(pt->step[i]!=0&&pt->step[i-1]!=0)
			f[i]=2*(pt->Jp[i]-pt->Jp[i-1])/(pt->step[i]+pt->step[i-1])+QE*pt->R[i]-QE*pt->G[i];
		else if(0==pt->step[i])
			f[i]=Efp1[i+1]-Efp1[i];//diffusion model
		else
			f[i]=2*(pt->Jp[i]-pt->Jp[i-2])/(pt->step[i]+pt->step[i-2])+QE*pt->R[i-1]-QE*pt->G[i-1];//?recombination model
	}
	f[num-1]=pt->Jp[num-2]-QE*S*(pt->Nv[num-1]*exp((y[num-1]-pt->Eg[num-1]-Efp1[num-1])/kT)-pt->Na[num-1]);

	delete[] p1;
}
void  Jacp(Point* pt,double *y, double *Efp1, double *n1,
		  int num,double kT,double *c,double *J)
{
	double tn=1e-10,tp=1e-10;//this should be an array	
	double S=1e7;double * dR=new double[num],*p1=new double[num];
	for(int i=0;i<num;i++)
	{
		p1[i]=pt->Nv[i]*exp((y[i]-pt->affi[i]+pt->affi[num-1]-pt->Eg[i]-Efp1[i])/kT);		
		dR[i]=(tn*pt->ni[i]+tp*n1[i])*(n1[i]+pt->ni[i])*(-p1[i])/pow((tp*(n1[i]+pt->ni[i])+tn*(p1[i]+pt->ni[i])),2);
	}
	J[0*num+0]=(c[0]*exp(-Efp1[0]/kT)-QE*S*pt->Nv[0]*exp((y[0]-pt->affi[0]+pt->affi[num-1]-pt->Eg[0]-Efp1[0])/kT))/kT;
	J[0*num+1]=-c[0]*exp(-Efp1[1]/kT)/kT;
	for(int i=1;i<num-1;i++)
	{
		if(pt->step[i]!=0&&pt->step[i-1]!=0)
		{
			J[i*num+i-1]=-2/(pt->step[i]+pt->step[i-1])/kT*c[i-1]*exp(-Efp1[i-1]/kT);
			J[i*num+i]=2/(pt->step[i]+pt->step[i-1])/kT*(c[i]+c[i-1])*exp(-Efp1[i]/kT)+QE*dR[i]/kT;
			J[i*num+i+1]=-2/(pt->step[i]+pt->step[i-1])/kT*c[i]*exp(-Efp1[i+1]/kT);
		}
		else if(0==pt->step[i])
		{
			J[i*num+i]=-1;J[i*num+i+1]=1;
		}
		else
		{
			J[i*num+i-2]=-2/(pt->step[i]+pt->step[i-2])/kT*c[i-2]*exp(-Efp1[i-2]/kT);
			J[i*num+i-1]=2/(pt->step[i]+pt->step[i-2])/kT*c[i-2]*exp(-Efp1[i-1]/kT)+QE*dR[i-1]/kT;
			J[i*num+i]=2/(pt->step[i]+pt->step[i-2])/kT*c[i]*exp(-Efp1[i]/kT);
			J[i*num+i+1]=-2/(pt->step[i]+pt->step[i-2])/kT*c[i]*exp(-Efp1[i+1]/kT);
		}
	}
	J[num*num-2]=c[num-2]*exp(-Efp1[num-2]/kT)/kT;
	J[num*num-1]=-c[num-2]*exp(-Efp1[num-1]/kT)/kT+QE*S*pt->Nv[num-1]*exp((y[num-1]-pt->Eg[num-1]-Efp1[num-1])/kT)/kT;

	delete[] dR; delete[] p1;
}
//calculate psi
void  IterPsi(Point* pt,double v,double *y, double*J,double*l,double*u,int num,double kT,wxProgressDialog *dlg)
{	
	double *y1=new double[num];double *dy=new double[num];
	double *f=new double[num];
//	double *J=new double[num*num];
	for(int i=0;i<num*num;i++)
	{
		J[i]=0;
	}
	//wxString str, title="Fupt->Nction:fpsi\nIteration:";

	double ep=1e-6,tol=1;int cnt=0;			
	while( tol>ep)
	{
		transfer(y,y1,num);
		fpsi(pt,v,y1,num,kT,f);
		Jpsi(pt,y1,num,kT,J);
		LU2(J,f,l,u,num,dy);
		clamp(dy,0.2,num);
		for(int i=0;i<num;i++)
		{
			y[i]=y1[i]-dy[i];
		}
		tol=norm(y1,y,num);

		cnt++;
		/*str.Printf("%d",cnt);str=title+str;
		if(!dlg->Update(cnt%100,str))
			return;*/
		if(cnt>500)                                       
		{	
			wxMessageBox(_("too many iterations, optimize algorithm parameters"));
			return;
		}
	}   
	delete[] y1;delete []dy;delete[] f;// delete[] J;
}
void fpsi(Point* pt,double v,double *y,int num,double kT,double *f)
{
	double * n1=new double[num], *p1=new double[num];
	for(int i=0;i<num;i++)
	{
		p1[i]=pt->Nv[i]*exp((y[i]-pt->affi[i]+pt->affi[num-1]-pt->Eg[i]-pt->Efp[i])/kT);
		n1[i]=pt->Nc[i]*exp((pt->Efn[i]+pt->affi[i]-pt->affi[num-1]-y[i])/kT);		
	}
	double b=pt->affi[0]-pt->affi[num-1]-kT*log(pt->Nd[0]*pt->Nc[num-1]/pt->Nd[num-1]/pt->Nc[0])-v;
	f[0]=y[0]-b;
	for (int i=1;i<num-1;i++)
	{
		if(pt->step[i]!=0&&pt->step[i-1]!=0)
			f[i]=2*pt->eps[i]*EPSON*(y[i-1]/pt->step[i-1]/(pt->step[i]+pt->step[i-1])-y[i]/pt->step[i]/pt->step[i-1]+y[i+1]/pt->step[i]/(pt->step[i]+pt->step[i-1]))
			-QE*(pt->Nd[i]-n1[i]+p1[i]-pt->Na[i]);
			
		else if(0==pt->step[i])
			f[i]=y[i]-y[i+1];
		else
		{
			f[i]=pt->eps[i]*(y[i+1]-y[i])/pt->step[i]-pt->eps[i-1]*(y[i-1]-y[i-2])/pt->step[i-2];
			f[i]*=pt->step[i-2];
		}		
	}
	f[num-1]=y[num-1];
	delete []n1; delete []p1;
}
void Jpsi(Point* pt,double *y,int num,double kT,double *J)
{
	double * n1=new double[num], *p1=new double[num];
	for(int i=0;i<num;i++)
	{
		p1[i]=pt->Nv[i]*exp((y[i]-pt->affi[i]+pt->affi[num-1]-pt->Eg[i]-pt->Efp[i])/kT);
		n1[i]=pt->Nc[i]*exp((pt->Efn[i]+pt->affi[i]-pt->affi[num-1]-y[i])/kT);		
	}
	//double b=-kT*log(pt->Nd[0]*pt->Nc[num-1]/pt->Nd[num-1]/pt->Nc[0]);
	J[0*num+0]=1;  J[num*num-1]=1;
	for(int i=1;i<num-1;i++)
	{
		if(pt->step[i]!=0&&pt->step[i-1]!=0)
		{
			J[i*num+i-1]=2*pt->eps[i]*EPSON/pt->step[i-1]/(pt->step[i]+pt->step[i-1]);
			J[i*num+i]=-2*pt->eps[i]*EPSON/pt->step[i]/pt->step[i-1]-
				QE/kT*(n1[i]+p1[i]);
			J[i*num+i+1]=2*pt->eps[i]*EPSON/pt->step[i]/(pt->step[i]+pt->step[i-1]);
		}
		else if(0==pt->step[i])
		{
			J[i*num+i]=1;
			J[i*num+i+1]=-1;
		}
		else
		{
			J[i*num+i-2]=pt->eps[i-1];
			J[i*num+i-1]=-pt->eps[i-1];
			J[i*num+i]=-pt->step[i-2]*pt->eps[i]/pt->step[i];
			J[i*num+i+1]=pt->step[i-2]*pt->eps[i]/pt->step[i];
		}
	}       
	delete[] n1; delete []p1;
}

