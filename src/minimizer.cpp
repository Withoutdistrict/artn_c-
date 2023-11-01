#include "minimizer.hpp"

bool steepess_descent::minimize(configuration &_configuration, field *_p_field) { return true; }

bool fire::minimize(configuration &_configuration, field *_p_field) { return true; }

bool minimizer::minimize(configuration &_configuration, enum minimizer_type _m, field *_p_field)
{
    // enum minimizer m = minimizer::steepess_descent;
    // m = minimizer::steepess_descent;

    minimizers[_m]->minimize(_configuration, _p_field);

    return true;
}
