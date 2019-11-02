#pragma once
#include "point.h"
#include "resdisplay.h"
#include "wx/progdlg.h"

class Point2 :
	public Point
{
public:
	Point2(MyList &LayerList,double);//:Point(LayerList,kT);
	~Point2(void);

	void ToPoint(Point & pt);
	void FromPoint(Point &pt);
	bool ExportG(double TopRF,double BtmRF,vector<double> photo0,vector<double> waveLength);
	void MemClear(double **y,double**dy,double**,double **f,double **a,double **b,double **c,double *cn,double *cp,int);	
	result * ThermalInit(bool IsWF,double TopWF,double BtmWF,
					 wxProgressDialog *dlg,int &prgCnt);
	void fpsiInit(double *y, int num,double kT,bool IsWF,double TopWF,double BtmWF,double *f);
	void JacoInit(double *y,int num,double kT,bool IsWF,double *J);
	void DefChargeEqui(double*y,int num, double kT);
	void DefChargeNon(double *n,double*p,int num,double kT);
	result * NonEquiNew(double vOld, double v, /*int num,*/
				/*bool IsWF,*/double TopWF,double TopSn,double TopSp,
				double BtmWF,double BtmSn,double BtmSp,wxProgressDialog *dlg,wxString str,int &prgCnt);
	void fnewton(double** y, int num,double dv,double *cn,double *cp,double TopSn,double TopSp,double BtmSn,double BtmSp,double **f);
	void Jnewton(double**y ,int num,double *cn,double *cp,double TopSn,double TopSp,double BtmSn,double BtmSp,double **a,double **b,double **c);

	void DefectNon(double **y,double *,double *,int,double);
	void calcGamma(double F, double Nc,double Nv,double delEn,double delEp,double s,bool dEn,bool dEp,
		double & GammaN,double &GammaP,double & dGndpsi,double & dGndpsi1,double &,double & dGpdpsi,double & dGpdpsi1,double &,double &dGndEfn, double &dGpdEfp);
	void calcSigTAT(double y,double affi,double Eg,double Et,double Nc,double Nv,double field,double s,double EcM,double EvM,
						double & SigN,double & SigP,double & dGndpsi,double & dGndpsi1,double&,double & dGpdpsi,double & dGpdpsi1,double &,double &dGndEfn, double &dGpdEfp);

	double * psiOld,*EfnOld,*EfpOld;
	double * un0,*up0;
	
private:
	vector<int> bIndex;vector<int> juncEdge; vector<double> EcMin, EvMax;	
	double kT;
	double * dntdpsi,*dntdEfn,*dntdEfp,*dptdpsi,*dptdEfn,*dptdEfp;
	double * dNddpsi,*dNddEfn,*dNddEfp,*dNadpsi,*dNadEfn,*dNadEfp;
	double * dRdpsi,*dRdEfn,*dRdEfp;
	double * dRdpsi1,*dntdpsi1,*dptdpsi1,*dNddpsi1,*dNadpsi1;
	double * dRdpsim1,*dntdpsim1,*dptdpsim1,*dNddpsim1,*dNadpsim1;
};
