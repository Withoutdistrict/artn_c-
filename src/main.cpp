#include <vector>
#include <string>
#include <map>
#include <iostream>

// #include <xtensor/xarray.hpp>
// #include <xtensor/xview.hpp>
// #include <xtensor/xrandom.hpp>
// #include <xtensor/xio.hpp>

#include "generator.hpp"
#include "eigen_solver.hpp"
#include "configuration.hpp"
#include "field.hpp"

#define I 161

int main(int argc, char *argv[])
{
	std::map<std::string, std::string> generator_parameter;
	generator *event = new generator(generator_parameter);

	configuration cfg;
	event->generate(cfg);

	// field *p_field = new field("lammps", field_model_parameter);
	// field->set_field("Lammps_Field");
	// p_field->evaluate(cfg1);

	// lanczos *p_eigen_solver = new lanczos();
	// p_eigen_solver->set_eigen_solver("Lanczos");
	// p_eigen_solver->eigen_solve(cfg1);

	// xt::xarray<double> matrix = xt::random::rand<double>({4, 4});
	// auto v = xt::view(matrix, xt::keep(0));
	// v += 1;
	// std::cout << xt::view(matrix, xt::all()) << std::endl << std::endl;
	// std::cout << v << std::endl;

	// std::vector<std::string> args;
	// std::map<std::string, std::string> opts;

	// Configuration initial_configuration;
	// initial_configuration.parameters = v;
	// initial_configuration.delta = *(new xt::xarray<double>(645));
	// for(auto &x: initial_configuration.delta)
	// 	x = 0;

	// initial_configuration.delta[I*1] = 0.2;
	// initial_configuration.delta[I*2] = 0.2;
	// initial_configuration.delta[I*3] = 0.2;
	// Generator *generator = new Generator(opts, initial_configuration);

	return 0;
}
