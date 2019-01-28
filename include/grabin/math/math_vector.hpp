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

#ifndef Z_GRABIN_MATH_MATH_VECTOR_HPP_INCLUDED
#define Z_GRABIN_MATH_MATH_VECTOR_HPP_INCLUDED

/** @file grabin/math/math_vector.hpp
 @brief Класс для представления математической концепции вектора с линейными
 операциями.
*/

#include <cassert>
#include <cstddef>
#include <vector>

namespace grabin
{
inline namespace v1
{
    /** @brief Математический вектор
    @tparam T тип элементов
    @todo Подумать, нужен ли конструктор по умолчанию?
    */
    template <class T>
    class math_vector
    {
        using Container = std::vector<T>;

    public:
        // Типы
        /// @brief Тип элементов
        using value_type = T;

        /// @brief Тип для представления размера и индексов
        using size_type = typename Container::difference_type;

        /// @brief Тип неконстантного итератора
        using iterator = typename Container::iterator;

        /// @brief Тип константного итератора
        using const_iterator = typename Container::const_iterator;

        // Создание, копирование, уничтожение
        /** @brief Конструктор с явным указанием размерности
        @param dim размерность вектора
        @post <tt> this->dim() == dim </tt>
        */
        explicit math_vector(size_type dim)
         : data_(dim)
        {}

        /** @brief Конструктор на основе интервала значений
        @param values интервал значений
        @post <tt> this->dim() == (values.end() - values.begin())</tt>
        @post Элементы <tt>*this</tt> равны соответствующим элементам @c values
        @todo Использовать begin/end, не являющиеся функциями-членами
        @todo Не допускать пустых контейнеров?
        */
        template <class Range>
        explicit math_vector(Range const & values)
         : data_(values.begin(), values.end())
        {}

        // Размер
        // @todo Свободная фукнция dim?
        /** @brief Размерность вектора
        @return Размерность вектора, заданная конструктором или последним
        присваиванием
        */
        size_type dim() const
        {
            return this->data_.size();
        }

        // Итераторы
        // @todo Упрощение определения пар константных/неконстантных операций
        //@{
        /// @brief Итератор начала последовательности элементов
        iterator begin()
        {
            return this->data_.begin();
        }

        const_iterator begin() const
        {
            return this->data_.begin();
        }
        //@}

        //@{
        /// @brief Итератор конца последовательности элементов
        iterator end()
        {
            return this->data_.end();
        }

        const_iterator end() const
        {
            return this->data_.end();
        }
        //@}

        // @todo cbegin/cend

        // Линейные операции
        /** @brief Умножение вектора на скаляр
        @param a скаляр
        @return <tt> *this </tt>
        @post Каждый элемент <tt>*this</tt> умножается на @c a
        */
        math_vector & operator*=(value_type const & a)
        {
            for(auto & elem : *this)
            {
                elem *= a;
            }
            return *this;
        }

        /** @brief Прибавление вектора
        @param x вектор
        @pre <tt>x.dim() == this->dim()</tt>
        @return <tt>*this</tt>
        @post К каждому элементу <tt>*this</tt> прибавляется соответствующий
        элемент @c x
        */
        math_vector & operator+=(math_vector const & x)
        {
            // @todo Проверять за счёт политики проверки
            assert(x.dim() == this->dim());

            for(auto i = this->begin(); i != this->end(); ++ i)
            {
                *i += *(x.begin() + (i - this->begin()));
            }

            return *this;
        }

    private:
        Container data_;
    };

    // Равенство и неравенство
    // @todo Автоматизация определения оператора ==
    /** @brief Оператор "равно"
    @param x,y аргументы
    @return <tt> std::equal(x.begin(), x.end(), y.begin(), y.end()) </tt>
    */
    template <class T>
    bool operator==(math_vector<T> const & x, math_vector<T> const & y)
    {
        return std::equal(x.begin(), x.end(), y.begin(), y.end());
    }

    // @todo Автоматизация определения оператора !=
    /** @brief Оператор "не равно"
    @param x,y аргументы
    @return <tt> !(x == y) </tt>
    */
    template <class T>
    bool operator!=(math_vector<T> const & x, math_vector<T> const & y)
    {
        return !(x == y);
    }

    // Линейные операции
    //@{
    /** @brief Умножение вектора на скаляр
    @param x вектор
    @param a скаляр
    @return Вектор, размерность которого равна размерности @c x, а элементы
    равны соответствующим элементам вектора @c x, умноженным на скаляр @c a
    @todo Смешанные типы аргументов
    @todo Автоматизация определения этого оператора
    */
    template <class T>
    math_vector<T>
    operator*(math_vector<T> x,
              typename math_vector<T>::value_type const & a)
    {
        x *= a;
        return x;
    }

    template <class T>
    math_vector<T>
    operator*(typename math_vector<T>::value_type const & a,
              math_vector<T> const & x)
    {
        // @todo Что если умножение скаляров не коммутативно
        return x * a;
    }
    //@}

    /** @brief Оператор сложения двух векторов
    @param x, y операнды
    @pre <tt>x.dim() == y.dim()</tt>
    @todo Автоматизация определения этого оператора
    @todo Оптимизация для случаев, когда один из аргументов является rvalue
    @return Вектор, размерность которого равна размерности операндов, а элементы
    равны сумме соответствующих элементов операндов.
    */
    template <class T>
    math_vector<T>
    operator+(math_vector<T> x, math_vector<T> const & y)
    {
        x += y;
        return x;
    }
}
// namespace v1
}
// namespace grabin


#endif
// Z_GRABIN_MATH_MATH_VECTOR_HPP_INCLUDED
