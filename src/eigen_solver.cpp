#include "eigen_solver.hpp"

lanczos::~lanczos() {}

bool lanczos::eigen_solve(configuration &_configuration, field *_p_field)
{
    s = _configuration.parameters.shape()[0];
    xt::xtensor<double, 3> lanczos_matrix = xt::zeros<double>({n_vec, s, 3});
    configuration t_configuration;

    // auto t_parameters = xt::view(t_configuration.parameters, xt::newaxis());
    // auto t_field = xt::view(t_configuration.field, xt::newaxis());

    // auto c_parameters = xt::view(_configuration.parameters, xt::newaxis());
    // auto c_field = xt::view(_configuration.field, xt::newaxis());
    // auto c_projection = xt::view(_configuration.parameters, xt::newaxis());

    auto &t_parameters = t_configuration.parameters;
    auto &t_field = t_configuration.field;
    auto &t_projection = t_configuration.parameters;

    auto &c_parameters = _configuration.parameters;
    auto &c_field = _configuration.field;
    auto &c_projection = _configuration.parameters;

    auto z0 = xt::view(lanczos_matrix, xt::keep(0));
    auto z1 = xt::view(lanczos_matrix, xt::keep(1));
    auto z2 = xt::view(lanczos_matrix, xt::keep(2));
    auto zk = xt::view(lanczos_matrix, xt::keep(0));

    z0 = c_projection;

    z0 *= 1 / xt::sqrt(xt::norm_sq(z0));
    t_parameters = c_parameters + z0 * dl;

    _p_field->evaluate(t_configuration, _f);

    auto df = -(t_field - c_field) / dl;

    double alpha = xt::sum(z0 * df)();

    if (eigenvalue != 0)
        pp = std::abs((alpha - eigenvalue) / eigenvalue);

    if (pp < eigval_conv_thr)
    {
        eigenvalue = alpha;
        converged = true;
        exit;
    }

    z1 = df - alpha * z0;
    double beta = xt::sqrt(xt::norm_sq(z1))();
    z1 *= 1 / xt::sqrt(xt::norm_sq(z1));

    xt::xtensor<double, 2> H = xt::zeros<double>({n_vec, n_vec});

    H[{0, 0}] = alpha;
    H[{0, 1}] = beta;
    H[{1, 0}] = beta;

    auto prev_evec = z0;
    double prev_eval = alpha;

    for (int i = 1; i < n_vec; i++)
    {
        auto z0 = xt::view(lanczos_matrix, xt::keep(i - 1));
        auto z1 = xt::view(lanczos_matrix, xt::keep(i));

        t_parameters = c_parameters + z1 * dl;
        _p_field->evaluate(t_configuration, _f);

        auto df = -(t_field - c_field) / dl;
        // df *= -1 / dl;
        alpha = xt::sum(z1 * df)();
        H[{i, i}] = alpha;

        // if (i < n_vec)
        // {
        auto z2 = xt::view(lanczos_matrix, xt::keep(i + 1));
        z2 = df - alpha * z1 - beta * z0;

        // Gramm-Schmidt up to i-1 because z2(:) is already orthogonal to z1(:)
        for (unsigned int k = 0; k < i - 1; k++)
        {
            auto zk = xt::view(lanczos_matrix, xt::keep(k));
            z2 += -xt::sum(z2 * zk) * zk;
        }
        if (xt::norm_sq(z2)() < 0.0001)
            exit;
        z2 *= 1 / xt::sqrt(xt::norm_sq(z2));

        beta = xt::sum(z2 * df)();
        beta *= 1 / xt::sqrt(xt::norm_sq(z2))();

        H[{i + 1, i}] = beta;
        H[{i, i + 1}] = beta;
        // }

        xt::xtensor<double, 2> H1 = xt::view(H, xt::range(0, i + 1), xt::range(0, i + 1));
        xt::xtensor<double, 1> eigen_values = xt::zeros<double>({i + 1});
        xt::xtensor<double, 2> eigen_vectors = xt::zeros<double>({i + 1, i + 1});

        double min_eigen_value;
        xt::xtensor<double, 1> min_eigen_vector = xt::zeros<double>({i + 1});

        auto eig = xt::linalg::eig(H1);
        eigen_values = xt::real(xt::get<0>(eig));
        eigen_vectors = xt::real(xt::get<1>(eig));

        int idx_emin = xt::argmin(eigen_values)();
        min_eigen_value = eigen_values[{idx_emin}];
        min_eigen_vector = xt::view(eigen_vectors, idx_emin);

        _configuration.eigen_value = min_eigen_value;
        _configuration.eigen_vector = min_eigen_vector;

        // xt::xtensor<double, 2> ii_mat = xt::ones<double>({i+1, 3, s});
        // std::cout << ii_mat*min_eigen_vector << std:: endl;
        // std::cout << xt::adapt(ii_mat.shape()) << std::endl;

        for(int q = 0; q < i+1; q++)
            xt::view(lanczos_matrix, xt::keep(q)) *= min_eigen_vector[q];

        // _configuration.projection = xt::linalg::dot(ii_mat, xt::view(lanczos_matrix, xt::range(0, i+1)));
        _configuration.projection = xt::sum(xt::view(lanczos_matrix, xt::range(0, i+1)), {0});
        _configuration.projection *= 1 / xt::linalg::norm(_configuration.projection);

        std::cout << min_eigen_value << std::endl;

        pp = std::abs((min_eigen_value - prev_eval) / prev_eval);
        if (pp < eigval_conv_thr)
        {
            converged = true;
            return true;
        }

        prev_eval = min_eigen_value;
        prev_evec = _configuration.projection;
        // prev_evec = min_eigen_vector;
    }

    // center
    return false;
}

bool eigen_solver::eigen_solve(configuration &_configuration, enum eigen_solver_type _s, field *_p_field)
{
    return eigen_solvers[_s]->eigen_solve(_configuration, _p_field);
}

void eigen_solver::set_eigen_solver(std::string _m) { m = _m; }
