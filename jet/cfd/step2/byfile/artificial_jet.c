/* Compile with:
 * 
 *      3D with MPI: CC99='mpicc -std=c99' qcc -Wall -O2 -grid=octree -D_MPI=1 artificial_jet.c -o artificial_jet -lm -L$BASILISK/gl -lglutils -lfb_glx -lGLU -lGLEW -lGL -lX11
 *      
 *      2D with MPI: CC99='mpicc -std=c99' qcc -Wall -O2 -D_MPI=1 artificial_jet.c -o artificial_jet -lm
 * 
 *      2D without MPI: qcc -Wall -O2 artificial_jet.c -o artificial_jet -lm
 * 
 * Run with:
 * 
 *      with MPI: 
 *      without MPI: ./artificial_jet
 *      docker (tensorflow): docker run -it --rm -v $PWD:/tmp -w /tmp tensorflow/tensorflow ./artificial_jet
 * 
 * Optional:
 * 
 * qcc -source artificial_jet.c
 * mpicc -O2 -Wall -std=c99 -D_MPI=1 artificial_jet.c -o artificial_jet -lm
 * mpirun -np 2 ./artificial_jet
 * 
 */

#include <time.h>

#include "navier-stokes/centered.h"
#include "tracer.h"


// It no longer makes sense in step 2
#define jet_radius .0625 

#define domain_length 50.*(2.*jet_radius)
#define aspect_ratio 1.
#define domain_height domain_length/aspect_ratio

// It no longer makes sense in step 2, but to scale the origin
#define jet_x 0.
#define jet_y .5*domain_height
#define quiescent 0.

#define reynolds 10.
#define density 1.

// It no longer makes sense in step 2, but just as an estimate for the final simulation time
#define jet_velocity 1.

#define viscosity density*(2.*jet_radius)*jet_velocity/reynolds

#define initial_grid 128

// It no longer makes sense in step 2, but just as an reference for the final simulation time
#define measurement_x .25*domain_length
#define measurement_time measurement_x/(.5*jet_velocity)

#define final_convection_distance measurement_x
#define final_time final_convection_distance/(.15*jet_velocity)

#define NUM_DATA (initial_grid - 1)

#define boundary_posx .5*domain_length
#define boundary_left (boundary_posx - .5*(domain_length/initial_grid))
#define boundary_right (boundary_posx + .5*(domain_length/initial_grid))

const double boundary_resolution = domain_length/initial_grid;


scalar f[];
scalar * tracers = {f};

scalar predicted_ux[];
scalar predicted_uy[];
// scalar predicted_f[];

double array_ux_prediction[NUM_DATA];
double array_uy_prediction[NUM_DATA];
// double array_f_prediction[NUM_DATA];


// void delay (int number_of_seconds) {   
//   int milli_seconds = 1000*number_of_seconds; 
//   clock_t start_time = clock();  
//   while (clock() < start_time + milli_seconds) 
//     ; 
// } 

void waitFor (unsigned int secs) {
    unsigned int retTime = time(0) + secs;   
    while (time(0) < retTime);               
}


/**
The domain is domain_length units long, centered vertically. */

int main() {
  
  printf("MAIN\n");
     
  /**
  Predict initial and subsequent batches */  
  system("python sentinel_predictor.py ux &");
  system("python sentinel_predictor.py uy &");
  
  // Delay execution in 30 seconds for tensorflow initialization
  waitFor (30);
  
    
  L0 = domain_length;
  origin (-jet_x, -jet_y); 
  init_grid (initial_grid);
  
  const face vector muc[] = {viscosity, viscosity};
  mu = muc;
  
  DT = 1.e-2;
  
  run(); 
  
}

/**
The fluid is injected on the left boundary with a jet_velocity at the nozzle; wall elsewhere. */

//u.n[left] = dirichlet((fabs(y)<=jet_radius)*jet_velocity); //dirichlet(predicted_ux[]);
//u.t[left] = dirichlet(predicted_uy[]);
//p[left] = neumann(0.);
//pf[left] = neumann(0.);
//f[left] = dirichlet((fabs(y)<=jet_radius)); //dirichlet(predicted_f[]);

u.n[left] = neumann(0.);
p[left]   = dirichlet(0.);
pf[left]  = dirichlet(0.);
f[left] = neumann(0.);

u.n[right] = neumann(0.);
p[right]   = dirichlet(0.);
pf[right]  = dirichlet(0.);
f[right] = neumann(0.);

