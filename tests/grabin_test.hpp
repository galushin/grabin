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

#ifndef Z_GRABIN_TEST_HPP_INCLUDED
#define Z_GRABIN_TEST_HPP_INCLUDED

#include <catch2/catch.hpp>

#include <ctime>
#include <random>
#include <tuple>
#include <utility>

namespace grabin_test
{
    // Генератор случайных чисел
    using Random_engine = std::mt19937;

    Random_engine & random_engine();

    // Получение приозвольных значений
    // @todo Задокументировать неполноту
    template <class T>
    struct Arbitrary;

    // Вспомогательные возможности для Arbitrary
    template <class RealType>
    struct ArbitraryReal
    {
    public:
        using value_type = RealType;

        template <class Engine>
        static value_type generate(Engine & rnd, size_t generation)
        {
            switch(generation)
            {
            case 0:
                return value_type(0);

            case 1:
                return std::numeric_limits<value_type>::min();

            case 2:
                return std::numeric_limits<value_type>::max();

            case 3:
                return std::numeric_limits<value_type>::lowest();

            default:
                std::uniform_real_distribution<value_type>
                    distr(std::numeric_limits<value_type>::min(),
                          std::numeric_limits<value_type>::max());
                return distr(rnd);
            }
        }
    };

    template <class IntType>
    struct ArbitraryInteger
    {
    public:
        using value_type = IntType;

        template <class Engine>
        static value_type generate(Engine & rnd, size_t generation)
        {
            switch(generation)
            {
            case 0:
                return value_type(0);

            case 1:
                return std::numeric_limits<value_type>::min();

            case 2:
                return std::numeric_limits<value_type>::max();

            default:
                std::uniform_int_distribution<value_type>
                    distr(std::numeric_limits<value_type>::min(),
                          std::numeric_limits<value_type>::max());
                return distr(rnd);
            }
        }
    };

    template <class Container>
    struct ArbitraryContainer;

    template <class T, class A>
    struct ArbitraryContainer<std::vector<T, A>>
    {
        using value_type = std::vector<T, A>;

        template <class Engine>
        static value_type generate(Engine & rnd, size_t generation)
        {
            // @todo Выбирать размер случайно, иначе все вектора одного
            // поколения будут иметь одинаковый размер, то есть мы ограничим
            // покрываемое тестами пространство
            if(generation == 0)
            {
                return {};
            }
            else
            {
                value_type result(generation);

                for(auto & x : result)
                {
                    // @todo Подумать, как больше разнообразить элементы
                    x = Arbitrary<T>::generate(rnd, generation - 1);
                }

                return result;
            }
        }
    };

    // Специализации Arbitrary
    template <>
    struct Arbitrary<int>
     : ArbitraryInteger<int>
    {};

    template <>
    struct Arbitrary<double>
     : ArbitraryReal<double>
    {};

    template <class... Types>
    struct Arbitrary<std::tuple<Types...>>
    {
        using value_type = std::tuple<Types...>;

        template <class Engine>
        static value_type generate(Engine & rnd, size_t generation)
        {
            return value_type(Arbitrary<Types>::generate(rnd, generation)...);
        }
    };

    // @todo Можно ли написать одну специализацию для всех контейнеров
    template <class T, class A>
    struct Arbitrary<std::vector<T, A>>
     : ArbitraryContainer<std::vector<T, A>>
    {};

    // Проверка свойств
    namespace detail
    {
        template <class F, class Tuple, std::size_t... I>
        constexpr decltype(auto) apply_impl(F&& f, Tuple&& t, std::index_sequence<I...>)
        {
            // @todo Реализовать и использовать здесь invoke
            return std::forward<F>(f)(std::get<I>(std::forward<Tuple>(t))...);
        }
    }
    // namespace detail

    template <class F, class Tuple>
    constexpr decltype(auto) apply(F&& f, Tuple&& t)
    {
        return detail::apply_impl(
            std::forward<F>(f), std::forward<Tuple>(t),
            std::make_index_sequence<std::tuple_size<std::remove_reference_t<Tuple>>::value>{});
    }

    namespace detail
    {
        template <class R, class... Args>
        void check_impl(R(property)(Args...))
        {
            using Value = std::tuple<std::remove_cv_t<std::remove_reference_t<Args>>...>;
            auto & rnd = grabin_test::random_engine();

            for(auto generation = 0; generation < 100; ++ generation)
            {
                auto args = grabin_test::Arbitrary<Value>::generate(rnd, generation);

                // @todo Анализировать возвращаемое значение?
                grabin_test::apply(property, args);
            }
        }
    }
    // namespace detail

    template <class Lambda>
    void check(Lambda property)
    {
        return detail::check_impl(+property);
    }

namespace Matchers
{
    inline auto WithinRel(double target, double rel_error)
    {
        return ::Catch::Matchers::WithinAbs(target, std::abs(target) * rel_error);
    }
}
// namespace Matchers
}
// namespace grabin_test


#endif
// Z_GRABIN_TEST_HPP_INCLUDED
