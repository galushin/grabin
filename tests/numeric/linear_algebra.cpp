/* (c) 2019 Галушин Павел Викторович, galushin@gmail.com

Данный файл -- часть библиотеки Grabin.

Grabin -- это свободной программное обеспечение: вы можете перераспространять ее и/или изменять ее
на условиях Стандартной общественной лицензии GNU в том виде, в каком она была опубликована Фондом
свободного программного обеспечения; либо версии 3 лицензии, либо (по вашему выбору) любой более
поздней версии.

Это программное обеспечение распространяется в надежде, что оно будет полезной, но БЕЗО ВСЯКИХ
ГАРАНТИЙ; даже без неявной гарантии ТОВАРНОГО ВИДА или ПРИГОДНОСТИ ДЛЯ ОПРЕДЕЛЕННЫХ ЦЕЛЕЙ.
Подробнее см. в Стандартной общественной лицензии GNU.

Вы должны были получить копию Стандартной общественной лицензии GNU вместе с этим программным
обеспечение. Если это не так, см. https://www.gnu.org/licenses/.
*/

#include <grabin/math/matrix.hpp>
#include <grabin/numeric/linear_algebra.hpp>

#include <grabin/algorithm.hpp>

#include <catch2/catch.hpp>
#include "../grabin_test.hpp"

TEST_CASE("matrix-vector product - incompatible dimensions")
{
    using Matrix = grabin::matrix<double>;
    using Vector = grabin::math_vector<double>;

    auto const n = 2;
    auto const m = 3;

    Matrix const A(n, m);
    Vector const x(m+2);

    REQUIRE(A.dim().second != x.dim());

    CHECK_THROWS_AS(A * x, std::logic_error);
}

TEST_CASE("matrix-vector product")
{
    using Value = int;
    using Matrix = grabin::matrix<Value>;
    using Vector = grabin::math_vector<Value>;

    auto const n = 2;
    auto const m = 3;

    Matrix A(n, m);
    Vector x(m);

    std::uniform_int_distribution<Value> distr(-20, 20);
    auto gen = [&]{ return distr(grabin_test::random_engine()); };

    grabin::generate(A, gen);
    grabin::generate(x, gen);

    auto const y = A * x;

    CHECK(y[0] == A(0, 0)*x[0] + A(0, 1)*x[1] + A(0, 2)*x[2]);
    CHECK(y[1] == A(1, 0)*x[0] + A(1, 1)*x[1] + A(1, 2)*x[2]);
}

TEST_CASE("minres solver")
{
    using Value = double;
    using Matrix = grabin::matrix<Value>;
    using Vector = grabin::math_vector<Value>;

    Matrix A(2, 2);
    A(0, 0) = 1;
    A(0, 1) = -0.5;
    A(1, 0) = - 0.5;
    A(1, 1) = 2;

    Vector x(2);

    std::uniform_real_distribution<Value> distr(-10, 10);
    auto gen = [&]{ return distr(grabin_test::random_engine()); };
    grabin::generate(x, gen);

    auto const b = A*x;

    auto const x0 = grabin::linear_algebra::minimal_residue(A, b);

    auto const eps = 1e-3;
    CHECK_THAT(x0, grabin_test::Matchers::elementwise_within_abs(x, eps));
}

#include <grabin/stochastic/all.hpp>

TEST_CASE("LU-solver: simplest queueing system")
{
    using Matrix = grabin::matrix<double>;
    using Vector = grabin::math_vector<double>;

    auto & rnd = grabin_test::random_engine();
    std::uniform_real_distribution<double> distr(0.1, 10);

    for(auto n = 1; n < 15; ++ n)
    {
        auto const nu_order = distr(rnd);
        auto const nu_service = distr(rnd);

        // Составляем систему уравнений
        Matrix lambda(n+1, n+1);

        lambda(0, 1) = nu_order;
        for(auto i : grabin::view::indices(n-1))
        {
            lambda(i+1, i) = nu_service;
            lambda(i+1, i+2) = nu_order;
        }
        lambda(n, n-1) = nu_service;

        // Решаем систему
        auto const P = grabin::stochastic::ctmc_stationary(lambda);

        // Находим вероятности по формулам
        auto const alpha = nu_order / nu_service;

        Vector P1(n+1);
        P1[0] = 1.0;

        for(auto const & i : grabin::view::indices(n))
        {
            P1[i+1] = alpha * P1[i];
        }

        auto const P1_sum = std::accumulate(P1.begin(), P1.end(), 0 * P1[0]);
        REQUIRE(std::abs(P1_sum) > 1e-10);

        for(auto & p : P1)
        {
            p /= P1_sum;
        }

        // Сравниваем решения
        CAPTURE(lambda, alpha);
        CHECK_THAT(P, grabin_test::Matchers::elementwise_within_abs(P1, 1e-6));
    }
}

