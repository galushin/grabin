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

#include <grabin/statistics/variance.hpp>

#include "../grabin_test.hpp"
#include <catch2/catch.hpp>

TEST_CASE("variance_accumulator : two values")
{
    using Value = double;

    auto checker = [](Value const & value_1, Value const & value_2)
    {
        CAPTURE(value_1, value_2);

        grabin::statistics::variance_accumulator<Value> acc;

        // @todo Добавить проверку типа value_type
        // @todo Добавить проверку типа среднеквадратического уклонения
        static_assert(std::is_same<decltype(acc)::count_type, std::size_t>::value, "");
        static_assert(std::is_same<decltype(acc)::mean_type, Value>::value, "");
        static_assert(std::is_same<decltype(acc)::variance_type, Value>::value, "");

        CHECK(acc.count() == 0);
        REQUIRE_THAT(acc.mean(), Catch::Matchers::WithinAbs(0.0, 1e-10));
        REQUIRE_THAT(acc.variance(), Catch::Matchers::WithinAbs(0.0, 1e-10));
        REQUIRE_THAT(acc.standard_deviation(), Catch::Matchers::WithinAbs(0.0, 1e-10));

        acc(value_1);

        static_assert(std::is_same<decltype(acc(value_1)), decltype(acc) &>::value, "");

        CHECK(acc.count() == 1);
        REQUIRE_THAT(acc.mean(), Catch::Matchers::WithinAbs(value_1, 1e-10));
        REQUIRE_THAT(acc.variance(), Catch::Matchers::WithinAbs(0.0, 1e-10));
        REQUIRE_THAT(acc.standard_deviation(), Catch::Matchers::WithinAbs(0.0, 1e-10));

        acc(value_2);

        CHECK(acc.count() == 2);
        auto const avg2 = value_1 + (value_2 - value_1) / 2.0;
        auto const var2 = grabin::square(value_2 - value_1)/4.0;
        REQUIRE_THAT(acc.mean(), Catch::Matchers::WithinAbs(avg2, 1e-10));
        REQUIRE_THAT(acc.variance(), Catch::Matchers::WithinAbs(var2, 1e-10));
        REQUIRE_THAT(acc.standard_deviation(), Catch::Matchers::WithinAbs(std::sqrt(var2), 1e-10));
    };

    for(auto generation = 0; generation < 100; ++ generation)
    {
        auto & rnd = grabin_test::random_engine();

        auto const value_1 = grabin_test::Arbitrary<Value>::generate(rnd, generation);
        auto const value_2 = grabin_test::Arbitrary<Value>::generate(rnd, generation);

        checker(value_1, value_2);
    }
}
