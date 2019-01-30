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

#include <grabin/utility/as_const.hpp>

#include <catch2/catch.hpp>

TEST_CASE("as_const")
{
    using Type = int;
    constexpr Type mutable_var = 42;

    using ConstView = decltype(grabin::as_const(mutable_var));
    static_assert(std::is_same<ConstView, Type const &>::value, "");
    static_assert(noexcept(grabin::as_const(mutable_var)), "must be noexcept");

    auto const & const_view = grabin::as_const(mutable_var);
    CHECK(std::addressof(mutable_var) == std::addressof(const_view));
}

TEST_CASE("as_const for const")
{
    using Type = int;
    Type const const_var = 42;

    using ConstView = decltype(grabin::as_const(const_var));
    static_assert(std::is_same<ConstView, Type const &>::value, "");
    static_assert(noexcept(grabin::as_const(const_var)), "must be noexcept");

    auto const & const_view = grabin::as_const(const_var);
    CHECK(std::addressof(const_var) == std::addressof(const_view));
}
