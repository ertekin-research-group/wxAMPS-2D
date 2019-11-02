#pragma once
#include <fstream>
#include "wx/progdlg.h"
#include "materialDlg.h"
#include <wx/dynarray.h>

using namespace std;

#define MGAUSS 23
#define NTAILS 20

class Point
{
public:
	Point::Point();
	Point(int num,ifstream& file,double kT);
	Point(MyList &LayerList,double kT);
	~Point(void);	

	void fpsiInit(double *y,int num,double kT,bool IsWF,double TopWF,double BtmWF,double *f);
	void JacoInit(double *y,int num,double kT,bool IsWF,double *J);

	bool IterN(double *y, double*,double*,double*,int num,double kT,
		double TopSn,double BtmSn,wxProgressDialog *dlg);
	void fn(double *y, double *Efn1,double*, double *p1,
		  int num,double kT,double TopSn,double BtmSn,
		  double *c,double *f);
	void	 Jacn(double *y, double *Efn1, double *,double *p1,
		  int num,double kT,double TopSn,double BtmSn,double *c,double *J);
	bool IterN(double v,double *y, double*,double*,double*,int num,double kT,
		double TopSn,double BtmSn,wxProgressDialog *dlg);
	void fn(double v,double *y, double *Efn1,double*u, double*n1,double *p1,
		  int num,double kT,double TopSn,double BtmSn,
		  double *c,double *f);
	void	 Jacn(double v,double *y, double *Efn1, double *u,double *n1,double *p1,
		  int num,double kT,double TopSn,double BtmSn,double *c,double *J);

	
	bool IterP(double v,double *y, double*J,double*l,double*u, int num,double kT,double TopSp,double BtmSp,wxProgressDialog *dlg);
	void fp(double v,double *y, double *Efp1,double*, double *n1,
		  int num,double kT,double TopSp,double BtmSp,double *c,double *f);
	void Jacp(double v,double *y, double *Efp1, double*,double *n1,
		  int num,double kT,double TopSp,double BtmSp,double *c,double *J);
	bool IterP(double *y, double*,double*,double*,int num,double kT,double TopSp,
				double BtmSp,wxProgressDialog *dlg);
	void fp(double *y, double *Efp1, double *n1,
		  int num,double kT,double TopSp,double BtmSp,double *c,double *f);
	void	 Jacp(double *y, double *Efp1, double *n1,
		  int num,double kT,double TopSp,double BtmSp,double *c,double *J);

	bool IterPsi(double v,double *y, double*,double*,double*,int num,double kT,
		wxProgressDialog *dlg);
	void fpsi(double v,double *y,int num,double kT,double *f);
	void Jpsi(double *y,int num,double kT,double *J);


	double *x,  *step;//?instead by vector
	double *psiInit;
	double * psiOld,*EfnOld,*EfpOld;
	double *psi, *Efn,*Efp;double *Ec,*Ev;
	double *eps, *Eg, *affi, *Nc, *Nv, *un, *up, *Nd, *Na;	
	double *Ne, *Ph,*Ntd,*Nta;double *dNd, *dNa;
	double *nt,*pt;double * dnt,*dpt;
	double *ni;//*taon, *taop;
	double *Jn,*Jp;
	double *G;double *R,*dR;
	double *tunC,*tunCp;
	double *EleF,*GammaN,*GammaP;
	double *nInit,*pInit;
	

	double n0L,n0R,p0L,p0R;
	int PointNumber;
	double precision,F0,limit,iterTimes;
	MyList * layerListP;
	void InitValue(int num);
	void DefChargeEqui(double *y,int num,double kT);
	void DefDEqui(int dist,double N,double Efd,double w,double kT,double &xNd,double &dxNd);
	void DefAEqui(int dist,double N,double Eaf,double w,double kT,double &xNa,double &dxNa);
	void Recombination(bool dnp,double *n,double *p,int,double kT);
	void DefChargeNon(double *n,double*p,int num,double kT);
//--------------tunneling---------------
	void IterNTun(double *y, double*,double*,double*,int num,double kT,
		double TopSn,double BtmSn,wxProgressDialog *dlg);
	void fn3(double *y, double *Efn1,double*, double *p1,
		  int num,double kT,double TopSn,double BtmSn,
		  double *c,double *f);
	void	 Jacn3(double *y, double *Efn1, double *,double *p1,
		  int num,double kT,double TopSn,double BtmSn,double *c,double *J);
	
	void calcTunC(double * y,double kT);
	double WKB(double *y,int start,int cur,double kT);
	double WKBP(double *y,int start,int cur,double kT);
	void calcGamma(double *y,double kT);
};

