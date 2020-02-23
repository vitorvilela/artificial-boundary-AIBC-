// https://isocpp.org/wiki/faq/mixing-c-and-cpp

// g++ -std=c++14 -O3 bc.cpp

#include <fdeep/fdeep.hpp>


extern "C" float get_next_ux(float dt, float y, float ux);
extern "C" float get_next_uy(float dt, float y, float uy);
extern "C" float get_next_f(float dt, float y, float f);

float get_next_ux(float dt, float y, float ux) 
{
  const auto model = fdeep::load_model("fd_model_ux.json");
  const auto result = model.predict(
        {fdeep::tensor5(fdeep::shape5(1, 1, 1, 1, 3), {dt, y, ux})}
                                   );
  std::size_t pos = 0;
  
  return result[0].get(pos, pos, pos, pos, pos);
  
  //std::cout << fdeep::show_tensor5s(result) << std::endl;
}


float get_next_uy(float dt, float y, float uy) 
{
  const auto model = fdeep::load_model("fd_model_uy.json");
  const auto result = model.predict(
        {fdeep::tensor5(fdeep::shape5(1, 1, 1, 1, 3), {dt, y, uy})}
                                   );
  std::size_t pos = 0;
  
  return result[0].get(pos, pos, pos, pos, pos);
  
  //std::cout << fdeep::show_tensor5s(result) << std::endl;
}


float get_next_f(float dt, float y, float f) 
{
  const auto model = fdeep::load_model("fd_model_f.json");
  const auto result = model.predict(
        {fdeep::tensor5(fdeep::shape5(1, 1, 1, 1, 3), {dt, y, f})}
                                   );
  std::size_t pos = 0;
  
  return result[0].get(pos, pos, pos, pos, pos);
  
  //std::cout << fdeep::show_tensor5s(result) << std::endl;
}


int main() {
  
  std::cout << get_next_f(1.e-3, 0.5, 0.5);
  
}