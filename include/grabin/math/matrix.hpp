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
     : grabin::operators::container_equality::enable_adl
    {
        // @todo Может быть использовать менее жёсткую стратегию проверок здесь?
        using Data = grabin::math_vector<T, Check>;

    public:
        // Типы
        /// @brief Тип элементов
        using value_type = T;

        /// @brief Тип для представления количества элементов и индексов
        using size_type = typename Data::size_type;

        /// @brief Тип неконстатного итератора
        using iterator = typename Data::iterator;

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

        /** @brief Размерности матрицы
        @return <tt>make_pair(this->dim1(), this->dim2())</tt>
        */
        std::pair<size_type, size_type> dim() const
        {
            return {this->dim1(), this->dim2()};
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

        const_iterator cbegin() const
        {
            return this->begin();
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

        const_iterator cend() const
        {
            return this->end();
        }
        //@}

        // Линейные операции
        /** @brief Умножение матрицы на скаляр
        @param a скаляр
        @post Каждый элементы <tt>*this</tt> умножается на @c y
        @return <tt>*this</tt>
        */
        matrix & operator*=(value_type const & a)
        {
            this->data_ *= a;
            return *this;
        }

        /** @brief Прибавление матрицы
        @param x вектор
        @pre <tt>x.dim() == this->dim()</tt>
        @return <tt>*this</tt>
        @post К каждому элементу <tt>*this</tt> прибавляется соответствующий
        элемент @c x
        @throws То же, что <tt>check_policy::ensure_equal_dimensions(*this, x)</tt>
        */
        matrix & operator+=(matrix const & x)
        {
            check_policy::ensure_equal_dimensions(*this, x);

            auto dest = this->begin();
            auto src = x.begin();
            auto const src_last = x.end();

            for(; src != src_last; ++src, ++ dest)
            {
                *dest += *src;
            }

            return *this;
        }

    private:
        Data data_;
        size_type rows_ = 0;
        size_type cols_ = 0;
    };

    // Линейные операции
    //@{
    /** @brief Умножение матрицы на скаляр
    @param x матрица
    @param a скаляр
    @return Матрица, размерности которой равны размерностям @c x, а элементы
    равны соответствующим элементам @c x, умноженным на скаляр @c a.
    */
    template <class T, class Check>
    matrix<T, Check>
    operator*(matrix<T, Check> x,
              typename matrix<T, Check>::value_type const & a)
    {
        x *= a;
        return x;
    }

    template <class T, class Check>
    matrix<T, Check>
    operator*(typename matrix<T, Check>::value_type const & a,
              matrix<T, Check> const & x)
    {
        return x * a;
    }
    //@}

    /** @brief Оператор сложения двух матриц
    @param x, y слагаемые
    @pre <tt>x.dim1() == y.dim1()</tt>
    @pre <tt>x.dim2() == y.dim2()</tt>
    @return Матрица, размерности которой равны размерностям слагаемых, а
    элементы равны сумме соответствующих элементов слагаемых.
    @throw То же, что <tt> Check::ensure_equal_dimensions(*this, x) </tt>
    */
    template <class T, class Check>
    matrix<T, Check>
    operator+(matrix<T, Check> x, matrix<T, Check> const & y)
    {
        x += y;
        return x;
    }
}
// namespace v1
}
// namespace grabin

#endif
// Z_GRABIN_MATH_MATRIX_HPP_INCLUDED
