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

#ifndef Z_GRABIN_ITERATOR_HPP_INCLUDED
#define Z_GRABIN_ITERATOR_HPP_INCLUDED

/** @file grabin/iterator.hpp
 @brief Функциональность, связанная с итераторами
*/

#include <iterator>

namespace grabin
{
inline namespace v1
{
    /// @cond false
    namespace detail
    {
        template <class T>
        struct static_empty_const
        {
            static constexpr T value{};
        };

        template <class T>
        constexpr T static_empty_const<T>::value;

        using std::begin;
        using std::end;

        struct begin_fn
        {
            template <class Range>
            auto operator()(Range && r) const
            -> decltype(begin(std::forward<Range>(r)))
            {
                return begin(std::forward<Range>(r));
            }
        };

        struct end_fn
        {
            template <class Range>
            auto operator()(Range && r) const
            -> decltype(end(std::forward<Range>(r)))
            {
                return end(std::forward<Range>(r));
            }
        };
    }
    // namespace detail
    /// @endcond

    namespace
    {
        /// @brief Функциональный объект для @c begin
        constexpr auto const & begin
            = detail::static_empty_const<detail::begin_fn>::value;

        /// @brief Функциональный объект для @c end
        constexpr auto const & end
            = detail::static_empty_const<detail::end_fn>::value;
    }
}
// namespace v1
}
// namespace grabin

#endif
// Z_GRABIN_ITERATOR_HPP_INCLUDED
