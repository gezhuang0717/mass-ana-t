#ifndef _NRUTIL_HH
#include "cmath"

#define _NRUTIL_HH

#define NR_END 1
#define FREE_ARG char*
#define SIGN(a,b) ((b) >=0.0 ? fabs(a) : -fabs(a))

//static double maxarg1, maxarg2;
//#define FMAX(a,b) (maxarg1=(a),maxarg2=(b), (maxarg1) > (maxarg2) ? (maxarg1) : (maxarg2) )


//static double minarg1, minarg2;
//#define FMIN(a,b) (minarg1=(a),minarg2=(b), (minarg1) < (minarg2) ? (maxarg1) : (maxarg2) )

#define NMAX 50
#define GET_PSUM \
	for (j=0;j<ndim;j++)\
	{\
	for (sum=0.0,i=0;i<mpts;i++) sum+=p[i][j];\
	psum[j]=sum;}

#define TINY 1.0E-20
class NRUTIL {

public:


	double * vector (int, int);
	int * ivector (int, int);

	double **matrix(int, int, int, int);
	double ***field(int, int ,int, int, int, int);

	void free_vector( double *, int, int);
	void free_ivector( int *, int, int);

	void free_matrix (double **, int, int, int, int);

	void free_field (double ***, int, int, int, int, int, int);

	void nrerror(char *);
};
#endif
