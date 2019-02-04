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

#include <grabin/math/matrix.hpp>

#include <catch2/catch.hpp>
#include "../grabin_test.hpp"

TEST_CASE("matrix : types and default ctor")
{
    using Value = int;
    using Matrix = grabin::matrix<Value>;

    // @todo Проверить тип элементов для нескольких разных типов?
    using Size = Matrix::size_type;

    static_assert(std::is_signed<Size>::value, "");

    Matrix const x{};

    CHECK(x.dim1() == 0);
    CHECK(x.dim2() == 0);
    CHECK(x.dim().first == x.dim1());
    CHECK(x.dim().second == x.dim2());
    CHECK(x.size() == 0);
}

TEST_CASE("matrix : ctor with size")
{
    using Value = int;
    using Matrix = grabin::matrix<Value>;

    auto property = [](grabin_test::container_size<int> rows, grabin_test::container_size<int> cols)
    {
        Matrix const A(rows, cols);

        CHECK(A.dim1() == rows);
        CHECK(A.dim2() == cols);
        CHECK(A.dim().first == A.dim1());
        CHECK(A.dim().second == A.dim2());
        CHECK(A.size() == rows*cols);

        for(auto i = 0*A.dim1(); i < A.dim1(); ++ i)
        for(auto j = 0*A.dim1(); j < A.dim2(); ++ j)
        {
            CHECK(A(i, j) == Value(0));

            CHECK_THROWS_AS(A(rows + i, cols + j), std::out_of_range);
            CHECK_THROWS_AS(A(-(i+1), -(j+1)), std::out_of_range);
        }
    };

    grabin_test::check(property);
}

TEST_CASE("matrix: non-const indexing")
{
    using Value = int;
    using Matrix = grabin::matrix<Value>;

    auto property = [](Matrix::size_type rows, Matrix::size_type cols)
    {
        Matrix x(rows, cols);

        auto const z = [&]()
        {
            auto & rnd = grabin_test::random_engine();
            std::vector<Value> result(rows*cols);
            for(auto & elem : result)
            {
                elem = grabin_test::Arbitrary<Value>::generate(rnd, result.size());
            }
            return result;
        }();


        for(auto i = 0*x.dim1(); i < x.dim1(); ++ i)
        for(auto j = 0*x.dim2(); j < x.dim2(); ++ j)
        {
            x(i, j) = z.at(i*cols + j);

            CHECK_THROWS_AS(x(x.dim1() + i, x.dim2() + j), std::out_of_range);
            CHECK_THROWS_AS(x(-(i+1), -(j+1)), std::out_of_range);
        }

        for(auto i = 0*x.dim1(); i < x.dim1(); ++ i)
        for(auto j = 0*x.dim2(); j < x.dim2(); ++ j)
        {
            CHECK(grabin::as_const(x)(i, j) == z.at(i*cols + j));
        }
    };

    for(auto generation = 0; generation < 100; ++ generation)
    {
        auto & rnd = grabin_test::random_engine();
        std::uniform_int_distribution<Matrix::size_type> distr(0, generation);

        auto const rows = distr(rnd);
        auto const cols = distr(rnd);

        property(rows, cols);
    }
}

namespace grabin_test
{
    template <class T, class Check>
    struct Arbitrary<grabin::matrix<T, Check>>
    {
        using value_type = grabin::matrix<T, Check>;

        template <class Engine>
        static value_type generate(Engine & rnd, generation_t generation)
        {
            using Size = typename value_type::size_type;
            auto const rows = Arbitrary<container_size<Size>>::generate(rnd, generation);
            auto const cols = Arbitrary<container_size<Size>>::generate(rnd, generation);

            value_type result(rows, cols);

            std::uniform_int_distribution<generation_t> distr(0, generation);
            for(auto i = 0*rows; i < rows; ++ i)
            for(auto j = 0*cols; j < cols; ++ j)
            {
                using Elem = typename value_type::value_type;
                result(i, j) = Arbitrary<Elem>::generate(rnd, distr(rnd));
            }

            return result;
        }
    };
}
// namespace grabin_test

