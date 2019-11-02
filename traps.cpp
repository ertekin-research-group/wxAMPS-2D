/*
Converted to C++ by Dan Heinzel
6/8/09 
Modified by Yiming Liu
Calculating the charge introduced by band tails
5/2/10
*/
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "Calculation.h"
#define NTAILS 20

void Rctngl (double Gt, double E1, double E2, double kT,
			 double kTi, double Ef, double &TrCh, double &dTrCh);

void Traps(double Ecf, double Efv,
		   double kT, double kTi, double Eg, double EgOpt,
		  //int iEgDif, double EgApp, 
		    double Gdo, double Gao,
		   double GMgD, double GMgA,
		   double Ed, double EdInv,
		   double Ea, double EaInv,
		   double Eda,int iTEq0K,double ELo,double EUp,
		   double &Pt,double&dPt,double&xNt,double&dxNt)
{
	double EgDif = 0;

	/*if(iEgDif == 1 && Eg > EgOpt)	??iEgDif==0 in the code, ELow will equal ELo
		EgDif = Eg - EgOpt;*/

	double EgVal = 0;//EgApp * EgDif;
	double EgCon = 0;//EgApp * EgDif;
	double ELow = EgVal + ELo;
	double EUpp = EgCon + EUp;
	double EdStep, EaStep;
	double Gt;
	double E1, E2;
	double TrCh, dTrCh;

	double xNtTl,PtTl,xNtMd,PtMd;
	{
		if(iTEq0K == 1)
		{	//use t=0k approximation.  Find trapped hole density.
			if(Efv >= Eda) //Efv > switchover energy, tail states/flat region don't contribute.
			{
				PtTl = 0;	//tail states equal 0.
				PtMd = 0;	//flat region states =0.
				dPt = 0;		//derivative of tail states =0.
			}
			else if(Efv < Eda && Efv >= ELow)	//Efv < switchover, > lower boundary flat
			{											//flat region only contributes.
				PtTl = 0;	//tail states =0.
				PtMd = GMgD * (Eda - Efv);	//contribution flat region
				dPt = GMgD;	//derivative.
			}
			else if(Efv < ELow && Efv >= EgVal)	//Efv < lower boundary flat, > EgVal
			{											//max contribute from flat. Partial from exponential region
				double EfvDxp = exp(-(Efv - EgVal) * EdInv);	//partial contribution tail states
				PtTl = Gdo * Ed * (EfvDxp - exp(-ELo * EdInv));	
				PtMd = GMgD * (Eda - ELow);	//max cont. flat region
				dPt = Gdo  * EfvDxp;
			}
			else if(Efv < EgVal && Efv >= 0)	//Efv < Egval, Efv>0, max contributions both
			{										//flat and exponential regions
				PtTl = Gdo * Ed * (1 - exp(-ELo * EdInv)) + Gdo * (EgVal - Efv);
				//Max Contribution from Tail states
				PtMd = GMgD * (Eda - ELow);	//max contribution from flat region
				dPt = Gdo ;
			}
			else if (Efv < 0)
			{	//max contribution from tail states
				PtTl = Gdo * Ed * (1- exp(-ELo * EdInv)) + Gdo * EgVal;
				PtMd = GMgD * (Eda - ELow);	//max contribution from flat region
				dPt = 0;	//derivative = 0.
			}

			//compute trapped electron density.
			//method similar to above. see above comments.

			if(Ecf >= (Eg - Eda))
			{
				xNtTl = 0;
				xNtMd = 0;
				dxNt = 0;
			}
			else if(Ecf < (Eg - Eda) && Ecf >= EUpp)
			{
				xNtTl = 0;
				xNtMd = GMgA * (Eg - Eda - Ecf);
				dxNt = -GMgA ;
			}
			else if(Ecf < EUpp && Ecf >= EgCon)
			{
				double EcfAxp = exp(-(Ecf - EgCon) * EaInv);
				xNtTl = Gao * Ea * (EcfAxp - exp(-EUp * EaInv));
				xNtMd = GMgA * (Eg - Eda - EUpp);
				dxNt = -Gao  * EcfAxp;
			}
			else if(Ecf < EgCon && Ecf >= 0)
			{
				xNtTl = Gao * Ea * (1 - exp(-EUp * EaInv));
				xNtMd = GMgA * (Eg - Eda - EUpp);
				dxNt = -Gao ;
			}
			else if(Ecf < 0)
			{
				xNtTl = Gao * Ea * (1 - exp(-EUp * EaInv)) + Gao * EgCon;
				xNtMd = GMgA * (Eg - Eda - EUpp);
				dxNt =0;
			}
		}
		else		//no T=0k approximation.
		{
			//if(j != jl)
			{	//divide ELo-Ef and EUp-Ec into NTAILS Steps
				EdStep = ELo / NTAILS;
				EaStep = EUp / NTAILS;
			}
			//Compute trapped hole concentration in midgap states
			E1 = ELow;		//lower boundary of midgap region J
			E2 = Eda;	//switch over energy in region J
			Gt = GMgD;	//MidGap Density of states in region J

			Rctngl(Gt, E1, E2, kT, kTi, Efv, TrCh, dTrCh);

			PtMd = TrCh;	//trapped holes in midgap states
			dPt = dTrCh;	//derivative of trapped holes in midgap states
			E1 = EUpp;		//upper boundary of midgap states
			E2 = Eg - Eda;	//switchover energy in region J
			Gt = GMgA;	//midgap density of states in region J

			Rctngl(Gt, E1, E2, kT, kTi, Ecf, TrCh, dTrCh);

			xNtMd= TrCh;	//trapped electrons in midgap states 
			dxNt = -dTrCh;	//derivative of trapped electrons in midgap states	dTrCh=d(TrCh)/d(E2-Ef), dxNt=dTrCh((Ec-E2)-(Ec-Ef))/dE2
			PtTl = 0;
			xNtTl = 0;

			//compute tail states. sum contributions at intervals of EdStep 0...ELo, or 0...EUp

			if(EdStep > 0)
			{
				for(int ie = 1; ie <= NTAILS; ie++)	//ie not used in arrays.
				{
					E1 = (ie - 1) * EdStep;	//lower limit integral
					E2 = ie * EdStep;	//upper limit integral
					Gt = Gdo * Ed / EdStep * (exp(-E1 * EdInv) - exp(-E2 * EdInv));//average,? approximation
					//density of states in region J
					E1 += EgVal;
					E2 += EgVal;

					Rctngl(Gt, E1, E2, kT, kTi, Efv, TrCh, dTrCh);

					PtTl += TrCh;	//trapped holes in tail states
					dPt += dTrCh;	//derivative trapped holes in tail states
				}
			}
			if(EaStep > 0)
			{
				for(int ie = 1; ie <= NTAILS; ie++)	//ie not used in arrays.
				{
					E1 = (ie - 1) * EaStep;	//lower limit integral
					E2 = ie * EaStep;	//upper limit integral
					Gt = Gao * Ea / EaStep * (exp(-E1 * EaInv) - exp(-E2 * EaInv));
					//density of states in region J
					E1 += EgCon;
					E2 += EgCon;

					Rctngl(Gt, E1, E2, kT, kTi, Ecf, TrCh, dTrCh);

					xNtTl += TrCh;	//trapped e- concentration
					dxNt -= dTrCh;	//derivative trapped e- concentration  
				}
			}
			E1 = 0;
			E2 = EgVal;
			Gt = Gdo;

			Rctngl(Gt, E1, E2, kT, kTi, Efv, TrCh, dTrCh);

			PtTl += TrCh;
			dPt += dTrCh;
			E1 = 0;
			E2 = EgCon;
			Gt = Gao;

			Rctngl(Gt, E1, E2, kT, kTi, Ecf, TrCh, dTrCh);

			xNtTl += TrCh;
			dxNt -= dTrCh;
		}	//end else (T != 0k)
	}
	//else	//polycrystalline material, no computations.
	//{
	//	PtTl = 0;
	//	xNtTl = 0;
	//	PtMd = 0;
	//	xNtMd = 0;
	//	dPt = 0;
	//	dxNt = 0;
	//}

	Pt = PtTl + PtMd;	//total trapped hole concentration
	xNt = xNtTl + xNtMd;	//total trapped electron concentration


	return;
}