u.n[top] = neumann(0.);
p[top]   = dirichlet(0.);
pf[top]  = dirichlet(0.);

u.n[bottom] = neumann(0.);
p[bottom]   = dirichlet(0.);
pf[bottom]  = dirichlet(0.);

// Default boundary conditions for [top] and [botton] 
// i.e., homogeneous neumann for the velocity and homogeneous dirichlet for the pressure


event init (t = 0) {
  
  printf("%d   INIT\n", i);
  
  system("rm ./results/*");

  /**
  To make a long channel, we set the *top* boundary for y > .5*domain_height 
  accounting for the origin at y = .5*domain_height */

  mask (y > .5*domain_height ? top : none);
   
}


event print_input (t = 0) {
  
  printf("%d   PRINT_INPUT\n", i);
  
  printf("\n \
  domain_length: %f\n \
  final_convection_distance: %f\n \
  final_time: %f\n \
  boundary_resolution: %f \n\n", 
  domain_length, final_convection_distance, final_time, boundary_resolution); 
  
}


event fill_predicted (i++) { 
  
  printf("%d   FILL_PREDICTED\n", i);
  
  // Delay execution in 3 seconds waiting for predictions
  // Necessary for low mesh size (fast) simulations
  waitFor (3);
    
  FILE * bpof;
  
  // ux 
  char *fname = "./output/ux_batch_prediction_output.csv";  
  bpof = fopen (fname, "r");
  if (bpof == NULL) {
    fprintf(stderr, "Error reading file\n");
    return 1;
  }   
  int count = 0;
  while (fscanf(bpof, " %lf", &array_ux_prediction[count]) == 1)
    count++;
  fclose(bpof);
  
  //printf("\nLoop array_ux_prediction\n");
  //for (int i = 0; i < count; i++)
    //printf("i: %d, prediction: %lf\n", (int)(i), array_ux_prediction[i]);
    
  
  
  // uy 
  fname = "./output/uy_batch_prediction_output.csv";  
  bpof = fopen (fname, "r");
  if (bpof == NULL) {
    fprintf(stderr, "Error reading file\n");
    return 1;
  }   
  count = 0;
  while (fscanf(bpof, " %lf", &array_uy_prediction[count]) == 1)
    count++;
  fclose(bpof);
  
  /*
  
  // f  
  fname = "./output/f_batch_prediction_output.csv";  
  bpof = fopen (fname, "r");
  if (bpof == NULL) {
    fprintf(stderr, "Error reading file\n");
    return 1;
  }   
  count = 0;
  while (fscanf(bpof, " %lf", &array_f_prediction[count]) == 1)
    count++;
  fclose(bpof);  
  
  */
    
  
  foreach() {
    
    int iy = (int)((y+jet_y)/boundary_resolution); 
    
    predicted_ux[] = iy < NUM_DATA ? array_ux_prediction[iy] : array_ux_prediction[NUM_DATA-1];  
    predicted_uy[] = iy < NUM_DATA ? array_uy_prediction[iy] : array_uy_prediction[NUM_DATA-1];
    // predicted_f[] = iy < NUM_DATA ? array_f_prediction[iy] : array_f_prediction[NUM_DATA-1];  
      
  }  
  // boundary ({predicted_ux, predicted_uy, predicted_f});
  boundary ({predicted_ux, predicted_uy});
  
  system("rm ./output/*");

}  


event artificial_bc(i++) {
  
  printf("%d   ARTIFICIAL_BC\n", i);
  
  /*
  printf("dt: %f\n", dt);
  face vector av = a;
  
  foreach_face(x) {
    
    int ix = (int)((x+jet_x)/boundary_resolution);
    
    if (ix == (int)(.5*NUM_DATA)) {    
      av.x[] = ( .5*(predicted_ux[]+predicted_ux[-1,0]) - .5*(u.x[]+u.x[-1,0]) ) / dt;
    }
    
  }
  */
  
    
  foreach() {
    int bflag = (x>boundary_left) && (x<=boundary_right);
    //if (bflag == 1) 
      //printf("bflag %d at x %.4f y %.4f\n", bflag, x, y);    
    u.x[] = bflag*predicted_ux[] + (1 - bflag)*u.x[];
    u.y[] = bflag*predicted_uy[] + (1 - bflag)*u.y[];
    //f[] = bflag*predicted_f[] + (1 - bflag)*f[];
  }   
  
  // boundary ({u.x, u.y, f});
  boundary ({u.x, u.y});
  
    
}



/**
We check the number of iterations of the Poisson and viscous problems. */

