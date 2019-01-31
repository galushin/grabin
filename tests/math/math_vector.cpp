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

#include <grabin/math/math_vector.hpp>

#include <catch2/catch.hpp>
#include "../grabin_test.hpp"

// @todo Перенести в ../grabin_test.hpp ?
namespace grabin_test
{
    template <class T, class Check>
    struct Arbitrary<grabin::math_vector<T, Check>>
    {
        using value_type = grabin::math_vector<T, Check>;

        template <class Engine>
        static value_type generate(Engine & rnd, size_t generation)
        {
            using Vec = std::vector<typename value_type::value_type>;

            return value_type(Arbitrary<Vec>::generate(rnd, generation));
        }
    };
}
// namespace grabin_test

// @todo Поддержка вывода на экран в Catch2

TEST_CASE("math_vector : types and default ctor")
{
    using Value = int;
    using Vector = grabin::math_vector<Value>;

    // @todo Проверить тип элементов для нескольких разных типов?
    // @todo Проверить Vector::size_type, а не только факт его наличия
    using Size = Vector::size_type;

    using CIter = typename Vector::const_iterator;

    static_assert(std::is_same<std::iterator_traits<CIter>::iterator_category,
                               std::random_access_iterator_tag>::value, "");
    static_assert(std::is_same<std::iterator_traits<CIter>::value_type, Value>::value, "");

    Vector x;

    CHECK(x.dim() == 0);
    CHECK(x.begin() == x.end());
}

TEST_CASE("math_vector : ctor with size")
{
    using Value = int;
    using Vector = grabin::math_vector<Value>;

    for(auto n = 1; n < 20; ++ n)
    {
        Vector x(n);

        CHECK(x.dim() == n);
        CHECK(x.end() == x.begin() + n);

        for(auto const & elem : x)
        {
            CHECK(elem == Value(0));
        }
    }
}

TEST_CASE("math_vector : range ctor")
{
    using Value = int;

    auto property = [](std::vector<Value> const & values)
    {
        grabin::math_vector<Value> const xs(values);

        CHECK(xs.dim() == values.size());

        CAPTURE(values, xs);
        CHECK(std::equal(xs.begin(), xs.end(), values.begin(), values.end()));
    };

    grabin_test::check(property);
}

TEST_CASE("math_vector : initializer list ctor")
{
    using Value = int;
    using Vector = grabin::math_vector<Value>;

    auto property = [](Value const & x, Value const & y, Value const & z)
    {
        Vector r{x, y, z};

        REQUIRE(r.dim() == 3);
        CHECK(r.begin()[0] == x);
        CHECK(r.begin()[1] == y);
        CHECK(r.begin()[2] == z);
    };

    grabin_test::check(property);
}

// @todo Конструктор с размером и значением элементов
// @todo Типы (неконстантных) итераторов и они должны быть произвольного доступа

TEST_CASE("math_vector : equality")
{
    using Value = int;
    using Vector = grabin::math_vector<Value>;

    auto property = [](Vector const & x, Vector const & y)
    {
        CHECK(x == x);
        CHECK(y == y);

        if(std::equal(x.begin(), x.end(), y.begin(), y.end()))
        {
            CHECK(x == y);
            CHECK_FALSE(x != y);
        }
        else
        {
            CHECK_FALSE(x == y);
            CHECK(x != y);
        }
    };

    grabin_test::check(property);
}

TEST_CASE("math_vector: copy ctor")
{
    using Value = int;
    using Vector = grabin::math_vector<Value>;

    auto property = [](Vector const & x)
    {
        Vector const y = x;

        CHECK(y == x);
    };

    grabin_test::check(property);
}

TEST_CASE("math_vector: move ctor")
{
    using Value = int;
    using Vector = grabin::math_vector<Value>;

    auto property = [](Vector const & x_old)
    {
        auto x = x_old;

        auto const first = x.begin();
        auto const last = x.end();

        auto y = std::move(x);

        CHECK(y == x_old);
        CHECK(y.begin() == first);
        CHECK(y.end() == last);
        CHECK(x == Vector());
    };

    grabin_test::check(property);
}

TEST_CASE("math_vector: assign")
{
    using Value = int;
    using Vector = grabin::math_vector<Value>;

    auto property = [](Vector x, Vector const & y)
    {
        x = y;

        CHECK(y == x);
    };

    grabin_test::check(property);
}

