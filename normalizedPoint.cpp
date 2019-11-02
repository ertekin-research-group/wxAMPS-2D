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

//------------------------calculate Efn--------------------------------
bool Point:: IterN(double volt,double *y, double*J,double*l,double*u,int num,double kT,double TopSn,double BtmSn,wxProgressDialog *dlg)
{
	double *c=new double[num],  *Efn1=new double[num],*dEfn=new double[num],*p1=new double[num],*n1=new double[num];
	double *f=new double[num];
	double *u0=new double[num], *u1=new double[num];
	for(int i=0;i<num*num;i++)
	{
		J[i]=0;
	}
	for(int i=0;i<num;i++)
	{
		p1[i]=/*Ph(Nv[i],(y[i]-affi[i]-Eg[i]-Efp[i])/kT,kT);*/Nv[i]*exp((y[i]-affi[i]-Eg[i]-Efp[i])/kT);		
		if(step[i]!=0)
		{
			//c[i]=QE*un[i]*Nc[i]*exp((affi[i])/kT)*(y[i+1]-y[i])/step[i]/(exp(y[i+1]/kT)-exp(y[i]/kT));/*GP12((Efn1[i]+affi[i]-y[i])/kT)*/
			c[i]=QE*un[i]*Nc[i]/step[i]*SafeDExp(-(y[i]-affi[i])/kT)*kT*xDexM1((y[i+1]-affi[i+1]-y[i]+affi[i])/kT);
		}
		else//this point is useless
		{
			c[i]=0;			
		}
	}

	double ep=1e-6,tol=1;int cnt=0;
	while( tol>ep)
	{
		transfer(Efn,Efn1,num);
		for(int i=0;i<num;i++)
			u1[i]=SafeDExp(Efn1[i]/kT)-1;
		fn(volt,y,Efn1,u1,n1,p1,num,kT,TopSn,BtmSn,c,f);
		Jacn(volt,y,Efn1,u1,n1,p1,num,kT,TopSn,BtmSn,c,J);
		LU2(J,f,l,u,num,dEfn);		
		for(int i=0;i<num;i++)
		{
			u0[i]=u1[i]-dEfn[i];
				if(u0[i]<=-1)
				u0[i]=.1-1;
			dEfn[i]=Efn1[i]-kT*log(u0[i]+1);
		}		
		clamp(dEfn,0.2,num);
		for(int i=0;i<num;i++)
		{
			Efn[i]=Efn1[i]-dEfn[i];
		}						
		tol=norm(Efn,Efn1,num);
		cnt++;		
		if(cnt>200)                                       
		{	
			wxMessageBox(_("too many iterations in calculating Efn , results may not be correct!"));
			delete[] c;delete[]Efn1;delete[]dEfn;delete[]p1;delete[] n1;delete[] f;
			delete[] u0;delete[] u1;
			return 0;
		}
	}
	delete[] c;delete[]Efn1;delete[]dEfn;delete[]p1;delete[] n1;delete[] f;
	delete[] u0;delete[] u1;
	return 1;
}
void Point:: fn(double volt,double *y, double *Efn1,double *u, double *n1,double *p1,
		  int num,double kT,double TopSn,double BtmSn,double *c,double *f)
{
	for(int i=0;i<num;i++)
	{		
		n1[i]=/*Ne(Nc[i],(Efn1[i]+affi[i]-y[i])/kT);*/Nc[i]*exp((Efn1[i]+affi[i]/*-affi[num-1]*/-y[i])/kT);		

		if(step[i]!=0)
			Jn[i]=c[i]*(u[i+1]-u[i]);
		else if(i!=num-1)
		{
			double AT2i=7.74813e-5*kT*pow(Nc[i]/2,2./3);			
			if(affi[i]>=affi[i+1])//thermal emission current
				Jn[i]=AT2i*SafeDExp((-y[i+1]+affi[i+1])/kT)*(u[i+1]-u[i]);
			else
				Jn[i]=AT2i*SafeDExp((-y[i+1]+affi[i])/kT)*(u[i+1]-u[i]);
		}
		else
			Jn[i]=Jn[i-1];
	}
	Recombination(0,n1,p1,num,kT);//dR=dR/dEfn

	f[0]=Jn[0]-QE*TopSn*(Nc[0]*exp((Efn1[0]+affi[0]-y[0])/kT)-n0L);// ?defect affect on n0L
	for (int i=1;i<num-1;i++)
	{
		if(step[i]!=0&&step[i-1]!=0)
			f[i]=2*(Jn[i]-Jn[i-1])/(step[i]+step[i-1])-QE*R[i]+QE*G[i];
		else if(0==step[i])			
			f[i]=2*(Jn[i]-Jn[i-1])/step[i-1]-QE*R[i]+QE*G[i];//thermal emission model
		else
			f[i]=2*(Jn[i]-Jn[i-1])/(step[i])-QE*R[i]+QE*G[i];
	}
	f[num-1]=Jn[num-2]+QE*BtmSn*(Nc[num-1]*exp((Efn1[num-1]-y[num-1]+affi[num-1])/kT)-n0R);

	
}
void Point:: Jacn(double volt,double *y, double *Efn1, double *u,double *n1,double *p1,
		  int num,double kT,double TopSn,double BtmSn,double *c,double *J)
{
	double AT2i;
	
	J[0*num+0]=-c[0]-QE*TopSn*(Nc[0]*SafeDExp((affi[0]-y[0])/kT));
	J[0*num+1]=c[0];
	for(int i=1;i<num-1;i++)
	{
		if(step[i]!=0&&step[i-1]!=0)
		{
			J[i*num+i-1]=2/(step[i]+step[i-1])*c[i-1];
			J[i*num+i]=-2/(step[i]+step[i-1])*(c[i]+c[i-1])-QE*dR[i]*kT/(u[i]+1);///kT;
			J[i*num+i+1]=2/(step[i]+step[i-1])*c[i];
		}
		else if(0==step[i])
		{
			AT2i=7.74813e-5*kT*pow(Nc[i]/2,2./3);			
			J[i*num+i-1]=2*c[i-1]/step[i-1];
			if(affi[i]>=affi[i+1])
			{
				J[i*num+i]=-2*c[i-1]/step[i-1]-2*AT2i*SafeDExp((-y[i]+affi[i+1])/kT)/step[i-1]-QE*dR[i]*kT/(u[i]+1);
				J[i*num+i+1]=2*AT2i*SafeDExp((-y[i+1]+affi[i+1])/kT)/step[i-1];
			}
			else 
			{
				J[i*num+i]=-2*c[i-1]/step[i-1]-2*AT2i*SafeDExp((-y[i]+affi[i])/kT)/step[i-1]-QE*dR[i]*kT/(u[i]+1);
				J[i*num+i+1]=2*AT2i*SafeDExp((-y[i+1]+affi[i])/kT)/step[i-1];
			}
		}
		else 
		{
			if(affi[i-1]>=affi[i])
			{
				J[i*num+i-1]=2*AT2i*SafeDExp((-y[i-1]+affi[i])/kT)/step[i];
				J[i*num+i]=-2/(step[i])*c[i]-2/step[i]*AT2i*SafeDExp((-y[i]+affi[i])/kT)-QE*dR[i]*kT/(u[i]+1);
			}
			else
			{
				J[i*num+i-1]=2*AT2i*SafeDExp((-y[i-1]+affi[i-1])/kT)/step[i];
				J[i*num+i]=-2/(step[i])*c[i]-2/step[i]*AT2i*SafeDExp((-y[i]+affi[i-1])/kT)-QE*dR[i]*kT/(u[i]+1);
			}
			J[i*num+i+1]=2/(step[i])*c[i];
		}
	}
	J[num*num-2]=-c[num-2];
	J[num*num-1]=c[num-2]+QE*BtmSn*(Nc[num-1]*SafeDExp((-y[num-1]+affi[num-1])/kT));

	//delete[] dR;
}

