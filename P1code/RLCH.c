#include <stdio.h>
#include <math.h>
#include <complex.h>

int main(){
  /*double R = 40;
  double Rl = 5.5;
  double L = 45.6*pow(10,-6);
  double Cl = 550*pow(10,-12);
  double C = 470*pow(10,-12);
  double a = Rl/L;
  double b = 1/(L*Cl);
  double p = 1/(Cl*R);
  double q = 1/(C*R);
  double r = Rl/(Cl*L*R);
  double s = Rl/(C*L*R);
  double t = 1/(Cl*C*L*R);

  printf("%f, %f, %f, %f, %f, %f, %f\n",a,b,p,q,r,s,t);
  */
  complex double jomega = 0;
  double amp;
  for(jomega = cpow(10,5)*I; cimag(jomega) <= 5*pow(10,6); jomega = jomega + cpow(10,3)*I){
    amp = (1/40) * cabs(-60500-6.32*cpow(10,6)*I-jomega) * cabs(-60500+6.32*cpow(10,6)*I-jomega) * cabs(jomega)
    / (cabs(-154000-4.64*cpow(10,6)*I-jomega) * cabs(-154000+4.64*cpow(10,6)*I-jomega) * cabs(-98500000-jomega));
    printf("%7.0f\t%e\n", cimag(jomega), amp);
  }
  return 0;
}
/*
R = 40
R_{L} = 5.5
L = 45.6*10^-6
C_{L} = 550*10^-12
C = 470*10^-12

R_l/L = 103070
1/LC_l = 3.987*10^13
1/C_lR = 4.568*10^7
1/CR = 5.346*10^7
R_l/C_lLR = 4.709*10^12
R_l/CLR = 5.510*10^12
1/C_lCLR = 2.132*10^21

零点
s = 0
s = -60500-6.32×10^6 i
s = -60500+6.32×10^6 i
極
s = -98500000
s = -154000-4.64×10^6 i
s = -154000+4.64×10^6 i
 */
