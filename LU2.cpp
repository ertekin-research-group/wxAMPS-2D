#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

//for banded matrix
#include "calculation.h"
void inverse33(double *a,double *out)
{
	double det;
	det=a[0]*(a[4]*a[8]-a[5]*a[7])-a[1]*(a[3]*a[8]-a[5]*a[6])+a[2]*(a[3]*a[7]-a[4]*a[6]);
	
	out[0]=a[4]*a[8]-a[5]*a[7];out[0]/=det;
	out[1]=a[2]*a[7]-a[1]*a[8];out[1]/=det;
	out[2]=a[1]*a[5]-a[2]*a[4];out[2]/=det;
	out[3]=a[5]*a[6]-a[3]*a[8];out[3]/=det;
	out[4]=a[0]*a[8]-a[2]*a[6];out[4]/=det;
	out[5]=a[2]*a[3]-a[5]*a[0];out[5]/=det;
	out[6]=a[3]*a[7]-a[4]*a[6];out[6]/=det;
	out[7]=a[1]*a[6]-a[0]*a[7];out[7]/=det;
	out[8]=a[4]*a[0]-a[1]*a[3];out[8]/=det;
	for(int i=0;i<8;i++)
		if(!_finite(out[i]))
			out[i]=1e300;
}
void minus(double *a,double *b,int num,double *out)//out=a-b
{
	for(int i=0;i<num;i++)
		out[i]=a[i]-b[i];
}
void multiply33(double *a, double *b,double *out)//out=a*b
{
	double sum;
	for(int i=0;i<3;i++)		
		for(int j=0;j<3;j++)
		{
			sum=0;
			for(int k=0;k<3;k++)
				sum+=a[i*3+k]*b[k*3+j];
			if(_finite(sum))
				out[i*3+j]=sum;
			else
				out[i*3+j]=1e300;
		}	
}
void multiply31(double *a33, double *b31,double *out31)
{
	double sum;
	for(int i=0;i<3;i++)
	{
		sum=0;
		for(int j=0;j<3;j++)
			sum+=a33[i*3+j]*b31[j];
		if(_finite(sum))
			out31[i]=sum;
		else
			out31[i]=1e300;
	}	
}
void LUnewton(double**a,double**b,double**c,double **f,int num,double **dy)
{
	double **l=new double*[num], **u1=new double*[num],**u2=new double*[num];
	double temp[9],temp2[3];
	double **z=new double *[num];
	for(int i=0;i<num;i++)
	{
		z[i]=new double[3];
		l[i]=new double[9];u1[i]=new double[9];u2[i]=new double[9];
	}
	//LU 
	transfer(b[0],u1[0],9);transfer(c[0],u2[0],9);
	for(int i=1;i<num;i++)
	{
		//l[i]=a[i]/u1[i-1], u1[i]=b[i]-l[i]*u2[i-1]
		inverse33(u1[i-1],temp); multiply33(a[i],temp,l[i]);multiply33(l[i],u2[i-1],temp);
		minus(b[i],temp,9,u1[i]);
		//u2[i]=c[i]
		if(i<num-1)//c[num-1] undefined
			transfer(c[i],u2[i],9);
	}
	//LZ=F
	transfer(f[0],z[0],3);
	for(int i=1;i<num;i++)
	{	
		//z[i]=f[i]-l[i]z[i-1];
		multiply31(l[i],z[i-1],temp2);
		minus(f[i],temp2,3,z[i]);
	}
	//U dy=Z
	inverse33(u1[num-1],temp);multiply31(temp,z[num-1],dy[num-1]);//dy[n-1]=1/u1[n-1] f[n-1]
	for(int i=num-2;i>=0;i--)
	{
		//dy[i]=1/u1[i](z[i]-u2[i]dy[i+1])
		multiply31(u2[i],dy[i+1],temp);minus(z[i],temp,3,temp2);inverse33(u1[i],temp);
		multiply31(temp,temp2,dy[i]);
		for(int dyi=0;dyi<3;dyi++)
		{
			if(!_finite(dy[i][dyi]))
				dy[i][dyi]=1e300;
		}
	}
	//postprocess
	for(int i=0;i<num;i++)
	{
		delete []z[i];
		delete []l[i];delete[] u1[i];delete[] u2[i];
	}
	
	delete [] z;delete[] l;delete[] u1;delete [] u2;
	//delete[] temp;delete[] temp2;

}
void LU2(double *a,double *b,int n,double *x)//change a to 1 dimension array
{
	int i,j,k;double sum;
	/*double **l=new double *[n], **u=new double*[n];
	for(i=0;i<n;i++)
	{
		l[i]=new double[n];
		u[i]=new double[n];
	}*/
	double *l=new double[n*n],*u=new double[n*n];
	double *z=new double[n];

	for(i=0;i<n;i++)
		for(j=0;j<n;j++)
			l[i*n+j]=(i==j)?1.:0;
	for(i=0;i<n;i++)
		for(j=0;j<n;j++)
			u[i*n+j]=0;
	//--------------------------------------------------------------------
	//AX=B
	//Decomposition of A into L and U	
	for(i=0;i<n;i++)
	{
		for(j=i-2;j<i+2;j++)
		{
			sum=0;
			if(j>=0&&j<n)
			{
				if(i<=j)
				{
					for(k=i-2;k<i;k++) 
						if(k>=0)
							sum=sum+l[i*n+k]*u[k*n+j];
					u[i*n+j]=a[i*n+j]-sum;
				}
				else
				{
					k=j-1;//for(k=0;k<j;k++)
					if(k>=0)
						sum=sum+l[i*n+k]*u[k*n+j];
					l[i*n+j]=(a[i*n+j]-sum)/u[j*n+j];
				}
			}
		}
	}
	//--------------------------------------------------------------------
	//Now LUX=B
	//i.e LZ=B
	//Obtaining Z by forward subtitiution 
	
	z[0]=b[0];
	for(i=1;i<n;i++)
	{
		sum=0;
		for(j=i-2;j<i;j++)
		{
			if(j>=0)
				sum=sum+z[j]*l[i*n+j];			
		}
		z[i]=b[i]-sum;
	}
	
	//--------------------------------------------------------------------
	//Now UX=Z
	//Finding X by backward substitution
	x[n-1]=z[n-1]/u[n*n-1];
	for(i=n-2;i>=0;i--)
	{
		sum=0;
		//for(j=i+1;j>i;j--)
		j=i+1;
			sum=sum+x[j]*u[i*n+j];
		x[i]=(z[i]-sum)/u[i*n+i];

	}
	//--------------------------------------------------------------------
	/*for(i=0;i<n;i++)
	{
		delete[] l[i];delete[] u[i];		
	}*/
	delete[] l;
	delete[] u;
	delete[] z;
	
}

