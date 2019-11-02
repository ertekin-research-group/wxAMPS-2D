#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "Point2.h"
#include "calculation.h"
#include "LU.h"
#include "constant.h"
#include <math.h>

Point2::Point2(MyList &LayerList,double kTPar)//:Point(LayerList,kTPar)
{
	kT=kTPar;
	PointNumber=2;bIndex.clear();
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
			grids[i]=int(L*1e3/hCenter);
			q[i]=1;
		}		
		num+=grids[i];//total number		
		bIndex.push_back(num-1);//end index of a layer
	}	
	num++;bIndex[lyrCnt-1]=num;//the end
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
		//	step[index]=0;
			/*curLyr->Epson.ToDouble(&eps[index]);curLyr->Eg.ToDouble(&Eg[index]);curLyr->affi.ToDouble(&affi[index]);
			curLyr->Nc.ToDouble(&Nc[index]);curLyr->Nv.ToDouble(&Nv[index]);curLyr->un.ToDouble(&un[index]);
			curLyr->up.ToDouble(&up[index]);curLyr->n.ToDouble(&Nd[index]);curLyr->p.ToDouble(&Na[index]);
			index++;*/
			start+=L;

			delete[] h;//,step;
		}		
	}
	step[index]=0;
	eps[index]=eps[index-1];Eg[index]=Eg[index-1];affi[index]=affi[index-1];Nc[index]=Nc[index-1];Nv[index]=Nv[index-1];
	un[index]=un[index-1];up[index]=up[index-1];Nd[index]=Nd[index-1];Na[index]=Na[index-1];
	
	delete[] grids;delete[] q;	

	dntdpsi=new double[PointNumber];dntdEfn=new double[PointNumber];dntdEfp=new double[PointNumber];
	dptdpsi=new double[PointNumber];dptdEfn=new double[PointNumber];dptdEfp=new double[PointNumber];
	dNddpsi=new double[PointNumber];dNddEfn=new double[PointNumber];dNddEfp=new double[PointNumber];
	dNadpsi=new double[PointNumber];dNadEfn=new double[PointNumber];dNadEfp=new double[PointNumber];
	dRdpsi=new double[PointNumber];dRdEfn=new double[PointNumber];dRdEfp=new double[PointNumber];
	dRdpsi1=new double[num];dntdpsi1=new double[num];dptdpsi1=new double[num];dNddpsi1=new double[num];dNadpsi1=new double[num];
	dRdpsim1=new double[num];dntdpsim1=new double[num];dptdpsim1=new double[num];dNddpsim1=new double[num];dNadpsim1=new double[num];
	un0=new double [num]; up0=new double [num];
	for(int i=0;i<num;i++)
	{
		double ni2=(Nc[i]*Nv[i]*exp(-Eg[i]/kT));
		ni[i]=sqrt(ni2);

		G[i]=0;R[i]=0;
		Ne[i]=0;Ph[i]=0;Ntd[i]=0;Nta[i]=0;nt[i]=0;pt[i]=0;
		dNd[i]=0;dNa[i]=0;dnt[i]=0;dpt[i]=0;

		dntdpsi[i]=0;dntdEfn[i]=0;dntdEfp[i]=0;
		dptdpsi[i]=0;dptdEfn[i]=0;dptdEfp[i]=0;
		dNddpsi[i]=0;dNddEfn[i]=0;dNddEfp[i]=0;
		dNadpsi[i]=0;dNadEfn[i]=0;dNadEfp[i]=0;
		dRdpsi[i]=0;dRdEfn[i]=0;dRdEfp[i]=0;
		dRdpsi1[i]=0;dntdpsi1[i]=0;dptdpsi1[i]=0;dNddpsi1[i]=0;dNadpsi1[i]=0;
		dRdpsim1[i]=0;dntdpsim1[i]=0;dptdpsim1[i]=0;dNddpsim1[i]=0;dNadpsim1[i]=0;
		Jn[i]=0;Jp[i]=0;
		un0[i]=un[i];up0[i]=up[i];
		//	tunC[i]=0;
	}

	psiOld=new double[PointNumber];EfnOld=new double[PointNumber];EfpOld=new double[PointNumber];//?initail value
}

