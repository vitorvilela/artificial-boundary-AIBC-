/*
 
Works:

$ qcc -cadna -Wall -O2 source.c -o source -L/home/vitor/App/cadna_c-2.0.2/src -lcadnaC -lstdc++ -lm -L/home/vitor/Dropbox/CFD-Lab/artificial-boundary/karman/bc/frugally-deep/basilisk-cpp -lbc

...but with warning: 
./bc.h:9:17: warning: 'get_next_ux' has C-linkage specified, but returns user-defined type 'float_st' which is incompatible with C [-Wreturn-type-c-linkage]
extern float_st get_next_ux(float_st dt, float_st y, float_st ux); 

$ ./source

...but with inconsistent results: 
Loading json ... done. elapsed time: 0.000514 s
Running test 1 of 1 ... done. elapsed time: 0.000132 s
Loading, constructing, testing of fd_model_ux.json took 0.001333 s overall.
Loading json ... done. elapsed time: 0.000515 s
Running test 1 of 1 ... done. elapsed time: 0.000132 s
Loading, constructing, testing of fd_model_uy.json took 0.001333 s overall.
Loading json ... done. elapsed time: 0.000520 s
Running test 1 of 1 ... done. elapsed time: 0.000132 s
Loading, constructing, testing of fd_model_f.json took 0.001338 s overall.
y:  0.118110236220472E-001, ux:  @.0                   , uy:  @.0                   , f:  @.0 



Tests:

1- clang -std=c++14 -x c++ -m64 -c bc.cpp 
2- ar -cvq libbc.a bc.o
3- qcc -cadna -Wall -O2 source.c -o source -lstdc++ -L/home/vitor/App/cadna_c-2.0.2/src -lcadnaC -L/home/vitor/Dropbox/CFD-Lab/artificial-boundary/karman/bc/frugally-deep/basilisk-cpp -lbc -lm

3- test: qcc -D_DEEP -Wall -O2 source.c -o source -lstdc++ -L/home/vitor/Dropbox/CFD-Lab/artificial-boundary/karman/bc/frugally-deep/basilisk-cpp -lbc -lm

./source

*/

#include "saint-venant.h"
#include "bc.h"




double Delta = 1./127;

// (dt) already defined in Basilisk
// double dt = 1.e-4;  

double ux = 0.5;
double uy = 0.;
double f = 0.5;

int main()
{
  

  run();
 
  
}


event init (i = 0)
{   
    
  for (int i=0; i<127; i++) {
    
    double y = (i+0.5)*Delta;
    
    ux = get_next_ux(dt, y, ux);
    uy = get_next_uy(dt, y, uy);
    f = get_next_f(dt, y, f);
        
    printf("y: %f, ux: %f, uy: %f, f: %f\n", y, ux, uy, f);
    
  }
    
}

