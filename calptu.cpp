#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif
#include "Calculation.h"
#include <cmath>
//calculate the integrals of Pt and Recombination rate under non-equilibrium
void calU(double gt,double E1,double E2,double Eg,double xNc,double xNv,
					double Sig_n,double Sig_p,double Vth,double kT,double en,double pe,double ni2,
					double &xR,double &dxR,bool dnp)
{
	double pnMni2=en*pe-ni2;
	double SigNP=Sig_n*Sig_p;
    double ptpre=kT*gt;
	double upre=ptpre*SigNP*Vth;
	double c1=/*GP12N0**/Sig_n*xNc;
	double c4=/*GP12P0**/Sig_p*xNv;
	double SigNn=Sig_n*en;
	double SigPp=Sig_p*pe;
	double SigNnPp=SigNn+SigPp;
//	double SigPpMNn=SigPp-SigNn;
	double SigNnPp2=SigNnPp*SigNnPp;
	double E2ME1=(E2-E1)/kT;
	double dExpme1=exp(-E1/kT);
	double dExpme2=exp(-E2/kT);
	double dExpE1ME2=exp(-E2ME1);
	double dExpE1MEg=exp((E1-Eg)/kT);
	double dExpe2meg=exp((E2-Eg)/kT);
	double dee21=2*c1*dExpE1MEg;
	double dee22=2*c1*dExpe2meg;
	double dee31=dee21+SigNnPp;
	double dee32=dee22+SigNnPp;
	double dee11=c1*dExpE1MEg+SigNnPp+c4*dExpme1;//denomerator
	double dee12=c1*dExpe2meg+SigNnPp+c4*dExpme2;

	double dee41=SigNnPp+2*c4*dExpme1;
	double dee42=SigNnPp+2*c4*dExpme2;
	double bracket4=(dee41/dee11-dee42/dee12);

	double uterm,essrt,dutermrat;
	double dutermdEfp,dutermdEfn;
	double Beta=SigNP*ni2;
	double FourBeta=4*Beta;
	double ess=SigNnPp2-FourBeta;
	
	if(ess>0)
	{
		essrt=sqrt(ess);
		double d31ps=essrt+dee31;
		double d32ps=essrt+dee32;
		double d31ms,d32ms;
		double ex=FourBeta/SigNnPp2;
		if(ex>=1e-3)
		{
			d31ms = dee31-essrt;
			d32ms = dee32-essrt;
			double y1= 2 * (dee22-dee21) / ( d31ms*d32ps);
			double essrty1 = essrt*y1;
			if(fabs(essrty1)>1e-2)
			{
				uterm=log(d31ps*d32ms/d31ms/d32ps)/essrt;
			}
			else
			{
				uterm=y1*( pow(essrty1, 8)/9 - pow(essrty1, 7)/8 + pow(essrty1, 6)/7
						  - pow(essrty1, 5)/6 + pow(essrty1, 4)/5 - pow(essrty1, 3)/4
						  + pow(essrty1, 2)/3 - pow(essrty1, 1)/2 + 1);
			}
		}
		else
		{
			double radicl=pow(ex,5)*21/1024 + pow(ex,4)*7/256 + pow(ex,3)*5/128 + pow(ex,2)/16
					+ ex/8 + 0.5;
			double y1num=d31ps*(SigNnPp+2*c4*radicl*dExpme2);
			double y1den=d32ps*(SigNnPp+2*c4*radicl*dExpme1);
			double y1=(y1num-y1den)/y1den;
			double dlogargmt;
			if( fabs(y1) >= 1e-2)
				dlogargmt=log(y1num/y1den);
			else
				dlogargmt=y1*( pow(y1, 8)/9 - pow(y1, 7)/8 + pow(y1, 6)/7 
							  - pow(y1, 5)/6 + pow(y1, 4)/5 - pow(y1, 3)/4 
							  + pow(y1, 2)/3 - y1/2 + 1);
			uterm=(E2ME1+dlogargmt)/essrt;
		}
		dutermrat=(-SigNnPp*uterm+bracket4)/ess;
		
	}
	else if(ess<0)
	{
		essrt=sqrt(-ess);
		double y2=(1-dExpE1ME2)/(2*c1*dExpE1MEg+(1+dExpE1ME2)*SigNnPp+2*c4*dExpme2);
		
		double essrty2=essrt*y2;
		double atanf21=atan(essrty2);
		if (essrty2 >= 1e-2)
			uterm=2*atanf21/essrt;
		else
		    uterm=2*y2*(pow(essrty2,8)/9 - pow(essrty2,6)/7	+ pow(essrty2,4)/5
						- pow(essrty2,2)/3 +1);
		
		
		dutermrat=(-SigNnPp*uterm+bracket4)/ess;
	}
	else
	{
		double oneddee3=2*c1*(dExpe2meg-dExpE1MEg)/dee31/dee32;
		uterm=2*oneddee3;
		dutermrat=-2*(1/dee31/dee31-1/dee32/dee32);
	}
	xR=upre*pnMni2*uterm;
	if(0==dnp)//dEfn
	{
		dutermdEfn=Sig_n*en/kT*dutermrat;//kT
		dxR=upre*(en*pe/kT*uterm+pnMni2*dutermdEfn);
	}
	else//dEfp
	{
		dutermdEfp=-Sig_p*pe/kT*dutermrat;//kT
		dxR=upre*(-en*pe/kT*uterm+pnMni2*dutermdEfp);
	}	

}
void calPt(double gt,double E1,double E2,double Eg,double xNc,double xNv,
					double Sig_n,double Sig_p,double Vth,double kT,double en,double pe,double ni2,
					double &Pt,double &dPt)
{
	double pnMni2=en*pe-ni2;
	double SigNP=Sig_n*Sig_p;
    double ptpre=kT*gt;
	double upre=ptpre*SigNP*Vth;
	double c1=/*GP12N0**/Sig_n*xNc;
	double c4=/*GP12P0**/Sig_p*xNv;
	double SigNn=Sig_n*en;
	double SigPp=Sig_p*pe;
	double SigNnPp=SigNn+SigPp;
	double SigPpMNn=SigPp-SigNn;
	double SigNnPp2=SigNnPp*SigNnPp;
	double E2ME1=(E2-E1)/kT;
	double dExpme1=exp(-E1/kT);
	double dExpme2=exp(-E2/kT);
	double dExpE1ME2=exp(-E2ME1);
	double dExpE1MEg=exp((E1-Eg)/kT);
	double dExpe2meg=exp((E2-Eg)/kT);
	double dee21=2*c1*dExpE1MEg;
	double dee22=2*c1*dExpe2meg;
	double dee31=dee21+SigNnPp;
	double dee32=dee22+SigNnPp;
	double dee11=c1*dExpE1MEg+SigNnPp+c4*dExpme1;//denomerator
	double dee12=c1*dExpe2meg+SigNnPp+c4*dExpme2;

	double dee41=SigNnPp+2*c4*dExpme1;
	double dee42=SigNnPp+2*c4*dExpme2;
	double bracket4=(dee41/dee11-dee42/dee12);

	double uterm,essrt,dutermrat;
	double ptterm1,dpttermrat,dptterm1dPsi;
	double dutermdPsi;//dutermdEfp,dutermdEfn;
	double Beta=SigNP*ni2;
	double FourBeta=4*Beta;
	double ess=SigNnPp2-FourBeta;
	double dSigNnPpdPsi=Sig_n*(-en)/kT+Sig_p*pe/kT;
	if(ess>0)
	{
		essrt=sqrt(ess);
		ptterm1=(E2ME1 + log(dee12/dee11))/2;
		double d31ps=essrt+dee31;
		double d32ps=essrt+dee32;
		double d31ms,d32ms;
		double ex=FourBeta/SigNnPp2;
		if(ex>=1e-3)
		{
			d31ms = dee31-essrt;
			d32ms = dee32-essrt;			
			double y1 = 2 * (dee22-dee21) / ( d31ms*d32ps);
			double essrty1 = essrt*y1;
			if(fabs(essrty1)>1e-2)
			{
				uterm=log(d31ps*d32ms/d31ms/d32ps)/essrt;
			}
			else
			{
				uterm=y1*( pow(essrty1, 8)/9 - pow(essrty1, 7)/8 + pow(essrty1, 6)/7
						  - pow(essrty1, 5)/6 + pow(essrty1, 4)/5 - pow(essrty1, 3)/4
						  + pow(essrty1, 2)/3 - pow(essrty1, 1)/2 + 1);
			}
		}
		else
		{
			double radicl=pow(ex,5)*21/1024 + pow(ex,4)*7/256 + pow(ex,3)*5/128 + pow(ex,2)/16
					+ ex/8 + 0.5;
			double y1num=d31ps*(SigNnPp+2*c4*radicl*dExpme2);
			double y1den=d32ps*(SigNnPp+2*c4*radicl*dExpme1);
			double y1=(y1num-y1den)/y1den;
			double dlogargmt;
			if( fabs(y1) >= 1e-2)
				dlogargmt=log(y1num/y1den);
			else
				dlogargmt=y1*( pow(y1, 8)/9 - pow(y1, 7)/8 + pow(y1, 6)/7 
							  - pow(y1, 5)/6 + pow(y1, 4)/5 - pow(y1, 3)/4 
							  + pow(y1, 2)/3 - y1/2 + 1);
			
			uterm=(E2ME1+dlogargmt)/essrt;
		}
		dutermrat=(-SigNnPp*uterm+bracket4)/ess;
	//	dutermdPsi=dSigNnPpdPsi*dutermrat;
		dpttermrat=0.5*(dee11-dee12)/(dee11*dee12);
		dptterm1dPsi=dSigNnPpdPsi*dpttermrat;
	}
	else if(ess<0)
	{
		essrt=sqrt(-ess);
		ptterm1=(E2ME1+log(dee12/dee11))*0.5;
		double y2=(1-dExpE1ME2)/(2*c1*dExpE1MEg+(1+dExpE1ME2)*SigNnPp+2*c4*dExpme2);		
		double essrty2=essrt*y2;
		double atanf21=atan(essrty2);
		if (essrty2 >= 1e-2)
			uterm=2*atanf21/essrt;
		else
		    uterm=2*y2*(pow(essrty2,8)/9 - pow(essrty2,6)/7	+ pow(essrty2,4)/5
						- pow(essrty2,2)/3 +1);
		
		dutermrat=(-SigNnPp*uterm+bracket4)/ess;
		//dutermdPsi=dSigNnPpdPsi*dutermrat;
		dpttermrat=0.5*(dee11-dee12)/(dee11*dee12);
		dptterm1dPsi=dSigNnPpdPsi*dpttermrat;
	}
	else
	{
		double oneddee3=2*c1*(dExpe2meg-dExpE1MEg)/dee31/dee32;
		uterm=2*oneddee3;
		ptterm1=log(dee32/dee31);
		dutermrat=-2*(1/dee31/dee31-1/dee32/dee32);
		dptterm1dPsi=dSigNnPpdPsi*(dee31-dee32)/(dee31*dee32);
	}
	Pt=ptpre*(ptterm1+SigPpMNn*uterm*0.5);
	dutermdPsi=dSigNnPpdPsi*dutermrat;
	dPt=ptpre*(dptterm1dPsi + ((Sig_p*pe/kT-Sig_n*(-en)/kT)*uterm+SigPpMNn*dutermdPsi)*0.5);
}