//---------------------------------------------------------------------
bool Point:: IterP(double volt,double *y, double*J,double*l,double*u, int num,double kT,double TopSp,double BtmSp,wxProgressDialog *dlg)
{
	double *c=new double[num],  *Efp1=new double[num],*dEfp=new double[num],*n1=new double[num];
	double *f=new double[num];
	double *v=new double[num],*v1=new double[num];
	for(int i=0;i<num*num;i++)
	{
		J[i]=0;
	}
	
	for(int i=0;i<num;i++)
	{
		n1[i]=Nc[i]*exp((Efn[i]+affi[i]-y[i])/kT);		
		if(step[i]!=0)
		{
			c[i]=QE*up[i]*Nv[i]/step[i]*exp((y[i]-affi[i]-Eg[i])/kT)*kT*(-xDexM1((-y[i+1]+affi[i+1]+Eg[i+1]+y[i]-affi[i]-Eg[i])/kT));
			//*(y[i+1]-y[i])*exp((-affi[i]-Eg[i])/kT)/(exp(-y[i+1]/kT)-exp(-y[i]/kT));		
		}
		else
			c[i]=0;					
	}
	
	//wxString str, title="Function:Calculate Efp\nIteration:";
	double ep=1e-6,tol=1;int cnt=0;//	double range;
	while( tol>ep)
	{
		transfer(Efp,Efp1,num);
		for(int i=0;i<num;i++)
			v1[i]=SafeDExp(-Efp1[i]/kT)-SafeDExp(volt/kT);
		fp(volt,y,Efp1,v1,n1,num,kT,TopSp,BtmSp,c,f);		
		Jacp(volt,y,Efp1,v1,n1,num,kT,TopSp,BtmSp,c,J);
		LU2(J,f,l,u,num,dEfp);
//#define out
#ifdef out		
		outEB(x,Efp,v1,dEfp,f,num);
#endif
		for(int i=0;i<num;i++)
		{
			v[i]=v1[i]-dEfp[i];
			if(v[i]<=-SafeDExp(volt/kT))
				v[i]=.1-SafeDExp(volt/kT);
			dEfp[i]=Efp1[i]+kT*log(v[i]+SafeDExp(volt/kT));
		}		
		clamp(dEfp,0.1,num);
		for(int i=0;i<num;i++)
		{
			Efp[i]=Efp1[i]-dEfp[i];
		}		
		tol=norm(Efp,Efp1,num);		
		cnt++;		
		if(cnt>150)                                       
		{	
#ifdef _DEBUG
			wxMessageBox(_("too many iterations in calculating Efp , results may be not correct!"));
#endif
			delete[] c;delete[]Efp1;delete[]dEfp;delete[]n1;delete[] f;//delete[] J,l,u;
			delete[] v;delete []v1;
			return 0;
		}		
	}
	
	delete[] c;delete[]Efp1;delete[]dEfp;delete[]n1;delete[] f;//delete[] J,l,u;
	delete[] v;delete []v1;
	return 1;
}
void Point:: fp(double volt,double *y, double *Efp1, double *v,double *n1,
		  int num,double kT,double TopSp,double BtmSp,double *c,double *f)
{
	double *p1=new double[num];// *v=new double[num];
	
	for(int i=0;i<num;i++)
	{
		p1[i]=Nv[i]*exp((y[i]-affi[i]-Eg[i]-Efp1[i])/kT);	
		if(step[i]!=0)
			//Jp[i]=c[i]*(exp(-Efp1[i+1]/kT)-exp(-Efp1[i]/kT));
			Jp[i]=c[i]*(v[i+1]-v[i]);
		else if(i!=num-1)
		{
			double AT2i;
			if(Nv[i]<Nv[i+1])
				AT2i=7.74813e-5*kT*pow(Nv[i]/2,2./3);
			else
				AT2i=7.74813e-5*kT*pow(Nv[i+1]/2,2./3);
			
		
			double delE=(affi[i+1]+Eg[i+1])-(affi[i]+Eg[i]);
			if(delE>=0)
				Jp[i]=AT2i*exp((y[i]-affi[i+1]-Eg[i+1])/kT)*(v[i]-v[i+1]);
			else
				Jp[i]=AT2i*exp((y[i]-affi[i]-Eg[i])/kT)*(v[i]-v[i+1]);
		}
		else
			Jp[i]=Jp[i-1];
	}
	Recombination(1,n1,p1,num,kT);//dR=dR/dEfp

	f[0]=Jp[0]+QE*TopSp*(p1[0]-p0L);//?direction
	for (int i=1;i<num-1;i++)
	{
		if(step[i]!=0&&step[i-1]!=0)
			f[i]=2*(Jp[i]-Jp[i-1])/(step[i]+step[i-1])+QE*R[i]-QE*G[i];
		else if(0==step[i])
			f[i]=2*(Jp[i]-Jp[i-1])/step[i-1]+QE*R[i]-QE*G[i];			
		else
			f[i]=2*(Jp[i]-Jp[i-1])/(step[i])+QE*(R[i])-QE*(G[i]);//?recombination model
	}
	f[num-1]=Jp[num-2]-QE*BtmSp*(p1[num-1]-p0R);

	delete[] p1;
}
void Point:: Jacp(double volt,double *y, double *Efp1,double*v, double *n1,
		  int num,double kT,double TopSp,double BtmSp,double *c,double *J)
{
	double AT2i,delE;
	J[0*num+0]=(-c[0]+QE*TopSp*Nv[0]*exp((y[0]-affi[0]-Eg[0])/kT));
	J[0*num+1]=c[0];

	for(int i=1;i<num-1;i++)
	{
		if(step[i]!=0&&step[i-1]!=0)
		{
			J[i*num+i-1]=2/(step[i]+step[i-1])*c[i-1];
			J[i*num+i]=-2/(step[i]+step[i-1])*(c[i]+c[i-1])+QE*dR[i]*(-kT)/(v[i]+exp(volt/kT));///kT;
			J[i*num+i+1]=2/(step[i]+step[i-1])*c[i];
		}
		else if(0==step[i])
		{
			//AT2i=7.74813e-5*kT*pow(Nv[i]/2,2./3);
			if(Nv[i]<Nv[i+1])
				AT2i=7.74813e-5*kT*pow(Nv[i]/2,2./3);
			else
				AT2i=7.74813e-5*kT*pow(Nv[i+1]/2,2./3);
			delE=(affi[i+1]+Eg[i+1])-(affi[i]+Eg[i]);
			J[i*num+i-1]=2*c[i-1]/step[i-1];
			if(delE>=0)
			{
				J[i*num+i]=-2*c[i-1]/step[i-1]+2*AT2i*exp((y[i]-affi[i+1]-Eg[i+1])/kT)/step[i-1]+QE*dR[i]*(-kT)/(v[i]+exp(volt/kT));
				J[i*num+i+1]=-2*AT2i*exp((y[i+1]-affi[i+1]-Eg[i+1])/kT)/step[i-1];
			}
			else
			{
				J[i*num+i]=(-2*c[i-1]+2*AT2i*exp((y[i]-affi[i]-Eg[i])/kT))/step[i-1]+QE*dR[i]*(-kT)/(v[i]+exp(volt/kT));
				J[i*num+i+1]=-2*AT2i*exp((y[i+1]-affi[i]-Eg[i])/kT)/step[i-1];
			}
			
		}
		else
		{
			//J[i*num+i-2]=2/(step[i]+step[i-2])*c[i-2];
			if(delE>=0)
			{J[i*num+i-1]=-2/(step[i])*AT2i*exp((y[i-1]-affi[i]-Eg[i])/kT);
			J[i*num+i]=2/(step[i])*(-c[i]+AT2i*exp((y[i]-affi[i]-Eg[i])/kT))+QE*(dR[i])*(-kT)/(v[i]+exp(volt/kT));}
			else
			{
				J[i*num+i-1]=-2/(step[i])*AT2i*exp((y[i-1]-affi[i-1]-Eg[i-1])/kT);
				J[i*num+i]=2/(step[i])*(-c[i]+AT2i*exp((y[i]-affi[i-1]-Eg[i-1])/kT))+QE*(dR[i])*(-kT)/(v[i]+exp(volt/kT));
			}
			J[i*num+i+1]=2/(step[i])*c[i];
		}
	}
	J[num*num-2]=-c[num-2];
	J[num*num-1]=c[num-2]-QE*BtmSp*Nv[num-1]*exp((y[num-1]-affi[num-1]-Eg[num-1])/kT);

}
