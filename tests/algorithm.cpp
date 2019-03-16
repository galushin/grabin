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

#include <grabin/algorithm.hpp>

#include <catch2/catch.hpp>
#include "../grabin_test.hpp"
#include "../istream_sequence.hpp"

#include <forward_list>

TEST_CASE("algorithm: logical quantors")
{
    using IntType = int;
    auto property = [](std::vector<IntType> const & src)
    {
        grabin_test::istream_sequence<IntType> xs_std(src.begin(), src.end());
        grabin_test::istream_sequence<IntType> xs_grabin(src.begin(), src.end());

        auto pred = [](IntType const & x) { return x % 2 == 0; };

        CHECK(std::all_of(xs_std.begin(), xs_std.end(), pred) ==
              grabin::all_of(xs_grabin, pred));
        CHECK(std::any_of(xs_std.begin(), xs_std.end(), pred) ==
              grabin::any_of(xs_grabin, pred));
        CHECK(std::none_of(xs_std.begin(), xs_std.end(), pred) ==
              grabin::none_of(xs_grabin, pred));
    };

    grabin_test::check(property);
}

TEST_CASE("algorithm: count_if")
{
    using IntType = int;
    auto property = [](std::vector<IntType> const & src)
    {
        grabin_test::istream_sequence<IntType> xs_std(src.begin(), src.end());
        grabin_test::istream_sequence<IntType> xs_grabin(src.begin(), src.end());

        auto pred = [](IntType const & x) { return x % 2 == 0; };

        CHECK(std::count_if(xs_std.begin(), xs_std.end(), pred) ==
              grabin::count_if(xs_grabin, pred));
    };

    grabin_test::check(property);
}

TEST_CASE("algorithm: count")
{
    using IntType = int;
    auto property = [](std::vector<IntType> const & src, IntType const & value)
    {
        grabin_test::istream_sequence<IntType> xs_std(src.begin(), src.end());
        grabin_test::istream_sequence<IntType> xs_grabin(src.begin(), src.end());

        CHECK(std::count(xs_std.begin(), xs_std.end(), value) ==
              grabin::count(xs_grabin, value));
    };

    grabin_test::check(property);
}

TEST_CASE("algorithm: count, front element")
{
    using IntType = int;
    auto property = [](std::vector<IntType> const & src)
    {
        if(src.empty())
        {
            return;
        }

        auto const & value = src.front();
        grabin_test::istream_sequence<IntType> xs_std(src.begin(), src.end());
        grabin_test::istream_sequence<IntType> xs_grabin(src.begin(), src.end());

        CHECK(std::count(xs_std.begin(), xs_std.end(), value) ==
              grabin::count(xs_grabin, value));
    };

    grabin_test::check(property);
}

TEST_CASE("fill")
{
    using Value = int;
    auto property = [](std::forward_list<Value> const & xs_old, Value const & value)
    {
        auto const xs_std = [&]
        {
            auto xs = xs_old;
            std::fill(xs.begin(), xs.end(), value);
            return xs;
        }();

        auto const xs_grabin = [&]
        {
            auto xs = xs_old;
            grabin::fill(xs, value);
            return xs;
        }();

        CHECK(xs_grabin == xs_std);
    };

    grabin_test::check(property);
}

TEST_CASE("generate")
{
    auto property = [](std::forward_list<int> const & xs_old)
    {
        auto const xs_std = [&]
        {
            auto xs = xs_old;
            int value = -5;
            auto gen = [&] { return value++;};
            std::generate(xs.begin(), xs.end(), gen);
            return xs;
        }();

        auto const xs_grabin = [&]
        {
            auto xs = xs_old;
            int value = -5;
            auto gen = [&] { return value++;};
            grabin::generate(xs, gen);
            return xs;
        }();

        CHECK(xs_grabin == xs_std);
    };

    grabin_test::check(property);
}

TEST_CASE("equal")
{
    auto property = [](std::forward_list<int> const & xs, std::list<int> const & ys)
    {
        CAPTURE(xs, ys);

        CHECK(grabin::equal(xs, ys) == std::equal(xs.begin(), xs.end(), ys.begin(), ys.end()));
    };

    grabin_test::check(property);
}

TEST_CASE("is_permutation")
{
    auto property = [](std::forward_list<int> const & xs, std::list<int> const & ys)
    {
        CAPTURE(xs, ys);

        REQUIRE(grabin::is_permutation(xs, ys) == std::is_permutation(xs.begin(), xs.end(), ys.begin(), ys.end()));
        REQUIRE(grabin::is_permutation(ys, xs) == std::is_permutation(xs.begin(), xs.end(), ys.begin(), ys.end()));
    };

    grabin_test::check(property);
}

TEST_CASE("is_permutation: with next permutation")
{
    auto property = [](std::forward_list<int> const & xs)
    {
        std::list<int> ys(xs.begin(), xs.end());
        std::next_permutation(ys.begin(), ys.end());

        CAPTURE(xs, ys);

        REQUIRE(grabin::is_permutation(xs, ys));
        REQUIRE(grabin::is_permutation(ys, xs));
    };

    grabin_test::check(property);
}

TEST_CASE("is_permutation with predicate")
{
    auto property = [](std::forward_list<int> const & xs, std::list<int> const & ys)
    {
        CAPTURE(xs, ys);

        auto const pred = std::greater<>{};

        REQUIRE(grabin::is_permutation(xs, ys, pred)
                == std::is_permutation(xs.begin(), xs.end(), ys.begin(), ys.end(), pred));
    };

    grabin_test::check(property);
}

TEST_CASE("next_permutation")
{
    auto property = [](std::list<int> const & xs_old)
    {
        auto xs_std = xs_old;
        auto const r_std = std::next_permutation(xs_std.begin(), xs_std.end());

        auto xs_grabin = xs_old;
        auto const r_grabin = grabin::next_permutation(xs_grabin);

        REQUIRE(xs_std == xs_grabin);
        REQUIRE(r_std == r_grabin);
    };

    grabin_test::check(property);
}

TEST_CASE("next_permutation with predicate")
{
    auto property = [](std::list<int> const & xs_old)
    {
        auto const pred = std::greater<>{};

        auto xs_std = xs_old;
        auto const r_std = std::next_permutation(xs_std.begin(), xs_std.end(), pred);

        auto xs_grabin = xs_old;
        auto const r_grabin = grabin::next_permutation(xs_grabin, pred);

        REQUIRE(xs_std == xs_grabin);
        REQUIRE(r_std == r_grabin);
    };

    grabin_test::check(property);
}

TEST_CASE("prev_permutation")
{
    auto property = [](std::list<int> const & xs_old)
    {
        auto xs_std = xs_old;
        auto const r_std = std::prev_permutation(xs_std.begin(), xs_std.end());

        auto xs_grabin = xs_old;
        auto const r_grabin = grabin::prev_permutation(xs_grabin);

        REQUIRE(xs_std == xs_grabin);
        REQUIRE(r_std == r_grabin);
    };

    grabin_test::check(property);
}

TEST_CASE("prev_permutation with predicate")
{
    auto property = [](std::list<int> const & xs_old)
    {
        auto const pred = std::greater<>{};

        auto xs_std = xs_old;
        auto const r_std = std::prev_permutation(xs_std.begin(), xs_std.end(), pred);

        auto xs_grabin = xs_old;
        auto const r_grabin = grabin::prev_permutation(xs_grabin, pred);

        REQUIRE(xs_std == xs_grabin);
        REQUIRE(r_std == r_grabin);
    };

    grabin_test::check(property);
}
