#ifndef FIELD_HPP
#define FIELD_HPP

#include <string>
#include <map>
#include <vector>
#include <xtensor/xadapt.hpp>
#include <xtensor/xbuilder.hpp>
#include <xtensor/xio.hpp>


#ifdef WITH_LAMMPS
// #define LAMMPS_LIB_MPI
#include <library.h>
#endif

#include "configuration.hpp"

enum field_type
{
    LAMMPS,
    MACHINE_LEARNING
};

class virtual_class_field
{
public:
    virtual_class_field() {}
    virtual_class_field(std::map<std::string, std::string> field_model_parameter) {}
    ~virtual_class_field() {}

    virtual void evaluate(configuration &_configuration) = 0;
};

class LammpsField : public virtual_class_field
{
private:
    void *liblammps;
    double xlo, xhi, ylo, yhi, zlo, zhi;

public:
    LammpsField(std::map<std::string, std::string> init_param)
    {
        liblammps = nullptr;
        init_param["lattice_x"] = 16.291494;
        init_param["lattice_y"] = 16.291494;
        init_param["lattice_z"] = 16.291494;

        xlo = 0, xhi = 16.291494, ylo = 0, yhi = 16.291494, zlo = 0, zhi = 16.291494;
    }
    ~LammpsField();

    void evaluate(configuration &_configuration);
};

class MlField : public virtual_class_field
{
private:
    double scalar;
    std::vector<double> field;

public:
    MlField(std::map<std::string, std::string> init_param);
    ~MlField();

    void evaluate(configuration &_configuration);
};

class LossField : public virtual_class_field
{
private:
    double scalar;
    std::vector<double> field;

public:
    LossField(std::map<std::string, std::string> init_param);
    ~LossField();

    void evaluate(configuration &_configuration);
};

class field
{
private:
    std::map<enum field_type, virtual_class_field *> fields;

    double scalar;
    std::vector<double> _field;

public:
    field(std::map<enum field_type, std::map<std::string, std::string>> init_fields)
    {
        for (auto const &[_field_class, _field_parameters] : init_fields)
        {
            switch (_field_class)
            {
            case LAMMPS:
                fields[_field_class] = new LammpsField(_field_parameters);
                break;
            case MACHINE_LEARNING:
                fields[_field_class] = new MlField(_field_parameters);
                break;

            default:
                /* code */
                break;
            }
        }
    }
    ~field();

    void set_field(std::string _m);

    void evaluate(configuration &_configuration, enum field_type _f);
};

#endif