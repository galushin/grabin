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

#include <grabin/view/indices.hpp>

#include <catch2/catch.hpp>
#include "../grabin_test.hpp"

TEST_CASE("range-for with indices")
{
    using Container = std::vector<int>;
    auto property = [](Container const & xs)
    {
        using Index = Container::difference_type;

        std::vector<Index> ins_obj(xs.size());
        std::iota(ins_obj.begin(), ins_obj.end(), Index(0));

        std::vector<Index> ins;
        for(auto const & i : grabin::view::indices_of(xs))
        {
            ins.push_back(i);
        }

        CHECK(ins == ins_obj);
    };

    grabin_test::check(property);
}

TEST_CASE("counter_range")
{
    using Container = std::vector<int>;
    auto property = [](Container const & xs)
    {
        using Index = Container::difference_type;

        std::vector<Index> ins_obj(xs.size());
        std::iota(ins_obj.begin(), ins_obj.end(), Index(0));

        std::vector<Index> ins;
        for(auto const & i : grabin::view::indices_of(xs))
        {
            ins.push_back(i);
        }

        CHECK(ins == ins_obj);
    };

    grabin_test::check(property);
}
