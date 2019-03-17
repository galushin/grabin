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

#ifndef Z_GRABIN_NUMERIC_HPP_INCLUDED
#define Z_GRABIN_NUMERIC_HPP_INCLUDED

/** @file grabin/numeric.hpp
 @brief Обобщённые численные операции
*/

#include <grabin/iterator.hpp>
#include <numeric>

namespace grabin
{
inline namespace v1
{
    /** @brief Присваивает элементам последовательности последовательные значения, начиная с @c init
    @param seq последовательность
    @param init начальное значение
    */
    template <class ForwardSequence, class T>
    void iota(ForwardSequence && seq, T init)
    {
        std::iota(grabin::begin(seq), grabin::end(seq), std::move(init));
    }

    /** @brief Вычисляет левую свёртку (left fold) последовательности элементов
    @param in последовательность элементов
    @param init_value начальное значение
    @param op бинарная операция, если она не задана явно, то используется бинарный плюс
    @return Левая свёртка элементов @c in, используя бинарную операцию @c op и начальное значение
    @c init_value
    */
    template <class InputSequence, class T, class BinaryOperation = std::plus<>>
    T accumulate(InputSequence && in, T const & init_value, BinaryOperation op = BinaryOperation())
    {
        return std::accumulate(grabin::begin(in), grabin::end(in), init_value, std::move(op));
    }

    /** @brief Перемножает соответствующие элементы двух последовательностей, а результаты
    последовательно прибавляет к @c init
    @param in1, in2 входные последовательности
    @param init исходное значение
    @pre <tt>size(in1) >= size(in2)</tt>
    */
    template <class InputSequence1, class InputSequence2, class T>
    T inner_product(InputSequence1 && in1, InputSequence2 && in2, T init)
    {
        return std::inner_product(grabin::begin(in1), grabin::end(in1),
                                  grabin::begin(in2), std::move(init));
    }

    /** @brief Свёртка последовательности элементов
    @param in последовательность элементов
    @param init_value начальное значение, если оно не задано явно, то используется <tt>T{}</tt>,
    где @c T -- тип элементов последовательности @c in
    @param op бинарная операция, если она не задана явно, то используется бинарный плюс
    @return Cвёртка элементов @c in, используя бинарную операцию @c op и начальное значение
    @c init_value
    */
    template <class InputSequence,
              class T = typename std::decay_t<InputSequence>::value_type,
              class BinaryOperation = std::plus<>>
    T reduce(InputSequence && in, T const & init_value = T{},
             BinaryOperation op = BinaryOperation())
    {
        return grabin::accumulate(std::forward<InputSequence>(in), init_value, std::move(op));
    }
}
// namespace v1
}
// namespace grabin


#endif // Z_GRABIN_NUMERIC_HPP_INCLUDED
