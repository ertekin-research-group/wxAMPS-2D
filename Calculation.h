#include "wx/progdlg.h"
#include "point.h"
#include "point2.h"
#include "resdisplay.h"
result * ThermalInit(Point*,int num,double kT,bool IsWF,double TopWF,double BtmWF,wxProgressDialog *,int &);

double norm(double **dy,int num);
void transfer(double **y1,double**y2,int num);
void clamp(double **,double, int);
void outEB(double *x,double **y1,int eleNum, char* fileName,int num);

double norm(double *y1,double *y2, int num);
void transfer(double *y1,double *y2, int num);
void clamp(double *dy,double ,int num);
void outEB(double *x,double *y1,double*y2, double *dy, double*f,int num);
void outJ(double *J,int num);
void ToRes(Point *pt,result *curRes,int num);
double Interp(vector<double> wave, vector<double> alpha, double ambWave);

double SafeDExp(double x);
double xDexM1(double x);
double Focc4(double a, double x);
double Focc1(double a, double x);
double Focc2(double a, double b, double x);
double Focc3(double a, double b, double x);

void CheckTail(bool BgShape,double Eg, double,double &Eda,
			   double &Ed,double&Ea,double &Gdo,double &Gao,double &GMgD,double &GMgA,double &ELo,double &EUp);
void Traps(double Ecf, double Efv,
		   double kT, double kTi, double Eg, double EgOpt,
		   double Gdo, double Gao,
		   double GMgD, double GMgA,
		   double Ed, double EdInv,
		   double Ea, double EaInv,
		   double Eda,int iTEq0K,double ELo,double EUp,
		   double &Pt,double&dPt,double&xNt,double&dxNt);
double GM12(double x);double GP12(double x);
void calU(double gt,double E1,double E2,double Eg,double xNc,double xNv,
					double Sig_n,double Sig_p,double Vth,double kT,double en,double pe,double ni2,
					double &xR,double &dxRdSig,bool);
void calPt(double gt,double E1,double E2,double Eg,double xNc,double xNv,
					double Sig_n,double Sig_p,double Vth,double kT,double en,double pe,double ni2,
					double &xR,double &dxRdSig);

void calptu(double gt,double E1,double E2,double Eg,double xNc,double xNv,
					double Sig_n,double Sig_p,double Vth,double kT,double en,double pe,double ni2,
					double dsigNdpsi,double dsigNdpsi1,double dsigNdpsim1,double dsigPdpsi,double dsigPdpsi1,double dsigPdpsim1,double,double,
					double &Pt,double &PtPsi, double &PtEfp,double &PtEfn, 
					double &u, double &uPsi, double &uEfp, double &uEfn,
					double &Ptpsi1,double &PtPsim1,double &uPsi1,double &uPsim1);

double Ne(double Nc,double eta);
double Neta(double Nc,double eta);double dNe(double Nc,double eta,double);
double Ph(double ,double eta);double dPh(double ,double eta,double);