void LU2(double *a,double *b,double*l,double*u,int n,double *x)//in case of memory leaking
{
	int i,j,k;double sum;
	
	double *z=new double[n];

	for(i=0;i<n;i++)
		for(j=0;j<n;j++)
			l[i*n+j]=(i==j)?1.:0;
	for(i=0;i<n;i++)
		for(j=0;j<n;j++)
			u[i*n+j]=0;
	//--------------------------------------------------------------------
	//AX=B
	//Decomposition of A into L and U	
	for(i=0;i<n;i++)
	{
		for(j=i-2;j<i+2;j++)
		{
			sum=0;
			if(j>=0&&j<n)
			{
				if(i<=j)
				{
					for(k=i-2;k<i;k++) 
						if(k>=0)
							sum=sum+l[i*n+k]*u[k*n+j];
					u[i*n+j]=a[i*n+j]-sum;
				}
				else
				{
					k=j-1;//for(k=0;k<j;k++)
					if(k>=0)
						sum=sum+l[i*n+k]*u[k*n+j];
					l[i*n+j]=(a[i*n+j]-sum)/u[j*n+j];
				}
			}
		}
	}
	//--------------------------------------------------------------------
	//Now LUX=B
	//i.e LZ=B
	//Obtaining Z by forward subtitiution 
	
	z[0]=b[0];
	for(i=1;i<n;i++)
	{
		sum=0;
		for(j=i-2;j<i;j++)
		{
			if(j>=0)
				sum=sum+z[j]*l[i*n+j];			
		}
		z[i]=b[i]-sum;
	}
	
	//--------------------------------------------------------------------
	//Now UX=Z
	//Finding X by backward substitution
	x[n-1]=z[n-1]/u[n*n-1];
	for(i=n-2;i>=0;i--)
	{
		sum=0;
		//for(j=i+1;j>i;j--)
		j=i+1;
			sum=sum+x[j]*u[i*n+j];
		x[i]=(z[i]-sum)/u[i*n+i];

	}

	delete[] z;
	
}

					

								

			