TEST_CASE("LU-solver: many processors, one bus")
{
    using Matrix = grabin::matrix<double>;
    using Vector = grabin::math_vector<double>;

    auto & rnd = grabin_test::random_engine();
    std::uniform_real_distribution<double> distr(0.1, 10);

    for(auto n = 1; n < 15; ++ n)
    {
        auto const nu_order = distr(rnd);
        auto const nu_service = distr(rnd);

        // Составляем систему уравнений
        Matrix lambda(n+1, n+1);

        lambda(0, 1) = n*nu_order;
        for(auto i : grabin::view::indices(n-1))
        {
            lambda(i+1, i) = nu_service;
            lambda(i+1, i+2) = (n - i - 1) * nu_order;
        }
        lambda(n, n-1) = nu_service;

        // Решаем систему
        auto const P = grabin::stochastic::ctmc_stationary(lambda);

        // Находим вероятности по формулам
        auto const alpha = nu_order / nu_service;

        Vector P1(n+1);

        P1[0] = 1;

        for(auto const & i : grabin::view::indices(n))
        {
            P1[i+1] = alpha * (n - i) * P1[i];
        }

        auto const P1_sum = std::accumulate(P1.begin(), P1.end(), 0 * P1[0]);
        REQUIRE(std::abs(P1_sum) > 1e-10);

        for(auto & p : P1)
        {
            p /= P1_sum;
        }

        // Сравниваем решения
        CAPTURE(lambda, alpha);
        REQUIRE_THAT(P, grabin_test::Matchers::elementwise_within_abs(P1, 1e-6));
    }
}

TEST_CASE("LU-solver: many processors, many bus")
{
    using Real = long double;
    using Matrix = grabin::matrix<Real>;
    using Vector = grabin::math_vector<Real>;

    auto & rnd = grabin_test::random_engine();
    std::uniform_real_distribution<Real> distr(0.1, 10);

    for(auto n_bus : grabin::view::indices(1, 5))
    for(auto n_cpu : grabin::view::indices(1, 6))
    {
        auto const nu_order = distr(rnd);
        auto const nu_service = distr(rnd);

        auto const h = std::min(n_cpu, n_bus);
        auto const n_states = 1 + h * (2 * n_cpu - h + 1) / 2;


        // Составляем систему уравнений

        Matrix lambda(n_states, n_states);
        auto max_index_2 = [=](Matrix::size_type k)
        {
            if(k == 0)
            {
                return 0*k;
            }

            return n_cpu - k;
        };

        auto index = [=](Matrix::size_type k, Matrix::size_type l)
        {
            if(!(0 <= k && k <= h) || !(0 <= l && l <= max_index_2(k)))
            {
                throw std::logic_error("bad state");
            }

            if(k == 0)
            {
                return 0*k;
            }

            return 1 + (k - 1) * (2 * n_cpu + 2 - k) / 2 + l;
        };

        for(auto k : grabin::view::indices(h+1))
        for(auto l : grabin::view::indices(max_index_2(k) + 1))
        {
            if(k > 0 && l <= max_index_2(k-1))
            {
                lambda(index(k-1, l), index(k, l)) = nu_order * (n_bus - k + 1) / n_bus * (n_cpu - k - l + 1);
                lambda(index(k, l), index(k-1, l)) = pow(Real(k - 1) / k, l) * k * nu_service;
            }

            if(k < h && l <= max_index_2(k+1))
            {
                lambda(index(k+1, l), index(k, l)) = pow(k / Real(k+1), l) * (k+1) * nu_service;
                lambda(index(k, l), index(k+1, l)) = nu_order * (n_bus - k) / n_bus * (n_cpu - k - l);
            }

            if(l > 0)
            {
                lambda(index(k, l-1), index(k, l)) = nu_order * k / n_bus * (n_cpu - k - l + 1);
                lambda(index(k, l), index(k, l-1)) = (1-pow(Real(k - 1) / k, l)) * k * nu_service;
            }

            if(l < max_index_2(k))
            {
                lambda(index(k, l+1), index(k, l)) = (1-pow(Real(k - 1) / k, l+1)) * k * nu_service;
                lambda(index(k, l), index(k, l+1)) = nu_order * k / n_bus * (n_cpu - k - l);
            }
        }

        // Решаем систему
        auto const P = grabin::stochastic::ctmc_stationary(lambda);

        // Находим вероятности по формулам
        auto const rho = nu_order / nu_service;

        Vector P1(n_states);

        P1[0] = 1;

        for(auto k : grabin::view::indices(h))
        {
            P1[index(k+1, 0)] = P1[index(k, 0)] * (n_cpu - k) / (k + 1) * rho * (n_bus - k) / n_bus;

            for(auto j : grabin::view::indices(max_index_2(k+1)))
            {
                P1[index(k+1, j+1)] = P1[index(k+1, j)] * (k+1);

                if(k>0)
                {
                    P1[index(k+1, j+1)] += P1[index(k, j+1)] * (n_bus - k);
                }

                P1[index(k+1, j+1)] *= (n_cpu - k - j - 1) * rho / (k+1) / n_bus;
            }
        }

        auto const P1_sum = std::accumulate(P1.begin(), P1.end(), 0 * P1[0]);
        REQUIRE(std::abs(P1_sum) > 1e-10);

        for(auto & p : P1)
        {
            p /= P1_sum;
        }

        // Сравниваем решения
        CAPTURE(n_cpu, n_bus, lambda, rho);
        REQUIRE_THAT(P, grabin_test::Matchers::elementwise_within_abs(P1, 0.1));
    }
}
