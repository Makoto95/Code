#include <stdio.h>
#include <math.h>

int main(){
  double b[2];
  double sR[12];
  double sI[12];
  double a[6];
  int i;
  int j;
  int k=0;
  for(i=0;i<6;i++){
    a[i] =(M_PI/6)*(2*i+1);
  }gc
  b[0] = asinh(1.0) / 3;
  b[1] = - asinh(1.0) / 3;
  //printf("%f, %f, %f\n\n",asinh(1), b[0], b[1]);
  for(i=0;i<6;i++){
    double sina = sin(a[i]);
    double cosa = cos(a[i]);
    for(j=0;j<2;j++){
      //printf("%f, %f\n",a[i], b[j]);
      sR[k] = sina * sinh(b[j]);
      sI[k] = cosa * cosh(b[j]);
      k++;
    }
  }
  for(k=0;k<12;k++){
    if(sR[k] <= 0){
      printf("%d, %.4f + j%.4f\n", k, sR[k],sI[k]);
    }
  }
  return 0;
}
