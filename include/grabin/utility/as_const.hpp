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

#ifndef Z_GRABIN_UTILITY_AS_CONST_HPP_INCLUDED
#define Z_GRABIN_UTILITY_AS_CONST_HPP_INCLUDED

/** @file grabin/utility/as_const.hpp
 @brief Создание константной ссылки на объект
*/

#include <type_traits>

namespace grabin
{
inline namespace v1
{
    /** @brief Создание константной ссылки на объект
    @param obj ссылка на объект, на который должна быть создана константная
    ссылка
    @return <tt>static_cast<std::add_const_t<T> &>(obj)</tt>
    @throw Ничего
    @todo Поддержка constexpr
    */
    template <class T>
    std::add_const_t<T> &
    as_const(T & obj) noexcept
    {
        return obj;
    }

    /** @brief Создание ссылок на временные значения запрещено
    @todo Как покрыть это тестами?
    */
    template <class T>
    void as_const(T const &&) = delete;
}
// namespace v1
}
// namespace grabin

#endif
// Z_GRABIN_UTILITY_AS_CONST_HPP_INCLUDED
