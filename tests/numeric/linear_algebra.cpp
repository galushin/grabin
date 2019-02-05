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

    std::generate(A.begin(), A.end(), gen);
    std::generate(x.begin(), x.end(), gen);

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
    std::generate(x.begin(), x.end(), gen);

    auto const b = A*x;

    auto const x0 = grabin::linear_algebra::minimal_residue(A, b);

    auto const eps = 1e-3;
    CHECK_THAT(x0, grabin_test::Matchers::elementwise_within_abs(x, eps));
}
