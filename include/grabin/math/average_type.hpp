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

#ifndef Z_GRABIN_MATH_AVERAGE_TYPE_HPP_INCLUDED
#define Z_GRABIN_MATH_AVERAGE_TYPE_HPP_INCLUDED

/** @file grabin/math/average_type.hpp
 @brief Класс-характеристика для определения типа среднего значения
*/

#include <type_traits>

namespace grabin
{
inline namespace v1
{
    /** @brief Класс-характеристика для определения типа среднего значения
    @tparam T тип элементов
    @tparam W тип весов
    */
    template <class T, class W>
    struct average_type
    {
    private:
        using R_T = decltype(std::declval<T>() * std::declval<W>() / std::declval<W>());
        using R_double = decltype(std::declval<double>() * std::declval<T>() / std::declval<W>());

    public:
        /// @brief Тип среднего значения
        using type = std::conditional_t<std::is_integral<T>::value, R_double, R_T>;
    };

    /** @brief Тип-синоним для типа среднего значения
    @tparam T тип элементов
    @tparam W тип весов
    */
    template <class T, class W>
    using average_type_t = typename average_type<T, W>::type;
}
// namespace v1
}
// namespace grabin


#endif
// Z_GRABIN_MATH_AVERAGE_TYPE_HPP_INCLUDED
