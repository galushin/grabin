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

#include <grabin/statistics/mean.hpp>

#include "../grabin_test.hpp"
#include <catch2/catch.hpp>

#include <type_traits>

TEST_CASE("mean_accumulator : two values")
{
    using Value = double;

    auto checker = [](Value const & value_1, Value const & value_2)
    {
        grabin::statistics::mean_accumulator<Value> acc;

        static_assert(std::is_same<decltype(acc)::count_type, std::ptrdiff_t>::value, "");
        static_assert(std::is_same<decltype(acc)::value_type, Value>::value, "");
        static_assert(std::is_same<decltype(acc)::mean_type, Value>::value, "");

        CHECK(acc.count() == 0);
        CHECK_THAT(acc.mean(), Catch::Matchers::WithinAbs(0.0, 1e-10));

        acc(value_1);

        static_assert(std::is_same<decltype(acc(value_1)), decltype(acc) &>::value, "");

        CHECK(acc.count() == 1);
        CHECK_THAT(acc.mean(), Catch::Matchers::WithinAbs(value_1, 1e-10));

        acc(value_2);

        CHECK(acc.count() == 2);
        auto const avg2 = value_1 + (value_2 - value_1) / 2;
        CHECK_THAT(acc.mean(), Catch::Matchers::WithinAbs(avg2, 1e-10));
    };

    grabin_test::check(checker);
}

TEST_CASE("mean_accumulator : floating-point arithmetic progression")
{
    using Value = double;
    using Counter = long;

    static_assert(!std::is_same<Counter, std::size_t>::value, "");

    auto checker = [](Value const & a, Value const & b, Counter const & n)
    {
        CAPTURE(a, b, n);

        auto const mean_expected = a + (b-a)/2;
        grabin::statistics::mean_accumulator<Value, Counter> acc;

        static_assert(std::is_same<decltype(acc)::count_type, Counter>::value,"");

        auto const dx = (b - a)/n;
        for(auto i = 0*n; i <= n; ++ i)
        {
            auto const x = a + dx*i;
            acc(x);
        }

        CHECK(acc.count() == n+1);
        CHECK(abs(acc.mean() - mean_expected) <= 1e-10 * std::abs(mean_expected));
    };

    for(auto generation = 0; generation < 100; ++ generation)
    {
        auto & rnd = grabin_test::random_engine();
        auto const value_1 = grabin_test::Arbitrary<Value>::generate(rnd, generation);
        auto const value_2 = grabin_test::Arbitrary<Value>::generate(rnd, generation);
        checker(value_1, value_2, generation+1);
    }
}

TEST_CASE("mean_accumulator : two integer values")
{
    using Value = int;

    auto checker = [](Value const & value_1, Value const & value_2)
    {
        grabin::statistics::mean_accumulator<Value> acc;

        static_assert(std::is_same<decltype(acc)::count_type, std::ptrdiff_t>::value, "");
        static_assert(std::is_same<decltype(acc)::value_type, Value>::value, "");
        static_assert(std::is_same<decltype(acc)::mean_type, double>::value, "");

        CHECK(acc.count() == 0);
        CHECK_THAT(acc.mean(), Catch::Matchers::WithinAbs(0.0, 1e-10));

        acc(value_1);

        static_assert(std::is_same<decltype(acc(value_1)), decltype(acc) &>::value, "");

        CHECK(acc.count() == 1);
        CHECK_THAT(acc.mean(), Catch::Matchers::WithinAbs(value_1, 1e-10));

        acc(value_2);

        CHECK(acc.count() == 2);
        auto const avg2 = value_1 + (double(value_2) - value_1) / 2;
        CHECK_THAT(acc.mean(), Catch::Matchers::WithinAbs(avg2, 1e-10));
    };

    grabin_test::check(checker);
}

TEST_CASE("mean_accumulator : integer arithmetic progression")
{
    using Value = size_t;

    auto checker = [](Value const & n)
    {
        auto const mean_expected = n/2.0;
        grabin::statistics::mean_accumulator<Value> acc;

        for(auto x = Value{0}; x <= n; ++ x)
        {
            acc(x);
        }

        CHECK(acc.count() == n+1);
        CHECK_THAT(acc.mean(), Catch::Matchers::WithinAbs(mean_expected, 1e-10));
    };

    for(auto generation = 0; generation < 100; ++ generation)
    {
        checker(generation);
    }
}
