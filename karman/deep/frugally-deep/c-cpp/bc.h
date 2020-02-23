
/* This header can be read by both C and C++ compilers */
#ifndef BC_H
#define BC_H

#ifdef __cplusplus
extern "C" {
#endif
  
extern float get_next_ux(float dt, float y, float ux);
extern float get_next_uy(float dt, float y, float uy);
extern float get_next_f(float dt, float y, float f);

#ifdef __cplusplus
}
#endif

#endif
