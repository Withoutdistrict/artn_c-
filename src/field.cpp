#include "field.hpp"

LammpsField::~LammpsField() {}

void LammpsField::evaluate(configuration &_configuration)
{
    auto &c_parameters = _configuration.parameters;
    auto &c_field = _configuration.field;
    auto &c_scalar = _configuration.scalar;

    const int S = c_parameters.size();
    const int N = S / 3;

    double x[3 * N];
    memcpy(&x[0], &c_parameters[0], 3 * N * sizeof(double));

    char *lmpargv[] = {(char *)"lmp", (char *)"-log", (char *)"none", (char *)"-screen", (char *)"none"};
    int lmpargc = sizeof(lmpargv) / sizeof(char *);
    liblammps = lammps_open_no_mpi(lmpargc, lmpargv, NULL);

    lammps_command(liblammps, (char *)"units metal");
    lammps_command(liblammps, (char *)"atom_style atomic");
    lammps_command(liblammps, (char *)"atom_modify map array");

    int type[N];
    for (int i = 0; i < N; i++)
        type[i] = 1;

    std::string str_region = "region simbox block " + std::to_string(xlo) + " " + std::to_string(xhi) + " " + std::to_string(ylo) + " " + std::to_string(yhi) + " " + std::to_string(zlo) + " " + std::to_string(zhi);
    char *chr_region = strcpy((char *)malloc(str_region.length() + 1), str_region.c_str());
    lammps_command(liblammps, chr_region);
    lammps_command(liblammps, (char *)"create_box 1 simbox");

    lammps_create_atoms(liblammps, N, NULL, type, x, NULL, NULL, 1);

    lammps_command(liblammps, (char *)"mass 1 28.0855");
    lammps_command(liblammps, (char *)"group Si type 1");
    lammps_command(liblammps, (char *)"pair_style sw");
    lammps_command(liblammps, (char *)"pair_coeff * * Si.sw Si");
    lammps_command(liblammps, (char *)"run 0");

    c_scalar = *(double *)lammps_extract_compute(liblammps, (char *)"thermo_pe", 0, 0);

    double f[3 * N];
    lammps_gather_atoms(liblammps, (char *)"f", 1, 3, f);

    std::vector<int> shape = {N, 3};
    c_field = xt::zeros<double>(shape);
    memcpy(&c_field[0], &f[0], 3 * N * sizeof(double));

    free(chr_region);
    lammps_close(liblammps);
}

MlField::MlField(std::map<std::string, std::string> field_model_parameter) {}
MlField::~MlField() {}
void MlField::evaluate(configuration &configuration) {}

LossField::LossField(std::map<std::string, std::string> field_model_parameter) {}
LossField::~LossField() {}
void LossField::evaluate(configuration &configuration) {}

void field::evaluate(configuration &_configuration, enum field_type _f)
{
    fields[_f]->evaluate(_configuration);
}
