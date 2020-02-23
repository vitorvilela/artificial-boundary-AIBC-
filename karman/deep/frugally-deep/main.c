// In C compile with : 
// (Doesn't work) gcc *.cpp *.c -Wall -I/home/vitor/Dropbox/CFD-Lab/artificial-boundary/karman/bc/frugally-deep -lstdc++ -o main
// In C++ made some changes and use:
// (Work) g++ -std=c++14 -O3 bc.cpp main.c -I/home/vitor/Dropbox/CFD-Lab/artificial-boundary/karman/bc/frugally-deep -o main

#include <stdlib.h>
#include <stdio.h>
#include "bc.h"

int main() {
  
  double Delta = 1./127;
    
  double dt = 1.e-4;
  
  double ux = 0.;
  double uy = 0.;
  double f = 0.;
    
  for (int i=0; i<127; i++) {
    
    double y = (i+0.5)*Delta;
    
    ux = get_next_ux(dt, y, ux);
    uy = get_next_uy(dt, y, uy);
    f = get_next_f(dt, y, f);
        
    printf("y: %f, ux: %f, uy: %f, f: %f\n", y, ux, uy, f);
    
  }
    
  return 0;
      
}