TEST_CASE("math_vector: move assign")
{
    using Value = int;
    using Vector = grabin::math_vector<Value>;

    auto property = [](Vector const & x_old, Vector y)
    {
        auto x = x_old;

        auto const first = x.begin();
        auto const last = x.end();

        y = std::move(x);

        CHECK(y == x_old);
        CHECK(y.begin() == first);
        CHECK(y.end() == last);

        // @todo Что должно быть с x после того, как его содержимое было перемещено?
    };

    grabin_test::check(property);
}

// Индексированный доступ
TEST_CASE("math_vector: const indexing")
{
    using Value = int;
    using Vector = grabin::math_vector<Value>;

    auto property = [](Vector const & x)
    {
        for(auto i = 0*x.dim(); i < x.dim(); ++ i)
        {
            CHECK(x[i] == x.begin()[i]);

            CHECK_THROWS_AS(x[x.dim() + i], std::out_of_range);

            if(i > 0)
            {
                CHECK_THROWS_AS(x[-i], std::out_of_range);
            }
        }
    };

    grabin_test::check(property);
}

TEST_CASE("math_vector: non-const indexing")
{
    using Value = int;
    using Vector = grabin::math_vector<Value>;

    auto property = [](Vector const & x_old, Vector::size_type index, Value const & a)
    {
        auto x = x_old;

        x[index] = a;

        auto const & x_c = x;
        CHECK(x_c[index] == a);

        for(auto i = 0*x.dim(); i < x.dim(); ++ i)
        {
            if(i != index)
            {
                CHECK(x_c[i] == x_old[i]);
            }
        }

        CHECK_THROWS_AS(x[x.dim() + index], std::out_of_range);

        if(index > 0)
        {
            CHECK_THROWS_AS(x[-index], std::out_of_range);
        }
    };

    for(auto generation = 0; generation < 100; ++ generation)
    {
        auto & rnd = grabin_test::random_engine();

        grabin::math_vector<Value> xs(generation+1);
        std::uniform_int_distribution<Value> distr(0, generation);
        std::generate(xs.begin(), xs.end(),
                      [&]{ return grabin_test::Arbitrary<Value>::generate(rnd, distr(rnd)); });

        auto const value = grabin_test::Arbitrary<Value>::generate(rnd, generation);
        auto const index = std::uniform_int_distribution<Value>(0, xs.dim()-1)(rnd);

        property(xs, index, value);
    }
}

TEST_CASE("math_vector: const at")
{
    using Value = int;
    using Vector = grabin::math_vector<Value, grabin::math_vector_no_checks_policy>;

    auto property = [](Vector const & x)
    {
        for(auto i = 0*x.dim(); i < x.dim(); ++ i)
        {
            CHECK(x.at(i) == x.begin()[i]);

            CHECK_THROWS_AS(x.at(x.dim() + i), std::out_of_range);

            if(i > 0)
            {
                CHECK_THROWS_AS(x.at(-i), std::out_of_range);
            }
        }
    };

    grabin_test::check(property);
}

TEST_CASE("math_vector: non-const at")
{
    using Value = int;
    using Vector = grabin::math_vector<Value, grabin::math_vector_no_checks_policy>;

    auto property = [](Vector const & x_old, Vector::size_type index, Value const & a)
    {
        auto x = x_old;

        x.at(index) = a;

        auto const & x_c = x;
        CHECK(x_c[index] == a);

        for(auto i = 0*x.dim(); i < x.dim(); ++ i)
        {
            if(i != index)
            {
                CHECK(x_c[i] == x_old[i]);
            }
        }

        CHECK_THROWS_AS(x.at(x.dim() + index), std::out_of_range);

        if(index > 0)
        {
            CHECK_THROWS_AS(x.at(-index), std::out_of_range);
        }
    };

    for(auto generation = 0; generation < 100; ++ generation)
    {
        auto & rnd = grabin_test::random_engine();

        Vector xs(generation+1);
        std::uniform_int_distribution<Value> distr(0, generation);
        std::generate(xs.begin(), xs.end(),
                      [&]{ return grabin_test::Arbitrary<Value>::generate(rnd, distr(rnd)); });

        auto const value = grabin_test::Arbitrary<Vector::size_type>::generate(rnd, generation);
        auto const index = std::uniform_int_distribution<Value>(0, xs.dim()-1)(rnd);

        property(xs, index, value);
    }
}

