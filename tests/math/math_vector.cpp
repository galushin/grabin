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
    template <class T>
    struct Arbitrary<grabin::math_vector<T>>
    {
        using value_type = grabin::math_vector<T>;

        template <class Engine>
        static value_type generate(Engine & rnd, size_t generation)
        {
            using Vec = std::vector<typename value_type::value_type>;

            // @todo Поддерживать ли пустые вектора?
            return value_type(Arbitrary<Vec>::generate(rnd, generation + 1));
        }
    };
}
// namespace grabin_test

// @todo Поддержка вывода на экран в Catch2

TEST_CASE("math_vector : types and ctor with size")
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
        grabin::math_vector<Value> xs(values);

        CHECK(xs.dim() == values.size());

        CAPTURE(values, xs);
        CHECK(std::equal(xs.begin(), xs.end(), values.begin(), values.end()));
    };

    grabin_test::check(property);
}

TEST_CASE("math_vector : equality")
{
    using Value = int;
    using Vector = grabin::math_vector<Value>;

    auto property = [](Vector const & x, Vector const & y)
    {
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

// @todo Сложение векторов - обработка случая разной размерности

// @todo Оператор индексирования
// @todo Стратегия проверки
// @todo Конструктор копий, присваивание
// @todo Конструктор копий и присваивание с перемещением
// @todo Тест конструктора, принимающего список инициализации
// @todo Конструктор с размером и значением элементов
// @todo Типы (неконстантных) итераторов и они должны быть произвольного доступа
