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

#include <grabin/optimization/local_search.hpp>

#include <catch2/catch.hpp>
#include "../grabin_test.hpp"

TEST_CASE("local search pseudo boolean - norm")
{
    using Input = std::vector<bool>;

    auto property = [](Input const & x_init)
    {
        auto const objective = [](Input const & x)
        {
            return std::count(x.begin(), x.end(), true);
        };

        auto const result = grabin::optimization::local_search_boolean(x_init, objective);

        REQUIRE(result.objective_value == objective(result.point));

        CHECK(result.objective_value == x_init.size());
        CHECK(result.point == Input(x_init.size(), true));
    };

    grabin_test::check(property);
}

TEST_CASE("local search pseudo boolean - changes")
{
    using Input = std::vector<bool>;

    auto property = [](Input const & x_init)
    {
        auto const objective = [](Input const & x)
        {
            auto result = 0 * x.size();

            for(auto i = 0*x.size(); i+1 < x.size(); ++ i)
            {
                result += (x[i] != x[i+1]);
            }

            return result;
        };

        auto const result = grabin::optimization::local_search_boolean(x_init, objective);

        CAPTURE(x_init);
        CAPTURE(result.point);
        REQUIRE(result.objective_value == objective(result.point));

        if(x_init.size() <= 1)
        {
            REQUIRE(result.objective_value == 0);
        }
        else if(x_init.size() == 2)
        {
            REQUIRE(result.objective_value == x_init.size() - 1);
        }
        else
        {
            auto x = result.point;
            for(auto i : grabin::view::indices_of(x))
            {
                x[i] = !x[i];

                REQUIRE(result.objective_value >= objective(x));

                x[i] = !x[i];
            }
        }
    };

    grabin_test::check(property);
}
