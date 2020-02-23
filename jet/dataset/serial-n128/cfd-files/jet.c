/* Compile with:
 * 
 *      3D with MPI: CC99='mpicc -std=c99' qcc -Wall -O2 -grid=octree -D_MPI=1 jet.c -o jet -lm -L$BASILISK/gl -lglutils -lfb_glx -lGLU -lGLEW -lGL -lX11
 *      
 *      2D with MPI: CC99='mpicc -std=c99' qcc -Wall -O2 -D_MPI=1 jet.c -o jet -lm
 * 
 *      2D without MPI: qcc -Wall -O2 jet.c -o jet -lm
 * 
 * Run with:
 * 
 *      with MPI: 
 *      without MPI: ./jet
 * 
 * Optional:
 * 
 * qcc -source jet.c
 * mpicc -O2 -Wall -std=c99 -D_MPI=1 _jet.c -o jet -lm
 * mpirun -np 2 ./jet
 * 
 */


#include "navier-stokes/centered.h"
#include "tracer.h"



#define jet_radius .0625
#define domain_length 50.*(2.*jet_radius)
#define aspect_ratio 1.
#define domain_height domain_length/aspect_ratio
#define jet_x 0.
#define jet_y .5*domain_height

#define quiescent 0.

#define reynolds 10.
#define density 1.
#define jet_velocity 1.
#define viscosity density*(2.*jet_radius)*jet_velocity/reynolds

#define initial_grid 128

#define measurement_x .25*domain_length
#define measurement_time measurement_x/(.5*jet_velocity)

#define final_convection_distance .5*domain_length
#define final_time final_convection_distance/(.15*jet_velocity)

const double boundary_resolution = domain_length/initial_grid;

scalar f[];
scalar * tracers = {f};

/**
The domain is domain_length units long, centered vertically. */

int main() {
    
  L0 = domain_length;
  origin (-jet_x, -jet_y); 
  init_grid (initial_grid);
  
  const face vector muc[] = {viscosity, viscosity};
  mu = muc;
  
  run(); 
  
}

/**
The fluid is injected on the left boundary with a jet_velocity at the nozzle; wall elsewhere. */

u.n[left] = dirichlet((fabs(y)<=jet_radius)*jet_velocity);
p[left] = neumann(0.);
pf[left] = neumann(0.);
f[left] = dirichlet((fabs(y)<=jet_radius));

u.n[right] = neumann(0.);
p[right]   = dirichlet(0.);
pf[right]  = dirichlet(0.);

// Default boundary conditions for [top] and [botton] 
// i.e., homogeneous neumann for the velocity and homogeneous dirichlet for the pressure

event init (t = 0) {

  /**
  To make a long channel, we set the *top* boundary for y > domain_height and
  the *bottom* boundary for y < domain_height.*/

  mask (y > .5*domain_height ? top : none);

  /**
  We set the initial velocity field. */
  
  foreach()
    foreach_dimension()
      u.x[] = quiescent;
  
}


event print_input (t = 0) {
  
  printf("\n \
  (2.*jet_radius): %f\n \
  domain_length: %f\n \
  viscosity: %f\n \
  measurement_x: %f\n \
  measurement_time: %f\n \
  final_convection_distance: %f\n \
  final_time: %f\n \
  boundary_resolution: %f \n\n", 
  (2.*jet_radius), domain_length, viscosity, measurement_x, measurement_time, final_convection_distance, final_time, boundary_resolution); 
  
}


/**
We check the number of iterations of the Poisson and viscous problems. */

event logfile (i++)
  fprintf (stderr, "%d %g %d %d\n", i, t, mgp.i, mgu.i);

  
event write_boundaryfile (i++) {  
  
  static FILE * bf = fopen ("boundary.csv", "w");
  
  if (i == 0) {
    
    fprintf (bf, "profile points: %d\n", (int)(((.5*domain_height-boundary_resolution)- 
    (-.5*domain_height+boundary_resolution))/boundary_resolution)+1);
    
    fprintf (bf, "i, t, dt, x, y, u.x, u.y, f, p\n");
    
  }

  else if (t >= measurement_time) { 
    
    scalar ff[];
    foreach()
      ff[] = clamp(f[], 0, 1);
    boundary ({ff});
    
    for (double y = -.5*domain_height+boundary_resolution; 
                y <= .5*domain_height-boundary_resolution; 
                y += boundary_resolution) 
    {      
      fprintf (bf, "%d, %.14g, %.14g, %.14g, %.14g, %.14g, %.14g, %.14g, %.14g\n", i, t, dt, measurement_x, y,  interpolate(u.x,measurement_x,y), interpolate(u.y,measurement_x,y), 
      interpolate(ff,measurement_x,y), interpolate(p,measurement_x,y));
    }
    
  }
    
  fflush (bf);
  
}  
  
  
/**
We produce animations of some fields */

event movies (i = 0; i += 5; t <= final_time) {
  
  output_ppm (u.x, file = "u.gif", 
              box = {{-jet_x,-.5*domain_height},{domain_length,.5*domain_height}}, 
              linear = true);
  
  scalar omega[];
  vorticity (u, omega);
  output_ppm (omega, file = "vort.gif", 
              box = {{-jet_x,-.5*domain_height},{domain_length,.5*domain_height}}, 
              linear = true); // min = -5, max = 5
  
  output_ppm (f, file = "f.gif", 
              box = {{-jet_x,-.5*domain_height},{domain_length,.5*domain_height}}, 
              linear = true, min = 0, max = 1);
  
//   scalar ll[];
//   foreach() 
//     ll[] = level;  
//   boundary({ll});
  
//   output_ppm (ll, file = "grid.gif", 
//               box = {{0,0},{domain_length,domain_height}},
//               linear = true);
      
}
