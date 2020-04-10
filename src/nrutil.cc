#include "cmath"
#include "cstdio"
#include "cstddef"
#include "cstdlib"
#include "nrutil.hh"


double *NRUTIL::vector(int nl, int nh)
{
	double *v;

	v=(double *)malloc((size_t) ((nh-nl+NR_END)*sizeof(double)));
	if (!v) nrerror("allocation failure in vector()");
	return v-nl+NR_END;
}

int *NRUTIL::ivector(int nl, int nh)
{
	int *v;

	v=(int *)malloc((size_t) ((nh-nl+NR_END)*sizeof(int)));
	if (!v) nrerror("allocation failure in vector()");
	return v-nl+NR_END;
}

void NRUTIL::free_vector(double *v, int nl, int nh)
{
	free((FREE_ARG) (v+nl-NR_END));
	return;
}

void NRUTIL::free_ivector(int *v, int nl, int nh)
{
	free((FREE_ARG) (v+nl-NR_END));
	return;
}

void NRUTIL::nrerror(char error_text[])
{
	printf(" %s \n", error_text);
	exit(1);
}

double **NRUTIL::matrix(int nrl, int nrh, int ncl, int nch)
{
	int i, nrow=nrh-nrl+1,ncol=nch-ncl+1;
	double **m;

	m=(double**) malloc((size_t)(nrow+NR_END)*sizeof(double*));
	if (!m)
			nrerror("allocation error 1 in matrix");

	m+=NR_END;
	m -= nrl;

	m[nrl]= (double *) malloc ( (size_t) ((nrow*ncol+NR_END)*sizeof(double)));
	if (!m[nrl])
			nrerror("allocation erros 2 in matrix");

	m[nrl] += NR_END;
	m[nrl] -= ncl;

	for (i=nrl+1;i<=nrh;i++)
		m[i]=m[i-1]+ncol;

	return m;
}
double ***NRUTIL::field(int nrl, int nrh, int ncl, int nch, int nzl, int nzh)
{
	int i,j, nrow=nrh-nrl+1,ncol=nch-ncl+1,nz=nzh-nzl+1;
	double ***f;

	f = (double***) malloc ((size_t) ((nrow+NR_END)*sizeof(double**)));
	if (!f)
		nrerror("allocation error 1 in field");
	f+=NR_END;
	f-=nrl;

	f[nrl]=(double**) malloc((size_t)((nrow*ncol+NR_END)*sizeof(double*)));
	if (!f[nrl])
		nrerror("alloacation error 2 in field");
	f[nrl]+=NR_END;
	f[nrl]-=ncl;

	f[nrl][ncl]= (double *) malloc ((size_t)((nrow*ncol*nz+NR_END)*sizeof(double)));
	if (!f[nrl][ncl])
		nrerror("allocation error 3 in field");
	f[nrl][ncl]+=NR_END;
	f[nrl][ncl]-= nzl;

	for (i=ncl+1;i<=nch;i++)
		f[nrl][i]=f[nrl][i-1]+nz;
	for (j=nrl+1;j<=nrh;j++)
	{
		f[j]=f[j-1]+ncol;
		f[j][ncl]=f[j-1][ncl]+nz*ncol;
		for (i=ncl+1;i<=nch;i++)
			f[j][i]=f[j][i-1]+nz;
	}

	return f;
}


void NRUTIL::free_matrix( double **m, int nrl, int nrh, int ncl, int nch)
{
	free((FREE_ARG) (m[nrl]+ncl-NR_END));
	free((FREE_ARG) (m+nrl-NR_END));
	return;
}
void NRUTIL::free_field( double ***m, int nrl, int nrh, int ncl, int nch, int nzl, int nzh)
{
	free((FREE_ARG) (m[nrl][ncl]+nzl-NR_END));
	free((FREE_ARG) (m[nrl]+ncl-NR_END));
	free((FREE_ARG) (m+nrl-NR_END));
	return;
}
