/* (c) 2018 Галушин Павел Викторович, galushin@gmail.com

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

#include <grabin/statistics/linear_regression.hpp>

#include "../grabin_test.hpp"
#include <catch2/catch.hpp>

TEST_CASE("linear_regression_accumulator : up to two points")
{
    using Value = double;

    auto checker = [](Value const & slope, Value const & intercept,
                      Value const & x_1, Value const & x_2)
    {
        auto const f = [=](Value const & x) { return slope * x + intercept; };
        auto const y_1 = f(x_1);
        auto const y_2 = f(x_2);

        grabin::statistics::linear_regression_accumulator<Value, Value> acc;

        static_assert(std::is_same<decltype(acc)::count_type, std::ptrdiff_t>::value, "");
        static_assert(std::is_same<decltype(acc)::intercept_type, Value>::value, "");
        static_assert(std::is_same<decltype(acc)::slope_type, Value>::value, "");

        // Проверка начальных значений
        CHECK(acc.count() == 0);
        REQUIRE_THAT(acc.intercept(), Catch::Matchers::WithinAbs(0.0, 1e-10));
        REQUIRE_THAT(acc.slope(), Catch::Matchers::WithinAbs(0.0, 1e-10));

        // Проверка на одном значении
        static_assert(std::is_same<decltype(acc(x_1, y_1)), decltype(acc)&>::value, "");

        acc(x_1, y_1);

        CHECK(acc.count() == 1);
        REQUIRE_THAT(acc.intercept(), Catch::Matchers::WithinAbs(y_1, 1e-10));
        REQUIRE_THAT(acc.slope(), Catch::Matchers::WithinAbs(0.0, 1e-10));

        // Проверка на двух значениях
        acc(x_2, y_2);

        CHECK(acc.count() == 2);
        REQUIRE_THAT(acc.intercept(), Catch::Matchers::WithinAbs(intercept, 1e-6*std::abs(intercept)));
        REQUIRE_THAT(acc.slope(), Catch::Matchers::WithinAbs(slope, 1e-6*std::abs(slope)));
    };

    for(auto generation = 0; generation < 100; ++ generation)
    {
        auto & rnd = grabin_test::random_engine();
        std::uniform_real_distribution<Value> distr(-1e6, +1e6);
        auto const a = distr(rnd);
        auto const b = distr(rnd);
        auto const x1 = distr(rnd);
        auto const x2 = distr(rnd);

        checker(a, b, x1, x2);
    }
}