#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "Point2.h"
#include "constant.h"
#include <math.h>

void Point2::calcGamma(double F, double Nc,double Nv,double delEn,double delEp,double s,bool dEn,bool dEp,
		double & GammaN,double &GammaP,double & dGndpsi,double & dGndpsi1,double & dGndpsim1,
		double & dGpdpsi,double & dGpdpsi1,double & dGpdpsim1,double &dGndEfn, double &dGpdEfp)
{	
	double Kn,Kp,Nroot, factor,u,v;
	double dKnpdpsi,dKnpdpsi1,dKnpdpsim1;
	double dterm2dpsi,dterm3dpsi,dterm4dpsi,dterm2dpsi1,dterm3dpsi1,dterm4dpsi1,dtdpsi,dtdpsi1;
	double dterm2dpsim1,dterm3dpsim1,dterm4dpsim1,dtdpsim1;
	double dKnpdEfn,dKnpdEfp;double dterm2dEfn,dterm2dEfp,dterm3dEfn,dterm3dEfp,dterm4dEfn,dterm4dEfp,dtdEfn,dtdEfp;
	
	dGndEfn=0;dGpdEfp=0;dGndpsi=0;dGpdpsi=0;
//	Nc=1e19;Nv=1e20;
	Nroot=pow(Nc/2,1./3);
	double Fr=kT*12.2799*Nroot;//sqrt(24*m*kT^3)/q/(h/2/PI)
	double a1,a2,a3,a4,t;
//	Knp=8*PI/3*sqrt(2*m*pow(delEnp,3))/QE/fabs(E)/H;
//	if(delEn>1) delEn=1;	if(delEp>1)	delEp=1;
	
	if(delEn<=0||0==F)
	{	
		GammaN=0;dGndpsi=0;dGndpsi1=0;		
		dGndEfn=0;		dGndpsim1=0;
	}
	else
	{
		Kn=8./3*sqrt(PI/kT)*Nroot*pow(delEn,3./2)/fabs(F);
		if(Kn>=(2./3*delEn/kT))
		{			
			GammaN=6.13996*fabs(F)/Fr*exp(pow(F/Fr,2));
			dGndpsim1=-GammaN/F/s*(1+2*F*F/Fr/Fr);//F=(psi1-psim1)/(step+stepm1)
			dGndpsi1=-dGndpsim1;	
			dGndEfn=0;dGndpsi=0;
		}
		else
		{
			t=1+0.61685*(delEn/kT-1.5*Kn)/sqrt(1.5*Kn);
			t=1/t;
			a1=delEn/kT;a2=sqrt(2*PI/3/Kn);a3=(0.3480242*t-0.0948798*t*t+0.7478556*t*t*t);a4=exp(delEn/kT-Kn);
			GammaN=a1*a2*a3*a4;
			//factor=8./3*sqrt(PI/kT)*Nroot/F/F;
			if(dEn)			//delEn=psi-affi-Efn
			{
				dKnpdpsi=1.5*Kn/delEn;//dKnpdpsi=factor*sqrt(delEn)*(1.5*F+delEn/s);	
				dKnpdEfn=-1.5*Kn/delEn;
			}
			else
			{dKnpdpsi=0;//dKnpdpsi=factor*pow(delEn,1.5)/s;	
			dKnpdEfn=0;}
			dKnpdpsi1=-Kn/F/s;
			dKnpdpsim1=-dKnpdpsi1;
			/*if(F<0)	
			{dKnpdpsi*=-1;dKnpdpsi1*=-1;}*/

			factor=sqrt(2*PI/3)*(-0.5)*pow(Kn,-1.5);
			dterm2dpsi=factor*dKnpdpsi;		dterm2dpsi1=factor*dKnpdpsi1; dterm2dEfn=factor*dKnpdEfn;
			dterm2dpsim1=factor*dKnpdpsim1; 

			factor=-t*t*0.61685/1.5/Kn;u=delEn/kT-1.5*Kn;v=sqrt(1.5*Kn);
			if(dEn)
			{
				dtdpsi=factor*((1/kT-1.5*dKnpdpsi)*v-u*0.75/sqrt(1.5*Kn)*dKnpdpsi);
				dtdEfn=factor*((-1/kT-1.5*dKnpdEfn)*v-u*0.75/sqrt(1.5*Kn)*dKnpdEfn);				
			}
			else
			{	
				dtdpsi=factor*((-1.5*dKnpdpsi)*sqrt(1.5*Kn)-(delEn/kT-1.5*Kn)*0.75/sqrt(1.5*Kn)*dKnpdpsi);
				dtdEfn=0;
			}
			//dtdpsi1=factor*(-1.5*dKnpdpsi1*sqrt(1.5*Kn)-(delEn/kT-1.5*Kn)*0.75/sqrt(1.5*Kn)*dKnpdpsi1);
			dtdpsi1=factor*(-1.5*dKnpdpsi1*v-u*0.75/sqrt(1.5*Kn)*dKnpdpsi1);
			dtdpsim1=factor*(-1.5*dKnpdpsim1*v-u*0.75/sqrt(1.5*Kn)*dKnpdpsim1);

			factor=(0.3480242-2*0.0948798*t+3*0.7478556*t*t);
			dterm3dpsi=factor*dtdpsi;	dterm3dpsi1=factor*dtdpsi1; dterm3dEfn=factor*dtdEfn;
			dterm3dpsim1=factor*dtdpsim1; 

			factor=exp(delEn/kT-Kn);
			if(dEn)
			{	dterm4dpsi=factor*(1/kT-dKnpdpsi);	dterm4dEfn=factor*(-1/kT-dKnpdEfn);}
			else
			{	dterm4dpsi=factor*(-dKnpdpsi);		dterm4dEfn=0;}
			dterm4dpsi1=factor*(-dKnpdpsi1);
			dterm4dpsim1=factor*(-dKnpdpsim1);

			if(dEn)
			{	
				dGndpsi=1/kT*a2*a3*a4+a1*dterm2dpsi*a3*a4+a1*a2*dterm3dpsi*a4+a1*a2*a3*dterm4dpsi;
				dGndEfn=-1/kT*a2*a3*a4+a1*dterm2dEfn*a3*a4+a1*a2*dterm3dEfn*a4+a1*a2*a3*dterm4dEfn;
			}
			else
			{
				dGndpsi=a1*dterm2dpsi*a3*a4+a1*a2*dterm3dpsi*a4+a1*a2*a3*dterm4dpsi;
				dGndEfn=0;
			}
			dGndpsi1=a1*dterm2dpsi1*a3*a4+a1*a2*dterm3dpsi1*a4+a1*a2*a3*dterm4dpsi1;
			dGndpsim1=a1*dterm2dpsim1*a3*a4+a1*a2*dterm3dpsim1*a4+a1*a2*a3*dterm4dpsim1;
		}
	}
	//------------------delEp---------------
	Nroot=pow(Nv/2,1./3);
	Fr=kT*12.2799*Nroot;
	if(delEp<=0||0==F)
	{
		GammaP=0;dGpdpsi=0;dGpdpsi1=0;		
		dGpdEfp=0;dGpdpsim1=0;
	}
	else{		
		Kp=8./3*sqrt(PI/kT)*Nroot*pow(delEp,3./2)/fabs(F);
		if(Kp>=(2./3*delEp/kT))
		{
			GammaP=6.13996*fabs(F)/Fr*exp(pow(F/Fr,2));
			dGpdpsim1=-GammaP/F/s*(1+2*F*F/Fr/Fr);//E=(psi1-psi)/step
			dGpdpsi1=-dGpdpsim1;		
			dGpdpsi=0;dGpdEfp=0;
		}
		else
		{
			t=1+0.61685*(delEp/kT-1.5*Kp)/sqrt(1.5*Kp);
			t=1/t;
			a1=delEp/kT;a2=sqrt(2*PI/3/Kp);a3=(0.3480242*t-0.0948798*t*t+0.7478556*t*t*t);a4=exp(delEp/kT-Kp);
			GammaP=a1*a2*a3*a4;
			
			//factor=8./3*sqrt(PI/kT)*Nroot/F/F;
			if(dEp)//F>0
			{
				dKnpdpsi=-1.5*Kp/delEp;//dKnpdpsi=factor*sqrt(delEp)*(-1.5*F+delEp/s);//delEp=Efp-Ev
				dKnpdEfp=1.5*Kp/delEp;
			}
			else
			{	dKnpdpsi=0;//dKnpdpsi=factor*pow(delEp,1.5)/s; 
			dKnpdEfp=0;}
			dKnpdpsi1=-Kp/F/s;
			dKnpdpsim1=-dKnpdpsi1;
			/*if(F<0)
			{
				dKnpdpsi=-dKnpdpsi;		dKnpdpsi1*=-1;				
			}*/

			factor=sqrt(2*PI/3)*(-0.5)*pow(Kp,-1.5);
			dterm2dpsi=factor*dKnpdpsi;		dterm2dpsi1=factor*dKnpdpsi1; dterm2dEfp=factor*dKnpdEfp;
			dterm2dpsim1=factor*dKnpdpsim1;

			factor=-t*t*0.61685/1.5/Kp;u=(delEp/kT-1.5*Kp);v=sqrt(1.5*Kp);
			if(dEp)
			{
				dtdpsi=factor*((-1/kT-1.5*dKnpdpsi)*v-u*0.75/sqrt(1.5*Kp)*dKnpdpsi);
				dtdEfp=factor*((1/kT-1.5*dKnpdEfp)*v-u*0.75/sqrt(1.5*Kp)*dKnpdEfp);
			}
			else
			{	
				dtdpsi=factor*((-1.5*dKnpdpsi)*v-u*0.75/sqrt(1.5*Kp)*dKnpdpsi); 
				dtdEfp=0;
			}
			dtdpsi1=factor*(-1.5*dKnpdpsi1*v-u*0.75/sqrt(1.5*Kp)*dKnpdpsi1);
			dtdpsim1=factor*(-1.5*dKnpdpsim1*v-u*0.75/sqrt(1.5*Kp)*dKnpdpsim1);

			factor=(0.3480242-2*0.0948798*t+3*0.7478556*t*t);
			dterm3dpsi=factor*dtdpsi;	dterm3dpsi1=factor*dtdpsi1; dterm3dEfp=factor*dtdEfp;
			dterm3dpsim1=factor*dtdpsim1;

			factor=exp(delEp/kT-Kp);
			if(dEp)
			{	dterm4dpsi=factor*(-1/kT-dKnpdpsi);	dterm4dEfp=factor*(1/kT-dKnpdEfp);	}
			else
			{	dterm4dpsi=factor*(-dKnpdpsi);dterm4dEfp=0;}
			dterm4dpsi1=factor*(-dKnpdpsi1);
			dterm4dpsim1=factor*(-dKnpdpsim1);

			if(dEp)
			{	
				dGpdpsi=-1/kT*a2*a3*a4+a1*dterm2dpsi*a3*a4+a1*a2*dterm3dpsi*a4+a1*a2*a3*dterm4dpsi;
				dGpdEfp=1/kT*a2*a3*a4+a1*dterm2dEfp*a3*a4+a1*a2*dterm3dEfp*a4+a1*a2*a3*dterm4dEfp;
			}
			else
			{	dGpdpsi=a1*dterm2dpsi*a3*a4+a1*a2*dterm3dpsi*a4+a1*a2*a3*dterm4dpsi; dGpdEfp=0;}
			dGpdpsi1=a1*dterm2dpsi1*a3*a4+a1*a2*dterm3dpsi1*a4+a1*a2*a3*dterm4dpsi1;
			dGpdpsim1=a1*dterm2dpsim1*a3*a4+a1*a2*dterm3dpsim1*a4+a1*a2*a3*dterm4dpsim1;

		}
	}	
}
void Point2::calcSigTAT(double y,double affi,double Eg,double Et,double Nc,double Nv,double field,double s,double EcM,double EvM,
						double &sigN,double &sigP,double &dGndpsi,double &dGndpsi1,double &dGndpsim1,
						double &dGpdpsi,double &dGpdpsi1,double &dGpdpsim1,double &dGndEfn, double &dGpdEfp)
{
	double GammaN=0,GammaP=0,delEn,delEp;bool dEn=1,dEp=1;
	dGndpsi=0;dGpdpsi=0;dGndEfn=0;dGpdEfp=0;dGndpsi1=0;dGpdpsi1=0; dGndpsim1=0;dGpdpsim1=0; 
	if(0==s)
	{		
		return;
	}
	if(fabs(field)>1e4)//only in high-field region
	{
		if((y-affi-Et)>EcM){ delEn=Et; dEn=0;}	else delEn=y-affi-EcM;			
		if((y-affi-Et)>EvM) delEp=EvM-(y-affi-Eg); else {delEp=Eg-Et;dEp=0;}
	
		calcGamma(field,Nc,Nv,delEn,delEp,s,dEn,dEp,
			GammaN,GammaP,dGndpsi,dGndpsi1,dGndpsim1,dGpdpsi,dGpdpsi1,dGpdpsim1,dGndEfn,dGpdEfp);								
	}	
	sigN*=1+GammaN;sigP*=1+GammaP;
}