void Rctngl (double Gt, double E1, double E2, double kT, double kTi, double Ef, double &TrCh, double &dTrCh)
{
	//compute trapped charge for a rectangle of states ranging in energy from E1 to E2,
	//with density per energ yGt
	//Mathematica:	Integrate[1/(1+Exp[(Ef-E)/kT]),{E,E1,E2}]
	//trapped charge Concentration.
	double E1F = kTi * (E1 - Ef);
	double E2F = kTi * (E2 - Ef);
	double ExpE12 = exp(E1F - E2F);
	double Term1 = log(Focc2(1, ExpE12, E2F));
	TrCh = Gt * kT * Term1;
	//derivative of trapped charge.
	double Term2 = Focc3(1, ExpE12, E2F);
	dTrCh = Gt * Term2;

	return;
}

void CheckTail(bool BgShape,double Eg, double EgOpt,double &Eda,
			   double &Ed,double&Ea,double &Gdo,double &Gao,double &GMgD,double &GMgA,double &ELo,double &EUp)
{
	double EgCon,EgVal, EgDif;
	const double EgApp=0.5;
	//if(Eg[i] <= EgOpt)
	EgDif = 0;	//optical bandgap cannot exceed mobility gap
	/*else
	EgDif = Eg[i] - EgOpt;*/	//assumption: band tail starts from Eg, not EgOpt
	EgVal = EgApp * EgDif;	EgCon = EgDif - EgVal;
	//check for input error
	if(GMgD == 0)				GMgD = 1;
	if(GMgA == 0)				GMgA = 1;
	if(Gdo < GMgD)				Gdo = GMgD;
	if(Gao < GMgA)				Gao = GMgA;
	if(Ed== 0)	//prevent divide by zeros? donor level can't be exactly at band edge
		Ed = 1e-4;
	if(Ea == 0)				Ea = 1e-4;

	double eMeet = Ed * ((Eg-EgDif) + Ea * log(Gdo/Gao)) / (Ed + Ea);//energy level at which tail states meet if extended
	double gMeet = Gdo * exp(-eMeet/Ed);				//density of states at eMeet
	eMeet += EgVal;	//put it in the proper reference
	
	if(BgShape)	//"U" shaped distribution
	{
		ELo = Ed * log(Gdo/GMgD);	//distance from opical valence band to lower boundary of midgap states
		EUp = Ea * log(Gao/GMgA);	//" " " conduction " " upper " " " "
		double ELow = EgVal + ELo;		//distance from mobility valence band to lower boundary of midgap states
		double EUpp = EgCon + EUp;		//" " " conduction " " upper " " " "

		if(ELow < (Eg - EUpp))
		{
			if(Eda < ELow)
				Eda = ELow;
			if(Eda > (Eg - EUpp))
				Eda = Eg - EUpp;
		}
		else	//really a "V" shaped distribution
		{
			GMgD = gMeet;
			GMgA = gMeet;
			Eda = eMeet;
			ELo = Ed * log(Gdo / GMgD);
			EUp = Ea * log(Gao / GMgA);
		}
	}
	else	//V shaped
	{
		GMgD = gMeet;
		GMgA = gMeet;
		Eda = eMeet;
		ELo = Ed * log(Gdo / GMgD);
		EUp = Ea * log(Gao / GMgA);
	}
}













