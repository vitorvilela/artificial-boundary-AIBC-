#include <Python.h>
#include <stdlib.h>
#include "bc.h"

// gcc *.c -Wall -I/usr/include/python2.7 -lpython2.7 -o main
// OR
// gcc *.c -Wall -I/usr/include/python3.5 -L/usr/lib/python3.5/config-3.5m-x86_64-linux-gnu -lpython3.5 -o main
// ./main

int main() {
  
  double Delta = 1./127;
    
  double dt = 1.e-4;
  
  double ux = 0.;
  double uy = 0.;
  double f = 0.;
    
  for (int i=0; i<127; i++) {
    
    double y = (i+0.5)*Delta;
    
    ux = get_new_ux(dt, y, ux);
    uy = get_new_uy(dt, y, uy);
    f = get_new_f(dt, y, f);
        
    printf("\ny: %f, ux: %f, uy: %f, f: %f", y, ux, uy, f);
    
  }
    
  return 0;
      
}
