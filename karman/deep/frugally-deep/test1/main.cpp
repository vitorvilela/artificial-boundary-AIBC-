// g++ -std=c++14 -O3 main.cpp
// ./a.out
#include <fdeep/fdeep.hpp>
int main()
{
    const auto model = fdeep::load_model("fd_model_f.json");
    const auto result = model.predict(
        {fdeep::tensor5(fdeep::shape5(1, 1, 1, 1, 3), {1.e-3, 0.5, 0.5})});
    std::cout << fdeep::show_tensor5s(result) << std::endl;
}
