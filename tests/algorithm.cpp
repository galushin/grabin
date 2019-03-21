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

TEST_CASE("reverse")
{
    auto property = [](std::list<int> const & src)
    {
        auto xs_std = src;
        std::reverse(xs_std.begin(), xs_std.end());

        auto xs_grabin = src;
        grabin::reverse(xs_grabin);

        REQUIRE(xs_std == xs_grabin);

        grabin::reverse(xs_grabin);

        REQUIRE(xs_grabin == src);
    };

    grabin_test::check(property);
}

TEST_CASE("shuffle")
{
    auto property = [](std::vector<int> const & xs_old, long seed)
    {
        auto xs = xs_old;

        std::minstd_rand rnd(seed);

        grabin::shuffle(xs, rnd);

        REQUIRE(grabin::is_permutation(xs, xs_old));
    };

    grabin_test::check(property);
}

TEST_CASE("is_partitioned")
{
    auto property = [](std::vector<int> const & src)
    {
        auto pred = [](int const & x) { return x % 2 == 0; };

        grabin_test::istream_sequence<int> xs(src.begin(), src.end());
        CAPTURE(xs);

        REQUIRE(grabin::is_partitioned(xs, pred)
                == std::is_partitioned(src.begin(), src.end(), pred));
    };

    grabin_test::check(property);
}

TEST_CASE("is_partitioned : true")
{
    auto property = [](std::vector<int> const & src)
    {
        auto pred = [](int const & x) { return x % 2 == 0; };

        auto const xs = [&]
        {
            auto ys = src;
            std::partition(ys.begin(), ys.end(), pred);
            return ys;
        }();

        CAPTURE(xs);

        REQUIRE(grabin::is_partitioned(xs, pred));
    };

    grabin_test::check(property);
}

TEST_CASE("is_sorted")
{
    auto property = [](std::vector<int> const & xs)
    {
        CAPTURE(xs);

        REQUIRE(grabin::is_sorted(xs) == std::is_sorted(xs.begin(), xs.end()));
    };

    grabin_test::check(property);
}

TEST_CASE("is_sorted : true")
{
    auto property = [](std::vector<int> const & xs_old)
    {
        auto xs = [&]
        {
            auto ys = xs_old;
            std::sort(ys.begin(), ys.end());
            return ys;
        }();

        CAPTURE(xs);

        REQUIRE(grabin::is_sorted(xs));
    };

    grabin_test::check(property);
}

TEST_CASE("is_sorted with compare")
{
    auto property = [](std::vector<int> const & xs)
    {
        CAPTURE(xs);

        auto const pred = std::greater<>{};

        REQUIRE(grabin::is_sorted(xs, pred)
                == std::is_sorted(xs.begin(), xs.end(), pred));
    };

    grabin_test::check(property);
}

TEST_CASE("is_sorted with compare : true")
{
    auto property = [](std::vector<int> const & xs_old)
    {
        auto const pred = std::greater<>{};

        auto xs = [&]
        {
            auto ys = xs_old;
            std::sort(ys.begin(), ys.end(), pred);
            return ys;
        }();

        CAPTURE(xs);

        REQUIRE(grabin::is_sorted(xs, pred));
    };

    grabin_test::check(property);
}

TEST_CASE("sort")
{
    auto property = [](std::vector<int> const & xs_old)
    {
        auto xs = xs_old;

        CAPTURE(xs);

        grabin::sort(xs);

        REQUIRE(grabin::is_permutation(xs, xs_old));
        REQUIRE(grabin::is_sorted(xs));
    };

    grabin_test::check(property);
}

TEST_CASE("sort with compare")
{
    auto property = [](std::vector<int> const & xs_old)
    {
        auto xs = xs_old;

        CAPTURE(xs);

        auto const pred = std::greater<>{};

        grabin::sort(xs, pred);

        REQUIRE(grabin::is_permutation(xs, xs_old));
        REQUIRE(grabin::is_sorted(xs, pred));
    };

    grabin_test::check(property);
}

TEST_CASE("binary_search")
{
    using Value = int;

    auto property = [](std::vector<Value> xs, Value const & value)
    {
        grabin::sort(xs);

        CAPTURE(xs, value);

        REQUIRE(grabin::binary_search(xs, value)
                == std::binary_search(xs.begin(), xs.end(), value));
    };

    grabin_test::check(property);
}

TEST_CASE("binary_search with compare")
{
    using Value = int;

    auto property = [](std::vector<Value> xs, Value const & value)
    {
        auto const cmp = std::greater<>{};

        grabin::sort(xs, cmp);

        CAPTURE(xs, value);

        REQUIRE(grabin::binary_search(xs, value, cmp)
                == std::binary_search(xs.begin(), xs.end(), value, cmp));
    };

    grabin_test::check(property);
}

