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
#include <forward_list>
#include <list>
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
    template <class T, class SFINAE = void>
    struct Arbitrary;

    /// @brief Специальный тип-обёртка для порождения размера контейнеров
    template <class T>
    struct container_size
    {
        T value;

        operator T const &() const
        {
            return this->value;
        }
    };

    using generation_t = std::intmax_t;

    // Вспомогательные возможности для Arbitrary
    template <class RealType>
    struct ArbitraryReal
    {
    public:
        using value_type = RealType;

        template <class Engine>
        static value_type generate(Engine & rnd, generation_t generation)
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
        static value_type generate(Engine & rnd, generation_t generation)
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
    struct ArbitraryContainer
    {
        using value_type = Container;

        template <class Engine>
        static value_type generate(Engine & rnd, generation_t generation)
        {
            using Elem = typename value_type::value_type;
            auto src = ArbitraryContainer<std::vector<Elem>>::generate(rnd, generation);

            // @todo Использовать move-итераторы?
            return value_type(src.begin(), src.end());
        }
    };

    template <class T, class A>
    struct ArbitraryContainer<std::vector<T, A>>
    {
        using value_type = std::vector<T, A>;

        template <class Engine>
        static value_type generate(Engine & rnd, generation_t generation)
        {
            using Size = typename value_type::size_type;
            auto const n = Arbitrary<container_size<Size>>::generate(rnd, generation);

            if(n == 0)
            {
                return {};
            }

            value_type result;
            result.reserve(n);
            std::uniform_int_distribution<Size> distr(0, generation);
            std::generate_n(std::back_inserter(result), n,
                            [&]{ return Arbitrary<T>::generate(rnd, distr(rnd));});

            return result;
        }
    };

    // Специализации Arbitrary
    template <class T>
    struct Arbitrary<T, std::enable_if_t<std::is_integral<T>::value>>
     : ArbitraryInteger<T>
    {};

    template <class T>
    struct Arbitrary<T, std::enable_if_t<std::is_floating_point<T>::value>>
     : ArbitraryReal<T>
    {};

    template <class T1, class T2>
    struct Arbitrary<std::pair<T1, T2>>
    {
        using value_type = std::pair<T1, T2>;

        template <class Engine>
        static value_type generate(Engine & rnd, generation_t generation)
        {
            return value_type(Arbitrary<T1>::generate(rnd, generation),
                              Arbitrary<T2>::generate(rnd, generation));
        }
    };

    template <class... Types>
    struct Arbitrary<std::tuple<Types...>>
    {
        using value_type = std::tuple<Types...>;

        template <class Engine>
        static value_type generate(Engine & rnd, generation_t generation)
        {
            return value_type(Arbitrary<Types>::generate(rnd, generation)...);
        }
    };

    template <class T>
    struct Arbitrary<container_size<T>>
    {
        using value_type = container_size<T>;

        template <class Engine>
        static value_type generate(Engine & rnd, generation_t generation)
        {
            return {std::uniform_int_distribution<T>(T(0), static_cast<T>(generation))(rnd)};
        }
    };

    template <class T, class A>
    struct Arbitrary<std::vector<T, A>>
     : ArbitraryContainer<std::vector<T, A>>
    {};

    template <class T, class A>
    struct Arbitrary<std::forward_list<T, A>>
     : ArbitraryContainer<std::forward_list<T, A>>
    {};

    template <class T, class A>
    struct Arbitrary<std::list<T, A>>
     : ArbitraryContainer<std::list<T, A>>
    {};

    // Проверка свойств
    namespace detail
    {
        template <class F, class Tuple, std::size_t... I>
        constexpr decltype(auto) apply_impl(F&& f, Tuple&& t, std::index_sequence<I...>)
        {
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

    template <class Container>
    class elementwise_within_abs_matcher
     : public Catch::Matchers::Impl::MatcherBase<Container>
    {
    public:
        elementwise_within_abs_matcher(Container target, double abs_error)
         : target_(std::move(target))
         , eps_(abs_error)
        {
            CATCH_ENFORCE(abs_error >= 0, "Invalid margin: " << abs_error << '.'
                          << " Margin has to be non-negative.");
        }

        bool match(Container const & actual) const override
        {
            auto i = actual.begin();
            auto i_end = actual.end();
            auto j = this->target_.begin();
            auto j_end = this->target_.end();

            for(; i != i_end && j != j_end; ++ i, ++ j)
            {
                if(*i > *j + this->eps_ || *j > this->eps_ + *i)
                {
                    return false;
                }
            }

            return (i == i_end) && (j == j_end);
        }

        std::string describe() const override
        {
            return "\nis within " + ::Catch::Detail::stringify(this->eps_) + " of\n"
                   + ::Catch::Detail::stringify(this->target_);
        }

    private:
        Container target_;
        double eps_;
    };

    template <class Container>
    auto elementwise_within_abs(Container target, double abs_error)
    {
        return elementwise_within_abs_matcher<Container>(std::move(target), abs_error);
    }
}
// namespace Matchers
}
// namespace grabin_test

#include <sstream>
#include <grabin/math/matrix.hpp>

namespace Catch
{

    template <class T, class Check>
    struct is_range<grabin::matrix<T, Check>>
     : std::false_type
    {};

    template <class T, class Check>
    struct StringMaker<grabin::matrix<T, Check>>
    {
        static std::string convert(grabin::matrix<T, Check> const & value)
        {
            std::ostringstream os;

            os << "{";
            for(auto i : grabin::view::indices(value.dim1()))
            {
                if(i != 0)
                {
                    os << ", ";
                }

                os << "{ ";
                for(auto j : grabin::view::indices(value.dim2()))
                {
                    if(j != 0)
                    {
                        os << ", ";
                    }
                    os << value(i, j);
                }
                os << " }";
            }
            os << "}";

            return os.str();
        }
    };
}
// namespace Catch


#endif
// Z_GRABIN_TEST_HPP_INCLUDED
