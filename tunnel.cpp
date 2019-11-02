#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "Point.h"
#include "calculation.h"
#include "constant.h"
#include "LU.h"

void Point:: IterNTun(double *y, double*J,double*l,double*u,int num,double kT,double TopSn,double BtmSn,wxProgressDialog *dlg)
{
	double *c=new double[num],  *Efn1=new double[num],*dEfn=new double[num],*p1=new double[num],*n1=new double[num];
	double *f=new double[num];
	
	for(int i=0;i<num*num;i++)
	{
		J[i]=0;
	}
	for(int i=0;i<num;i++)
	{
		p1[i]=Nv[i]*SafeDExp((y[i]-affi[i]-Eg[i]-Efp[i])/kT);		
		if(step[i]!=0)
		{
			c[i]=QE*un[i]*Nc[i]/step[i]*SafeDExp(-(y[i]-affi[i])/kT)*kT*xDexM1((y[i+1]-affi[i+1]-y[i]+affi[i])/kT);
			//*exp((affi[i])/kT)*(y[i+1]-y[i])/(exp(y[i+1]/kT)-exp(y[i]/kT));/*GP12((Efn1[i]+affi[i]-y[i])/kT)*/
		}
		else//this point is useless
		{
			c[i]=0;			
		}
	}
	//outEB(x,Efn,c,p1,num);

	double ep=1e-6,tol=1;int cnt=0;
	ep=precision;
	while( tol>ep)
	{
		transfer(Efn,Efn1,num);
		fn3(y,Efn1,n1,p1,num,kT,TopSn,BtmSn,c,f);
		Jacn3(y,Efn1,n1,p1,num,kT,TopSn,BtmSn,c,J);
		LU2(J,f,l,u,num,dEfn);
		clamp(dEfn,limit,num);
		for(int i=0;i<num;i++)
		{
			Efn[i]=Efn1[i]-dEfn[i];
		}		
		tol=norm(Efn,Efn1,num);
		cnt++;
		
		if(cnt>iterTimes)                                       
		{	
#ifdef _DEBUG
			wxMessageBox(_("too many iterations in calculating Efn , results may not be correct!"));
#endif
			delete[] c;delete[]Efn1;delete[]dEfn;delete[]p1;delete[] n1;delete[] f;
			return;
		}
	}
	delete[] c;delete[]Efn1;delete[]dEfn;delete[]p1;delete[] n1;delete[] f;//,J,l,u;
}
void Point:: fn3(double *y, double *Efn1, double *n1,double *p1,
		  int num,double kT,double TopSn,double BtmSn,double *c,double *f)
{
	for(int i=0;i<num;i++)
	{		
		n1[i]=Nc[i]*SafeDExp((Efn1[i]+affi[i]-y[i])/kT);		
		if(step[i]!=0)
			Jn[i]=c[i]*SafeDExp(Efn1[i]/kT)*(SafeDExp((Efn1[i+1]-Efn1[i])/kT)-1);//;*(exp(Efn1[i+1]/kT)-exp(Efn1[i]/kT));
		else if(i!=num-1)
		{//2*QE^2/h=7.74813e-5
			double AT2i;
			if(Nc[i]<Nc[i+1])
				AT2i=7.74813e-5*kT*pow(Nc[i]/2,2./3);
			else
				AT2i=7.74813e-5*kT*pow(Nc[i+1]/2,2./3);
			if(affi[i]>=affi[i+1])//thermal emission current
				Jn[i]=AT2i*SafeDExp((Efn1[i+1]-y[i+1]+affi[i+1])/kT)*(1-exp((Efn1[i]-Efn1[i+1])/kT));//-AT2i*SafeDExp((Efn1[i]-y[i]+affi[i+1])/kT);
			else
				Jn[i]=AT2i*SafeDExp((Efn1[i+1]-y[i+1]+affi[i])/kT)*(1-exp((Efn1[i]-Efn1[i+1])/kT));//AT2i*SafeDExp((Efn1[i]-y[i]+affi[i])/kT);
			Jn[i]*=1+tunC[i];//inject all tunneling current at interface
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
			f[i]=2*(Jn[i]-Jn[i-1])/step[i-1]-QE*(R[i]/*+R[i+1]*/)+QE*G[i];
		else
			f[i]=2*(Jn[i]-Jn[i-1])/(step[i])-QE*(R[i]/*+R[i-1]*/)+QE*G[i];//?recombination model
	}
	f[num-1]=Jn[num-2]+QE*BtmSn*(Nc[num-1]*SafeDExp((Efn1[num-1]-y[num-1]+affi[num-1])/kT)-n0R);
	f[num-1]/=step[num-2];

	
}
void Point:: Jacn3(double *y, double *Efn1, double *n1,double *p1,
		  int num,double kT,double TopSn,double BtmSn,double *c,double *J)
{
	double AT2i;
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
			if(Nc[i]<Nc[i+1])
				AT2i=7.74813e-5*kT*pow(Nc[i]/2,2./3);
			else
				AT2i=7.74813e-5*kT*pow(Nc[i+1]/2,2./3);	
			
			J[i*num+i-1]=2*c[i-1]*SafeDExp(Efn1[i-1]/kT)/kT/step[i-1];
			if(affi[i]>=affi[i+1])
			{
				J[i*num+i]=-2*c[i-1]*SafeDExp(Efn1[i]/kT)/kT/step[i-1]-2*(1+tunC[i])*AT2i*SafeDExp((Efn1[i]-y[i]+affi[i+1])/kT)/kT/step[i-1]-QE*(dR[i]/*+dR[i+1]*/);
				J[i*num+i+1]=2*(1+tunC[i])*AT2i*SafeDExp((Efn1[i+1]-y[i+1]+affi[i+1])/kT)/kT/step[i-1];
			}
			else 
			{
				J[i*num+i]=-2*c[i-1]*SafeDExp(Efn1[i]/kT)/kT/step[i-1]-2*(1+tunC[i])*AT2i*SafeDExp((Efn1[i]-y[i]+affi[i])/kT)/kT/step[i-1]-QE*(dR[i]/*+dR[i+1]*/);
				J[i*num+i+1]=2*(1+tunC[i])*AT2i*SafeDExp((Efn1[i+1]-y[i+1]+affi[i])/kT)/kT/step[i-1];
			}
		}
		else 
		{
			if(affi[i-1]>=affi[i])
			{
				J[i*num+i-1]=2*(1+tunC[i-1])*AT2i*SafeDExp((Efn1[i-1]-y[i-1]+affi[i])/kT)/kT/step[i];
				J[i*num+i]=-2/(step[i])/kT*c[i]*SafeDExp(Efn1[i]/kT)-2/step[i]/kT*(1+tunC[i-1])*AT2i*SafeDExp((Efn1[i]-y[i]+affi[i])/kT)-QE*(dR[i]/*+dR[i-1]*/);
			}
			else
			{
				J[i*num+i-1]=2*(1+tunC[i-1])*AT2i*SafeDExp((Efn1[i-1]-y[i-1]+affi[i-1])/kT)/kT/step[i];
				J[i*num+i]=-2/(step[i])/kT*c[i]*SafeDExp(Efn1[i]/kT)-2/step[i]/kT*(1+tunC[i-1])*AT2i*SafeDExp((Efn1[i]-y[i]+affi[i-1])/kT)-QE*(dR[i]/*+dR[i-1]*/);
			}
			J[i*num+i+1]=2/(step[i])/kT*c[i]*SafeDExp(Efn1[i+1]/kT);
		}
	}
	J[num*num-2]=-c[num-2]*SafeDExp(Efn1[num-2]/kT)/kT;
	J[num*num-1]=c[num-2]*SafeDExp(Efn1[num-1]/kT)/kT+QE*BtmSn*(Nc[num-1]*SafeDExp((Efn1[num-1]-y[num-1]+affi[num-1])/kT))/kT;	
	J[num*num-2]/=step[num-2];J[num*num-1]/=step[num-2];

}
double Point::WKB(double *y,int start,int cur,double kT)
{
	double factor=4*sqrt(PI/kT)*pow(Nc[cur]/2,1./3);
	double sum=0;//integral
	for(int i=start;i<cur;i++)
	{
		sum+=(x[i+1]-x[i])*(sqrt(y[cur]-y[i])+sqrt(y[cur]-y[i+1]))/2;
	}
	if(factor*sum<400)
		return exp(-factor*sum);
	else
		return 0;
}
double Point::WKBP(double *yv,int start,int cur,double kT)
{
	double factor=4*sqrt(PI/kT)*pow(Nv[cur]/2,1./3);
	double sum=0;//integral
	for(int i=start;i<cur;i++)
	{
		sum+=(x[i+1]-x[i])*(sqrt(yv[i]-yv[cur])+sqrt(yv[i+1]-yv[cur]))/2;
	}
	if(factor*sum<400)
		return exp(-factor*sum);
	else
		return 0;

}
void Point::calcTunC(double * y,double kT)
{
	double Emin,Emax,Ei;int num=PointNumber;
	int inter=0,interLast=0,interP=0,interLastP=0;
	int shape;//1-pn;2-np;
	bool tunnel=false,tunnelP=false; 
	for(int i=0;i<num-1;i++)//exclude end point
	{
		tunC[i]=0;
		if(0==step[i])//at boundary
		{
			if((affi[i]>affi[i+1])&&(y[i-1]>y[i]))//barrier
			{
				tunnel=true;			shape=1;				
				Emin=y[i]-affi[i];		Emax=y[i]-affi[i+1];
			}
			if((affi[i]<affi[i+1])&&(y[i-1]<y[i]))
			{
				tunnel=true;			shape=2;	
				Emin=y[i]-affi[i+1];	Emax=y[i]-affi[i];
				for(int j=i-1;j>=interLast;j--)//calculate former region
				{
					Ei=y[j]-affi[j];
					if(Ei>Emin&&Ei<=Emax)
					{
						double TEi=WKB(y,j,i,kT);
						if(TEi!=0)
							tunC[j]=TEi*SafeDExp((y[i]-y[j])/kT)*(y[j+1]-y[j])/kT;//?integral not trapezoid
						else// no tunneling current
						{	tunnel=false;
							break;}
					}
					else
					{	tunnel=false;
						break;}
					tunC[i]+=tunC[j];
				}
			}
			interLast=inter;
			inter=i;
		}
		else if(tunnel)//calculate coming region
		{
			Ei=y[i+1]-affi[i+1];
			if(1==shape)
			{
				if(Ei>Emin&&Ei<=Emax)
				{
					double TEi=WKB(y,i+1,inter+1,kT);
					if(TEi!=0)
						tunC[i+1]=TEi*SafeDExp((y[inter+1]-y[i+1])/kT)*(y[i]-y[i+1])/kT;//?integral not trapezoid
					else// no tunneling current
						tunnel=false;
				}
				else
					tunnel=false;
				tunC[inter]+=tunC[i+1];//inject all the tunneling current to interface
			}			
			
		}
		//for holes
		tunCp[i]=0;double Evmax,Evmin;
		if(0==step[i])
		{
			if(((y[i]-affi[i]-Eg[i])>(y[i+1]-affi[i+1]-Eg[i+1]))&&(y[i-1]<y[i]))
			{
				tunnelP=true; shape=1;
				Evmax=y[i]-affi[i]-Eg[i];Evmin=y[i+1]-affi[i+1]-Eg[i+1];
			}
			if(((y[i]-affi[i]-Eg[i])<(y[i+1]-affi[i+1]-Eg[i+1]))&&(y[i-1]>y[i]))
			{
				tunnelP=true; shape=2;
				Evmin=y[i]-affi[i]-Eg[i];Evmax=y[i+1]-affi[i+1]-Eg[i+1];
				for(int j=i-1;j>=interLastP;j--)
				{
					Ei=y[j]-affi[j]-Eg[j];
					if(Ei>=Evmin&&Ei<Evmax)
					{
						double TEi=WKBP(y,j,i,kT);
						if(TEi!=0)
							tunCp[j]=TEi*SafeDExp((y[j]-y[i])/kT)*(y[j]-y[j+1])/kT;
						else// no tunneling current
						{	tunnelP=false;
						break;}
					}
					else
					{	tunnelP=false;
					break;}
					tunCp[i]+=tunCp[j];
				}
				interLastP=interP;
				interP=i;
			}
			else if(tunnelP)
			{
				Ei=y[i+1]-affi[i+1]-Eg[i+1];
				if(1==shape)
				{
					if(Ei>=Evmin&&Ei<Evmax)
					{
						double TEi=WKBP(y,i+1,interP+1,kT);
						if(TEi!=0)
							tunCp[i+1]=TEi*SafeDExp((y[i+1]-y[inter+1])/kT)*(y[i+1]-y[i])/kT;//?integral not trapezoid
						else// no tunneling current
							tunnelP=false;
					}
					else
						tunnelP=false;
					tunCp[inter]+=tunCp[i+1];
				}							
			}
		}
	}
}


