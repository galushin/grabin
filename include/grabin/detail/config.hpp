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

#ifndef Z_GRABIN_DETAIL_CONFIG_HPP_INCLUDED
#define Z_GRABIN_DETAIL_CONFIG_HPP_INCLUDED

namespace grabin
{
inline namespace v1
{
    template <class T>
    struct empty_constexpr_constant
    {
        static constexpr T value{};
    };

    template <class T>
    constexpr T empty_constexpr_constant<T>::value;
}
// namespace v1
}
// namespace grabin

// @todo Определять в зависимости от типа компилятора и его версии
#define GRABIN_HAS_INLINE_VARIABLES() 0

#if GRABIN_HAS_INLINE_VARIABLES()
#define GRABIN_INLINE_CONSTEXPR_EMPTY_CONSTANT(Type, Name) inline constexpr const auto Name = Type{}
#else
#define GRABIN_INLINE_CONSTEXPR_EMPTY_CONSTANT(Type, Name)\
namespace\
{\
    constexpr auto const & Name = ::grabin::empty_constexpr_constant<Type>::value;\
}
#endif

#endif
// Z_GRABIN_DETAIL_CONFIG_HPP_INCLUDED
