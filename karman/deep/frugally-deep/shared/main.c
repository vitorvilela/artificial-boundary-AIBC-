// https://isocpp.org/wiki/faq/mixing-c-and-cpp
// https://stackoverflow.com/questions/3178342/compiling-a-c-program-with-gcc

// In C compile with : 

// gcc -c bc.cpp -lstdc++

// OR

// gcc *.cpp *.c -Wall -I/header/path/frugally-deep/c-cpp -lstdc++ -o main

// (Doesn't work) 
// A lot of errors

// In C++ made some changes and use:

// g++ -std=c++14 -O3 bc.cpp main.c -I/header/path/frugally-deep/c-cpp/frugally-deep/c-cpp -o main

// (Works) 

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
