// clang -std=c++14 -x c++ -m64 -lstdc++ -lm bc.cpp main.c



#include <stdlib.h>
#include <stdio.h>
#include "bc.h"

int main() {
  
  double Delta = 1./127;
    
  double dt = 1.e-4;
  
  double ux = 0.;
  double uy = 0.;
  double f = 0.;
  
  float new_ux = 10.;
  float new_uy = 10.;
  float new_f = 10.;
    
  for (int i=0; i<127; i++) {
    
    double y = (i+0.5)*Delta;
    
    get_next_ux(dt, y, ux, new_ux);
    get_next_uy(dt, y, uy, new_uy);
    get_next_f(dt, y, f, new_f);
        
    printf("y: %f, ux: %f, uy: %f, f: %f\n", y, new_ux, new_uy, new_f);
    
  }
    
  return 0;
      
}