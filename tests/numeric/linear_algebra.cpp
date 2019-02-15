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

TEST_CASE("LU-solver: simplest queueing system")
{
    using Matrix = grabin::matrix<double>;
    using Vector = grabin::math_vector<double>;

    auto & rnd = grabin_test::random_engine();
    std::uniform_real_distribution<double> distr(0.1, 10);

    for(auto n = 1; n < 15; ++ n)
    {
        auto const mu = distr(rnd);
        auto const nu = distr(rnd);

        // Составляем систему уравнений
        Matrix Lambda(n+1, n+1);

        Lambda(0, 0) = - mu;
        Lambda(0, 1) = nu;

        for(auto i : grabin::view::indices<Matrix::size_type>(1, n))
        {
            Lambda(i,i) = - (mu + nu);
            Lambda(i,i-1) = mu;
            Lambda(i,i+1) = nu;
        }

        for(auto i : grabin::view::indices(n+1))
        {
            Lambda(n, i) = 1;
        }

        Vector b(n+1, 0);
        b[n] = 1;

        // Решаем систему
        auto const P = grabin::linear_algebra::LU_solver{}(Lambda, b);

        // Находим вероятности по формулам
        auto const alpha = mu / nu;

        Vector P1(n+1);

        if(std::abs(alpha - 1) < 1e-3)
        {
            P1[0] = 1.0 / (n+1);
        }
        else
        {
            P1[0] = (1 - alpha) / (1 - std::pow(alpha, n+1));
        }

        for(auto const & i : grabin::view::indices(n))
        {
            P1[i+1] = alpha * P1[i];
        }

        // Сравниваем решения
        CAPTURE(Lambda, b, alpha);
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
        auto const mu = distr(rnd);
        auto const nu = distr(rnd);

        // Составляем систему уравнений
        Matrix Lambda(n+1, n+1);

        Lambda(0, 0) = -n*mu;
        Lambda(0, 1) = nu;

        for(auto i : grabin::view::indices<Matrix::size_type>(1, n))
        {
            Lambda(i,i-1) = mu * (n - i + 1);
            Lambda(i,i)   = - (nu + mu * (n - i));
            Lambda(i,i+1) = nu;
        }

        for(auto i : grabin::view::indices(n+1))
        {
            Lambda(n, i) = 1;
        }

        Vector b(n+1, 0);
        b[n] = 1;

        // Решаем систему
        auto const P = grabin::linear_algebra::LU_solver{}(Lambda, b);

        // Находим вероятности по формулам
        auto const alpha = mu / nu;

        Vector P1(n+1);

        P1[0] = 1;

        for(auto const & i : grabin::view::indices(n))
        {
            P1[i+1] = alpha * (n - i) * P1[i];
        }

        auto const P1_sum = std::accumulate(P1.begin(), P1.end(), 0 * P1[0]);
        for(auto & p : P1)
        {
            p /= P1_sum;
        }

        // Сравниваем решения
        CAPTURE(Lambda, b, alpha);
        REQUIRE_THAT(P, grabin_test::Matchers::elementwise_within_abs(P1, 1e-6));
    }
}
