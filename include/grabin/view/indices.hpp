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

#ifndef Z_GRABIN_ITERATOR_COUNTING_ITERATOR_HPP_INCLUDED
#define Z_GRABIN_ITERATOR_COUNTING_ITERATOR_HPP_INCLUDED

/** @file grabin/view/indices.hpp
 @brief Последовательности индексов
*/

#include <grabin/utility/rel_ops.hpp>

#include <utility>

namespace grabin
{
inline namespace v1
{
    /** @brief Шаблон итератора, добавляющий объекту оператор*, возвращающий
    значение самого объекта.
    @tparam Incrementable тип адаптируемого объекта, который должен поддерживать
    все операции итератора, кроме operator*.
    */
    template <class Incrementable>
    class counting_iterator
     : public grabin::rel_ops::enable_adl
    {
    public:
        /** @brief Конструктор
        @param value начальное значение
        @post <tt>*(*this) == value</tt>
        */
        explicit counting_iterator(Incrementable value)
         : value_(std::move(value))
        {}

        /** @brief Оператор перехода к следующему значению
        @post К текущему значению применяется оператор++
        @return *this
        */
        counting_iterator& operator++()
        {
            ++ this->value_;
            return *this;
        }

        /** @brief Текущее значение
        @return Ссылка на текущее значение
        */
        Incrementable const & operator*() const
        {
            return this->value_;
        }

    private:
        Incrementable value_;
    };

    /** @brief Оператор "равно"
    @param x,y аргументы
    @return <tt>*x == *y</tt>
    */
    template <class Incrementable>
    bool operator==(counting_iterator<Incrementable> const & x,
                    counting_iterator<Incrementable> const & y)
    {
        return *x == *y;
    }

namespace view
{
    /** @brief Интервал, итераторы которого добавляют объекту оператор*,
    возвращающий значение самого объекта.
    @tparam Incrementable тип адаптируемого объекта, который должен поддерживать
    все операции итератора, кроме operator*.
    */
    template <class Incrementable>
    class counting_range
    {
    public:
        /// @brief Тип итератора
        using iterator = grabin::counting_iterator<Incrementable>;

        /** @brief Конструктор
        @param first начало интервала
        @param last конец интервала
        @pre @c last должен быть достижим из @c first
        @post <tt>*this->begin() == first</tt>
        @post <tt>*this->end() == last</tt>
        */
        counting_range(Incrementable first, Incrementable last)
         : first_(std::move(first))
         , last_(std::move(last))
        {}

        iterator begin() const
        {
            return iterator(this->first_);
        }

        iterator end() const
        {
            return iterator(this->last_);
        }

    private:
        Incrementable first_;
        Incrementable last_;
    };

    /** @brief Создание интервалов индексов
    @param first начало интервала
    @param last конец интервала
    @pre @c last должен быть достижим из @c first
    @return <tt>counting_range<Incrementable>(std::move(first), std::move(last)</tt>
    */
    template <class Incrementable>
    counting_range<Incrementable>
    indices(Incrementable first, Incrementable last)
    {
        return counting_range<Incrementable>(std::move(first), std::move(last));
    }

    /** @brief Создание интервала индексов от нуля до заданного значения
    @param n предельное значение индекса
    @return <tt>grabin::view::indices<Incrementable>(0, std::move(n))</tt>
    */
    template <class Incrementable>
    counting_range<Incrementable>
    indices(Incrementable n)
    {
        return ::grabin::view::indices<Incrementable>(0, std::move(n));
    }

    /** @brief Создание интервала индексов контейнера известного размера
    @param c контейнер
    @return <tt>counting_range<Difference>(0, c.size())</tt>, где @c Difference
    -- это <tt>typename SizedContainer::difference_type</tt>
    */
    template <class SizedContainer>
    counting_range<typename SizedContainer::difference_type>
    indices_of(SizedContainer const & c)
    {
        using Difference = typename SizedContainer::difference_type;
        return ::grabin::view::indices<Difference>(c.size());
    }
}
// namespace view
}
// namespace v1
}
// namespace grabin

#endif
// Z_GRABIN_ITERATOR_COUNTING_ITERATOR_HPP_INCLUDED
