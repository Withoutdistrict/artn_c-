#ifndef MINIMIZER_HPP
#define MINIMIZER_HPP

#include <string>
#include <map>

#include "configuration.hpp"
#include "field.hpp"

enum minimizer_type
{
    FIRE,
    STEEPESS_DESCENT
};

class virtual_class_minimizer
{
public:
    virtual bool minimize(configuration &_configuration, field *_p_field) = 0;
};

class steepess_descent : public virtual_class_minimizer
{
public:
    steepess_descent(std::map<std::string, std::string> init_param)
    {
        init_param["aaa"] = "0";
    }

    bool minimize(configuration &_configuration, field *_p_field);
};

class fire : public virtual_class_minimizer
{
public:
    fire(std::map<std::string, std::string> init_param)
    {
        init_param["aaa"] = "0";
    }

    bool minimize(configuration &_configuration, field *_p_field);
};

class minimizer
{
private:
    std::map<enum minimizer_type, virtual_class_minimizer *> minimizers;

public:
    minimizer(std::map<enum minimizer_type, std::map<std::string, std::string>> init_minimizers)
    {
        for (auto const &[_minimizer_class, _minimizer_parameters] : init_minimizers)
        {
            // std::pair<enum minimizer_type, virtual_class_minimizer*> init_minimizer(_minimizer_class, new steepess_descent(_minimizer_parameters));
            switch (_minimizer_class)
            {
            case FIRE:
                minimizers[_minimizer_class] = new fire(_minimizer_parameters);
                break;
            case STEEPESS_DESCENT:
                minimizers[_minimizer_class] = new steepess_descent(_minimizer_parameters);
                break;

            default:
                /* code */
                break;
            }
        }
    }

    bool minimize(configuration &_configuration, enum minimizer_type _m, field *_p_field);
};

#endif