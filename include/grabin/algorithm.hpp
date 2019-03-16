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

#ifndef Z_GRABIN_ALGORITHM_HPP_INCLUDED
#define Z_GRABIN_ALGORITHM_HPP_INCLUDED

/** @file grabin/algorithm.hpp
 @brief Аналоги алгоритмов STL, определённые в терминах последовательностей и курсоров
*/

#include <grabin/iterator.hpp>

#include <algorithm>

namespace grabin
{
inline namespace v1
{
    /** @brief Проверка, что все элементы интервала удовлетворяют заданному
    предикату
    @param input входной интервал
    @param pred унарный предикат
    @return @b true, если для каждого элемента @c x интервала @c input
    выполняется <tt>pred(x) != false</tt>, иначе -- @b false
    */
    template <class InputRange, class UnaryPredicate>
    bool all_of(InputRange && input, UnaryPredicate pred)
    {
        return std::all_of(grabin::begin(input), grabin::end(input),
                           std::move(pred));
    }

    /** @brief Проверка, что некоторый элемент интервала удовлетворяют заданному
    предикату
    @param input входной интервал
    @param pred унарный предикат
    @return @b true, если для некоторого элемента @c x интервала @c input
    выполняется <tt>pred(x) != false</tt>, иначе -- @b false
    */
    template <class InputRange, class UnaryPredicate>
    bool any_of(InputRange && input, UnaryPredicate pred)
    {
        return std::any_of(grabin::begin(input), grabin::end(input),
                           std::move(pred));
    }

    /** @brief Проверка, что ни один элемент интервала не удовлетворяют
    заданному предикату
    @param input входной интервал
    @param pred унарный предикат
    @return @b true, если ни для одного элемента @c x интервала @c input
    не выполняется <tt>pred(x) != false</tt>, иначе -- @b false
    */
    template <class InputRange, class UnaryPredicate>
    bool none_of(InputRange && input, UnaryPredicate pred)
    {
        return std::none_of(grabin::begin(input), grabin::end(input),
                            std::move(pred));
    }

    /** @brief Подсчёт количества элементов, равных заданному значению
    @param input входной интервал
    @param value искомое значение
    @return Количество элементов интервала @c input, равных значению @c value
    */
    template <class InputRange, class T>
    auto count(InputRange && input, T const & value)
    {
        return std::count(grabin::begin(input), grabin::end(input), value);
    }

    /** @brief Подсчёт количества элементов, удовлетворяющих заданному предикату
    @param input входной интервал
    @param pred унарный предикат
    @return Количество элементов интервала @c input, удовлетворяющих предикату
    @c pred
    */
    template <class InputRange, class UnaryPredicate>
    auto count_if(InputRange && input, UnaryPredicate pred)
    {
        return std::count_if(grabin::begin(input), grabin::end(input),
                             std::move(pred));
    }

    /** @brief Присваивает каждому элементу последовательности заданное значение.
    @param seq последовательность
    @param value значение
    */
    template <class ForwardSequence, class T>
    void fill(ForwardSequence && seq, T const & value)
    {
        std::fill(grabin::begin(seq), grabin::end(seq), value);
    }

    /** @brief Присваивает каждому элементу последовательности значение, полученное в результате
    вызова заданной функции без аргументов.
    @param seq последовательность
    @param gen функция без аргументов
    */
    template <class ForwardSequence, class Generator>
    void generate(ForwardSequence && seq, Generator gen)
    {
        std::generate(grabin::begin(seq), grabin::end(seq), std::move(gen));
    }

    /** @brief Проверка равенства двух последовательностей
    @param in1, in2 входные последовательности
    @return @b true, если последовательности содержать одинаковое количество элементов и их
    соответствующие элементы равны
    */
    template <class InputSequence1, class InputSequence2>
    bool equal(InputSequence1 const & in1, InputSequence2 const & in2)
    {
        return std::equal(grabin::begin(in1), grabin::end(in1),
                          grabin::begin(in2), grabin::end(in2));
    }

    /** @brief Проверяет, что две последовательности являются перестановками друг друга
    @param in1, in2 последовательности
    @param bin_pred бинарный предикат, задающий отношение порядка
    @return @b true, если @c in1 и @c in2 являются перестановками друг друга, иначе -- @b false
    */
    template <class ForwardSequence1, class ForwardSequence2, class BinaryPredicate = std::equal_to<>>
    bool is_permutation(ForwardSequence1 const & in1, ForwardSequence2 const & in2,
                        BinaryPredicate bin_pred = BinaryPredicate())
    {
        return std::is_permutation(grabin::begin(in1), grabin::end(in1),
                                   grabin::begin(in2), grabin::end(in2),
                                   std::move(bin_pred));
    }

    /** @brief Построение следующей перестановки последовательности
    @param seq последовательность
    @param bin_pred бинарный предикат, задающий отношение порядка
    @return @b false, если @c seq упорядочена в обратном лексикографическом порядке относительно
    бинарного предиката @c bin_pred, иначе --- @b false.
    @post Преобразует @c seq в лексикографически следующую перестановку, если @c seq уже
    упорядочена в обратном лексикографическом порядке относительно @c bin_pred, то упорядочивает её
    в прямом порядке относительно @c bin_pred.
    */
    template <class BidirectionalSequence, class BinaryPredicate = std::less<>>
    bool next_permutation(BidirectionalSequence && seq,
                          BinaryPredicate bin_pred = BinaryPredicate())
    {
        return std::next_permutation(grabin::begin(seq), grabin::end(seq), std::move(bin_pred));
    }

    /** @brief Построение предыдущей перестановки последовательности
    @param seq последовательность
    @param bin_pred бинарный предикат, задающий отношение порядка
    @return @b false, если @c seq упорядочена в лексикографическом порядке относительно бинарного
    предиката @c bin_pred, иначе --- @b false.
    @post Преобразует @c seq в лексикографически следующую перестановку, если @c seq уже
    упорядочена в лексикографическом порядке относительно @c bin_pred, то упорядочивает её
    в обратном лексикографическом порядке относительно @c bin_pred.
    */
    template <class BidirectionalSequence, class BinaryPredicate = std::less<>>
    bool prev_permutation(BidirectionalSequence && seq,
                          BinaryPredicate bin_pred = BinaryPredicate())
    {
        return std::prev_permutation(grabin::begin(seq), grabin::end(seq), std::move(bin_pred));
    }
}
// namespace v1
}
// namespace grabin

#endif
// Z_GRABIN_ALGORITHM_HPP_INCLUDED
