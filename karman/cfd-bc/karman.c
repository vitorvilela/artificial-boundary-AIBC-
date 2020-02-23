/*
 * Genann library:
 * 
 * gcc -c genann.c
 * ar -cvq libgenann.a genann.o
 * qcc -Wall -O2 karman.c -o karman -lm -L/home/vitor/Dropbox/CFD-Lab/artificial-boundary/karman/cfd-bc -lgenann
 * 
 */




/* Compile with:
 * 
 *      3D with MPI: CC99='mpicc -std=c99' qcc -Wall -O2 -grid=octree -D_MPI=1 karman.c -o karman -lm -L$BASILISK/gl -lglutils -lfb_glx -lGLU -lGLEW -lGL -lX11
 *      
 *      2D with MPI: CC99='mpicc -std=c99' qcc -Wall -O2 -D_MPI=1 karman.c -o karman -lm
 * 
 *      2D without MPI: qcc -Wall -O2 karman.c -o karman -lm
 * 
 * Run with:
 * 
 *      with MPI: mpirun -np 2 ./karman
 *      without MPI: ./karman
 * 
 * Optional:
 * 
 * qcc -source karman.c
 * mpicc -O2 -Wall -std=c99 -D_MPI=1 _karman.c -o karman -lm
 * mpirun -np 2 ./karman
 * 
 */

/**
# Bénard–von Kármán Vortex Street for flow around a cylinder at Re=160

An example of 2D viscous flow around a simple solid boundary. Fluid is
injected to the left of a channel bounded by solid walls with a slip
boundary condition. A passive tracer is injected in the bottom half of
the inlet.

We use the centered Navier-Stokes solver and advect the passive tracer
*f*. */

#include <assert.h>
#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "navier-stokes/centered.h"
#include "tracer.h"
#include "genann.h"


#define cylinder_radius 0.0625
#define domain_length 8.
#define cutted_y_length 1
#define measurement (cylinder_radius+3*cylinder_radius)

#define cylinder_x measurement
#define cylinder_y 0.5*domain_length

#define maxlevel 9
#define minlevel 4
#define final_time 6.

const double boundary_resolution = domain_length/(1<<maxlevel);

scalar f[];
scalar * tracers = {f};

double input[127][5];


/**
The domain is eight units long, centered vertically. */

int main() {


  // Loading models   
  
  char model_name[30];
  sprintf(model_name, "./models/model-ux.ann");  
  FILE *model = fopen(model_name, "r");  
  genann *ann_ux = genann_read(model);
  fclose(model);

  sprintf(model_name, "./models/model-uy.ann");  
  model = fopen(model_name, "r");  
  genann *ann_uy = genann_read(model);
  fclose(model);
    
  sprintf(model_name, "./models/model-f.ann");  
  model = fopen(model_name, "r");  
  genann *ann_f = genann_read(model);
  fclose(model);
  
  
  // Reading input values from boundary.csv
    
  
  char dataset_name[30];
  sprintf(dataset_name, "./dataset/initialBoundary.csv");  
  FILE *in = fopen(dataset_name, "r");
  
  int i, rc;
  double trash;  
  
  errno = 0;  
  printf("Input:\n");
  for (i=0; i<127; i++) {
    rc = fscanf(in, "%lf %lf %lf %lf %lf %lf %lf %lf %lf", &trash, &trash, &input[i][0], &trash, &input[i][1], &input[i][2], &input[i][3], &input[i][4], &trash);
    if (rc < 9 || errno != 0) 
      perror("fscanf");   
    printf("[%1.8e, %1.8e, %1.8e, %1.8e, %1.8e]\n", input[i][0], input[i][1], input[i][2], input[i][3], input[i][4]);
  }
   

//     const double input[1][3] = {{1.e-4, 0., 0.5}};
//     *genann_run(ann, input[0]);
//     genann_free(ann);

  
  L0 = domain_length;
  origin (-cylinder_x, -cylinder_y); 
  N = 1 << maxlevel;

  /**
  We set a constant viscosity corresponding to a Reynolds number of
  160, based on the cylinder diameter (0.125) and the inflow velocity
  (1). We also set the initial velocity field and tracer
  concentration. */
  
  const face vector muc[] = {0.00078125, 0.00078125};
  mu = muc;
//   run(); 
  
}



// Set initial values for var(t+dt) = f(dt, y, var(t)) and then update it with the response
// From cfd/boundary.csv read all y values of the last i (e.g., 846) for the variables ux, uy, f
// Example from at basilisk site: http://www.basilisk.fr/sandbox/oystelan/waveflume_example/simple_waveflume.c
// Build a matrix reading values from the last 127 rows of boundary.csv (initially do it manually, and without commas for easier routine)
// look at genann/train.c to see how to do it with .csv

