#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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

/* deallocate matrix of double */
static inline void free_dmatrix(double **mat) {
  free(mat[0]);
  free(mat);
}

int main(){
  double **temp, **work; //tempが温度，workは一時保存用の2次元配列
  int h_inverse=5; //空間のステップ幅の逆数(単位長さを何分割するか)
  double dt=0.005; //時間のステップ幅
  int p=2; //境界の効果を減らすために系の大きさをp倍して計算する
  int n=20*p*h_inverse+1; //実際のシステムサイズは20で，単位長さを何分割したかで計算上のシステムサイズが決まる
  double time; //何秒後を計算するか
  double k = 1; //温度拡散率
  double t0=1.5, t1=1.7; //t0はポリエステルの温度，t1はマンガニン部分の温度

  printf("Input time length(seconds): ");
  scanf("%lf", &time);

  int i,j,m;
  double t;
  double x, y;
  FILE *file;

  //初期条件の設定
  temp = alloc_dmatrix(n,n);
  work = alloc_dmatrix(n,n);
  for(i=0; i<n; ++i){
    for(j=0; j<n; ++j){
      temp[i][j] = t0;
    }
  }
  for(i=0; i<20*h_inverse/2; ++i){
    for(j=0; j<20*h_inverse/2; ++j){
      temp[(n-1)/2-20*h_inverse/4+i][(n-1)/2-20*h_inverse/4+j] = t1; //マンガニン部分
    }
  }

  t = 0;

  //時間発展の計算
  for(m=1; t<time; ++m){
    t=t+dt;
    t1= 1.5+0.2*pow(cos(M_PI/20*t),2.0);
    for(i=1; i<n-1; ++i){
      for(j=1; j<n-1; ++j){
	work[i][j] = temp[i][j]+ k*dt*h_inverse*h_inverse*(temp[i+1][j]+temp[i-1][j]+temp[i][j+1]+temp[i][j-1]-4*temp[i][j]);
      }
    }
    for(i=1; i<n-1; ++i){
      for(j=1; j<n-1; ++j){
	temp[i][j] = work[i][j];
      }
    }
    for(i=0; i<20*h_inverse/2; ++i){
      for(j=0; j<20*h_inverse/2; ++j){
	temp[(n-1)/2-20*h_inverse/4+i][(n-1)/2-20*h_inverse/4+j] = t1; //マンガニン部分を次の温度にする
      }
    }
  }
  file = fopen("data.txt","a");
  fprintf(file,"#time:%f\n", t);
  for(i=0; i<(n-1)/2+1; ++i){
    x = i /(double)h_inverse;
    for(j=0; j<(n-1)/2+1; ++j){
      y = j /(double)h_inverse;
      fprintf(file,"%lf %lf %lf\n",x, y, temp[(n-1)/2-20*h_inverse/2+i][(n-1)/2-20*h_inverse/2+j]); //境界の効果を減らすためにくっつけた部分は出力しない
    }
    fprintf(file, "\n");
  }
  fclose(file);

  free_dmatrix(temp);
  return 0;
}
