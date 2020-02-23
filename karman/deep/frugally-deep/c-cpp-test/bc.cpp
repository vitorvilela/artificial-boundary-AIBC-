#include <fdeep/fdeep.hpp>
#include "bc.h"

void get_next_ux(float dt, float y, float ux, float &new_ux) 
{
  const auto model = fdeep::load_model("fd_model_ux.json");
  const auto result = model.predict(
        {fdeep::tensor5(fdeep::shape5(1, 1, 1, 1, 3), {dt, y, ux})}
                                   );
  std::size_t pos = 0;
  
  new_ux = result[0].get(pos, pos, pos, pos, pos);
  
}


void get_next_uy(float dt, float y, float uy, float &new_uy) 
{
  const auto model = fdeep::load_model("fd_model_uy.json");
  const auto result = model.predict(
        {fdeep::tensor5(fdeep::shape5(1, 1, 1, 1, 3), {dt, y, uy})}
                                   );
  std::size_t pos = 0;
  
  new_uy = result[0].get(pos, pos, pos, pos, pos);
  
}


void get_next_f(float dt, float y, float f, float &new_f) 
{
  const auto model = fdeep::load_model("fd_model_f.json");
  const auto result = model.predict(
        {fdeep::tensor5(fdeep::shape5(1, 1, 1, 1, 3), {dt, y, f})}
                                   );
  std::size_t pos = 0;
  
  new_f = result[0].get(pos, pos, pos, pos, pos);
  
}


