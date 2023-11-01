#include "generator.hpp"
#include "tmp.hpp"

// generator::generator(std::map<std::string, std::string> generator_parameter)
// {
//     field = new Field("lammps", generator_parameter);
//     field->set_field(generator_parameter["field_m"]);

// std::map<enum minimizer_type, std::map<std::string, std::string>> init_minimizers;
// p_minimizer = new minimizer(init_minimizers);

//     c_iterator_location = initial_configuration;
//     // minimize(initial_configuration);

//     bool saddle_sucess = false;
//     do
//     {
//         push(c_iterator_location);
//         minimize(c_iterator_location);
//     } while (eigen_solve(c_iterator_location));

// bool min_sucess = false;
// c_initial_minimum_location = c_iterator_location;
// c_final_minimum_location = c_iterator_location;
// while (!min_sucess)
// {
// min_sucess = minimize(c_initial_minimum_location);
// min_sucess = minimize(c_final_minimum_location);
// }
// }

void generator::push(configuration &_configuration)
{
    // field->evaluate(configuration);
    // if(std::stoi(generator_parameter["p_iter"]) < std::stoi(generator_parameter["p_iter_min"]))
    // {
    _configuration.parameters += _configuration.delta;
    // generator_parameter["p_iter"] = std::stoi(generator_parameter["p_iter"]) + 1;
    // }

    // push_direction =  (1.0 - smooth_iter/(intern_smooth+1))*initial_direction - SIGN(1.0,fpar)*(smooth_iter/(intern_smooth+1))*projection
    // push_direction =  push_direction/SQRT(DOT_PRODUCT(push_direction, push_direction))
    // pos            =  pos + MIN(step_size, ABS(fpar)/MAX(ABS(eigenvalue), 0.5))*push_direction

    // else
    // {
    //     _configuration.configuration_parameters +=
    // }
}
void generator::minimize(configuration &_configuration)
{
    // minimizer->minimize();
}
bool generator::eigen_solve(configuration &_configuration)
{
    return true;
}

using namespace xt::placeholders;

configuration generator::generate(configuration _configuration)
{
    // p_minimizer->minimize(_configuration, minimizer_type::STEEPESS_DESCENT, p_field);
    // p_eigen_solver->eigen_solve(_configuration, eigen_solver_type::LANCZOS, p_field);
    configuration cfg1;
    cfg1.parameters = v1;
    cfg1.projection = xt::zeros<double>({215, 3});
    xt::view(cfg1.projection, xt::keep(106)) = xt::random::rand<double>({3}) * 2 - 1;
    std::cout << xt::view(cfg1.projection, xt::all()) << "\n";

    _configuration = cfg1;
    p_field->evaluate(_configuration, field_type::LAMMPS);
    p_eigen_solver->eigen_solve(_configuration, eigen_solver_type::LANCZOS, p_field);

    return _configuration;
}