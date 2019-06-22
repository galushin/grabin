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

#ifndef Z_GRABIN_UTILITY_IN_PLACE_HPP_INCLUDED
#define Z_GRABIN_UTILITY_IN_PLACE_HPP_INCLUDED

#include <grabin/detail/config.hpp>

namespace grabin
{
inline namespace v1
{
    struct in_place_t
    {
        struct tag{};

        in_place_t() = delete;

        constexpr explicit in_place_t(tag)
        {}
    };

#if GRABIN_HAS_INLINE_VARIABLES()
// @todo Реализовать для этого случая
#else
    /// @cond false
    namespace detail
    {
        template <class T>
        struct in_place_holder
        {
            static constexpr in_place_t in_place{in_place_t::tag{}};
        };

        template <class T>
        constexpr in_place_t in_place_holder<T>::in_place;
    }
    // namespace detail
    /// @endcond
    namespace
    {
        constexpr auto const & in_place = detail::in_place_holder<void>::in_place;
    }
#endif
}
// namespace v1
}
// namespace grabin

#endif
// Z_GRABIN_UTILITY_IN_PLACE_HPP_INCLUDED
