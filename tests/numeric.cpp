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
#include "../istream_sequence.hpp"

#include <forward_list>

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

TEST_CASE("accumulate")
{
    using Value = int;

    auto property = [](std::vector<Value> const & src, Value const & value)
    {
        auto xs = src;
        xs.push_back(value);
        std::sort(xs.begin(), xs.end());
        std::adjacent_difference(xs.begin(), xs.end(), xs.begin());
        auto const init_value = xs.front();

        std::vector<Value> xs_std(xs.begin() + 1, xs.end());

        grabin_test::istream_sequence<Value> xs_grabin(xs_std.begin(), xs_std.end());

        auto const r_std = std::accumulate(xs_std.begin(), xs_std.end(), init_value);
        auto const r_grabin = grabin::accumulate(xs_grabin, init_value);

        CHECK(r_grabin == r_std);
    };

    grabin_test::check(property);
}

TEST_CASE("accumulate with custom operations")
{
    using Value = int;

    auto property = [](std::forward_list<Value> const & xs_std, Value const & init_value)
    {
        grabin_test::istream_sequence<Value> xs_grabin(xs_std.begin(), xs_std.end());

        auto const op = [](Value const & x, Value const & y) { return std::max(x, y); };

        auto const r_std = std::accumulate(xs_std.begin(), xs_std.end(), init_value, op);
        auto const r_grabin = grabin::accumulate(xs_grabin, init_value, op);

        CHECK(r_grabin == r_std);
    };

    grabin_test::check(property);
}

TEST_CASE("reduce")
{
    using Value = int;

    auto property = [](std::vector<Value> const & src)
    {
        auto xs_std = [&]
        {
            auto xs = src;
            std::sort(xs.begin(), xs.end());
            std::adjacent_difference(xs.begin(), xs.end(), xs.begin());
            return xs;
        }();

        grabin_test::istream_sequence<Value> xs_grabin(xs_std.begin(), xs_std.end());

        auto const r_std = std::accumulate(xs_std.begin(), xs_std.end(), Value{0});
        auto const r_grabin = grabin::reduce(xs_grabin);

        CHECK(r_grabin == r_std);
    };

    grabin_test::check(property);
}

TEST_CASE("reduce with init_value")
{
    using Value = int;

    auto property = [](std::vector<Value> const & src, Value const & value)
    {
        auto xs = src;
        xs.push_back(value);
        std::sort(xs.begin(), xs.end());
        std::adjacent_difference(xs.begin(), xs.end(), xs.begin());
        auto const init_value = xs.front();

        std::vector<Value> xs_std(xs.begin() + 1, xs.end());

        grabin_test::istream_sequence<Value> xs_grabin(xs_std.begin(), xs_std.end());

        auto const r_std = std::accumulate(xs_std.begin(), xs_std.end(), init_value);
        auto const r_grabin = grabin::reduce(xs_grabin, init_value);

        CHECK(r_grabin == r_std);
    };

    grabin_test::check(property);
}

TEST_CASE("reduce with custom operations")
{
    using Value = int;

    auto property = [](std::forward_list<Value> const & xs_std, Value const & init_value)
    {
        grabin_test::istream_sequence<Value> xs_grabin(xs_std.begin(), xs_std.end());

        auto const op = [](Value const & x, Value const & y) { return std::max(x, y); };

        auto const r_std = std::accumulate(xs_std.begin(), xs_std.end(), init_value, op);
        auto const r_grabin = grabin::reduce(xs_grabin, init_value, op);

        CHECK(r_grabin == r_std);
    };

    grabin_test::check(property);
}
