/*
Converted to CPP 2/3/09
C***********************************************************************
C***********************************************************************
C     FILE:    calexp.f
C     VERSION: 6.1
C     Modified at 12:02:53 on 96/05/21
C     SCCS file name: /model/mar117/amps/src/sim/SCCS/s.calexp.f
C***********************************************************************
C	VAX Name: CALEXP.FOR;5					10/31/90
C       Functions in file:  FOCC1, FOCC2, and FOCC3 (double precision)
C
C***********************************************************************
C***********************************************************************
C
C	Write 1/(1+Aexp(X)) so it will not blow up on the computer.
C
*/
#include <cmath>

double Focc1(double a, double x);
double Focc2(double a, double b, double x);
double Focc3(double a, double b, double x);
double Focc4(double a,double x);
double SafeDExp(double x)	//prEvent overflow of double value.
{
	double y=0;

	if(x > 700)
		y=1E300;
	else
		y=exp(x);

	return(y);
}
double xDexM1(double x)
{
	double out ;
	
	if(fabs(x) < 1e-2)
	{
		out=1+x/2+pow(x,2)/6+pow(x,3)/24+pow(x,4)/120+pow(x,5)/720+pow(x,6)/5040+pow(x,7)/40320+pow(x,8)/362880;
		out = 1 / out;
	}
	else if(x < 40)	//just output the answer
	{
		out = x / (exp(x) - 1);
	}
	else	//x is very large, multiply num and denom by e^-x (so one)
	{
		double expMX = exp(-x);
		out = (x * expMX) / (1 - expMX);
	}
	
	return (out);
}

double Focc4(double a,double x)//for dJ/dpsi, x=+_ (psi1-psi0), a->kT
{
	double focc4;
	double y=x/a;

	if(fabs(x)<(1e-6))//big-big
		focc4=1/a*(-.5-y/12+pow(y,3)/720-pow(y,5)/30240+pow(y,7)/1209600-pow(y,9)/47900160);
	else if(y>-40)
		focc4=1/a*(1/y-1/(1-exp(-y)));
	else 
		focc4=1/a*(1/y-exp(y)/(exp(y)-1));

	return focc4;
}

double Focc1(double a, double x)
{
	double focc1=0;
	double dexpmx=0;

	if(x<40)
	{
		focc1=1/(1+a*exp(x));
	}
	else
	{
		double dexpmx=exp(-x);
		focc1=dexpmx/(dexpmx+a);
	}
	return(focc1);
}
/*

	DOUBLE PRECISION FUNCTION FOCC1(A,X)
	IMPLICIT DOUBLE PRECISION (A-H,O-Z)
C
        INCLUDE './misc/const.inc.f'
C
	IF (X.LT.40.0D0) THEN
	    FOCC1=ONE/(ONE+A*DEXP(X))
	ELSE
	    DEXPMX=DEXP(-X)
	    FOCC1=DEXPMX/(DEXPMX+A)
	END IF
	RETURN
	END
*/


//	Write (1+Aexp(X))/(1+Bexp(X)) so it will not blow up on the computer.

double Focc2(double a, double b, double x)
{
	double dexpx=0;
	double focc2=0;
	double dexpmx=0;

	if(x<40)
	{
		dexpx=exp(x);
		focc2=(1+a*dexpx)/(1+b*dexpx);
	}
	else
	{
		dexpmx=exp(-x);
		focc2=(dexpmx+a)/(dexpmx+b);
	}
	return(focc2);
}
/*
	DOUBLE PRECISION FUNCTION FOCC2(A,B,X)
	IMPLICIT DOUBLE PRECISION (A-H,O-Z)
C
        INCLUDE './misc/const.inc.f'
C
	IF (X.LT.40.0D0) THEN
	    DEXPX=DEXP(X)
	    FOCC2=(ONE+A*DEXPX)/(ONE+B*DEXPX)
	ELSE
	    DEXPMX=DEXP(-X)
	    FOCC2=(DEXPMX+A)/(DEXPMX+B)
	END IF
	RETURN
	END
*/
//	Write exp(X)(A-B)/(1+Aexp(X))/(1+Bexp(X)) so it will not blow up on the computer.
double Focc3( double a, double b, double x)
{
	double dexpx=0;
	double focc3=0;
	double dexpmx=0;
	if(x<40)
	{
		dexpx=exp(x);
		focc3=dexpx*(a-b)/(1+a*dexpx)/(1+b*dexpx);
	}
	else
	{
		dexpmx=exp(-x);
		focc3=dexpmx*(a-b)/(dexpmx+a)/(dexpmx+b);
	}
	return(focc3);
}
/*
	DOUBLE PRECISION FUNCTION FOCC3(A,B,X)
	IMPLICIT DOUBLE PRECISION (A-H,O-Z)

        INCLUDE './misc/const.inc.f'

	IF (X.LT.40.0D0) THEN
	    DEXPX=DEXP(X)
	    FOCC3=DEXPX*(A-B)/(ONE+A*DEXPX)/(ONE+B*DEXPX)
	ELSE
	    DEXPMX=DEXP(-X)
	    FOCC3=DEXPMX*(A-B)/(DEXPMX+A)/(DEXPMX+B)
	END IF
	RETURN
	END

*/