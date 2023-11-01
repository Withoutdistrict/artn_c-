#ifndef GENERATOR_HPP
#define GENERATOR_HPP

#include <vector>
#include <map>
#include <string>

#include <xtensor/xarray.hpp>

#include "configuration.hpp"
#include "field.hpp"
#include "minimizer.hpp"
#include "eigen_solver.hpp"

class generator
{
private:
    // std::map<std::string, std::string> generator_parameter;

    field *p_field;
    minimizer *p_minimizer;
    eigen_solver *p_eigen_solver;

    void push(configuration &configuration);
    void minimize(configuration &configuration);
    bool eigen_solve(configuration &configuration);

public:
    configuration c_iterator_location;
    configuration c_initial_minimum_location;
    configuration c_saddle_location;
    configuration c_final_minimum_location;

    generator(std::map<std::string, std::string> generator_parameter)
    {
        std::map<field_type, std::map<std::string, std::string>> init_field;
        init_field[field_type::LAMMPS] = std::map<std::string, std::string>();
        p_field = new field(init_field);

        std::map<minimizer_type, std::map<std::string, std::string>> init_minimizer;
        p_minimizer = new minimizer(init_minimizer);

        std::map<eigen_solver_type, std::map<std::string, std::string>> init_eigen_solver;
        init_eigen_solver[eigen_solver_type::LANCZOS] = std::map<std::string, std::string>();
        p_eigen_solver = new eigen_solver(init_eigen_solver);
    }

    configuration generate(configuration configuration);

    ~generator();
};

#endif