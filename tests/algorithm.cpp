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

TEST_CASE("equal with predicate")
{
    using Value = int;
    auto property = [](std::vector<Value> const & xs_old,
                       std::vector<Value> const & ys_old)
    {
        CAPTURE(xs_old, ys_old);

        grabin_test::istream_sequence<Value, struct Tag1> xs(xs_old.begin(), xs_old.end());
        grabin_test::istream_sequence<Value, struct Tag2> ys(ys_old.begin(), ys_old.end());

        auto const pred
            = [](Value const & x, Value const & y) { return x % 2 == y % 2; };

        auto const r_grabin = grabin::equal(xs, ys, pred);
        auto const r_std = std::equal(xs_old.begin(), xs_old.end(),
                                      ys_old.begin(), ys_old.end(), pred);
        CHECK(r_grabin == r_std);
    };

    grabin_test::check(property);
}

TEST_CASE("is_heap")
{
    auto property = [](std::vector<int> const & xs)
    {
        CAPTURE(xs);

        REQUIRE(grabin::is_heap(xs) == std::is_heap(xs.begin(), xs.end()));
    };

    grabin_test::check(property);
}

TEST_CASE("is_heap : true")
{
    auto property = [](std::vector<int> const & xs_old)
    {
        auto xs = [&]
        {
            auto xs = xs_old;
            std::make_heap(xs.begin(), xs.end());
            return xs;
        }();

        CAPTURE(xs);

        REQUIRE(grabin::is_heap(xs));
    };

    grabin_test::check(property);
}

TEST_CASE("is_heap with predicate")
{
    auto property = [](std::vector<int> const & xs)
    {
        CAPTURE(xs);

        auto const pred = std::greater<>{};

        REQUIRE(grabin::is_heap(xs, pred)
                == std::is_heap(xs.begin(), xs.end(), pred));
    };

    grabin_test::check(property);
}

TEST_CASE("is_heap with predicate : true")
{
    auto property = [](std::vector<int> const & xs_old)
    {
        auto const pred = std::greater<>{};

        auto xs = [&]
        {
            auto xs = xs_old;
            std::make_heap(xs.begin(), xs.end(), pred);
            return xs;
        }();

        CAPTURE(xs);

        REQUIRE(grabin::is_heap(xs, pred));
    };

    grabin_test::check(property);
}

TEST_CASE("make_heap")
{
    auto property = [](std::vector<int> const & xs_old)
    {
        auto xs = xs_old;

        CAPTURE(xs);

        grabin::make_heap(xs);

        REQUIRE(grabin::is_permutation(xs, xs_old));
        REQUIRE(grabin::is_heap(xs));
    };

    grabin_test::check(property);
}

TEST_CASE("make_heap with predicate")
{
    auto property = [](std::vector<int> const & xs_old)
    {
        auto xs = xs_old;

        CAPTURE(xs);

        auto const pred = std::greater<>{};

        grabin::make_heap(xs, pred);

        REQUIRE(grabin::is_permutation(xs, xs_old));
        REQUIRE(grabin::is_heap(xs, pred));
    };

    grabin_test::check(property);
}

TEST_CASE("push_heap")
{
    auto property = [](std::vector<int> xs, int const & value)
    {
        grabin::make_heap(xs);
        xs.push_back(value);
        CAPTURE(xs);

        auto const xs_old = xs;

        grabin::push_heap(xs);

        REQUIRE(grabin::is_permutation(xs, xs_old));
        REQUIRE(grabin::is_heap(xs));
    };

    grabin_test::check(property);
}


TEST_CASE("push_heap with predicate")
{
    auto property = [](std::vector<int> xs, int const & value)
    {
        auto const pred = std::greater<>{};

        grabin::make_heap(xs, pred);
        xs.push_back(value);
        CAPTURE(xs);

        auto const xs_old = xs;

        grabin::push_heap(xs, pred);

        REQUIRE(grabin::is_permutation(xs, xs_old));
        REQUIRE(grabin::is_heap(xs, pred));
    };

    grabin_test::check(property);
}

TEST_CASE("pop_heap")
{
    auto property = [](std::vector<int> xs, int const & value)
    {
        xs.push_back(value);
        grabin::make_heap(xs);
        CAPTURE(xs);

        auto const xs_old = xs;

        grabin::pop_heap(xs);

        REQUIRE(xs.back() == xs_old.front());

        xs.pop_back();
        REQUIRE(grabin::is_heap(xs));
    };

    grabin_test::check(property);
}


TEST_CASE("pop_heap with predicate")
{
    auto property = [](std::vector<int> xs, int const & value)
    {
        auto const pred = std::greater<>{};

        xs.push_back(value);
        grabin::make_heap(xs, pred);
        CAPTURE(xs);

        auto const xs_old = xs;

        grabin::pop_heap(xs, pred);

        REQUIRE(xs.back() == xs_old.front());

        xs.pop_back();
        REQUIRE(grabin::is_heap(xs, pred));
    };

    grabin_test::check(property);
}

