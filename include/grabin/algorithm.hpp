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
    @param bin_pred бинарный предикат, определяющий равенство элементов. Если
    он не задан явно, то используется оператор "равно"
    @return @b true, если последовательности содержать одинаковое количество элементов и их
    соответствующие элементы равны (в смысле придиката @c bin_pred)
    */
    template <class InputSequence1, class InputSequence2,
              class BinaryPredicate = std::equal_to<>>
    bool equal(InputSequence1 && in1, InputSequence2 && in2,
               BinaryPredicate bin_pred = BinaryPredicate())
    {
        return std::equal(grabin::begin(in1), grabin::end(in1),
                          grabin::begin(in2), grabin::end(in2),
                          std::move(bin_pred));
    }

    /** @brief Проверяет, является ли последовательность сортированной
    @param seq последовательность
    @param cmp функция сравнения. Если не задана явно, то используется оператор "меньше"
    @return @b true, если @c seq отсортирована относительно функции сравнения @c cmp, иначе --
    @b false
    */
    template <class RandomAccessSequence, class Compare = std::less<>>
    bool is_sorted(RandomAccessSequence && seq, Compare cmp = Compare())
    {
        return std::is_sorted(grabin::begin(seq), grabin::end(seq), std::move(cmp));
    }

    /** @brief Сортировка последовательности с лог-линейным в худшем случае временем выполнения
    @param seq последовательность
    @param cmp функция сравнения. Если не задана явно, то используется оператор "меньше"
    @post <tt>is_sorted(seq, cmp)</tt>
    */
    template <class RandomAccessSequence, class Compare = std::less<>>
    void sort(RandomAccessSequence && seq, Compare cmp = Compare())
    {
        std::sort(grabin::begin(seq), grabin::end(seq), std::move(cmp));
    }

    /** @brief Устойчивая сортировка последовательности
    @param seq последовательность
    @param cmp функция сравнения. Если не задана явно, то используется оператор "меньше"
    @post <tt>is_sorted(seq, cmp)</tt>
    @post Относительный порядок эквивалентных элементов сохраняется

    Сложность составляет <tt>N*log(N)*log(N)</tt>, где <tt>N = size(seq)</tt>. Если доступно
    достаточно дополнительной памяти, то сложность составляет <tt>N*log(N)</tt>
    */
    template <class RandomAccessSequence, class Compare = std::less<>>
    void stable_sort(RandomAccessSequence && seq, Compare cmp = Compare())
    {
        return std::stable_sort(grabin::begin(seq), grabin::end(seq), std::move(cmp));
    }

    /** @brief Проверяет, если ли элемент, эквивалентный @c value, в сортированной
    последовательности @c seq за логарифмическое время
    @param seq последовательность
    @param value значение
    @param cmp функция сравнения. Если не задана явно, то используется оператор "меньше"
    @pre @c seq разделена элементом @c value относительно функции сравнения @c cmp
    @return @b true, если @c seq содержит элемент, эквивалентный @c value, иначе -- @b false.
    */
    template <class RandomAccessSequence, class T, class Compare = std::less<>>
    bool binary_search(RandomAccessSequence && seq, T const & value, Compare cmp = Compare())
    {
        return binary_search(grabin::begin(seq), grabin::end(seq), value, std::move(cmp));
    }

    /** @brief Проверка того, что одна сортированная последовательность включает другую как
    подмножество
    @param in1 первая последовательность
    @param in2 вторая последовательность
    @param cmp функция сравнения. Если не задана явно, то используется оператор "меньше"
    @pre <tt>is_sorted(in1, cmp)</tt>
    @pre <tt>is_sorted(in2, cmp)</tt>
    @return @b true, если @c in2 является подмножеством @c in1, иначе -- @b false
    */
    template <class InputSequence1, class InputSequence2, class Compare = std::less<>>
    bool includes(InputSequence1 && in1, InputSequence2 && in2, Compare cmp = Compare())
    {
        return std::includes(grabin::begin(in1), grabin::end(in1),
                             grabin::begin(in2), grabin::end(in2), std::move(cmp));
    }

    /** @brief Проверяет, является ли @c seq максимальной бинарной кучей
    @param seq последовательность
    @param cmp функция сравнения. Если не задана явно, то используется оператор
    "меньше"
    @return @b true, если @c seq является максимальной бинарной кучей относительно функции
    сравнения @c cmp, иначе -- @b false
    */
    template <class RandomAccessSequence, class Compare = std::less<>>
    bool is_heap(RandomAccessSequence && seq, Compare cmp = Compare())
    {
        return std::is_heap(grabin::begin(seq), grabin::end(seq), std::move(cmp));
    }

    /** @brief Превращает @c seq в максимальную бинарную кучу
    @param seq последовательность
    @param cmp функция сравнения. Если не задана явно, то используется оператор
    "меньше"
    @post @c seq после вызова является перестановкой @c seq до вызова
    @post <tt>grabin::is_heap(seq, cmp)</tt>
    */
    template <class RandomAccessSequence, class Compare = std::less<>>
    void make_heap(RandomAccessSequence && seq, Compare cmp = Compare())
    {
        return std::make_heap(grabin::begin(seq), grabin::end(seq), std::move(cmp));
    }

    /** @brief Вставка нового элемента в бинарную кучу
    @param seq последовательность
    @param cmp функция сравнения. Если не задана явно, то используется оператор
    "меньше"
    @pre Последовательность элементов кроме последнего является максимальной
    бинарной кучей
    @post @c seq после вызова является перестановкой @c seq до вызова
    @post <tt>grabin::is_heap(seq, cmp)</tt>
    */
    template <class RandomAccessSequence, class Compare = std::less<>>
    void push_heap(RandomAccessSequence && seq, Compare cmp = Compare())
    {
        return std::push_heap(grabin::begin(seq), grabin::end(seq), std::move(cmp));
    }

    /** @brief Удаление наибольшего элемента из максимальной бинарной кучи
    @param seq последовательность
    @param cmp функция сравнения. Если не задана явно, то используется оператор
    "меньше"
    @pre <tt>grabin::is_heap(seq, cmp)</tt>
    @pre @c seq не должна быть пустой
    @post Обменивает наибольший элемент кучи и её последний элемент,
    последовательность элементов кроме последнего остаётся максимальной бинарной
    кучей
    */
    template <class RandomAccessSequence, class Compare = std::less<>>
    void pop_heap(RandomAccessSequence && seq, Compare cmp = Compare())
    {
        return std::pop_heap(grabin::begin(seq), grabin::end(seq), std::move(cmp));
    }

    /** @brief Сортировка максимальной бинарной кучи
    @param seq последовательность
    @param cmp функция сравнения. Если не задана явно, то используется оператор
    "меньше"
    @pre <tt>grabin::is_heap(seq, cmp)</tt>
    @post <tt>grabin::is_sorted(seq, cmp)</tt>
    */
    template <class RandomAccessSequence, class Compare = std::less<>>
    void sort_heap(RandomAccessSequence && seq, Compare cmp = Compare())
    {
        return std::sort_heap(grabin::begin(seq), grabin::end(seq), std::move(cmp));
    }

    /** @brief Обрезает значения, выходящие за заданные границы
    @param x значение
    @param low нижняя граница
    @param high верхняя граница
    @param cmp функция сравнения. Если не задана явно, то используется оператор
    "меньше"
    @pre <tt>!cmp(high, low)</tt>
    @return Если <tt>cmp(x, low)</tt>, возвращает @c low, если
    <tt>cmp(high, x)</tt>, возвращает @c high, в остальные случаях -- @c x
    */
    template <class T, class Compare = std::less<>>
    T const & clamp(T const & x, T const & low, T const & high,
                    Compare cmp = Compare())
    {
        if(cmp(x, low))
        {
            return low;
        }
        else if(cmp(high, x))
        {
            return high;
        }
        else
        {
            return x;
        }
    }

    /** @brief Лексикографическое сравнение двух последователостей
    @param in1, in2 сравниваемые последовательности
    @param cmp функция сравнения. Если не задана явно, то используется оператор
    "меньше"
    @return @b true, если @c in1 лексикографически предшествует @c in2, иначе --
    @b false.
    */
    template <class InputSequence1, class InputSequence2,
              class Compare = std::less<>>
    bool lexicographical_compare(InputSequence1 && in1, InputSequence2 && in2,
                                 Compare cmp = Compare())
    {
        return std::lexicographical_compare(grabin::begin(in1), grabin::end(in1),
                                            grabin::begin(in2), grabin::end(in2),
                                            std::move(cmp));
    }

    /** @brief Проверяет, что две последовательности являются перестановками друг друга
    @param in1, in2 последовательности
    @param bin_pred бинарный предикат, задающий отношение порядка
    @return @b true, если @c in1 и @c in2 являются перестановками друг друга, иначе -- @b false
    */
    template <class ForwardSequence1, class ForwardSequence2, class BinaryPredicate = std::equal_to<>>
    bool is_permutation(ForwardSequence1 && in1, ForwardSequence2 && in2,
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
