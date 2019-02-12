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

#include <grabin/numeric.hpp>

#include <catch2/catch.hpp>
#include "../grabin_test.hpp"

TEST_CASE("iota")
{
    auto property = [](std::forward_list<int> const & xs_old, int const & init_value)
    {
        auto xs_std = xs_old;
        auto xs_grabin = xs_old;

        std::iota(xs_std.begin(), xs_std.end(), init_value);
        grabin::iota(xs_grabin, init_value);

        CHECK(xs_grabin == xs_std);
    };

    grabin_test::check(property);
}
