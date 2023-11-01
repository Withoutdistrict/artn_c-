#ifndef EIGEN_SOLVER_HPP
#define EIGEN_SOLVER_HPP

#include <string>
#include <map>

#include <xtensor/xtensor.hpp>
#include <xtensor/xarray.hpp>
#include <xtensor/xbuilder.hpp>
#include <xtensor/xnorm.hpp>
#include <xtensor/xview.hpp>
#include <xtensor/xsort.hpp>
#include <xtensor-blas/xlinalg.hpp>
#include <xtensor/xadapt.hpp>

#include "configuration.hpp"
#include "field.hpp"

enum eigen_solver_type
{
    LANCZOS,
};

class virtual_class_eigen_solver
{
public:
    virtual bool eigen_solve(configuration &_configuration, field *_p_field) = 0;
};

class lanczos : public virtual_class_eigen_solver
{
private:
    std::map<std::string, std::string> eigen_solver_parameter;
    field_type _f;
    int n_lvec;
    double eigenvalue;
    double eigvec_conv_thr;
    double eigval_conv_thr;
    double pp;
    bool converged;
    int s;
    int n_vec;
    double dl;

public:
    lanczos(std::map<std::string, std::string> init_param)
    {
        eigenvalue = 0;
        eigvec_conv_thr = 0.0001;
        eigval_conv_thr = 0.01;
        pp = 9999;
        converged = false;
        n_vec = 10;
        dl = 0.01;
        n_lvec = 10;
        _f = field_type::LAMMPS;
    }
    ~lanczos();
    bool eigen_solve(configuration &_configuration, field *_p_field);
};

class eigen_solver
{
private:
    std::map<enum eigen_solver_type, virtual_class_eigen_solver *> eigen_solvers;
    std::string m;
    // field *p_field;

public:
    eigen_solver(std::map<enum eigen_solver_type, std::map<std::string, std::string>> init_eigen_solvers)
    {
        for (auto const &[_eigen_solver_class, _eigen_solver_parameters] : init_eigen_solvers)
        {
            switch (_eigen_solver_class)
            {
            case LANCZOS:
                eigen_solvers[_eigen_solver_class] = new lanczos(_eigen_solver_parameters);
                break;
            default:
                /* code */
                break;
            }
        }
    }

    ~eigen_solver();
    bool eigen_solve(configuration &_configuration, enum eigen_solver_type _s, field *_p_field);
    void set_eigen_solver(std::string _m);
};

#endif