event logfile (i++) {
  
  printf("%d   LOGFILE\n", i);
  
  fprintf (stderr, "%d %g %d %d\n", i, t, mgp.i, mgu.i);
  
}



event write_batch_prediction_inputs (i++) {
  
  printf("%d   WRITE_BATCH_PREDICTION_INPUTS\n", i);
  
  FILE * bpif;
    
  // ux
  char *fname = "./input/ux_batch_prediction_inputs.csv";  
  bpif = fopen (fname, "w");
  fprintf (bpif, "dt,y,ux\n");   
  for (double y = -.5*domain_height+boundary_resolution; // + .5*boundary_resolution; (NUM_DATA = init_grid) ??
              y <= .5*domain_height-boundary_resolution; // - .5*boundary_resolution;
              y += boundary_resolution) {       
    int iy = (int)((y+jet_y)/boundary_resolution); 
    //printf("y %.4f   (y+jet_y) %.4f   boundary_resolution %.4f   (y+jet_y)/boundary_resolution %.4f   iy %d\n", y, (y+jet_y), boundary_resolution, (y+jet_y)/boundary_resolution, iy);
    fprintf (bpif, "%.14g,%.14g,%.14g\n", dt, y, (iy < NUM_DATA ? array_ux_prediction[iy] : array_ux_prediction[NUM_DATA-1]));
  }
  fflush (bpif);
  fclose (bpif);
  
    
  // uy
  fname = "./input/uy_batch_prediction_inputs.csv";  
  bpif = fopen (fname, "w");
  fprintf (bpif, "dt,y,uy\n");    
  for (double y = -.5*domain_height+boundary_resolution;  
              y <= .5*domain_height-boundary_resolution; 
              y += boundary_resolution) {   
    int iy = (int)((y+jet_y)/boundary_resolution); 
    fprintf (bpif, "%.14g,%.14g,%.14g\n", dt, y, (iy < NUM_DATA ? array_uy_prediction[iy] : array_uy_prediction[NUM_DATA-1]));
  }
  fflush (bpif);
  fclose (bpif);
  
  /*
  
  // f
  fname = "./input/f_batch_prediction_inputs.csv";  
  bpif = fopen (fname, "w");
  fprintf (bpif, "dt,y,f\n");    
  for (double y = -.5*domain_height+boundary_resolution;  
              y <= .5*domain_height-boundary_resolution; 
              y += boundary_resolution) {   
    int iy = (int)((y+jet_y)/boundary_resolution); 
    fprintf (bpif, "%.14g,%.14g,%.14g\n", dt, y, (iy < NUM_DATA ? array_f_prediction[iy] : array_f_prediction[NUM_DATA-1]));
  }
  fflush (bpif);
  fclose (bpif);
  
  */
   
}  



  

/**
We produce animations of some fields */

event movies (i=0; i+=5; i<11) { // (i=0; i++; i<1) {
  
  printf("%d   MOVIES\n", i);
    
//   scalar ff[];
//   foreach()
//     ff[] = clamp(f[], 0, 1);
//   boundary ({ff});
  
  
  FILE *fields;
  
  char fname[80];
  sprintf (fname, "./results/fields-%1.6e.txt", t);  
  fields = fopen (fname, "w");  
  output_field (fp = fields, linear = false);
  
  fclose (fields);
  
  
/*  
  output_ppm (u.x, file = "ux.gif",
              box = {{-jet_x,-.5*domain_height},{domain_length,.5*domain_height}}, 
              linear = true);
  
  output_ppm (u.y, file = "uy.gif",
              box = {{-jet_x,-.5*domain_height},{domain_length,.5*domain_height}}, 
              linear = true);  
  
  scalar omega[];
  vorticity (u, omega);
  output_ppm (omega, file = "vort.gif", 
              box = {{-jet_x,-.5*domain_height},{domain_length,.5*domain_height}}, 
              linear = true); // min = -5, max = 5
  
  output_ppm (ff, file = "f.gif",
              box = {{-jet_x,-.5*domain_height},{domain_length,.5*domain_height}}, 
              linear = true, min = 0, max = 1);
*/
              
  
}

event sentinel_predictor_control (t=end) {
  
  FILE *sf; 
  
  char fname[80];
  sprintf (fname, "./sentinel_predictor_ux.end");  
  sf = fopen (fname, "w");  
  fclose (sf);
  
  sprintf (fname, "./sentinel_predictor_uy.end");  
  sf = fopen (fname, "w");  
  fclose (sf);
    
}