TEST_CASE("includes")
{
    using Value = int;

    auto property = [](std::vector<Value> xs_src, std::vector<Value> ys_src)
    {
        grabin::sort(xs_src);
        grabin::sort(ys_src);

        CAPTURE(xs_src, ys_src);

        grabin_test::istream_sequence<Value, struct Tag1> xs(xs_src.begin(), xs_src.end());
        grabin_test::istream_sequence<Value, struct Tag2> ys(ys_src.begin(), ys_src.end());

        REQUIRE(grabin::includes(xs_src, xs_src));
        REQUIRE(grabin::includes(ys_src, ys_src));
        REQUIRE(grabin::includes(xs, ys)
                == std::includes(xs_src.begin(), xs_src.end(), ys_src.begin(), ys_src.end()));
        REQUIRE(grabin::includes(ys_src, xs_src)
                == std::includes(ys_src.begin(), ys_src.end(), xs_src.begin(), xs_src.end()));
    };

    grabin_test::check(property);
}

TEST_CASE("includes wtih compare")
{
    using Value = int;

    auto property = [](std::vector<Value> xs_src, std::vector<Value> ys_src)
    {
        auto cmp = std::greater<>{};

        grabin::sort(xs_src, cmp);
        grabin::sort(ys_src, cmp);

        CAPTURE(xs_src, ys_src);

        grabin_test::istream_sequence<Value, struct Tag1> xs(xs_src.begin(), xs_src.end());
        grabin_test::istream_sequence<Value, struct Tag2> ys(ys_src.begin(), ys_src.end());

        REQUIRE(grabin::includes(xs_src, xs_src, cmp));
        REQUIRE(grabin::includes(ys_src, ys_src, cmp));
        REQUIRE(grabin::includes(xs, ys, cmp)
                == std::includes(xs_src.begin(), xs_src.end(), ys_src.begin(), ys_src.end(), cmp));
        REQUIRE(grabin::includes(ys_src, xs_src, cmp)
                == std::includes(ys_src.begin(), ys_src.end(), xs_src.begin(), xs_src.end(), cmp));
    };

    grabin_test::check(property);
}

TEST_CASE("stable_sort")
{
    using Value = int;

    struct Wrapper
    {
        Wrapper(Value val)
         : value(std::move(val))
        {}

        Value value;

        bool operator==(Wrapper const & that) const
        {
            return this->value == that.value;
        }

        bool operator<(Wrapper const & that) const
        {
            return abs(this->value) < abs(that.value);
        }
    };

    auto property = [](std::vector<Value> const & xs_src)
    {
        std::vector<Wrapper> xs_old;
        for(auto const & x : xs_src)
        {
            xs_old.emplace_back(x);
        }

        auto xs_grabin = xs_old;
        grabin::stable_sort(xs_grabin);

        auto xs_std = xs_old;
        std::stable_sort(xs_std.begin(), xs_std.end());

        REQUIRE(grabin::is_permutation(xs_grabin, xs_old));
        REQUIRE(grabin::is_sorted(xs_grabin));
        REQUIRE(xs_std == xs_grabin);
    };

    grabin_test::check(property);
}

TEST_CASE("stable_sort with compare")
{
    using Value = int;
    auto property = [](std::vector<Value> const & xs_old)
    {
        auto cmp = [](Value const & x, Value const & y)
        {
            using std::abs;
            return abs(x) < abs(y);
        };

        auto xs_grabin = xs_old;
        grabin::stable_sort(xs_grabin, cmp);

        auto xs_std = xs_old;
        std::stable_sort(xs_std.begin(), xs_std.end(), cmp);

        REQUIRE(grabin::is_permutation(xs_grabin, xs_old));
        REQUIRE(grabin::is_sorted(xs_grabin, cmp));
        REQUIRE(xs_std == xs_grabin);
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
            auto ys = xs_old;
            std::make_heap(ys.begin(), ys.end());
            return ys;
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
            auto ys = xs_old;
            std::make_heap(ys.begin(), ys.end(), pred);
            return ys;
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

    auto property = [](Value const & value, Value const & bound1, Value const & bound2)
    {
        auto const cmp = [](Value const & x, Value const & y)
        {
            return x.first < y.first;
        };

        auto const bounds = std::minmax(bound1, bound2);

        auto const value_clamped = grabin::clamp(value, bounds.first, bounds.second, cmp);

        REQUIRE(!cmp(value_clamped, bounds.first));
        REQUIRE(!cmp(bounds.second, value_clamped));

        if(cmp(value, bounds.first))
        {
            REQUIRE(value_clamped == bounds.first);
        }
        else if(cmp(bounds.second, value))
        {
            REQUIRE(value_clamped == bounds.second);
        }
        else
        {
            REQUIRE(value_clamped == value);
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
