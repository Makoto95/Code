#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <string.h>

#define FS 44100
#define CHNL 1
#define PI M_PI

int getnumf(char*);
int getbaseans(char);
int getsmplnum(int, char*);

int main(int argc, char** argv){
  char* filename = argv[1];
  double A = atof(argv[2]);
  int tempo = atoi(argv[3]);
  FILE* fp  = fopen(filename, "r");
  int i;

  short* data = (short*)malloc(sizeof(short)*FS);
  int smplban = FS;
  for(i = 0; i < FS; i++){
    data[i] = 0;
  }
  double f[35];
  int node_count = 0;
  char buf[6];
  char bufnum[3];
  for(i = 12; i< 24; i++){
    f[i] = 440 * pow(2, ((double)(i-12)-9)/12);
  }
  for(i = 0; i < 12; i++){
    f[i] = f[i+12] / 2;
  }
  for(i = 24; i<36; i++){
    f[i] = f[i-12] * 2;
  }

  while(fscanf(fp, "%s\t%s", buf, bufnum) != EOF){
    node_count++;
    int smplnum = getsmplnum(tempo, bufnum);
    short* ndata = (short*)malloc(sizeof(short)*(smplnum+smplban));
    assert(ndata);
    bcopy(data, ndata, smplban);
    int otonum = getnumf(buf);

    if(otonum == -1){
      for(i = smplban; i < smplban + smplnum; i++){
        ndata[i] = 0;
        fwrite(&ndata[i],2,1,stdout);
      }
    }
    else{
      double end = ((double)smplban + (double)smplnum) * (1023.0 / 1024);
      for(i = smplban; i <= end ; i++){
        double t = (double)i / FS;
        double x = A * sin(2 * PI * f[otonum] * t);
        ndata[i] = (short)x;
        fwrite(&ndata[i],2,1,stdout);
      }
      while(i < smplban + smplnum){
        ndata[i] = 0;
        fwrite(&ndata[i],2,1,stdout);
        i++;
      }
    }
    free(data);
    data = ndata;
    smplban += smplnum;
  }
fclose(fp);
return 0;
}

int getnumf(char* s){
  int ans;
  switch(s[0]){
    case 'h':
    ans = getbaseans(s[2]) + 12;
    if(s[3] == 'i') ans++;
    if(s[3] == 'e') ans--;
    break;
    case 'l':
    ans = getbaseans(s[3]) - 12;
    if(s[4] == 'i') ans++;
    if(s[4] == 'e') ans--;
    break;
    case 'S':
    ans = -1;
    break;
    default:
    ans = getbaseans(s[0]);
    if(s[1] == 'i') ans++;
    if(s[1] == 'e') ans--;
    break;
  }
  return ans;
}

int getbaseans(char s){
  switch(s){
    case 'A':
    return 12;
    break;
    case 'B':
    return 14;
    break;
    case 'C':
    return 15;
    break;
    case 'D':
    return 17;
    break;
    case 'E':
    return 19;
    break;
    case 'F':
    return 20;
    break;
    case 'G':
    return 22;
    break;
    default:
    return -1;
    break;
  }
}

int getsmplnum(int tempo, char* s){
  double ans;
  ans = FS * (double)tempo / 15 / atof(s);
  if(s[1] == 'd' || s[3] == 'd'){
    ans = ans * 3 / 2;
  }
  return (int)ans;
}