// Линейные операции
TEST_CASE("math_vector: multiplication by scalar")
{
    using Value = int;
    using Vector = grabin::math_vector<Value>;

    auto property = [](Vector const & x, Value const & a)
    {
        auto const y1 = [&]()
        {
            auto y = x;

            for(auto & e : y)
            {
                e *= a;
            }
            return y;
        }();

        auto const y2 = a * x;
        auto const y3 = x * a;

        auto const y4 = [&]()
        {
            auto y = x;
            y *= a;
            return y;
        }();

        REQUIRE(y1.dim() == x.dim());
        for(auto i = y1.begin(); i != y1.end(); ++ i)
        {
            CHECK(*i == a * *(x.begin() + (i - y1.begin())));
        }

        CHECK(y2 == y1);
        CHECK(y3 == y1);
        CHECK(y4 == y1);
    };

    for(auto generation = 0; generation < 100; ++ generation)
    {
        auto & rnd = grabin_test::random_engine();
        std::uniform_int_distribution<Value> distr(-1000, +1000);

        auto const a = distr(rnd);

        grabin::math_vector<Value> xs(generation+1);
        std::generate(xs.begin(), xs.end(), [&]{ return distr(rnd); });

        property(xs, a);
    }
}

TEST_CASE("math_vector: division by zero")
{
    using Value = int;
    using Vector = grabin::math_vector<Value>;

    auto property = [](Vector const & x)
    {
        REQUIRE_THROWS_AS(x / 0, std::logic_error);
    };

    grabin_test::check(property);
}

TEST_CASE("math_vector: division by non-zero")
{
    using Value = int;
    using Vector = grabin::math_vector<Value>;

    auto property = [](Vector const & x, Value const & a)
    {
        if(a == 0)
        {
            return;
        }

        auto const y1 = [&]()
        {
            auto y = x;

            for(auto & e : y)
            {
                e /= a;
            }
            return y;
        }();

        auto const y2 = x / a;

        auto const y3 = [&]()
        {
            auto y = x;
            y /= a;
            return y;
        }();

        REQUIRE(y1.dim() == x.dim());
        for(auto i = y1.begin(); i != y1.end(); ++ i)
        {
            CHECK(*i == *(x.begin() + (i - y1.begin())) / a);
        }

        CHECK(y2 == y1);
        CHECK(y3 == y1);
    };

    for(auto generation = 0; generation < 100; ++ generation)
    {
        auto & rnd = grabin_test::random_engine();
        std::uniform_int_distribution<Value> distr(-1000, +1000);

        auto const a = distr(rnd);

        grabin::math_vector<Value> xs(generation+1);
        std::generate(xs.begin(), xs.end(), [&]{ return distr(rnd); });

        property(xs, a);
    }
}

TEST_CASE("math_vector: operator plus")
{
    using Value = int;
    using Vector = grabin::math_vector<Value>;

    auto property = [](Vector const & x, Vector const & y)
    {
        auto const n = x.dim();
        REQUIRE(y.dim() == n);

        auto const z1 = [&]()
        {
            auto z = Vector(n);

            std::transform(x.begin(), x.end(), y.begin(), z.begin(),
                           std::plus<>{});

            return z;
        }();

        auto const z2 = x + y;

        auto const z3 = [&]()
        {
            auto z = x;
            z += y;
            return z;
        }();

        REQUIRE(z1.dim() == x.dim());
        for(auto i = z1.begin(); i != z1.end(); ++ i)
        {
            auto const expected
                = *(x.begin() + (i - z1.begin()))
                + *(y.begin() + (i - z1.begin()));

            CHECK(*i == expected);
        }

        CHECK(z2 == z1);
        CHECK(z3 == z1);
    };

    for(auto generation = 0; generation < 100; ++ generation)
    {
        auto & rnd = grabin_test::random_engine();
        std::uniform_int_distribution<Value> distr(-1000, +1000);

        grabin::math_vector<Value> xs(generation+1);
        std::generate(xs.begin(), xs.end(), [&]{ return distr(rnd); });

        grabin::math_vector<Value> ys(generation+1);
        std::generate(ys.begin(), ys.end(), [&]{ return distr(rnd); });

        property(xs, ys);
    }
}

#include <stdexcept>

TEST_CASE("math_vector: operator plus throws on different dimensions")
{
    using Value = int;
    using Vector = grabin::math_vector<Value>;

    auto property = [](Vector x, Vector const & y)
    {
        if(x.dim() != y.dim())
        {
            CHECK_THROWS_AS(x += y, std::logic_error);
            CHECK_THROWS_AS(x + y, std::logic_error);
        }
    };

    grabin_test::check(property);
}