void calptu(double gt,double E1,double E2,double Eg,double xNc,double xNv,
					double Sig_n,double Sig_p,double Vth,double kT,double en,double pe,double ni2,
					double dsigNdpsi,double dsigNdpsi1,double dsigNdpsim1,double dsigPdpsi,double dsigPdpsi1,double dsigPdpsim1,double dsigNdEfn,double dsigPdEfp,//TAT
					double &Pt,double &PtPsi, double &PtEfp,double &PtEfn, 
					double &u, double &uPsi, double &uEfp, double &uEfn,
					double &PtPsi1,double &PtPsim1,double &uPsi1,double &uPsim1)
{
	double pnMni2=en*pe-ni2;
	double SigNP=Sig_n*Sig_p;
    double ptpre=kT*gt;
	double upre=ptpre*SigNP*Vth;
	double c1=/*GP12N0**/Sig_n*xNc;
	double c4=/*GP12P0**/Sig_p*xNv;
	double SigNn=Sig_n*en;
	double SigPp=Sig_p*pe;
	double SigNnPp=SigNn+SigPp;
	double SigPpMNn=SigPp-SigNn;
	double SigNnPp2=SigNnPp*SigNnPp;
	double E2ME1=(E2-E1)/kT;
	double dExpme1=exp(-E1/kT);
	double dExpme2=exp(-E2/kT);
	double dExpE1ME2=exp(-E2ME1);
	double dExpE1MEg=exp((E1-Eg)/kT);
	double dExpe2meg=exp((E2-Eg)/kT);
	double dee21=2*c1*dExpE1MEg;
	double dee22=2*c1*dExpe2meg;
	double dee31=dee21+SigNnPp;
	double dee32=dee22+SigNnPp;
	double dee11=c1*dExpE1MEg+SigNnPp+c4*dExpme1;//denomerator
	double dee12=c1*dExpe2meg+SigNnPp+c4*dExpme2;

	double dee41=SigNnPp+2*c4*dExpme1;
	double dee42=SigNnPp+2*c4*dExpme2;
	double bracket4=(dee41/dee11-dee42/dee12);

	double uterm,essrt,dutermrat;
	double ptterm1,dpttermrat,dptterm1dPsi,dptterm1dEfn,dptterm1dEfp; double dptterm1dPsi1,dptterm1dPsim1;
	double dutermdPsi,dutermdEfp,dutermdEfn;	double dutermdPsi1,dutermdPsim1;
	double Beta=SigNP*ni2;
	double FourBeta=4*Beta;
	double ess=SigNnPp2-FourBeta;
	double dSigNnPpdPsi=dsigNdpsi*en+Sig_n*(-en)/kT+dsigPdpsi*pe+Sig_p*pe/kT;//Sig_n*(-en)/kT+Sig_p*pe/kT;
	double dSigNnPpdPsi1=dsigNdpsi1*en+dsigPdpsi1*pe;//TAT
	double dSigNnPpdPsim1=dsigNdpsim1*en+dsigPdpsim1*pe;

	double dSigNnPpdEfp=dsigPdEfp*pe+Sig_p*(-pe)/kT;//dPdEfp;
	double dSigNnPpdEfn=dsigNdEfn*en+Sig_n*en/kT;
	double dSigPpMNndPsi=Sig_p*(pe/kT)-Sig_n*(-en)/kT+dsigPdpsi*pe-dsigNdpsi*en;
	double dSigPpMNndPsi1=dsigPdpsi1*pe-dsigNdpsi1*en;	double dSigPpMNndPsim1=dsigPdpsim1*pe-dsigNdpsim1*en;
	double dSigPpMNndEfp=dsigPdEfp*pe+Sig_p*(-pe/kT);
	double dSigPpMNndEfn=-Sig_n*en/kT-dsigNdEfn*en;
	if(ess>0)
	{
		essrt=sqrt(ess);
		ptterm1=(E2ME1 + log(dee12/dee11))/2;
		double d31ps=essrt+dee31;
		double d32ps=essrt+dee32;
		double d31ms,d32ms;
		double ex=FourBeta/SigNnPp2;
		if(ex>=1e-3)
		{
			d31ms = dee31-essrt;
			d32ms = dee32-essrt;			
			double y1 = 2 * (dee22-dee21) / ( d31ms*d32ps);
			double essrty1 = essrt*y1;
			if(fabs(essrty1)>1e-2)
			{
				uterm=log(d31ps*d32ms/d31ms/d32ps)/essrt;
			}
			else
			{
				uterm=y1*( pow(essrty1, 8)/9 - pow(essrty1, 7)/8 + pow(essrty1, 6)/7
						  - pow(essrty1, 5)/6 + pow(essrty1, 4)/5 - pow(essrty1, 3)/4
						  + pow(essrty1, 2)/3 - pow(essrty1, 1)/2 + 1);
			}
		}
		else
		{
			double radicl=pow(ex,5)*21/1024 + pow(ex,4)*7/256 + pow(ex,3)*5/128 + pow(ex,2)/16
					+ ex/8 + 0.5;
			double y1num=d31ps*(SigNnPp+2*c4*radicl*dExpme2);
			double y1den=d32ps*(SigNnPp+2*c4*radicl*dExpme1);
			double y1=(y1num-y1den)/y1den;
			double dlogargmt;
			if( fabs(y1) >= 1e-2)
				dlogargmt=log(y1num/y1den);
			else
				dlogargmt=y1*( pow(y1, 8)/9 - pow(y1, 7)/8 + pow(y1, 6)/7 
							  - pow(y1, 5)/6 + pow(y1, 4)/5 - pow(y1, 3)/4 
							  + pow(y1, 2)/3 - y1/2 + 1);
			
			uterm=(E2ME1+dlogargmt)/essrt;
		}
		dutermrat=(-SigNnPp*uterm+bracket4)/ess;
		dutermdPsi=dSigNnPpdPsi*dutermrat;		dutermdPsi1=dSigNnPpdPsi1*dutermrat;	dutermdPsim1=dSigNnPpdPsim1*dutermrat;
		dutermdEfp=dSigNnPpdEfp*dutermrat;
		dutermdEfn=dSigNnPpdEfn*dutermrat;

		dpttermrat=0.5*(dee11-dee12)/(dee11*dee12);
		dptterm1dPsi=dSigNnPpdPsi*dpttermrat;	dptterm1dPsi1=dSigNnPpdPsi1*dpttermrat;	dptterm1dPsim1=dSigNnPpdPsim1*dpttermrat;	
		dptterm1dEfp=dSigNnPpdEfp*dpttermrat;
		dptterm1dEfn=dSigNnPpdEfn*dpttermrat;
	}
	else if(ess<0)
	{
		essrt=sqrt(-ess);
		ptterm1=(E2ME1+log(dee12/dee11))*0.5;
		double y2=(1-dExpE1ME2)/(2*c1*dExpE1MEg+(1+dExpE1ME2)*SigNnPp+2*c4*dExpme2);		
		double essrty2=essrt*y2;
		double atanf21=atan(essrty2);
		if (essrty2 >= 1e-2)
			uterm=2*atanf21/essrt;
		else
		    uterm=2*y2*(pow(essrty2,8)/9 - pow(essrty2,6)/7	+ pow(essrty2,4)/5
						- pow(essrty2,2)/3 +1);
		
		dutermrat=(-SigNnPp*uterm+bracket4)/ess;
		dutermdPsi=dSigNnPpdPsi*dutermrat;			dutermdPsi1=dSigNnPpdPsi1*dutermrat;	dutermdPsim1=dSigNnPpdPsim1*dutermrat;
		dutermdEfp=dSigNnPpdEfp*dutermrat;
		dutermdEfn=dSigNnPpdEfn*dutermrat;
		
		dpttermrat=0.5*(dee11-dee12)/(dee11*dee12);
		dptterm1dPsi=dSigNnPpdPsi*dpttermrat;		dptterm1dPsi1=dSigNnPpdPsi1*dpttermrat;	dptterm1dPsim1=dSigNnPpdPsim1*dpttermrat;	
		dptterm1dEfp=dSigNnPpdEfp*dpttermrat;
		dptterm1dEfn=dSigNnPpdEfn*dpttermrat;
	}
	else
	{
		double oneddee3=2*c1*(dExpe2meg-dExpE1MEg)/dee31/dee32;
		uterm=2*oneddee3;
		ptterm1=log(dee32/dee31);
		double oneddee3sq=1/dee31/dee31-1/dee32/dee32;

		dutermdPsi=-2*oneddee3sq*dSigNnPpdPsi;					dutermdPsi1=-2*oneddee3sq*dSigNnPpdPsi1;	dutermdPsim1=-2*oneddee3sq*dSigNnPpdPsim1;
		dutermdEfp=-2*oneddee3sq*dSigNnPpdEfp;
		dutermdEfn=-2*oneddee3sq*dSigNnPpdEfn;

		dptterm1dPsi=dSigNnPpdPsi*(dee31-dee32)/(dee31*dee32);	dptterm1dPsi1=dSigNnPpdPsi1*(dee31-dee32)/(dee31*dee32);	dptterm1dPsim1=dSigNnPpdPsim1*(dee31-dee32)/(dee31*dee32);
		dptterm1dEfp=dSigNnPpdEfp*(dee31-dee32)/(dee31*dee32);
		dptterm1dEfn=dSigNnPpdEfn*(dee31-dee32)/(dee31*dee32);
	}	
	//dPt=ptpre*(dptterm1dPsi + ((Sig_p*pe/kT-Sig_n*(-en)/kT)*uterm+SigPpMNn*dutermdPsi)*0.5);
	u=upre*pnMni2*uterm;
	Pt=ptpre*(ptterm1+SigPpMNn*uterm*0.5);

	uPsi=upre*(pnMni2*dutermdPsi)+u/Sig_n*dsigNdpsi+u/Sig_p*dsigPdpsi;	
	uPsi1=upre*(pnMni2*dutermdPsi1)+u/Sig_n*dsigNdpsi1+u/Sig_p*dsigPdpsi1;	uPsim1=upre*(pnMni2*dutermdPsim1)+u/Sig_n*dsigNdpsim1+u/Sig_p*dsigPdpsim1;
	uEfp=upre*(-en*pe/kT*uterm+pnMni2*dutermdEfp)+u/Sig_p*dsigPdEfp;
	uEfn=upre*(en*pe/kT*uterm+pnMni2*dutermdEfn)+u/Sig_n*dsigNdEfn;

	PtPsi=ptpre*(dptterm1dPsi + (dSigPpMNndPsi*uterm+SigPpMNn*dutermdPsi)*0.5);	
	PtEfp=ptpre*(dptterm1dEfp + (dSigPpMNndEfp*uterm+SigPpMNn*dutermdEfp)*0.5);
	PtEfn=ptpre*(dptterm1dEfn + (dSigPpMNndEfn*uterm+SigPpMNn*dutermdEfn)*0.5);
	PtPsi1=ptpre*(dptterm1dPsi1 + (dSigPpMNndPsi1*uterm+SigPpMNn*dutermdPsi1)*0.5);	PtPsim1=ptpre*(dptterm1dPsim1 + (dSigPpMNndPsim1*uterm+SigPpMNn*dutermdPsim1)*0.5);
}