// https://isocpp.org/wiki/faq/mixing-c-and-cpp

#include <fdeep/fdeep.hpp>
#include "bc.h"

float get_next_ux(float dt, float y, float ux) 
{
  const auto model = fdeep::load_model("fd_model_ux.json");
  const auto result = model.predict(
        {fdeep::tensor5(fdeep::shape5(1, 1, 1, 1, 3), {dt, y, ux})}
                                   );
  std::size_t pos = 0;
  
  return result[0].get(pos, pos, pos, pos, pos);
  
}


float get_next_uy(float dt, float y, float uy) 
{
  const auto model = fdeep::load_model("fd_model_uy.json");
  const auto result = model.predict(
        {fdeep::tensor5(fdeep::shape5(1, 1, 1, 1, 3), {dt, y, uy})}
                                   );
  std::size_t pos = 0;
  
  return result[0].get(pos, pos, pos, pos, pos);
  
}


float get_next_f(float dt, float y, float f) 
{
  const auto model = fdeep::load_model("fd_model_f.json");
  const auto result = model.predict(
        {fdeep::tensor5(fdeep::shape5(1, 1, 1, 1, 3), {dt, y, f})}
                                   );
  std::size_t pos = 0;
  
  return result[0].get(pos, pos, pos, pos, pos);
  
}