TEST_CASE("sort_heap")
{
    auto property = [](std::vector<int> xs)
    {
        grabin::make_heap(xs);
        CAPTURE(xs);

        auto const xs_old = xs;

        grabin::sort_heap(xs);

        REQUIRE(grabin::is_permutation(xs, xs_old));
        REQUIRE(std::is_sorted(xs.begin(), xs.end()));
    };

    grabin_test::check(property);
}


TEST_CASE("sort_heap with predicate")
{
    auto property = [](std::vector<int> xs)
    {
        auto const pred = std::greater<>{};

        grabin::make_heap(xs, pred);
        CAPTURE(xs);

        auto const xs_old = xs;

        grabin::sort_heap(xs, pred);

        REQUIRE(grabin::is_permutation(xs, xs_old));
        REQUIRE(std::is_sorted(xs.begin(), xs.end(), pred));
    };

    grabin_test::check(property);
}

TEST_CASE("clamp")
{
    using Value = long;

    auto property = [](Value const & x, Value const & y1, Value const & y2)
    {
        auto const bounds = std::minmax(y1, y2);

        auto const x_clamped = grabin::clamp(x, bounds.first, bounds.second);

        REQUIRE(bounds.first <= x_clamped);
        REQUIRE(x_clamped <= bounds.second);

        if(x < bounds.first)
        {
            REQUIRE(x_clamped == bounds.first);
        }
        else if(x >= bounds.second)
        {
            REQUIRE(x_clamped == bounds.second);
        }
        else
        {
            REQUIRE(x_clamped == x);
        }
    };

    grabin_test::check(property);
}

TEST_CASE("clamp with predicate")
{
    using Value = std::pair<int, long>;

    auto property = [](Value const & x, Value const & y1, Value const & y2)
    {
        auto const cmp = [](Value const & x, Value const & y)
        {
            return x.first < y.first;
        };

        auto const bounds = std::minmax(y1, y2);

        auto const x_clamped = grabin::clamp(x, bounds.first, bounds.second, cmp);

        REQUIRE(!cmp(x_clamped, bounds.first));
        REQUIRE(!cmp(bounds.second, x_clamped));

        if(cmp(x, bounds.first))
        {
            REQUIRE(x_clamped == bounds.first);
        }
        else if(cmp(bounds.second, x))
        {
            REQUIRE(x_clamped == bounds.second);
        }
        else
        {
            REQUIRE(x_clamped == x);
        }
    };

    grabin_test::check(property);
}

TEST_CASE("lexicographical_compare")
{
    using Value = int;
    auto property = [](std::vector<Value> const & xs_old,
                       std::vector<Value> const & ys_old)
    {
        CAPTURE(xs_old, ys_old);

        grabin_test::istream_sequence<Value, struct Tag1> xs(xs_old.begin(), xs_old.end());
        grabin_test::istream_sequence<Value, struct Tag2> ys(ys_old.begin(), ys_old.end());

        auto const r_grabin = grabin::lexicographical_compare(xs, ys);
        auto const r_std
            = std::lexicographical_compare(xs_old.begin(), xs_old.end(),
                                           ys_old.begin(), ys_old.end());
        CHECK(r_grabin == r_std);
    };

    grabin_test::check(property);
}

TEST_CASE("lexicographical_compare with predicate")
{
    using Value = int;
    auto property = [](std::vector<Value> const & xs_old,
                       std::vector<Value> const & ys_old)
    {
        CAPTURE(xs_old, ys_old);

        grabin_test::istream_sequence<Value, struct Tag1> xs(xs_old.begin(), xs_old.end());
        grabin_test::istream_sequence<Value, struct Tag2> ys(ys_old.begin(), ys_old.end());

        auto const pred = std::greater<>{};

        auto const r_grabin = grabin::lexicographical_compare(xs, ys, pred);
        auto const r_std
            = std::lexicographical_compare(xs_old.begin(), xs_old.end(),
                                           ys_old.begin(), ys_old.end(), pred);
        CHECK(r_grabin == r_std);
    };

    grabin_test::check(property);
}

TEST_CASE("is_permutation")
{
    auto property = [](std::forward_list<int> const & xs, std::list<int> const & ys)
    {
        CAPTURE(xs, ys);

        REQUIRE(grabin::is_permutation(xs, ys)
                == std::is_permutation(xs.begin(), xs.end(), ys.begin(), ys.end()));
        REQUIRE(grabin::is_permutation(ys, xs)
                == std::is_permutation(xs.begin(), xs.end(), ys.begin(), ys.end()));
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