Point2::~Point2(void)
{
	delete [] dntdpsi;delete []dntdEfn;delete []dntdEfp;
	delete [] dptdpsi;delete []dptdEfn;delete []dptdEfp;
	delete [] dNddpsi;delete []dNddEfn;delete []dNddEfp;
	delete []dNadpsi;delete []dNadEfn;delete []dNadEfp;
	delete [] dRdpsi;delete []dRdEfn;delete []dRdEfp;
	delete []psiOld;delete []EfnOld;delete []EfpOld;
	delete []dRdpsi1;delete [] dntdpsi1;delete [] dptdpsi1;delete [] dNddpsi1; delete [] dNadpsi1;
	delete []un0; delete up0;
}
result * Point2::ThermalInit(bool IsWF,double TopWF,double BtmWF,
					 wxProgressDialog *dlg,int &prgCnt)
{
	double *J=new double[PointNumber*PointNumber];
	for(int i=0;i<PointNumber*PointNumber;i++)
	{
		J[i]=0;
	}
	
	wxString str, title=_("Function:Thermal Equilibrium\nIteration:");
	//------------iteration----------- 
	double ep=1e-6,tol=1;	
	double *y1=new double[PointNumber];double *y2=new double[PointNumber];
	double *f=new double[PointNumber];double *dy=new double[PointNumber];
	int cnt=0;		
	transfer(psiInit,y2,PointNumber);
	while( tol>ep)
	{
		transfer(y2,y1,PointNumber);
		fpsiInit(y1,PointNumber,kT,IsWF,TopWF,BtmWF,f);		
		JacoInit(y1,PointNumber,kT,IsWF,J);
		LU2(J,f,PointNumber,dy);//y=J\f;
		clamp(dy,limit,PointNumber);//jo=find(abs(y)>(y1(end)-y1(1))/10);   %clamping
		for(int i=0;i<PointNumber;i++)
		{
			y2[i]=y1[i]-dy[i];
		}//y2=y1-y;
		tol=norm(y1,y2,PointNumber);//tol=norm(y2-y1);
//		outEB(x,step,y1,dy,f,PointNumber);
		cnt++;
		str.Printf(_("%d"),prgCnt++);str=title+str;
		//if(dlg!=NULL)
		if(!dlg->Update(prgCnt%100,str))
		{
			delete[] y1;delete[] y2;delete[] f;delete[] dy;
			delete[] J;
			return NULL;
		}
		if(cnt>500)                                       
		{	
			wxMessageBox(_("too many iterations, optimize algorithm parameters"));			
			//break;
			return NULL;
		}
	}   
	transfer(y2,psiInit,PointNumber);
			
	double Ef=0;//-kT*log(pt->Nc[PointNumber-1]/pt->Nd[PointNumber-1]);
	for(int i=1;i<PointNumber-1;i++)
			EleF[i]=(psiInit[i+1]-psiInit[i-1])/(step[i]+step[i-1]);	
	EleF[0]=EleF[1];EleF[PointNumber-1]=EleF[PointNumber-2];
	/*for(int i=0;i<PointNumber-1;i++)
			EleF[i]=(psiInit[i+1]-psiInit[i])/(step[i]);	
	EleF[PointNumber-1]=EleF[PointNumber-2];*/
	for(int i=0;i<PointNumber;i++)
	{
		psi[i]=psiInit[i];
		Efn[i]=0;
		Efp[i]=0;
		psiOld[i]=psi[i];EfnOld[i]=0;EfpOld[i]=0;	
		if(fabs(EleF[i])>1e4)//TAT tunneling
		{
			un[i]=un0[i]*exp(fabs(EleF[i])/F0);up[i]=up0[i]*exp(fabs(EleF[i])/F0);
		}
	}	

		n0L=Nc[0]*exp((-psiInit[0]+affi[0])/kT);n0R=Nc[PointNumber-1]*exp((-psiInit[PointNumber-1]+affi[PointNumber-1])/kT);
		p0L=Nv[0]*exp((psiInit[0]-affi[0]-Eg[0])/kT);p0R=Nv[PointNumber-1]*exp((psiInit[PointNumber-1]-affi[PointNumber-1]-Eg[PointNumber-1])/kT);		
		/*n0L=Neta(Nc[0],(-psiInit[0]+affi[0])/kT);
		n0R=Neta(Nc[PointNumber-1],(-psiInit[PointNumber-1]+affi[PointNumber-1])/kT);
		p0L=Neta(Nv[0],(psiInit[0]-affi[0]-Eg[0])/kT);
		p0R=Neta(Nv[PointNumber-1],(psiInit[PointNumber-1]-affi[PointNumber-1]-Eg[PointNumber-1])/kT);*/
	
	//----------------postprocess----------------	
	result *curRes=new result;
	curRes->v=0;
	for(int i=0;i<PointNumber;i++)
	{
		Ec[i]=psi[i]-affi[i];//+affi[PointNumber-1];
		Ev[i]=Ec[i]-Eg[i];
		curRes->x.push_back(x[i]*1e4);
		curRes->Ec.push_back(Ec[i]);curRes->Ev.push_back(Ev[i]);
		curRes->Efn.push_back(Efn[i]);curRes->Efp.push_back(Efp[i]);
		curRes->n.push_back(Ne[i]);curRes->p.push_back(Ph[i]);//log
		curRes->nt.push_back(nt[i]);curRes->pt.push_back(pt[i]);
		curRes->Ntd.push_back(Ntd[i]);curRes->Nta.push_back(Nta[i]);
		curRes->R.push_back(R[i]);curRes->G.push_back(G[i]);
		curRes->Jn.push_back(Jn[i]);curRes->Jp.push_back(Jp[i]);
		curRes->EleF.push_back(EleF[i]);
	}
//	outEB(x,psi,EleF,EleF,EleF,PointNumber);
	delete[] y1;delete[] y2;delete[] f;delete[] dy;
	//------------------------
	delete[] J;
	return curRes;
}
void Point2::fpsiInit(double *y, int num,double kT,bool IsWF,double TopWF,double BtmWF,double *f)
{	
	double Ef=0;//-kT*log(Nc[num-1]/n[num-1])-affi[num-1];
	
	//double Ne,Ph;	//free electrons and holes, Fermi-Dirac distribution
	for(int i=0;i<num;i++)
	{
		Ne[i]=Nc[i]*exp((Ef+affi[i]-y[i])/kT);//*GP12((affi[i]-y[i])/kT);
		Ph[i]=Nv[i]*exp((y[i]-affi[i]-Eg[i]-Ef)/kT);//*GP12((y[i]-affi[i]-Eg[i])/kT);
	}
	//if(zeroK)
	//	DefChargeEqui(y,num,kT);//calculate nt,pt,dntdpsi,dptdpsi
	
		DefChargeNon(Ne,Ph,num,kT);
	//Ne[0]=/*GP12((affi[0]-y[0])/kT)**/Nc[0]*exp((affi[0]-y[0])/kT);
	//Ph[0]=/*GP12((y[0]-affi[0]-Eg[0])/kT)**/Nv[0]*exp((y[0]-affi[0]-Eg[0])/kT);
	if(!IsWF)
		f[0]=QE*(Nd[0]-Na[0]-Ne[0]+Ph[0]+pt[0]-nt[0]);
	else
		f[0]=y[0]-affi[0]-TopWF;
	for (int i=1;i<num-1;i++)
	{
	//	if(step[i]!=0&&step[i-1]!=0)
		{
			//Ne[i]=Nc[i]*exp((Ef+affi[i]-y[i])/kT);//*GP12((affi[i]-y[i])/kT);
			//Ph[i]=Nv[i]*exp((y[i]-affi[i]-Eg[i]-Ef)/kT);//*GP12((y[i]-affi[i]-Eg[i])/kT);
			double a1,a2;
			a1=4*eps[i]*eps[i-1]/(eps[i-1]+eps[i])*EPSON/step[i-1]/(step[i]+step[i-1]);
			a2=4*eps[i]*eps[i+1]/(eps[i+1]+eps[i])*EPSON/step[i]/(step[i]+step[i-1]);

			//f[i]=2*eps[i]*EPSON*(y[i-1]/step[i-1]/(step[i]+step[i-1])-y[i]/step[i]/step[i-1]+y[i+1]/step[i]/(step[i]+step[i-1]))
			f[i]=a1*y[i-1]-(a1+a2)*y[i]+a2*y[i+1]
			-QE*(Nd[i]-Na[i]+Ph[i]-Ne[i]+pt[i]-nt[i]);
		}	
	/*	else if(0==step[i])
			f[i]=y[i]-y[i+1];
		else
		{
			f[i]=eps[i]*(y[i+1]-y[i])/step[i]-eps[i-1]*(y[i-1]-y[i-2])/step[i-2];
			f[i]*=step[i-2];
		}		*/
	}
	Ph[num-1]=Nv[num-1]*exp((y[num-1]-affi[num-1]-Eg[num-1])/kT);//*GP12((y[num-1]-affi[num-1]-Eg[num-1])/kT);
	Ne[num-1]=Nc[num-1]*exp((affi[num-1]-y[num-1])/kT);//*GP12((affi[num-1]-y[num-1])/kT);
	if(!IsWF)
		f[num-1]=QE*(Nd[num-1]-Na[num-1]+Ph[num-1]-Ne[num-1]+pt[num-1]-nt[num-1]);
	else
		f[num-1]=y[num-1]-affi[num-1]-BtmWF;
        
}
//--------------J----------------------
void Point2::JacoInit(double *y,int num,double kT,bool IsWF,double *J)
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
	//	if(step[i]!=0&&step[i-1]!=0)
		{
			double a1,a2;
			a1=4*eps[i]*eps[i-1]/(eps[i-1]+eps[i])*EPSON/step[i-1]/(step[i]+step[i-1]);
			a2=4*eps[i]*eps[i+1]/(eps[i+1]+eps[i])*EPSON/step[i]/(step[i]+step[i-1]);

			J[i*num+i-1]=a1;
			dPh=Nv[i]*exp((y[i]-affi[i]-Eg[i]-Ef)/kT)/kT;//*GM12((y[i]-affi[i]-Eg[i]-Ef)/kT);
			dNe=-Nc[i]*exp((Ef+affi[i]-y[i])/kT)/kT;//*GM12((affi[i]-y[i])/kT);
			J[i*num+i]=-(a1+a2)-
				QE*(dPh-dNe+dNd[i]-dNa[i]+dpt[i]-dnt[i]);
			J[i*num+i+1]=a2;
		}
	/*	else if(0==step[i])
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
		}*/
	}       
}
const double EgApp=0.5;
void Point2::DefChargeEqui(double*y,int num, double kT)
{
	int rgn=0;double N, Et,width;int type, dist;	
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
		if(i==bIndex[rgn])
		{
			rgn++;
			node=layerListP->Item(rgn);	curLyr=node->GetData();	defCnt=curLyr->defList.GetCount();					
		}
	}
	
}
extern double xStDv[MGAUSS+1];
extern double xProb[MGAUSS];
void Point2::DefChargeNon(double *n,double*p,int num,double kT)
{
	int rgn=0;double N,Et,width,sigP,sigN;int type,dist;
	double Vth=sqrt(3.8682e15 * kT);//m*=0.45me
	double Gdo,Ed,VBTSigN,VBTSigP,Gao,Ea,CBTSigN,CBTSigP; double Eda, GMgD,SigNMGD,SigPMGD,GMgA,SigNMGA,SigPMGA;
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
			double xNd=0,dxNd=0, xNa=0,dxNa=0;double En1,Pe1;
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
		if(i==bIndex[rgn])	
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
void Point2::MemClear(double **y,double **dy,double **y1, double **f,double **a,double **b,double **c,double *cn,double *cp,int num)
{	
	for(int i=0;i<num;i++)
	{
		delete[] y[i];delete []dy[i];delete []f[i];		delete []y1[i];
		delete[] a[i];		delete[] b[i];		delete[] c[i];
	}	
	delete[] a;delete[] b;delete[] c;
	delete[] y;delete[] dy;delete []f;delete []y1;

	delete[] cn;delete[] cp;
}
result* Point2::NonEquiNew(double vOld,double v,
				/*bool IsWF,*/double TopWF,double TopSn,double TopSp,
				double BtmWF,double BtmSn,double BtmSp,wxProgressDialog *dlg,wxString str,int &prgCnt)
{
	//preparation, y[index][psi,fn,fp]
	int num=PointNumber;
	double **y=new double*[num], **dy=new double*[num], **y1=new double*[num], **f=new double*[num];
	for(int i=0;i<num;i++)
	{
		y[i]=new double [3],dy[i]=new double [3], f[i]=new double [3];//variables
		y1[i]=new double[3];
	}
	double dv;
	for(int i=0;i<num;i++)//initail with variables of last iteration
	{
		dv=(1-x[i]/x[num-1])*(v-vOld);
		y[i][0]=psiOld[i]-dv;
		y[i][1]=EfnOld[i]-dv;
		y[i][2]=EfpOld[i]-dv;
	}
//	outEB(x,y,3,num);
	//Jacobian matrix
	double **a=new double*[num],**b=new double*[num],**c=new double*[num];
	for(int i=0;i<num;i++)
	{
		a[i]=new double[9];		b[i]=new double[9];		c[i]=new double[9];
	}
	double *cn=new double[num], *cp=new double[num];
	//----------get minimum of Ec and maximum of Ev--------------	
	//int drtn=EleF[0]>0?1:-1;double EcM=y[0][0]-affi[0], EvM=y[0][0]-affi[0]-Eg[0];
	//int drtn2;juncEdge.clear();EcMin.clear();EvMax.clear();
	//for(int i=1;i<PointNumber-1;i++)
	//{
	//	drtn2=EleF[i]>0?1:-1;
	//	if(drtn2!=drtn&&fabs(EleF[i])>10)//? how about heterojunction
	//	{
	//		juncEdge.push_back(i);
	//		if(drtn>0)
	//			EvM=y[i][0]-affi[i]-Eg[i];
	//		else
	//			EcM=y[i][0]-affi[i];
	//					
	//		EcMin.push_back(EcM);EvMax.push_back(EvM);
	//		EcM=y[i][0]-affi[i];EvM=y[i][0]-affi[i]-Eg[i];
	//		drtn=drtn2;
	//	}		
	//}
	//if(drtn>0)
	//	EvM=y[PointNumber-1][0]-affi[PointNumber-1]-Eg[PointNumber-1];
	//else
	//	EcM=y[PointNumber-1][0]-affi[PointNumber-1];
	//juncEdge.push_back(PointNumber);EcMin.push_back(EcM);EvMax.push_back(EvM);
	//--------field&Mobility------
	//if(v!=vOld)
	/*{for(int i=1;i<PointNumber-1;i++)
			EleF[i]=(psiInit[i+1]-psiInit[i-1])/(step[i]+step[i-1]);	
	EleF[0]=EleF[1];EleF[PointNumber-1]=EleF[PointNumber-2];}*/
	//for(int i=0;i<PointNumber;i++)
	//	if(fabs(EleF[i])>1e4)
	//	{	un[i]=un0[i]*exp(fabs(EleF[i])/2e5);up[i]=up0[i]*exp(fabs(EleF[i])/2e5);}
	//-----iteration----------
	double ep=1e-6,tol=1;int cnt=0;
	ep=precision;
	transfer(y,y1,num);
	while( tol>ep)
	{
		cnt++;	
		fnewton(y1,num,v-vOld,cn,cp,TopSn,TopSp,BtmSn,BtmSp,f);
		Jnewton(y1,num,cn,cp,TopSn,TopSp,BtmSn,BtmSp,a,b,c);
		LUnewton(a,b,c,f,num,dy);
		clamp(dy,limit,num);//clamp(dy[1],0.2,num);clamp(dy[2],0.2,num);
		
		for(int i=0;i<num;i++)
		{	
			y[i][0]=y1[i][0]-dy[i][0];
			y[i][1]=y1[i][1]-dy[i][1];
			y[i][2]=y1[i][2]-dy[i][2];
		}
		tol=norm(dy,num);
		if(_isnan(tol))
		{
#ifdef _DEBUG
			wxMessageBox(_("Numerical error in Newton Method"));
			outEB(x,f,3,"temp/f.tmp",num);outEB(x,a,9,"temp/a.tmp",num);
			outEB(x,b,9,"temp/b.tmp",num);outEB(x,c,9,"temp/c.tmp",num);
#endif
			MemClear(y,dy,y1,f,a,b,c,cn,cp,num);	
			return NULL;
		}
		transfer(y,y1,num);	
		if(!dlg->Update(prgCnt%100,str))
		{
			//wxMessageBox(_("user interrupt!"));
			dlg->Destroy();
			MemClear(y,dy,y1,f,a,b,c,cn,cp,num);	
			return NULL;
			
		}
		if(cnt>iterTimes)   
		{	
			wxString str;
			str.Printf(_("at v=%.4f"),v);str=_("too many iterations, failed in converging ")+str;
			if(vOld!=0&&v!=0)
				wxMessageBox(str);
			MemClear(y,dy,y1,f,a,b,c,cn,cp,num);	
			return NULL;
			
		}
	}
	//------postprocess-------
//	outEB(x,y1,9,num);
	result *curRes=new result;
	curRes->v=v;
	for(int i=0;i<num;i++)
	{
		psi[i]=y[i][0];Efn[i]=y[i][1];Efp[i]=y[i][2];
		psiOld[i]=psi[i];EfnOld[i]=Efn[i];EfpOld[i]=Efp[i];
	}
	ToRes(this,curRes,num);

	MemClear(y,dy,y1,f,a,b,c,cn,cp,num);	

	return curRes;

}
void Point2::fnewton(double** y, int num, double dv,double *cn,double *cp,
					 double TopSn,double TopSp,double BtmSn,double BtmSp,double ** f)
{
	for(int i=0;i<num;i++)//carriers
	{
		Ph[i]=Nv[i]*SafeDExp((y[i][0]-affi[i]-Eg[i]-y[i][2])/kT);//*GP12((y[i]-affi[i]-Eg[i]-Efp[i])/kT);
		Ne[i]=Nc[i]*SafeDExp((y[i][1]+affi[i]-y[i][0])/kT);//*GP12((Efn[i]+affi[i]-y[i])/kT);		
	}
	for(int i=1;i<PointNumber-1;i++)
			EleF[i]=(y[i+1][0]-y[i-1][0])/(step[i]+step[i-1]);	
	EleF[0]=EleF[1];EleF[PointNumber-1]=EleF[PointNumber-2];
	DefectNon(y,Ne,Ph,num,kT);	

	for(int i=0;i<num-1;i++)//currents
	{
		//if(step[i]!=0)
		{
			cn[i]=QE*(un[i]+un[i+1])/2*sqrt(Nc[i]*Nc[i+1])/step[i]//*SafeDExp(-(y[i][0]-affi[i])/kT)
				*kT*xDexM1((y[i+1][0]-affi[i+1]-y[i][0]+affi[i])/kT);				
				//*(y[i+1][0]-affi[i+1]-y[i][0]+affi[i])/(exp((y[i+1][0]-affi[i+1])/kT)-exp((y[i][0]-affi[i])/kT));//?high round error
			cp[i]=QE*(up[i]+up[i+1])/2*sqrt(Nv[i]*Nv[i+1])/step[i]*kT//*SafeDExp((y[i][0]-affi[i]-Eg[i])/kT)
				*(-xDexM1((-y[i+1][0]+affi[i+1]+Eg[i+1]+y[i][0]-affi[i]-Eg[i])/kT));			
				//*(y[i+1][0]-affi[i+1]-Eg[i+1]-y[i][0]+affi[i]+Eg[i])/(exp((-y[i+1][0]+affi[i+1]+Eg[i+1])/kT)-exp((-y[i][0]+affi[i]+Eg[i])/kT));	
		/*	if(fabs(EleF[i])>1e5)
			{cn[i]*=exp(fabs(EleF[i])/2e5);	cp[i]*=exp(fabs(EleF[i])/2e5);}*/
			Jn[i]=cn[i]*SafeDExp((y[i][1]-y[i][0]+affi[i])/kT)*(SafeDExp((y[i+1][1]-y[i][1])/kT)-1);
			Jp[i]=cp[i]*SafeDExp((y[i][0]-affi[i]-Eg[i]-y[i][2])/kT)*(SafeDExp((y[i][2]-y[i+1][2])/kT)-1);
		}		
	}
	//this point is useless
	{
		cn[num-1]=cn[num-2];	cp[num-1]=cp[num-2];	
		Jn[num-1]=Jn[num-2];Jp[num-1]=Jp[num-2];
	}
	
	f[0][0]=y[0][0]-(psiOld[0]-dv);
	f[0][1]=Jn[0]-QE*TopSn*(Nc[0]*SafeDExp((y[0][1]+affi[0]-y[0][0])/kT)-n0L);f[0][1]/=step[0];
	f[0][2]=Jp[0]+QE*TopSp*(Nv[0]*SafeDExp((y[0][0]-affi[0]-Eg[0]-y[0][2])/kT)-p0L);	f[0][2]/=step[0];
	
	for(int i=1;i<num-1;i++)
	{
		//if(step[i]!=0&&step[i-1]!=0)
		{
			double a1,a2;
			a1=4*eps[i]*eps[i-1]/(eps[i-1]+eps[i])*EPSON/step[i-1]/(step[i]+step[i-1]);
			a2=4*eps[i]*eps[i+1]/(eps[i+1]+eps[i])*EPSON/step[i]/(step[i]+step[i-1]);
			f[i][0]=a1*y[i-1][0]-(a1+a2)*y[i][0]+a2*y[i+1][0]-QE*(Nd[i]-Ne[i]+Ph[i]-Na[i]+pt[i]-nt[i]);
			f[i][1]=2*(Jn[i]-Jn[i-1])/(step[i]+step[i-1])-QE*R[i]+QE*G[i];
			f[i][2]=2*(Jp[i]-Jp[i-1])/(step[i]+step[i-1])+QE*R[i]-QE*G[i];
		}
	
	}
	f[num-1][0]=y[num-1][0]-psiOld[num-1];
	f[num-1][1]=Jn[num-2]+QE*BtmSn*(Nc[num-1]*SafeDExp((y[num-1][1]-y[num-1][0]+affi[num-1])/kT)-n0R);f[num-1][1]/=step[num-2];
	f[num-1][2]=Jp[num-2]-QE*BtmSp*(Nv[num-1]*SafeDExp((y[num-1][0]-Eg[num-1]-y[num-1][2]-affi[num-1])/kT)-p0R);f[num-1][2]/=step[num-2];

}
void Point2::Jnewton(double** y, int num, double *cn,double *cp,
					 double TopSn,double TopSp,double BtmSn,double BtmSp,
					 double ** a,double **b,double **c)
{
	double dcndpsi1,dcndpsi,dcpdpsi1,dcpdpsi,dFdpsi,dFdpsi1,dF1dpsi,dF1dpsi1;// dcndpsi=dcndpsi1(-x)=-1/kT-dcndpsi1(x); dJpdpsi1=1/kT+df1=-df, dJpdpsi=-df1(x)=1/kT+df(x)=1/kT-dJpdpsi1
	//a[0][] not defined
	dcndpsi1=Focc4(kT,y[1][0]-affi[1]-y[0][0]+affi[0]);dcndpsi=-1/kT-dcndpsi1;
	dcpdpsi1=1/kT+Focc4(kT,y[1][0]-affi[1]-Eg[1]-y[0][0]+affi[0]+Eg[0]);dcpdpsi=1/kT-dcpdpsi1;
	b[0][0]=1;b[0][1]=0;b[0][2]=0;b[0][5]=0;b[0][7]=0;		
	//b[0][3]=Jn[0]*dcndpsi+QE*TopSn*Nc[0]*SafeDExp((y[0][1]+affi[0]-y[0][0])/kT)/kT;	
	/*if(EleF[0]>1e5)	dFdpsi=-1/step[0]/2e5;
	else if(EleF[0]<-1e5)		dFdpsi=1/step[0]/2e5;
	else	*/			
		dFdpsi=0;
	dFdpsi1=-dFdpsi;
	
	b[0][3]=Jn[0]*(dcndpsi+dFdpsi)+QE*TopSn*Nc[0]*SafeDExp((y[0][1]+affi[0]-y[0][0])/kT)/kT;			b[0][3]/=step[0];
	b[0][4]=-cn[0]*SafeDExp((y[0][1]-y[0][0]+affi[0])/kT)/kT-QE*TopSn*Nc[0]*SafeDExp((y[0][1]+affi[0]-y[0][0])/kT)/kT;b[0][4]/=step[0];
	//b[0][6]=Jp[0]*dcpdpsi+QE*TopSp*Nv[0]*SafeDExp((y[0][0]-affi[0]-Eg[0]-y[0][2])/kT)/kT;
	b[0][6]=Jp[0]*(dcpdpsi+dFdpsi)+QE*TopSp*Nv[0]*SafeDExp((y[0][0]-affi[0]-Eg[0]-y[0][2])/kT)/kT;	b[0][6]/=step[0];
	b[0][8]=cp[0]*SafeDExp((y[0][0]-affi[0]-Eg[0]-y[0][2])/kT)/kT-QE*TopSp*Nv[0]*SafeDExp((y[0][0]-affi[0]-Eg[0]-y[0][2])/kT)/kT;b[0][8]/=step[0];

	c[0][0]=0;c[0][1]=0;c[0][2]=0;c[0][5]=0;c[0][7]=0;
	//c[0][3]=Jn[0]*(dcndpsi1+1/step[0]/2e5);	c[0][3]/=step[0];
	c[0][3]=Jn[0]*(dcndpsi1+dFdpsi1);	c[0][3]/=step[0];
	c[0][4]=cn[0]*SafeDExp((y[1][1]-y[0][0]+affi[0])/kT)/kT;c[0][4]/=step[0];
	//c[0][6]=Jp[0]*(dcpdpsi1+1/step[0]/2e5);	c[0][6]/=step[0];
	c[0][6]=Jp[0]*(dcpdpsi1+dFdpsi1);	c[0][6]/=step[0];
	c[0][8]=-cp[0]*SafeDExp((y[0][0]-affi[0]-Eg[0]-y[1][2])/kT)/kT;c[0][8]/=step[0];

	for(int i=1;i<num-1;i++)
	{	
		double dcni,dcni1,dcpi,dcpi1;
	//	if(step[i]!=0)
		{//node i-1
			double a1,a2;
			a1=4*eps[i]*eps[i-1]/(eps[i-1]+eps[i])*EPSON/step[i-1]/(step[i]+step[i-1]);
			a2=4*eps[i]*eps[i+1]/(eps[i+1]+eps[i])*EPSON/step[i]/(step[i]+step[i-1]);
			dcni=Focc4(kT,-(y[i][0]-affi[i]-y[i-1][0]+affi[i-1]));//dcn_i-1/dpsi_i-1
			dcpi=1/kT+Focc4(kT,-(y[i][0]-affi[i]-Eg[i]-y[i-1][0]+affi[i-1]+Eg[i-1]));
			a[i][0]=a1-QE*(dNddpsim1[i]-dNadpsim1[i]+dptdpsim1[i]-dntdpsim1[i]);a[i][1]=0;a[i][2]=0;
		/*	if(EleF[i-1]>1e5)		dFdpsi=-1/step[i-1]/2e5;	
			else if(EleF[i-1]<-1e5)	dFdpsi=1/step[i-1]/2e5;	
			else */
				dFdpsi=0;
			/*if(EleF[i]>1e5)			dF1dpsi=-1/step[i]/2e5;
			else if(EleF[i]<-1e5)	dF1dpsi=1/step[i]/2e5;
			else */
				dF1dpsi=0;
			dFdpsi1=-dFdpsi; dF1dpsi1=-dF1dpsi;

			a[i][3]=-2/(step[i]+step[i-1])*Jn[i-1]*(dcni+dFdpsi)-QE*dRdpsim1[i];//mobility enhance
			a[i][4]=2/(step[i]+step[i-1])*cn[i-1]*SafeDExp((y[i-1][1]-y[i-1][0]+affi[i-1])/kT)/kT;a[i][5]=0;
			a[i][6]=-2/(step[i]+step[i-1])*Jp[i-1]*(dcpi+dFdpsi)+QE*dRdpsim1[i];
			a[i][7]=0;a[i][8]=-2/(step[i]+step[i-1])*cp[i-1]*SafeDExp((y[i-1][0]-affi[i-1]-Eg[i-1]-y[i-1][2])/kT)/kT;
		//node i+1
			dcni1=Focc4(kT,y[i+1][0]-affi[i+1]-y[i][0]+affi[i]);//dcn_i/dpsi_i+1
			dcpi1=1/kT+Focc4(kT,y[i+1][0]-affi[i+1]-Eg[i+1]-y[i][0]+affi[i]+Eg[i]);
			c[i][0]=a2-QE*(dNddpsi1[i]-dNadpsi1[i]+dptdpsi1[i]-dntdpsi1[i]);
			c[i][1]=0;c[i][2]=0;
			c[i][3]=2/(step[i]+step[i-1])*Jn[i]*(dcni1+dF1dpsi1)-QE*dRdpsi1[i];
			c[i][4]=2/(step[i]+step[i-1])*cn[i]*SafeDExp((y[i+1][1]-y[i][0]+affi[i])/kT)/kT;c[i][5]=0;
			c[i][6]=2/(step[i]+step[i-1])*Jp[i]*(dcpi1+dF1dpsi1)+QE*dRdpsi1[i];			
			c[i][7]=0;c[i][8]=-2/(step[i]+step[i-1])*cp[i]*SafeDExp((y[i][0]-affi[i]-Eg[i]-y[i+1][2])/kT)/kT;
		//node i
			b[i][0]=-a1-a2
			-QE*(Nv[i]*SafeDExp((y[i][0]-affi[i]-Eg[i]-y[i][2])/kT)/kT+Nc[i]*SafeDExp((y[i][1]+affi[i]-y[i][0])/kT)/kT+dNddpsi[i]-dNadpsi[i]+dptdpsi[i]-dntdpsi[i]);
			b[i][1]=-QE*(-Nc[i]*SafeDExp((y[i][1]+affi[i]-y[i][0])/kT)/kT+dNddEfn[i]-dNadEfn[i]+dptdEfn[i]-dntdEfn[i]);
			b[i][2]=-QE*(-Nv[i]*SafeDExp((y[i][0]-affi[i]-Eg[i]-y[i][2])/kT)/kT+dNddEfp[i]-dNadEfp[i]+dptdEfp[i]-dntdEfp[i]);
			b[i][3]=2/(step[i]+step[i-1])*(Jn[i]*(-1/kT-dcni1+dF1dpsi)-Jn[i-1]*(-1/kT-dcni+dFdpsi1))-QE*dRdpsi[i];
			//b[i][3]=2/(step[i]+step[i-1])*(Jn[i]*(-1/kT-dcni1)-Jn[i-1]*(-1/kT-dcni))-QE*dRdpsi[i];
			b[i][4]=2/(step[i]+step[i-1])*(-cn[i]-cn[i-1]*exp((-y[i-1][0]+affi[i-1]+y[i][0]-affi[i])/kT))*SafeDExp((y[i][1]-y[i][0]+affi[i])/kT)/kT-QE*dRdEfn[i];
			b[i][5]=-QE*dRdEfp[i];
			b[i][6]=2/(step[i]+step[i-1])*(Jp[i]*(1/kT-dcpi1+dF1dpsi)-Jp[i-1]*(1/kT-dcpi+dFdpsi1))+QE*dRdpsi[i];
			//b[i][6]=2/(step[i]+step[i-1])*(Jp[i]*(1/kT-dcpi1)-Jp[i-1]*(1/kT-dcpi))+QE*dRdpsi[i];
			b[i][7]=QE*dRdEfn[i];
			b[i][8]=2/(step[i]+step[i-1])*(cp[i]+cp[i-1]*exp((y[i-1][0]-affi[i-1]-Eg[i-1]-y[i][0]+affi[i]+Eg[i])/kT))
				*SafeDExp((y[i][0]-affi[i]-Eg[i]-y[i][2])/kT)/kT+QE*dRdEfp[i];
		}
		/*else
		{
			a[i][0]=0;a[i][1]=0;a[i][2]=0;a[i][3]=0;a[i][4]=0;a[i][5]=0;a[i][6]=0;a[i][7]=0;a[i][8]=0;
			b[i][0]=-1;b[i][4]=-1;b[i][8]=-1;b[i][1]=0;b[i][2]=0;b[i][3]=0;b[i][5]=0;b[i][6]=0;b[i][7]=0;
			c[i][0]=1;c[i][1]=0;c[i][2]=0;c[i][3]=0;c[i][4]=1;c[i][5]=0;c[i][6]=0;c[i][7]=0;c[i][8]=1;
		}*/
	}
	//c[][num-1] not defined
	dcndpsi1=Focc4(kT,y[num-1][0]-affi[num-1]-y[num-2][0]+affi[num-2]);dcndpsi=-1/kT-dcndpsi1;
	dcpdpsi1=1/kT+Focc4(kT,y[num-1][0]-affi[num-1]-Eg[num-1]-y[num-2][0]+affi[num-2]+Eg[num-2]);dcpdpsi=1/kT-dcpdpsi1;
	/*if(EleF[num-2]>1e5)	dFdpsi=-1/step[num-2]/2e5;
	else if(EleF[num-2]<-1e5)	dFdpsi=1/step[num-2]/2e5;
	else */
		dFdpsi=0;
	dFdpsi1=-dFdpsi;
	a[num-1][0]=0;a[num-1][1]=0;a[num-1][2]=0;a[num-1][5]=0;a[num-1][7]=0;
	a[num-1][3]=Jn[num-2]*(dcndpsi+dFdpsi);a[num-1][3]/=step[num-2];
	a[num-1][4]=-cn[num-2]*SafeDExp((y[num-2][1]-y[num-2][0]+affi[num-2])/kT)/kT;a[num-1][4]/=step[num-2];
	a[num-1][6]=Jp[num-2]*(dcpdpsi+dFdpsi);a[num-1][6]/=step[num-2];
	a[num-1][8]=cp[num-2]*SafeDExp((y[num-2][0]-affi[num-2]-Eg[num-2]-y[num-2][2])/kT)/kT;a[num-1][8]/=step[num-2];

	b[num-1][0]=1;b[num-1][1]=0;b[num-1][2]=0;b[num-1][5]=0;b[num-1][7]=0;
	b[num-1][3]=Jn[num-2]*(dcndpsi1+dFdpsi1)-QE*BtmSn*Nc[num-1]*SafeDExp((y[num-1][1]-y[num-1][0]+affi[num-1])/kT)/kT;b[num-1][3]/=step[num-2];
	b[num-1][4]=cn[num-2]*SafeDExp((y[num-1][1]-y[num-2][0]+affi[num-2])/kT)/kT+QE*BtmSn*Nc[num-1]*SafeDExp((y[num-1][1]-y[num-1][0]+affi[num-1])/kT)/kT;b[num-1][4]/=step[num-2];
	b[num-1][6]=Jp[num-2]*(dcpdpsi1+dFdpsi1)-QE*BtmSp*Nv[num-1]*SafeDExp((y[num-1][0]-Eg[num-1]-y[num-1][2]-affi[num-1])/kT)/kT;b[num-1][6]/=step[num-2];
	b[num-1][8]=-cp[num-2]*SafeDExp((y[num-2][0]-affi[num-2]-Eg[num-2]-y[num-1][2])/kT)/kT+QE*BtmSp*Nv[num-1]*SafeDExp((y[num-1][0]-Eg[num-1]-y[num-1][2]-affi[num-1])/kT)/kT;b[num-1][8]/=step[num-2];

}


extern double xStDv[MGAUSS+1];
extern double xProb[MGAUSS];//gaussian probability

void Point2::DefectNon(double **y, double *n,double*p,int num,double kT)
{
	int rgn=0;double N,Et,width,sigP,sigN;int type,dist;
	double Vth=sqrt(3.8682e15 * kT);//m*=1.08me, Vth=sqrt(8kT/pi/m*)
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
	//double EcM,EvM;
	//int pnRgn=0;EcM=EcMin[pnRgn];EvM=EvMax[pnRgn];
#ifdef _DEBUG
	ofstream file("temp/gamma.tmp");
#endif
	//-----------------calculate R--------------
	for(int i=0;i<num;i++)
	{
		curLyr->n.ToDouble(&N);Nd[i]=N;//initial Nd,Na
		curLyr->p.ToDouble(&N);Na[i]=N;
		Ntd[i]=0;Nta[i]=0;dNddpsi[i]=0;dNadpsi[i]=0;dNddEfn[i]=0;dNadEfn[i]=0;dNddEfp[i]=0;dNadEfp[i]=0;//initial dNd, dNa
		R[i]=0;dRdpsi[i]=0,dRdEfn[i]=0,dRdEfp[i]=0;//initial	
		dRdpsi1[i]=0;dNddpsi1[i]=0;dNadpsi1[i]=0;
		dRdpsim1[i]=0;dNddpsim1[i]=0;dNadpsim1[i]=0;
		double dGndpsi=0,dGndpsi1=0,dGndpsim1=0,dGpdpsi=0,dGpdpsi1=0,dGpdpsim1=0,dGndEfn=0,dGpdEfp=0;
		
		for(int j=0;j<defCnt;j++)
		{			
			defect *curDef=curLyr->defList.Item(j)->GetData();
			type=curDef->type;dist=curDef->dist;
			curDef->den.ToDouble(&N);curDef->el.ToDouble(&Et);curDef->width.ToDouble(&width);
			curDef->sigN.ToDouble(&sigN);curDef->sigP.ToDouble(&sigP);
			double xNd=0,dxNd=0, xNa=0,dxNa=0,dxNdFn=0,dxNdFp=0,dxNaFn=0,dxNaFp=0;
			double xR=0,dxR=0,dxRFn=0,dxRFp=0;
			double GammaN=0,GammaP=0;double dxNd1=0,dxNa1=0,dxR1=0,dxNdm1=0,dxNam1=0,dxRm1=0;bool dEn=1,dEp=1;//TAT
			double delEn,delEp;
			En1 = Nc[i] * exp(-Et/kT);
			Pe1 = Nv[i] * exp((Et-Eg[i])/kT);	
			if(fabs(EleF[i])>1e4&&i>0&&i<num-1)//only in high-field region
			{
				if((y[i][0]-affi[i]-Et)>y[i][1]){ delEn=Et; dEn=0;}	else delEn=y[i][0]-affi[i]-y[i][1];			
				if((y[i][0]-affi[i]-Et)>y[i][2]) delEp=y[i][2]-(y[i][0]-affi[i]-Eg[i]); else {delEp=Eg[i]-Et;dEp=0;}
				/*if((y[i][0]-affi[i]-Et)>EcM){ delEn=Et; dEn=0;}	else delEn=y[i][0]-affi[i]-EcM;			
				if((y[i][0]-affi[i]-Et)>EvM) delEp=EvM-(y[i][0]-affi[i]-Eg[i]); else {delEp=Eg[i]-Et;dEp=0;}*/
				calcGamma(EleF[i],Nc[i],Nv[i],delEn,delEp,step[i]+step[i-1],dEn,dEp,
					GammaN,GammaP,dGndpsi,dGndpsi1,dGndpsim1,dGpdpsi,dGpdpsi1,dGpdpsim1,dGndEfn,dGpdEfp);								
			}
			double dsigNdpsi=sigN*dGndpsi,dsigNdpsi1=sigN*dGndpsi1,dsigNdpsim1=sigN*dGndpsim1,dsigPdpsi=sigP*dGpdpsi,dsigPdpsi1=sigP*dGpdpsi1,dsigPdpsim1=sigP*dGpdpsim1;
			double dsigNdEfn=sigN*dGndEfn,dsigPdEfp=sigP*dGpdEfp;
#ifdef _DEBUG
			file<<i<<" "<<j<<" "<<GammaN<<" "<<GammaP<<" "
				<<dsigNdpsi<<" "<<dsigNdpsi1<<" "<<dsigNdpsim1<<" "<<endl<<dsigPdpsi<<" "<<dsigPdpsi1<<" "<<dsigPdpsim1<<endl;
#endif
			
			if(0==width||0==dist)//discrete
			{				
				//incorporate TAT coefficient
				sigN*=1+GammaN;sigP*=1+GammaP;
				double numer;			
				double denom=sigN*(n[i] + /*GP12n0*/ En1) + sigP*(p[i] + /*GP12p0*/  Pe1);
				double ddenomdpsi=dsigNdpsi*(n[i]+En1)-sigN*n[i]/kT+dsigPdpsi*(p[i]+Pe1)+sigP*p[i]/kT;
				double ddenomdpsi1=dsigNdpsi1*(n[i]+En1)+dsigPdpsi1*(p[i]+Pe1);
				double ddenomdpsim1=dsigNdpsim1*(n[i]+En1)+dsigPdpsim1*(p[i]+Pe1);
				
				switch(type)
				{
				case 0://acceptor
					numer =  sigN*n[i] + sigP * Pe1;
					xNa=N*numer/denom;
					//dxNa=N*(-sigN*n[i]/kT*denom-numer*(-sigN*n[i]+sigP*p[i])/kT)/denom/denom;//nonzero.cpp 
					dxNa=N/denom/denom*((dsigNdpsi*n[i]-sigN*n[i]/kT+dsigPdpsi*Pe1)*denom-numer*ddenomdpsi);
					dxNa1=N/denom/denom*((dsigNdpsi1*n[i]+dsigPdpsi1*Pe1)*denom-numer*ddenomdpsi1);
					dxNam1=N/denom/denom*((dsigNdpsim1*n[i]+dsigPdpsim1*Pe1)*denom-numer*ddenomdpsim1);
					dxNaFn=N*((dsigNdEfn*n[i]+sigN*n[i]/kT)*denom-numer*(dsigNdEfn*(n[i]+En1)+sigN*n[i]/kT))/denom/denom;
					//dxNaFp=N*(numer*sigP*p[i]/kT)/denom/denom;	
					dxNaFp=N*(dsigPdEfp*Pe1*denom-numer*(dsigPdEfp*(p[i]+Pe1)-sigP*p[i]/kT))/denom/denom;
					break;
				case 1:
					numer =sigN*En1 + sigP * p[i];
					xNd=N*numer/denom;
					//dxNd=N*(sigP*p[i]/kT*denom-numer*(-sigN*n[i]+sigP*p[i])/kT)/denom/denom;
					dxNd=N/denom/denom*((dsigNdpsi*En1+dsigPdpsi*p[i]+sigP*p[i]/kT)*denom-numer*ddenomdpsi);
					dxNd1=N/denom/denom*((dsigNdpsi1*En1+dsigPdpsi1*p[i])*denom-numer*ddenomdpsi1);
					dxNdm1=N/denom/denom*((dsigNdpsim1*En1+dsigPdpsim1*p[i])*denom-numer*ddenomdpsim1);
					//dxNdFn=N*(-numer*sigN*n[i]/kT)/denom/denom;
					dxNdFn=N*((dsigNdEfn*En1)*denom-numer*(dsigNdEfn*(n[i]+En1)+sigN*n[i]/kT))/denom/denom;
					//dxNdFp=N*(-sigP*p[i]/kT*(denom-numer))/denom/denom;
					dxNdFp=N*((dsigPdEfp*p[i]-sigP*p[i]/kT)*denom-numer*(dsigPdEfp*(p[i]+Pe1)-sigP*p[i]/kT))/denom/denom;
					break;
				case 2:
					break;
				}
				double npmni2=n[i]*p[i]-ni[i]*ni[i];
				xR=N*sigN*sigP*Vth*npmni2/denom;
				//dxR=N*sigN*sigP*Vth/denom/denom;
				dxRFn=N*Vth/denom/denom*((dsigNdEfn*sigP*npmni2+sigN*sigP*(n[i]*p[i]/kT))*denom
					-sigN*sigP*npmni2*(dsigNdEfn*(n[i]+En1)+sigN*n[i]/kT));//dxR*((n[i]*p[i]/kT)*denom-npmni2*(dsigNdEfn*(n[i]+En1)+sigN*n[i]/kT));
				//dxRFn+=xR/sigN*dsigNdEfn;//?
				dxRFp=N*Vth/denom/denom*((dsigPdEfp*sigN*npmni2+sigN*sigP*(-n[i]*p[i]/kT))*denom
					-sigN*sigP*npmni2*(dsigPdEfp*(p[i]+Pe1)-sigP*p[i]/kT));
					//dxR*((-n[i]*p[i]/kT)*denom-npmni2*(dsigPdEfp*(p[i]+Pe1)-sigP*p[i]/kT));
				//dxRFp+=xR/sigP*dsigPdEfp;
				//dxR*=-(n[i]*p[i]-ni[i]*ni[i])*(-sigN*n[i]+sigP*p[i])/kT;//dR/dpsi, if fermi distribution, dnp/dpsi!=0
				dxR=N*Vth/denom/denom*((dsigNdpsi*sigP*npmni2+sigN*dsigPdpsi*npmni2)*denom-(sigN*sigP*npmni2)*ddenomdpsi);
				dxR1=N*Vth/denom/denom*((dsigNdpsi1*sigP*npmni2+sigN*dsigPdpsi1*npmni2)*denom-(sigN*sigP*npmni2)*ddenomdpsi1);
				dxRm1=N*Vth/denom/denom*((dsigNdpsim1*sigP*npmni2+sigN*dsigPdpsim1*npmni2)*denom-(sigN*sigP*npmni2)*ddenomdpsim1);
			}
			else if(1==dist)
			{
				double E1=Eg[i]-Et-width/2; double E2=Eg[i]-Et+width/2;//Ev as reference
				double gt=N/width;
				double xPt,dxPt,dxPtFn,dxPtFp; double dxPt1,dxPtm1;
				sigN*=1+GammaN;sigP*=1+GammaP;
				calptu(gt,E1,E2,Eg[i],Nc[i],Nv[i],
					sigN,sigP,Vth,kT,n[i],p[i],ni[i]*ni[i],
					dsigNdpsi,dsigNdpsi1,dsigNdpsim1,dsigPdpsi,dsigPdpsi1,dsigPdpsim1,dsigNdEfn,dsigPdEfp,
					xPt,dxPt,dxPtFp,dxPtFn,xR,dxR,dxRFp,dxRFn,
					dxPt1,dxPtm1,dxR1,dxRm1);
				switch(type)
				{
				case 0://acceptor
					xNa=N-xPt;
					dxNa=-dxPt;	dxNaFn=-dxPtFn;	dxNaFp=-dxPtFp;
					dxNa1=-dxPt1;	dxNam1=-dxPtm1;
					break;
				case 1:
					xNd=xPt;
					dxNd=dxPt;dxNdFn=dxPtFn;dxNdFp=dxPtFp;
					dxNd1=dxPt1;	dxNdm1=dxPtm1;
					break;
				case 2:
					break;
				}
#ifdef _DEBUG
				file<<i<<" "<<j<<" "<<dxNa1<<" "<<dxNam1<<" "<<dxNd1<<" "<<dxNdm1<<" "<<dxR1<<" "<<dxRm1<<endl;
#endif
			}
			else//gaussian
			{
				double xN,xWidth,xEt;curDef->sigN.ToDouble(&sigN);curDef->sigP.ToDouble(&sigP);
				for(int k=0;k<MGAUSS;k++)
				//int k=12;
				{			
					xN = N*xProb[k];	
					xEt= Et+width*(xStDv[k]+xStDv[k+1])/2;
					xWidth = width*(xStDv[k+1]-xStDv[k]);
					double E1=Eg[i]-xEt-xWidth/2; double E2=Eg[i]-xEt+xWidth/2;//Ev as reference
					double gt=xN/xWidth; double xGPt,dxGPt,dxGPtFn,dxGPtFp;double  xGR,dxGR,dxGRFn,dxGRFp;
					double dxGPt1,dxGR1,dxGPtm1,dxGRm1;//TAT
					double sig_N=sigN, sig_P=sigP;
					if(i>0&&i<num-1)
						calcSigTAT(y[i][0],affi[i],Eg[i],xEt,Nc[i],Nv[i],EleF[i],step[i]+step[i-1],y[i][1],y[i][2],
						sig_N,sig_P, dGndpsi, dGndpsi1,dGndpsim1, dGpdpsi, dGpdpsi1,dGpdpsim1,dGndEfn,dGpdEfp);
					dsigNdpsi=sigN*dGndpsi,dsigNdpsi1=sigN*dGndpsi1,dsigNdpsim1=sigN*dGndpsim1,dsigPdpsi=sigP*dGpdpsi,dsigPdpsi1=sigP*dGpdpsi1,dsigPdpsim1=sigP*dGpdpsim1;
					dsigNdEfn=sigN*dGndEfn,dsigPdEfp=sigP*dGpdEfp;
					calptu(gt,E1,E2,Eg[i],Nc[i],Nv[i],
						sig_N,sig_P,Vth,kT,n[i],p[i],ni[i]*ni[i],
						dsigNdpsi,dsigNdpsi1,dsigNdpsim1,dsigPdpsi,dsigPdpsi1,dsigPdpsim1,dsigNdEfn,dsigPdEfp,
						xGPt,dxGPt,dxGPtFp,dxGPtFn,xGR,dxGR,dxGRFp,dxGRFn,
						dxGPt1,dxGPtm1,dxGR1,dxGRm1);
//#ifdef _DEBUG
//					file<<i<<" "<<j<<" "<<k<<" "<<GammaN<<" "<<GammaP<<" "<<endl
//						<<dsigNdpsi<<" "<<dsigNdpsi1<<" "<<dsigNdpsim1<<" "<<dsigPdpsi<<" "<<dsigPdpsi1<<" "<<dsigPdpsim1<<endl;
//#endif
					switch(type)
					{
					case 0://acceptor
						xNa+=xN-xGPt;
						dxNa+=-dxGPt;dxNaFn+=-dxGPtFn;	dxNaFp+=-dxGPtFp;
						dxNa1+=-dxGPt1;dxNam1+=-dxGPtm1;
						break;
					case 1:
						xNd+=xGPt;
						dxNd+=dxGPt;dxNdFn+=dxGPtFn;dxNdFp+=dxGPtFp;
						dxNd1+=dxGPt1;dxNdm1+=dxGPtm1;
						break;
					case 2:
						break;
					}
					xR+=xGR;
					dxR+=dxGR;dxRFn+=dxGRFn;dxRFp+=dxGRFp;
					dxR1+=dxGR1;dxRm1+=dxGRm1;
				}
			}
			Ntd[i]+=xNd;dNddpsi[i]+=dxNd;dNddEfn[i]+=dxNdFn;dNddEfp[i]+=dxNdFp;
			Nta[i]+=xNa;dNadpsi[i]+=dxNa;dNadEfn[i]+=dxNaFn;dNadEfp[i]+=dxNaFn;	
			R[i]+=xR;		dRdpsi[i]+=dxR;dRdEfn[i]+=dxRFn;dRdEfp[i]+=dxRFp; 
			dRdpsi1[i]+=dxR1;dNddpsi1[i]+=dxNd1;dNadpsi1[i]+=dxNa1;
			dRdpsim1[i]+=dxRm1;dNddpsim1[i]+=dxNdm1;dNadpsim1[i]+=dxNam1;
#ifdef _DEBUG
//			file<<i<<" "<<j<<" "<<dRdpsi1[i]<<" "<<dNddpsi1[i]<<" "<<dNadpsi1[i]<<endl;
#endif
		}
		Nd[i]+=Ntd[i];
		Na[i]+=Nta[i];
		
		//------------band tail---------------
		if(curLyr->IsBT)
		{
			//pt[i]=0;dpt[i]=0;nt[i]=0;dnt[i]=0;
			double PtMd=0,NtMd=0,PtTl=0,NtTl=0;
			double dPtMd=0,dPtMdFn=0,dPtMdFp=0,dNtMd=0,dNtMdFn=0,dNtMdFp=0,dPtTl=0,dPtTlFn=0,dPtTlFp=0,dNtTl=0,dNtTlFn=0,dNtTlFp=0;
			double RdMd=0,dRdMd=0,dRdMdFn=0,dRdMdFp=0,RaMd=0,dRaMd=0,dRaMdFn=0,dRaMdFp=0;
			double RdTl=0,dRdTl=0,dRdTlFn=0,dRdTlFp=0,RaTl=0,dRaTl=0,dRaTlFn=0,dRaTlFp=0;
			double Pt,dPt,dPtFn,dPtFp,xR,dxR,dxRFn,dxRFp;

			double dPt1,dxR1,dPtMd1=0,dNtMd1=0,dPtTl1=0,dNtTl1=0;//TAT
			double dRdMd1=0,dRaMd1=0,dRdTl1=0,dRaTl1=0;
			double dPtm1,dxRm1,dPtMdm1=0,dNtMdm1=0,dPtTlm1=0,dNtTlm1=0;//TAT
			double dRdMdm1=0,dRaMdm1=0,dRdTlm1=0,dRaTlm1=0;
			double dsigNdpsi,dsigNdpsi1,dsigNdpsim1,dsigPdpsi,dsigPdpsi1,dsigPdpsim1,dsigNdEfn,dsigPdEfp;

			Eda=Eg[i]-Eda;//Ev as reference
			CheckTail(curLyr->BgShape,Eg[i],EgOpt,Eda,
				Ed,Ea,Gdo,Gao,GMgD,GMgA,ELo,EUp);
			double Sig_N = SigNMGD;			double Sig_P = SigPMGD;
			double E1 =/* EgVal +*/ ELo;	double E2 = Eda;
			double Gt = GMgD;	//donor-like midgap density of states
			if(E1 < E2)	//If lower bound on midgap states is less than the current energy of interest
			{
				if(i>0&&i<num-1)
				calcSigTAT(y[i][0],affi[i],Eg[i],Eg[i]-(E2+E1)/2,Nc[i],Nv[i],EleF[i],step[i]+step[i-1],y[i][1],y[i][2],//EcM,EvM,
						Sig_N,Sig_P, dGndpsi, dGndpsi1,dGndpsim1, dGpdpsi, dGpdpsi1,dGpdpsim1,dGndEfn,dGpdEfp);//? banded
				dsigNdpsi=SigNMGD*dGndpsi,dsigNdpsi1=SigNMGD*dGndpsi1,dsigNdpsim1=SigNMGD*dGndpsim1,dsigPdpsi=SigPMGD*dGpdpsi,dsigPdpsi1=SigPMGD*dGpdpsi1,dsigPdpsim1=SigPMGD*dGpdpsim1;
				dsigNdEfn=SigNMGD*dGndEfn,dsigPdEfp=SigPMGD*dGpdEfp;
				calptu(Gt,E1,E2,Eg[i],Nc[i],Nv[i],
					Sig_N,Sig_P,Vth,kT,n[i],p[i],ni[i]*ni[i],
					dsigNdpsi,dsigNdpsi1,dsigNdpsim1,dsigPdpsi,dsigPdpsi1,dsigPdpsim1,dsigNdEfn,dsigPdEfp,
					Pt,dPt,dPtFp,dPtFn,xR,dxR,dxRFp,dxRFn,
					dPt1,dPtm1,dxR1,dxRm1);
				PtMd=Pt;	dPtMd=dPt;	dPtMdFn=dPtFn;	dPtMdFp=dPtFp;
				RdMd=xR;	dRdMd=dxR;	dRdMdFn=dxRFn;	dRdMdFp=dxRFp;
				dPtMd1=dPt1; dRdMd1=dxR1;dPtMdm1=dPtm1; dRdMdm1=dxRm1;
			}

			Sig_N = SigNMGA;
			Sig_P = SigPMGA;
			E1 = Eda;
			E2 = Eg[i]- EUp;
			Gt = GMgA;
			if(E1 < E2)	//current energy is less than top of midgap states
			{
				if(i>0&&i<num-1)
				calcSigTAT(y[i][0],affi[i],Eg[i],Eg[i]-(E2+E1)/2,Nc[i],Nv[i],EleF[i],step[i]+step[i-1],y[i][1],y[i][2],//EcM,EvM,
						Sig_N,Sig_P, dGndpsi, dGndpsi1,dGndpsim1, dGpdpsi, dGpdpsi1,dGpdpsim1,dGndEfn,dGpdEfp);
				dsigNdpsi=SigNMGA*dGndpsi,dsigNdpsi1=SigNMGA*dGndpsi1,dsigNdpsim1=SigNMGA*dGndpsim1,dsigPdpsi=SigPMGA*dGpdpsi,dsigPdpsi1=SigPMGA*dGpdpsi1,dsigPdpsim1=SigPMGA*dGpdpsim1;
				dsigNdEfn=SigNMGA*dGndEfn,dsigPdEfp=SigPMGA*dGpdEfp;
				calptu(Gt,E1,E2,Eg[i],Nc[i],Nv[i],
					Sig_N,Sig_P,Vth,kT,n[i],p[i],ni[i]*ni[i],
					dsigNdpsi,dsigNdpsi1,dsigNdpsim1,dsigPdpsi,dsigPdpsi1,dsigPdpsim1,dsigNdEfn,dsigPdEfp,
					Pt,dPt,dPtFp,dPtFn,xR,dxR,dxRFp,dxRFn,
					dPt1,dPtm1,dxR1,dxRm1);
				NtMd=Gt*(E2-E1)-Pt;	dNtMd=-dPt;	dNtMdFn=-dPtFn;	dNtMdFp=-dPtFp;
				RaMd=xR;	dRaMd=dxR;	dRaMdFn=dxRFn;	dRaMdFp=dxRFp;
				dNtMd1=-dPt1; dRaMd1=dxR1;dNtMdm1=-dPtm1; dRaMdm1=dxRm1;
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
					if(i>0&&i<num-1)
					calcSigTAT(y[i][0],affi[i],Eg[i],Eg[i]-(E2+E1)/2,Nc[i],Nv[i],EleF[i],step[i]+step[i-1],y[i][1],y[i][2],//EcM,EvM,
						Sig_N,Sig_P, dGndpsi, dGndpsi1,dGndpsim1, dGpdpsi, dGpdpsi1,dGpdpsim1,dGndEfn,dGpdEfp);
					dsigNdpsi=VBTSigN*dGndpsi,dsigNdpsi1=VBTSigN*dGndpsi1,dsigNdpsim1=VBTSigN*dGndpsim1,dsigPdpsi=VBTSigP*dGpdpsi,dsigPdpsi1=VBTSigP*dGpdpsi1,dsigPdpsim1=VBTSigP*dGpdpsim1;
					dsigNdEfn=VBTSigN*dGndEfn,dsigPdEfp=VBTSigP*dGpdEfp;
					calptu(Gt,E1,E2,Eg[i],Nc[i],Nv[i],
						Sig_N,Sig_P,Vth,kT,n[i],p[i],ni[i]*ni[i],
						dsigNdpsi,dsigNdpsi1,dsigNdpsim1,dsigPdpsi,dsigPdpsi1,dsigPdpsim1,dsigNdEfn,dsigPdEfp,
					Pt,dPt,dPtFp,dPtFn,xR,dxR,dxRFp,dxRFn,
					dPt1,dPtm1,dxR1,dxRm1);
					PtTl+=Pt;	dPtTl+=dPt;	dPtTlFn+=dPtFn;	dPtTlFp+=dPtFp;
					RdTl+=xR;	dRdTl+=dxR;	dRdTlFn+=dxRFn;	dRdTlFp+=dxRFp;
					dPtTl1=dPt1; dRdTl1=dxR1;dPtTlm1=dPtm1; dRdTlm1=dxRm1;
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
					if(i>0&&i<num-1)
					calcSigTAT(y[i][0],affi[i],Eg[i],Eg[i]-(E2+E1)/2,Nc[i],Nv[i],EleF[i],step[i]+step[i-1],y[i][1],y[i][2],//EcM,EvM,
						Sig_N,Sig_P, dGndpsi, dGndpsi1,dGndpsim1, dGpdpsi, dGpdpsi1,dGpdpsim1,dGndEfn,dGpdEfp);
					dsigNdpsi= CBTSigN*dGndpsi,dsigNdpsi1= CBTSigN*dGndpsi1,dsigNdpsim1= CBTSigN*dGndpsim1,dsigPdpsi= CBTSigP*dGpdpsi,dsigPdpsi1= CBTSigP*dGpdpsi1,dsigPdpsim1= CBTSigP*dGpdpsim1;
					dsigNdEfn=CBTSigN*dGndEfn,dsigPdEfp=CBTSigP*dGpdEfp;
					calptu(Gt,E1,E2,Eg[i],Nc[i],Nv[i],
						Sig_N,Sig_P,Vth,kT,n[i],p[i],ni[i]*ni[i],
						dsigNdpsi,dsigNdpsi1,dsigNdpsim1,dsigPdpsi,dsigPdpsi1,dsigPdpsim1,dsigNdEfn,dsigPdEfp,
					Pt,dPt,dPtFp,dPtFn,xR,dxR,dxRFp,dxRFn,
					dPt1,dPtm1,dxR1,dxRm1);
					NtTl+=Gt*(E2-E1)-Pt;	dNtTl+=-dPt;	dNtTlFn+=-dPtFn;	dNtTlFp+=-dPtFp;
					RaTl+=xR;	dRaTl+=dxR;	dRaTlFn+=dxRFn;	dRaTlFp+=dxRFp;
					dNtTl1=-dPt1; dRaTl1=dxR1;dNtTlm1=-dPtm1; dRaTlm1=dxRm1;
				}
			}
			pt[i]=PtMd+PtTl;		nt[i]=NtMd+NtTl;
			dptdpsi[i]=dPtMd+dPtTl;		dptdEfn[i]=dPtMdFn+dPtTlFn;	dptdEfp[i]=dPtMdFp+dPtTlFp;
			dntdpsi[i]=dNtMd+dNtTl;		dntdEfn[i]=dNtMdFn+dNtTlFn;	dntdEfp[i]=dNtMdFp+dNtTlFp;
			R[i]+=RdTl+RdMd+RaTl+RaMd;
			dRdpsi[i]+=dRdTl+dRdMd+dRaTl+dRaMd;	dRdEfn[i]+=dRdTlFn+dRdMdFn+dRaTlFn+dRaMdFn;	dRdEfp[i]+=dRdTlFp+dRdMdFp+dRaTlFp+dRaMdFp;
			dptdpsi1[i]=dPtMd1+dPtTl1;dntdpsi1[i]=dNtMd1+dNtTl1; dRdpsi1[i]+=dRdTl1+dRdMd1+dRaTl1+dRaMd1;
			dptdpsim1[i]=dPtMdm1+dPtTlm1;dntdpsim1[i]=dNtMdm1+dNtTlm1; dRdpsim1[i]+=dRdTlm1+dRdMdm1+dRaTlm1+dRaMdm1;
		}
		/*if(i==juncEdge[pnRgn])
		{
			pnRgn++;EcM=EcMin[pnRgn];EvM=EvMax[pnRgn];
		}*/
		if(i==bIndex[rgn])
		{
			rgn++;
			
			curLyr=layerListP->Item(rgn)->GetData();
			defCnt=curLyr->defList.GetCount();
			
			curLyr->EgOpt.ToDouble(&EgOpt);//if wxString==NULL, then ToDouble->0.0
			curLyr->VBTEnergy.ToDouble(&Ed);curLyr->VBTGo.ToDouble(&Gdo);curLyr->VBTSigN.ToDouble(&VBTSigN);curLyr->VBTSigP.ToDouble(&VBTSigP);
			curLyr->CBTEnergy.ToDouble(&Ea);curLyr->CBTGo.ToDouble(&Gao);curLyr->CBTSigN.ToDouble(&CBTSigN);curLyr->CBTSigP.ToDouble(&CBTSigP);
			curLyr->MGEnergy.ToDouble(&Eda);curLyr->GMGD.ToDouble(&GMgD);curLyr->GMGA.ToDouble(&GMgA);
			curLyr->SigNMGD.ToDouble(&SigNMGD);curLyr->SigPMGD.ToDouble(&SigPMGD);
			curLyr->SigNMGA.ToDouble(&SigNMGA);curLyr->SigPMGA.ToDouble(&SigPMGA);
		}
	}
#ifdef _DEBUG
	file.close();
#endif
}
void Point2::FromPoint(Point &pt)
{
	int rgn=0;
	for(int i=0;i<pt.PointNumber;i++)
	{
//		eps[i-rgn]=pt.eps[i];affi[i-rgn]=pt.affi[i];Eg[i-rgn]=pt.Eg[i];Nc[i-rgn]=pt.Nc[i];Nv[i-rgn]=pt.Nv[i];
//		un[i-rgn]=pt.un[i];up[i-rgn]=pt.up[i];Nd[i-rgn]=pt.Nd[i];Na[i-rgn]=pt.Na[i];		

		psi[i-rgn]=pt.psi[i];Efn[i-rgn]=pt.Efn[i];Efp[i-rgn]=pt.Efp[i];

		if((i-rgn)==bIndex[rgn])
		{
			rgn++;
			i++;
		}
	}
	for(int i=0;i<PointNumber;i++)
	{
		psiOld[i]=psi[i];EfnOld[i]=Efn[i];EfpOld[i]=Efp[i];
	}
}
void Point2::ToPoint(Point & pt)
{
	int rgn=0;
	pt.n0L=n0L;pt.n0R=n0R;pt.p0L=p0L;pt.p0R=p0R;
	for(int i=0;i<pt.PointNumber;i++)
	{
//		pt.eps[i]=eps[i-rgn];pt.affi[i]=affi[i-rgn];pt.Eg[i]=Eg[i-rgn];pt.Nc[i]=Nc[i-rgn];pt.Nv[i]=Nv[i-rgn];
//		pt.un[i]=un[i-rgn];pt.up[i]=up[i-rgn];pt.Nd[i]=Nd[i-rgn];pt.Na[i]=Na[i-rgn];		
		
		pt.psi[i]=psi[i-rgn];pt.Efn[i]=Efn[i-rgn];pt.Efp[i]=Efp[i-rgn];
		pt.psiInit[i]=psi[i-rgn];
		//pt.un[i]=un[i];pt.up[i]=up[i];//TAT
		if((i-rgn)==bIndex[rgn])
		{
			rgn++;
//			pt.eps[i+1]=eps[i];pt.affi[i+1]=affi[i];pt.Eg[i+1]=Eg[i];pt.Nc[i+1]=Nc[i];pt.Nv[i+1]=Nv[i];
//			pt.un[i+1]=un[i];pt.up[i+1]=up[i];pt.Nd[i+1]=Nd[i];pt.Na[i+1]=Na[i];		
			pt.psi[i+1]=pt.psi[i];pt.Efn[i+1]=pt.Efn[i];pt.Efp[i+1]=pt.Efp[i];
			pt.psiInit[i+1]=pt.psi[i];
			i++;
		}
	}

}
bool Point2::ExportG(double TopRF,double BtmRF,vector<double> photo0,vector<double> waveLength)
{
	int region=0,rgn=layerListP->GetCount();
	int size=waveLength.size();double absi,geni;
	double genSum;
	double x0=0;
	for(int i=0;i<size;i++)
		photo0[i]*=(1-TopRF);
	MyList::compatibility_iterator node;
	node=layerListP->Item(region);
	layer *curLyr=node->GetData();
	/*if(!curLyr->absWave.size())
	{
		wxString str=_("No absorption coefficient in material ");
		str+=curLyr->name;
		wxMessageBox(str);
		return 0;
	}*/
	for(int i=0;i<PointNumber;i++)//x[i]
	{
		{
			genSum=0;
			for(int j=0;j<size;j++)//waveLength[j]
			{
				absi=1e-2*Interp(curLyr->absWave,curLyr->alpha,waveLength[j]);
				geni=absi*photo0[j]*exp(-absi*(x[i]-x0));
				genSum+=geni;
			}
			G[i]=genSum;
		}
		if(i==(bIndex[region]+1))// into another region
		{					
			int waveNum=photo0.size();
			for(int k=0;k<waveNum;k++)
			{
				absi=1e-2*Interp(curLyr->absWave,curLyr->alpha,waveLength[k]);
				photo0[k]=photo0[k]*exp(-absi*(x[i]-x0));
			}
			region++;	
			if(region<rgn)
			{
				node=layerListP->Item(region);
				curLyr=node->GetData();
			}
			x0=x[i];
		}
	}
	for(int j=0;j<size;j++)//reverse
	{
		absi=1e-2*Interp(curLyr->absWave,curLyr->alpha,waveLength[j]);
		photo0[j]=photo0[j]*exp(-absi*(x[PointNumber-1]-x0));
		photo0[j]*=BtmRF;
	}
	x0=x[PointNumber-1];region--;
	for(int i=PointNumber-1;i>=0;i--)
	{
		genSum=0;
		for(int j=0;j<size;j++)//waveLength[j]
		{
			absi=1e-2*Interp(curLyr->absWave,curLyr->alpha,waveLength[j]);
			geni=absi*photo0[j]*exp(-absi*(x0-x[i]));
			genSum+=geni;
		}
		G[i]+=genSum;
		if(i==(bIndex[region]+1))// into another region
		{
			int waveNum=photo0.size();	
			for(int k=0;k<waveNum;k++)
			{
				absi=1e-2*Interp(curLyr->absWave,curLyr->alpha,waveLength[k]);
				photo0[k]=photo0[k]*exp(-absi*(x0-x[i]));
			}
			if(region>=0)
			{
				node=layerListP->Item(region);
				curLyr=node->GetData();
			}					
			x0=x[i];
			if(region>0)
				region--;
		}
	}
	return 1;
}