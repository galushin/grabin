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

#ifndef Z_GRABIN_UTILITY_REL_OPS_HPP_INCLUDED
#define Z_GRABIN_UTILITY_REL_OPS_HPP_INCLUDED

/** @file grabin/utility/rel_ops.hpp
 @brief Упрощение определения операторов сравнения
*/

namespace grabin
{
inline namespace v1
{
namespace rel_ops
{
    /** @brief Тип-тэг, наследование от которого импортирует операторы из
    данного пространства имён
    */
    struct enable_adl
    {
    protected:
        ~enable_adl() = default;
    };

    /** @brief Оператор "не равно"
    @param x,y аргументы
    @return <tt> !(x == y) </tt>
    */
    template <class T>
    bool operator!=(T const & x, T const & y)
    {
        return !(x == y);
    }
}
// namespace rel_ops
}
// namespace v1
}
// namespace grabin

#endif
// Z_GRABIN_UTILITY_REL_OPS_HPP_INCLUDED
