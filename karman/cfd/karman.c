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

#include "navier-stokes/centered.h"
#include "tracer.h"


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

/**
The domain is eight units long, centered vertically. */

int main() {
  
  L0 = domain_length;
  origin (-cylinder_x, -cylinder_y); 
  N = 1 << maxlevel;

  /**
  We set a constant viscosity corresponding to a Reynolds number of
  160, based on the cylinder diameter (0.125) and the inflow velocity
  (1). We also set the initial velocity field and tracer
  concentration. */
  
  const face vector muc[] = {0.00078125,0.00078125};
  mu = muc;
  run(); 
  
}

/**
The fluid is injected on the left boundary with a unit velocity. The
tracer is injected in the lower-half of the left boundary. An outflow
condition is used on the right boundary. */

u.n[left]  = dirichlet(1.);
p[left]    = neumann(0.);
pf[left]   = neumann(0.);
f[left]    = dirichlet(y < 0);

u.n[right] = neumann(0.);
p[right]   = dirichlet(0.);
pf[right]  = dirichlet(0.);

/**
We add a new boundary condition for the cylinder. The tangential
velocity on the cylinder is set to zero. */

bid cylinder;
u.t[cylinder] = dirichlet(0.);

event init (t = 0) {

  /**
  To make a long channel, we set the *top* boundary for $y > 0.5$ and
  the *bottom* boundary for $y < -0.5$. The *cylinder* has a radius of
  0.0625. */

  mask (y >  cutted_y_length ? top :
	y < -cutted_y_length ? bottom :
	sq(x) + sq(y) < sq(cylinder_radius) ? cylinder :
	none);

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
