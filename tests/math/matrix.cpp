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
TEST_CASE("matrix : begin/end")
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

// @todo Неконстантные begin/end
// @todo cbegin/cend

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
// @todo Умножение матрицы на число
// @todo Сложение матриц
