
#include "resdisplay.h"
#include "wx/progdlg.h"

result* NonEquiInit(double v,  Point* pt,  int PointNumber,double kT,
				bool IsWF,double TopWF,double TopSn,double TopSp,
				double BtmWF,double BtmSn,double BtmSp,
				wxProgressDialog *dlg,wxString str,int &);
result* NonEquiTun(double v,  Point* pt,  int PointNumber,double kT,
				bool IsWF,double TopWF,double TopSn,double TopSp,
				double BtmWF,double BtmSn,double BtmSp,
				wxProgressDialog *dlg,wxString str,int &);
void NonEqui(double v,  Point* pt,  int PointNumber,double kT);
void fpsiInit(Point* pt,double *y,int num,double kT,double *f);
	void JacoInit(Point* pt,double *y,int num,double kT,double *J);

	void IterN(Point* pt,double *y, double*,double*,double*,int num,double kT,wxProgressDialog *dlg);
	void fn(Point* pt,double *y, double *Efn1, double *p1,
		  int num,double kT,double *c,double *f);
	void	 Jacn(Point* pt,double *y, double *Efn1, double *p1,
		  int num,double kT,double *c,double *J);
	
	void IterP(Point* pt,double *y, double*,double*,double*,int num,double kT,wxProgressDialog *dlg);
	void fp(Point* pt,double *y, double *Efp1, double *n1,
		  int num,double kT,double *c,double *f);
	void	 Jacp(Point* pt,double *y, double *Efp1, double *n1,
		  int num,double kT,double *c,double *J);

	void IterPsi(Point* pt,double v,double *y, double*,double*,double*,int num,double kT,wxProgressDialog *dlg);
	void fpsi(Point* pt,double v,double *y,int num,double kT,double *f);
	void Jpsi(Point* pt,double *y,int num,double kT,double *J);
