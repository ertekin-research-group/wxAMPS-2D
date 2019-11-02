#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "Calculation.h"
#include "LU.h"
#include "Constant.h"
#include "wx/progdlg.h"//#include "processdlg.h"
#include <iomanip>
#include <fstream>
#include <cmath>
#include <wx/msgdlg.h> 
#include <limits>
using namespace std;

result * ThermalInit(Point *pt,int PointNumber,double kT,bool IsWF,double TopWF,double BtmWF,
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
	transfer(pt->psiInit,y2,PointNumber);
	while( tol>ep)
	{
		transfer(y2,y1,PointNumber);//y1=y2;
		pt->fpsiInit(y1,PointNumber,kT,IsWF,TopWF,BtmWF,f);//f=fy(y1);
		pt->JacoInit(y1,PointNumber,kT,IsWF,J);//J=Jac(y1);
		LU2(J,f,PointNumber,dy);//y=J\f;
//		outEB(pt->x,pt->Efn,y1,dy,f,PointNumber);
		clamp(dy,0.2,PointNumber);//jo=find(abs(y)>(y1(end)-y1(1))/10);   %clamping
		//	y(jo)=(y1(end)-y1(1))/10*sign(y(jo));
		for(int i=0;i<PointNumber;i++)
		{
			y2[i]=y1[i]-dy[i];
		}//y2=y1-y;
		tol=norm(y1,y2,PointNumber);//tol=norm(y2-y1);
		
	//	outEB(pt->x,pt->Efn,y1,dy,f,PointNumber);
		cnt++;
		str.Printf(_("%d"),prgCnt++);str=title+str;
		//if(dlg!=NULL)
		if(!dlg->Update(prgCnt%100,str))
		{
			delete[] y1;delete[] y2;delete[] f;delete[] dy;
			delete[] J;
			return NULL;
		}
		if(cnt>5000)                                       
		{	
			wxMessageBox(_("too many iterations, optimize algorithm parameters"));			
			break;
			//return;
		}
	}   
	transfer(y2,pt->psiInit,PointNumber);
			
	double Ef=0;//-kT*log(pt->Nc[PointNumber-1]/pt->Nd[PointNumber-1]);
	for(int i=0;i<PointNumber;i++)
	{
		pt->psi[i]=pt->psiInit[i];
		pt->Efn[i]=0;
		pt->Efp[i]=0;
//		pt->psiOld[i]=pt->psi[i];pt->EfnOld[i]=0;pt->EfpOld[i]=0;

		pt->n0L=Ne(pt->Nc[0],(-pt->psiInit[0]+pt->affi[0])/kT);
		pt->n0R=Ne(pt->Nc[PointNumber-1],(-pt->psiInit[PointNumber-1]+pt->affi[PointNumber-1])/kT);
		pt->p0L=Ne(pt->Nv[0],(pt->psiInit[0]-pt->affi[0]-pt->Eg[0])/kT);
		pt->p0R=Ne(pt->Nv[PointNumber-1],(pt->psiInit[PointNumber-1]-pt->affi[PointNumber-1]-pt->Eg[PointNumber-1])/kT);
	}
	//----------------postprocess----------------	
	result *curRes=new result;
	curRes->v=0;
	for(int i=0;i<PointNumber;i++)
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
	delete[] y1;delete[] y2;delete[] f;delete[] dy;
	//------------------------
	delete[] J;
	return curRes;
}
void  transfer(double **y1,double**y2,int num)
{
	for(int i=0;i<num;i++)
	{
		y2[i][0]=y1[i][0];
		y2[i][1]=y1[i][1];
		y2[i][2]=y1[i][2];
	}
}
double norm(double **dy,int num)
{
	double sum=0;
	for(int i=0;i<num;i++)
	{
		sum+=dy[i][0]*dy[i][0];
		sum+=dy[i][1]*dy[i][1];
		sum+=dy[i][2]*dy[i][2];
	}
	return sqrt(sum/3/num);
}
void clamp(double **dy,double range,int num)
{
	for(int i=0;i<num;i++)
	{
		for(int j=0;j<3;j++)
		if(fabs(dy[i][j])>range)
			dy[i][j]*=range/fabs(dy[i][j]);
	}
}
void transfer(double *y1,double *y2, int num)
{
	for(int i=0;i<num;i++)
	{
		y2[i]=y1[i];
	}
}
double norm(double* y1,double *y2,int num)//second order norm
{
	double sum=0;
	for(int i=0;i<num;i++)
	{
		sum+=(y1[i]-y2[i])*(y1[i]-y2[i]);
	}
	return sqrt(sum/num);
}
void clamp(double *dy,double range,int num)
{
	//double range=0.2;//fabs(y[num-1]-y[0])/5;
	for(int i=0;i<num;i++)
	{
		if(fabs(dy[i])>range)
			dy[i]*=range/fabs(dy[i]);
	}
}
void outEB(double *x,double *y1,double*y2, double *dy, double*f,int num)
{
	ofstream outfile("temp/EB.tmp");
	outfile<<num<<endl;
	for(int i=0;i<num;i++)
	{
		outfile<<setprecision(17)<<i<<" "<<x[i]<<" "<<y1[i]<<" "<<y2[i]<<" "<<dy[i]<<" "<<f[i]<<endl;
	}
	outfile.close();
}
void outEB(double *x,double **y1,int eleNum, char* fileName,int num)
{
	ofstream outfile(fileName);
	outfile<<num<<endl;
	for(int i=0;i<num;i++)
	{
		outfile<<setprecision(17)<<i<<" "<<x[i]<<" ";
		for(int j=0;j<eleNum;j++)
			outfile<<y1[i][j]<<" ";
		outfile<<endl;
	}
	outfile.close();

}
void outJ(double *J,int num)
{
	ofstream outfile("temp/J.html");
	outfile<<"<table>"<<endl;
	for(int i=0;i<num;i++)
	{
		outfile<<"<tr>"<<endl;
		for(int j=0;j<num;j++)
		{
			outfile<<"<td>"<<J[i*num+j]<<"</td>"<<endl;
		}
		outfile<<"</tr>"<<endl;
	}
	outfile<<"</table>"<<endl;
	outfile.close();
}
double GP12(double x)//degeneracy factor
{
	double gp12;
	double dexpx=0;
	double sqrtpi=sqrt(PI);
	double a;
	double Brckt;
	
	if (x <= -4)
	{
		gp12=1;
	}
	else
	{
		if(x>700)
		{
			dexpx=1E308;
		}
		else
		{
			dexpx=exp(x);
		}
		//sqrtpi already set.
		if(x <= 10)
		{
			double dexpbr=exp(-0.17*pow(x+1,2));
			Brckt=x*(1-0.68*dexpbr);
			a=pow(x,4)+33.6*Brckt+50;
			gp12=0.75*sqrtpi*pow(a,-0.375)*dexpx+1;
			gp12=1/gp12;
		}
		else
		{
			gp12=4/3/sqrtpi*(pow(x*x+PI*PI/6,0.75))/dexpx;
		}
	}
	return(gp12);
}
double GM12(double x)//GP12+dGP12dx, d(Gp*n)
{
	double gm12;
	double a;
	double dadx;
	double Brckt;
	double DBrckt;
	double dexpx=0;
	double sqrtpi=sqrt(PI);
	

	if (x <= -4)
	{
		gm12=1;
	}
	else
	{
		if(x>700)
		{
			dexpx=1E308;
		}
		else
		{
			dexpx=exp(x);
		}
		//sqrtpi already set.
		if(x <= 10)
		{
			double dexpbr=exp(-0.17*pow(x+1,2));
			Brckt=x*(1-0.68*dexpbr);
			DBrckt=1+0.68*dexpbr*(0.34*x*(x+1)-1);
			a=pow(x,4)+33.6*Brckt+50;
			dadx=4*pow(x,3)+33.6*DBrckt;
			gm12=0.75*sqrtpi*pow(a,-0.375)*dexpx+1;
			gm12=1/gm12/gm12;
			gm12=gm12*(0.28125*sqrtpi*pow(a,-1.375)*dadx*dexpx+1);
		}
		else
		{
			gm12=(2*x/sqrtpi)*(pow(x*x+PI*PI/6,-0.25))/dexpx;
		}
	}
	return(gm12);
}
//result *ToResult(Point * pt,int PointNumber)
//{
//	result *curRes=new result;
//	for(int i=0;i<PointNumber;i++)
//	{
//		pt->Ec[i]=pt->psi[i]-pt->affi[i];//+pt->affi[PointNumber-1];
//		pt->Ev[i]=pt->Ec[i]-pt->Eg[i];
//		curRes->v.push_back(0);curRes->x.push_back(pt->x[i]*1e4);
//		curRes->Ec.push_back(pt->Ec[i]);curRes->Ev.push_back(pt->Ev[i]);
//		curRes->Efn.push_back(pt->Efn[i]);curRes->Efp.push_back(pt->Efp[i]);
//		curRes->n.push_back(log(pt->Ne[i]+1));curRes->p.push_back(log(pt->Ph[i]+1));//log
//		curRes->nt.push_back(log(pt->nt[i]+1));curRes->pt.push_back(log(pt->pt[i]+1));
//		curRes->Ntd.push_back(log(pt->Ntd[i]+1));curRes->Nta.push_back(log(pt->Nta[i]+1));
//		curRes->R.push_back(pt->R[i]);curRes->G.push_back(pt->G[i]);
//		curRes->Jn.push_back(pt->Jn[i]);curRes->Jp.push_back(pt->Jp[i]);
//	}
//}
//------free carriers, fermi distribution----------------
double Neta(double Nc,double eta)
{
	return Nc*exp(eta)*GP12(eta);
}
double Ne(double Nc,double eta)
{
	return Nc*exp(eta)*GP12(eta);
}
double Ph(double Nv,double eta)
{
	return Nv*exp(eta)*GP12(eta);
}
double dNe(double Nc,double eta,double kT)
{
	return -Nc*exp(eta)*GM12(eta)/kT;
}
double dPh(double Nv,double eta,double kT)
{
	return Nv*exp(eta)*GM12(eta)/kT;
}
double Interp(vector<double> wave, vector<double> alpha, double ambWave)
{
	double ambAlpha;
	int size=wave.size();
	if(size==0)
	{
		/*wxMessageBox("no absorption coefficient! ");*/

		return 0;
	}
	else{
		if(ambWave<=wave[0])
			return alpha[0];
		else if(ambWave>=wave[size-1])
			return 0;
		else
		{
			int i=0;
			while(ambWave>wave[i])
				i++;
			ambAlpha=alpha[i-1]+(alpha[i]-alpha[i-1])/(wave[i]-wave[i-1])*(ambWave-wave[i-1]);
			return ambAlpha;
		}
	}

}