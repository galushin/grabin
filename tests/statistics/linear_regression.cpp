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

TEST_CASE("linear_regression_accumulator : on sample")
{
    using Value = double;
    using Counter = std::size_t;

    auto checker = [](Value const & slope, Value const & intercept, std::vector<Value> const & xs)
    {
        grabin::statistics::linear_regression_accumulator<Value, Value, Counter> acc;

        static_assert(std::is_same<decltype(acc)::count_type, Counter>::value, "");

        auto const f = [&](Value const & x) { return slope * x + intercept; };
        for(auto const & x : xs)
        {
            acc(x, f(x));
        }

        CHECK(acc.count() == xs.size());
        REQUIRE_THAT(acc.intercept(), Catch::Matchers::WithinAbs(intercept, 1e-6*std::abs(intercept)));
        REQUIRE_THAT(acc.slope(), Catch::Matchers::WithinAbs(slope, 1e-6*std::abs(slope)));
    };

    for(auto generation = 0; generation < 100; ++ generation)
    {
        auto & rnd = grabin_test::random_engine();
        std::uniform_real_distribution<Value> distr(-1e6, +1e6);

        auto const a = distr(rnd);
        auto const b = distr(rnd);

        std::vector<Value> xs;
        std::generate_n(std::back_inserter(xs), generation+2, [&]{ return distr(rnd); });

        checker(a, b, xs);
    }
}

TEST_CASE("linear_regression_accumulator : on sample, with noise")
{
    using Value = double;
    using Counter = std::size_t;

    auto checker = [](Value const & slope, Value const & intercept,
                      std::vector<Value> const & xs, std::vector<Value> const & es)
    {
        CAPTURE(xs.size());

        grabin::statistics::linear_regression_accumulator<Value, Value, Counter> acc;

        static_assert(std::is_same<decltype(acc)::count_type, Counter>::value, "");

        auto const f = [&](Value const & x) { return slope * x + intercept; };

        REQUIRE(xs.size() == es.size());

        for(auto i = 0*xs.size(); i < xs.size(); ++ i)
        {
            auto const & x = xs[i];
            acc(x, f(x) + es[i]);
        }

        CHECK(acc.count() == xs.size());
        // @todo Статистически обоснованные величины уклонений
        CHECK_THAT(acc.intercept(), Catch::Matchers::WithinAbs(intercept, 1e-3*std::abs(intercept)));
        CHECK_THAT(acc.slope(), Catch::Matchers::WithinAbs(slope, 1e-3*std::abs(slope)));

        // @todo Проверить среднее уклонение от эталонных значений
    };

    for(auto generation = 0; generation < 100; ++ generation)
    {
        auto & rnd = grabin_test::random_engine();
        std::uniform_real_distribution<Value> distr(-1e6, +1e6);

        auto const a = distr(rnd);
        auto const b = distr(rnd);

        auto const sample_size = 100;

        std::vector<Value> xs;
        std::generate_n(std::back_inserter(xs), sample_size, [&]{ return distr(rnd); });

        std::vector<Value> es;
        std::normal_distribution<Value> err_distr(0, 1e-2);
        std::generate_n(std::back_inserter(es), sample_size, [&]{ return err_distr(rnd); });

        checker(a, b, xs, es);
    }
}