// Extract from boundary.csv
// profile points: 127
// i, t, ( dt ), x, ( y ), [ u.x ], [ u.y ], [ f ], p
// 846, 6.0026818354927, ( 0.0071728474019575 ), 0.25, ( -0.984375 ), [ 1.0335110199444 ], [ -0.0017810991855747 ], [ 1 ], -0.075011810205963
// 846, 6.0026818354927, ( 0.0071728474019575 ), 0.25, ( -0.96875 ), [ 1.0335110199444 ], [ -0.0035621983711493 ], [ 1 ], -0.075011810205963
// ...
// 846, 6.0026818354927, ( 0.0071728474019575 ), 0.25, ( 0.96875 ), [ 1.0368510467965 ], [ 0.0022036522399983 ], [ 7.5119908244187e-09 ], 0.0056192477535567
// 846, 6.0026818354927, ( 0.0071728474019575 ), 0.25, ( 0.984375 ), [ 1.0368510467965 ], [ 0.0011018261199991 ], [ 7.5119908244187e-09 ], 0.0056192477535567

// ( dt )       ( y )        [ u.x ]      [ u.y ]      [ f ]
// input[i][0], input[i][1], input[i][2], input[i][3], input[i][4]

// OR load a dump file (much simpler) into vector u and scalar f and then clean them





u.n[left]  = dirichlet( *genann_run(ann_ux, {{dt, y, interpolate(u.x, 0., y)}}) );
u.t[left]  = dirichlet( *genann_run(ann_uy, {{dt, y, interpolate(u.y, 0., y)}}) );
f[left]    = dirichlet( *genann_run(ann_ux, {{dt, y, interpolate(f, 0., y)}}) ); 

p[left]    = neumann(0.);
pf[left]   = neumann(0.);

u.n[right] = neumann(0.);
p[right]   = dirichlet(0.);
pf[right]  = dirichlet(0.);


event init (t = 0) {

  /**
  We set the initial velocity field. */
  
  foreach()
    u.x[] = 1.;
}

/**
We check the number of iterations of the Poisson and viscous
problems. */

event logfile (i++)
  fprintf (stderr, "%d %g %d %d\n", i, t, mgp.i, mgu.i);

  
event boundaryfile (i++) {  
  
  static FILE * bf = fopen ("boundary.csv", "w");
  
  if (i == 0) {
    fprintf (bf, "profile points: %d\n", (int)(((cutted_y_length-boundary_resolution)-(-cutted_y_length+boundary_resolution))/boundary_resolution)+1);
    fprintf (bf, "i, t, dt, x, y, u.x, u.y, f, p\n");
  }

  else if (t > 2.5) { 
    
    scalar ff[];
    foreach()
      ff[] = clamp(f[],0,1);
    boundary ({ff});
    
    for (double y = -cutted_y_length+boundary_resolution; y <= cutted_y_length-boundary_resolution; y += boundary_resolution)    
      fprintf (bf, "%d, %.14g, %.14g, %.14g, %.14g, %.14g, %.14g, %.14g, %.14g\n", i, t, dt, measurement, y, interpolate(u.x,measurement,y), interpolate(u.y,measurement,y), interpolate(ff,measurement,y), interpolate(p,measurement,y));
  }   
    
  fflush (bf);
  
}  
  
/**
We produce animations of the vorticity and tracer fields... 

![Animation of the vorticity field.](karman/vort.gif)

![Animation of the tracer field.](karman/f.gif) 
*/

event movies (i += 4; t <= final_time) {
  
  scalar omega[];
  vorticity (u, omega);
  output_ppm (omega, file = "vort.gif", box = {{-cylinder_x,-cutted_y_length},{domain_length-cylinder_x,cutted_y_length}}, linear = true, min = -10, max = 10);
  
  output_ppm (f, file = "f.gif", box = {{-cylinder_x,-cutted_y_length},{domain_length-cylinder_x,cutted_y_length}}, linear = true, min = 0, max = 1);
  
  scalar ll[];
  foreach() 
    ll[] = level;  
  boundary({ll});
  output_ppm (ll, file = "grid.gif", box = {{-cylinder_x,-cutted_y_length},{domain_length-cylinder_x,cutted_y_length}}, linear = true, min = minlevel, max = maxlevel);
      
}

/**
If gfsview is installed on your system you can use this to visualise
the simulation as it runs. */

#if 0
event gfsview (i += 10) {
  static FILE * fp = popen ("gfsview2D -s ../karman.gfv", "w");
  output_gfs (fp);
}
#endif

/**
We adapt according to the error on the velocity and tracer fields. */

event adapt (i++) {
  adapt_wavelet ({u, f}, (double[]){3e-2, 3e-2, 3e-2}, maxlevel, minlevel);
}


/**
## See also

* [Same example with Gerris](http://gerris.dalembert.upmc.fr/gerris/examples/examples/cylinder.html)
*/
