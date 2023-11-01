#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP

#include <vector>
#include <xtensor/xarray.hpp>
#include <xtensor/xtensor.hpp>
#include <xtensor/xrandom.hpp>
#include <xtensor/xadapt.hpp>

class configuration
{
public:
    xt::xarray<double, xt::layout_type::row_major> parameters;

    double scalar;
    xt::xarray<double, xt::layout_type::row_major> field;

    xt::xarray<double, xt::layout_type::row_major> delta;
    xt::xarray<double, xt::layout_type::row_major> projection;

    double eigen_value;
    xt::xarray<double, xt::layout_type::row_major> eigen_vector;

    configuration(/* args */)
    {
        std::vector<size_t> s = {215, 3, 0, 0};

        parameters = xt::xarray<double>(s);
        field = xt::xarray<double>(s);
        delta = xt::xarray<double>(s);
        projection = xt::xarray<double>(s);
        // std::cout << parameters.shape()[0] << " " << parameters.shape()[1] << " " << parameters.shape()[2] << " " << parameters.shape()[3] << std::endl;
    }
    ~configuration();
};

#endif