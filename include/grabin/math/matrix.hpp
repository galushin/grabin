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

#ifndef Z_GRABIN_MATH_MATRIX_HPP_INCLUDED
#define Z_GRABIN_MATH_MATRIX_HPP_INCLUDED

/** @file grabin/math/matrix.hpp
 @brief Класс матрицы и связанные с ней операторы/функции
*/

#include <grabin/math/math_vector.hpp>

namespace grabin
{
inline namespace v1
{
    /** @brief Шаблон класса матрицы
    @tparam T тип элементов
    @tparam Check стратегия проверок и обработки ошибок
    */
    template <class T, class Check = grabin::math_vector_throws_check_policy>
    class matrix
    {
        // @todo Может быть использовать менее жёсткую стратегию проверок здесь?
        using Data = grabin::math_vector<T, Check>;

    public:
        // Типы
        /// @brief Тип элементов
        using value_type = T;

        /// @brief Тип для представления количества элементов и индексов
        using size_type = typename Data::size_type;

        /// @brief Тип констатного итератора
        using const_iterator = typename Data::const_iterator;

        /// @brief Стратегия проверок и обработки ошибок
        using check_policy = Check;

        // Создание, копирование, уничтожение
        /** @brief Конструктор без параметров
        @post <tt>this->dim1() == 0</tt>
        @post <tt>this->dim2() == 0</tt>
        @post <tt>this->size() == 0</tt>
        */
        matrix() = default;

        /** @brief Конструктор с указанием размерностей
        @param rows количество строк
        @param cols количество столбцов
        @post <tt> this->dim1() == rows</tt>
        @post <tt> this->dim2() == cols</tt>
        @post <tt> this->size() == rows*cols</tt>
        @post Для @c i из интервала <tt>[0;rows)</tt> и @c j из интервала <tt>[0;cols)</tt>
        выполняется <tt>(*this)(i, j) == 0</tt>
        */
        matrix(size_type rows, size_type cols)
         : data_(rows*cols)
         , rows_(rows)
         , cols_(cols)
        {}

        // Размерность
        /// @brief Количество строк матрицы
        size_type dim1() const
        {
            return this->rows_;
        }

        /// @brief Количество столбцов матрицы
        size_type dim2() const
        {
            return this->cols_;
        }

        /// @brief Количество элементов матрицы
        size_type size() const
        {
            return this->data_.size();
        }

        // Доступ к элементам
        //@{
        /** @brief Доступ к элементам
        @param rows номер строки
        @param cols номер столбца
        @return Ссылка на элемент, расположенный в строке @c row и столбце @c col
        @throw То же, что <tt>check_policy::check_index(*this, row, col)</tt>
        */
        value_type const & operator()(size_type row, size_type col) const
        {
            check_policy::check_index(*this, row, col);
            return this->data_[row*this->cols_ + col];
        }

        value_type & operator()(size_type row, size_type col)
        {
            return const_cast<value_type&>(grabin::as_const(*this)(row, col));
        }
        //@}

        // Итераторы
        /// @brief Итератор начала последовательности элементов
        const_iterator begin() const
        {
            return this->data_.begin();
        }

        /// @brief Итератор конца последовательности элементов
        const_iterator end() const
        {
            return this->data_.end();
        }

    private:
        Data data_;
        size_type rows_ = 0;
        size_type cols_ = 0;
    };

    /** @brief Оператор "равно"
    @param x,y аргументы
    @return <tt> std::equal(x.begin(), x.end(), y.begin(), y.end()) </tt>
    */
    template <class T, class Check>
    bool operator==(matrix<T, Check> const & x, matrix<T, Check> const & y)
    {
        return std::equal(x.begin(), x.end(), y.begin(), y.end());
    }

    /** @brief Оператор "не равно"
    @param x,y аргументы
    @return <tt> !(x == y) </tt>
    */
    template <class T, class Check>
    bool operator!=(matrix<T, Check> const & x, matrix<T, Check> const & y)
    {
        return !(x == y);
    }
}
// namespace v1
}
// namespace grabin

#endif
// Z_GRABIN_MATH_MATRIX_HPP_INCLUDED
