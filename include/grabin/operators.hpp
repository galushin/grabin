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

#ifndef Z_GRABIN_OPERATORS_HPP_INCLUDED
#define Z_GRABIN_OPERATORS_HPP_INCLUDED

/** @file grabin/operators.hpp
 @brief Упрощение определения операторов
*/

#include <grabin/utility/rel_ops.hpp>

#include <algorithm>

namespace grabin
{
inline namespace v1
{
namespace operators
{
/// @brief Определение операторов "равно" и "не равно" для контейнеров
namespace container_equality
{
    /** @brief Тип-тэг, наследование от которого импортирует операторы из
    данного пространства имён
    */
    struct enable_adl
     : grabin::rel_ops::enable_adl
    {
    protected:
        ~enable_adl() = default;
    };

    /** @brief Оператор "равно"
    @param x,y аргументы
    @return <tt> std::equal(x.begin(), x.end(), y.begin(), y.end()) </tt>
    */
    template <class Container>
    bool operator==(Container const & x, Container const & y)
    {
        return std::equal(x.begin(), x.end(), y.begin(), y.end());
    }
}
}
// namespace operators
}
// namespace v1
}
// namespace grabin

#endif
// Z_GRABIN_OPERATORS_HPP_INCLUDED
