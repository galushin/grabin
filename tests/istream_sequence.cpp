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

// @todo Обновить makefile.gcc

#include "istream_sequence.hpp"

#include <catch2/catch.hpp>
#include "../grabin_test.hpp"

TEST_CASE("istream_sequence")
{
    using Value = int;

    auto property = [](std::vector<Value> const & src)
    {
        grabin_test::istream_sequence<Value> seq(src.begin(), src.end());

        using IStreamIterator = std::istream_iterator<Value>;

        static_assert(std::is_same<decltype(seq.begin()), IStreamIterator>::value, "");
        static_assert(std::is_same<decltype(seq.end()), IStreamIterator>::value, "");

        std::vector<Value> const dest(seq.begin(), seq.end());

        CHECK(dest == src);
    };

    grabin_test::check(property);
}

TEST_CASE("istream_sequence: initializer list ctor")
{
    using Value = int;

    using IStreamSequence = grabin_test::istream_sequence<Value>;

    std::vector<Value> const expected{1, 2, 3, 5, 8, 13};
    IStreamSequence          seq     {1, 2, 3, 5, 8, 13};

    std::vector<Value> const actual(seq.begin(), seq.end());

    CHECK(actual == expected);
}
