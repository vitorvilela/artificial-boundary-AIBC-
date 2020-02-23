
/* This header can be read by both C and C++ compilers */
#ifndef BC_H
#define BC_H

#ifdef __cplusplus
extern "C" {
#endif
  
extern void get_next_ux(float dt, float y, float ux, float &new_ux);
extern void get_next_uy(float dt, float y, float uy, float &new_uy);
extern void get_next_f(float dt, float y, float f, float &new_f);

#ifdef __cplusplus
}
#endif

#endif