// Итераторы
TEST_CASE("matrix : constant begin/end")
{
    using Value = int;
    using Matrix = grabin::matrix<Value>;

    auto property = [](Matrix const & x)
    {
        std::vector<Value> const z1(x.begin(), x.end());

        auto const z2 = [&]()
        {
            std::vector<Value> result;
            result.reserve(x.size());

            for(auto i = 0*x.dim1(); i < x.dim1(); ++ i)
            for(auto j = 0*x.dim2(); j < x.dim2(); ++ j)
            {
                result.push_back(x(i, j));
            }

            return result;
        }();

        CHECK(z1 == z2);
    };

    grabin_test::check(property);
}

TEST_CASE("matrix : non-const begin/end")
{
    using Value = int;
    using Matrix = grabin::matrix<Value>;

    auto property = [](Matrix const & x_old)
    {
        auto x = x_old;
        auto & rnd = grabin_test::random_engine();

        std::vector<Value> vec(x.size());
        for(auto & elem : vec)
        {
            using Generator = grabin_test::Arbitrary<Value>;
            elem = Generator::generate(rnd, x_old.size());
        }

        std::copy(vec.begin(), vec.end(), x.begin());

        CAPTURE(vec, x_old, x);
        CHECK(std::equal(x.begin(), x.end(), vec.begin(), vec.end()));
    };

    grabin_test::check(property);
}

TEST_CASE("matrix: cbegin/cend")
{
    using Value = int;
    using Matrix = grabin::matrix<Value>;

    auto property = [](Matrix x)
    {
        static_assert(std::is_same<decltype(x.cbegin()), Matrix::const_iterator>::value, "");
        static_assert(std::is_same<decltype(x.cend()), Matrix::const_iterator>::value, "");

        CHECK(x.cbegin() == grabin::as_const(x).begin());
        CHECK(x.cend() == grabin::as_const(x).end());
    };

    grabin_test::check(property);
}

TEST_CASE("matrix : equality")
{
    using Value = int;
    using Matrix = grabin::matrix<Value>;

    auto property = [](Matrix const & x, Matrix const & y)
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

// Линейные операции
TEST_CASE("matrix: multiplication by scalar")
{
    using Value = int;
    using Matrix = grabin::matrix<Value>;

    auto property = [](Matrix const & x, Value const & a)
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

        REQUIRE(y1.dim1() == x.dim1());
        REQUIRE(y1.dim2() == x.dim2());

        for(auto i = 0*y1.dim1(); i < y1.dim1(); ++ i)
        for(auto j = 0*y1.dim2(); j < y1.dim2(); ++ j)
        {
            CHECK(y1(i, j) == a * x(i, j));
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

        using Size = Matrix::size_type;
        using Size_generator = grabin_test::Arbitrary<grabin_test::container_size<Size>>;
        auto const rows = Size_generator::generate(rnd, generation);
        auto const cols = Size_generator::generate(rnd, generation);
        Matrix xs(rows+1, cols+1);
        std::generate(xs.begin(), xs.end(), [&]{ return distr(rnd); });

        property(xs, a);
    }
}

TEST_CASE("matrix: operator plus")
{
    using Value = int;
    using Matrix = grabin::matrix<Value>;

    auto property = [](Matrix const & x, Matrix const & y)
    {
        auto const rows = x.dim1();
        auto const cols = x.dim2();

        REQUIRE(y.dim1() == rows);
        REQUIRE(y.dim2() == cols);

        auto const z1 = [&]()
        {
            auto z = Matrix(rows, cols);

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

        REQUIRE(z1.dim1() == x.dim1());
        REQUIRE(z1.dim2() == x.dim2());

        for(auto i = 0*z1.dim1(); i < z1.dim1(); ++ i)
        for(auto j = 0*z1.dim2(); j < z1.dim2(); ++ j)
        {
            CHECK(z1(i, j) == x(i, j) + y(i, j));
        }

        CHECK(z2 == z1);
        CHECK(z3 == z1);
    };

    for(auto generation = 0; generation < 100; ++ generation)
    {
        auto & rnd = grabin_test::random_engine();
        std::uniform_int_distribution<Value> distr(-1000, +1000);

        using Size = Matrix::size_type;
        using Size_generator = grabin_test::Arbitrary<grabin_test::container_size<Size>>;
        auto const rows = Size_generator::generate(rnd, generation);
        auto const cols = Size_generator::generate(rnd, generation);

        Matrix xs(rows+1, cols+1);
        std::generate(xs.begin(), xs.end(), [&]{ return distr(rnd); });

        Matrix ys(rows+1, cols+1);
        std::generate(ys.begin(), ys.end(), [&]{ return distr(rnd); });

        property(xs, ys);
    }
}
