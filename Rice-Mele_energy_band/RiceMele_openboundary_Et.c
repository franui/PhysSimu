#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define M_PI 3.1415926535897932384626433832795

/* http://www.netlib.org/lapack/explore-html/dd/d4c/dsyev_8f.html */
void dsyev_(char *JOBZ, char *UPLO, int *N, double *A, int *LDA, double *W,
            double *WORK, int *LWORK, int *INFO);

/* allocate m x n matrix of double */
static inline double **alloc_dmatrix(int m, int n) {
  int i;
  double **mat;
  mat = (double**)malloc((size_t)(m * sizeof(double*)));
  if (mat == NULL) {
    fprintf(stderr, "Error: allocation failed in alloc_dmatrix\n");
    exit(1);
  }
  mat[0] = (double*)malloc((size_t)(m * n * sizeof(double)));
  if (mat[0] == NULL) {
    fprintf(stderr, "Error: allocation failed in alloc_dmatrix\n");
    exit(1);
  }
  for (i = 1; i < m; ++i) mat[i] = mat[i-1] + n;
  return mat;
}

/* allocate vector of double */
static inline double *alloc_dvector(int n) {
  double *vec;
  vec = (double*)malloc((size_t)(n * sizeof(double)));
  if (vec == NULL) {
    fprintf(stderr, "Error: allocation failed in alloc_dvector\n");
    exit(1);
  }
  return vec;
}

/* deallocate vector of double */
static inline void free_dvector(double *vec) {
  free(vec);
}

/* deallocate matrix of double */
static inline void free_dmatrix(double **mat) {
  free(mat[0]);
  free(mat);
}

int main(int argc, char** argv) {
  FILE *data, *gp;
  char *filename="RiceMele_openboundary_Et.txt";
  data=fopen(filename,"w");
  int i, j;

  //プログラムに与える初期値
  if (argc < 6) {
    fprintf(stderr, "%sに続けて行列の次元、時間を刻む回数、J0、delta、Deltaの順に入力して下さい．\n", argv[0]);
    exit(1);
  }
  int dim=atof(argv[1]);
  double step=atoi(argv[2]);
  double J0=atof(argv[3]);
  double delta=atof(argv[4]);
  double Delta=atof(argv[5]);
  double h=2*M_PI/step;

  for(int k=0;k<=step;k++){
    double t=h*k;

    double **mat;
    double *ene; /* eigenenergies */
    double *work; /* working area */

    int lwork, info;
    char jobz = 'V'; /* Compute eigenvalues and eigenvectors */
    char uplo = 'U'; /* Upper triangle of A is stored */

    /*行列の設定*/
    mat = alloc_dmatrix(dim, dim);
    for(i=0;i<dim;i++){
      for(j=0;j<dim;j++){
	mat[i][j]=0;
      }
    }
    int parity;
    for (i = 0; i < dim; ++i) {
      if(i%2==0){
	parity=1;
      }else{
	parity=-1;
      }
      if (i > 0) {
	mat[i][i-1] = -J0+delta*cos(t)*parity;
      }
      mat[i][i] = Delta*sin(t)*parity;
      if (i < (dim - 1)) {
	mat[i][i+1] = -J0-delta*cos(t)*parity;
      }
    }

    /*固有値の計算*/
    ene = alloc_dvector(dim);
    lwork = 3*dim - 1;
    work = alloc_dvector(lwork);
    dsyev_(&jobz, &uplo, &dim, &mat[0][0], &dim, &ene[0], &work[0],
	   &lwork, &info);
    if (info != 0) {
      fprintf(stderr, "Error: LAPACK::dsyev failed\n");
      exit(1);
    }

    /*テキストファイルへの出力*/
    fprintf(data,"%f ",t);
    for (i = 0; i < dim; ++i){
      if(i!=dim){
	fprintf(data,"%f ", ene[i]);
      }else{
	fprintf(data,"%f", ene[i]);
      }
    }
    fprintf(data,"\n");

    free_dmatrix(mat);
    free_dvector(ene);
    free_dvector(work);
  }
}
