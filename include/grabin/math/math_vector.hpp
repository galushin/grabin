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

#include <grabin/math/average_type.hpp>
#include <grabin/operators.hpp>
#include <grabin/utility/as_const.hpp>

#include <cassert>
#include <cstddef>
#include <stdexcept>
#include <vector>

namespace grabin
{
inline namespace v1
{
    /** @brief Стратегия проверок для шаблона класса @c math_vector, которая при
    обнаружении ошибок порождает исключения.
    */
    struct math_vector_throws_check_policy
    {
        /** @brief Обеспечение совпадения размерностей
        @param x, y векторы, размерности которых должны совпадать
        @throw std::logic_error, если равенство <tt>x.dim() == y.dim()</tt> не
        выполняется
        */
        template <class T>
        static void ensure_equal_dimensions(T const & x, T const & y)
        {
            if(x.dim() != y.dim())
            {
                throw std::logic_error("Dimensions must be equal");
            }
        }

        /** @brief Обеспечение корректности индекса
        @param x вектор
        @param index индекс
        @throw std::out_of_range, если @c index не принадлежит интервалу
        <tt>[0;x.dim())</tt>
        */
        template <class Vector>
        static void check_index(Vector const & x,
                                typename Vector::size_type index)
        {
            if(index < 0 || x.dim() <= index)
            {
                throw std::out_of_range("Invalid index");
            }
        }

        /** @brief Обеспечение корректности индекса матрицы
        @param x матрица
        @param row номер строки
        @param col номер столбца
        @throw std::out_of_range, если @c row не принадлежит интервалу
        <tt>[0;x.dim1())</tt> или @c col не принадлежит интервалу <tt>[0;x.dim2())</tt>
        */
        template <class Matrix>
        static void check_index(Matrix const & x,
                                typename Matrix::size_type row, typename Matrix::size_type col)
        {
            if(row < 0 || x.dim1() <= row || col < 0 || x.dim2() <= col)
            {
                throw std::out_of_range("Invalid index");
            }
        }

        template <class Scalar>
        static void check_division_by_zero(Scalar const & value)
        {
            // @todo Правильно ли это для числе с плавающей точкой?
            if(value == Scalar(0))
            {
                throw std::logic_error("Division by zero");
            }
        }
    };

    /** @brief Стратегия проверок для шаблона класса @c math_vector, которая не
    выполняется никаких проверок.
    */
    struct math_vector_no_checks_policy
    {
        /** @brief Обеспечение совпадения размерностей
        @param x, y векторы, размерности которых должны совпадать
        */
        template <class T>
        static void ensure_equal_dimensions(T const &, T const &)
        {}

        /** @brief Обеспечение корректности индекса
        @param x вектор
        @param index индекс
        */
        template <class Vector>
        static void check_index(Vector const &, typename Vector::size_type)
        {}

        // @todo Покрыть тестом
        template <class Matrix>
        static void check_index(Matrix const & x,
                                typename Matrix::size_type row, typename Matrix::size_type col);

        template <class Scalar>
        static void check_division_by_zero(Scalar const & value);
    };

    /** @brief Математический вектор
    @tparam T тип элементов
    @tparam CheckPolicy стратегия проверок и обработки ошибок

    Первоночально была идея запретить конструктор без аргументов и вообще
    векторы нулевой размерности. Но тогда возникает вопрос: в каком состоянии
    находится вектор, содержимое которого было перемещено? Так как перемщеение
    является важной современной техникой оптимизации, запрещать его было бы не
    целесообразно. Поэтому было решено добавить конструктор без аругментов,
    создающий вектор нулевой размерности.
    */
    template <class T, class CheckPolicy = math_vector_throws_check_policy>
    class math_vector
     : grabin::operators::container_equality::enable_adl
    {
        using Container = std::vector<T>;

    public:
        // Типы
        /// @brief Тип элементов
        using value_type = T;

        /// @brief Тип для представления размера и индексов
        using size_type = typename Container::difference_type;

        /// @brief Тип для представления разности итераторов
        using difference_type = typename Container::difference_type;

        /// @brief Тип неконстантного итератора
        using iterator = typename Container::iterator;

        /// @brief Тип константного итератора
        using const_iterator = typename Container::const_iterator;

        /// @brief Стратегия проверок и обработки ошибок
        using check_policy = CheckPolicy;

        // Создание, копирование, уничтожение
        /** @brief Конструктор по-умолчанию
        @brief <tt>this->dim() == 0</tt>
        */
        math_vector() = default;

        /** @brief Конструктор с явным указанием размерности
        @param dim размерность вектора
        @post <tt> this->dim() == dim </tt>
        @post Все элементы <tt>*this</tt> равны <tt>value_type()</tt>
        */
        explicit math_vector(size_type dim)
         : data_(dim)
        {}

        /** @brief Конструктор с явным указанием размерности и значения
        элементов
        @param dim размерность вектора
        @post <tt> this->dim() == dim </tt>
        @post Все элементы <tt>*this</tt> равны @c value
        */
        math_vector(size_type dim, value_type const & value)
         : data_(dim, value)
        {}

        /** @brief Конструктор на основе интервала значений
        @param values интервал значений
        @post <tt> this->dim() == (values.end() - values.begin())</tt>
        @post Элементы <tt>*this</tt> равны соответствующим элементам @c values
        */
        template <class Range, class = decltype(std::declval<Range&>().begin())>
        explicit math_vector(Range const & values)
         : data_(values.begin(), values.end())
        {}

        /// @brief Конструктор копий
        math_vector(math_vector const &) = default;
        math_vector(math_vector &&) = default;

        /** @brief Конструктор на основе списка инициализации
        @param values список значений
        @post <tt>this->dim() == values.size()</tt>
        @post Элементы <tt>*this</tt> равны соответствующим элементам @c values
        */
        math_vector(std::initializer_list<value_type> values)
         : data_(values.begin(), values.end())
        {}

        /// @brief Оператор присваивания
        math_vector & operator=(math_vector const &) = default;

        /// @brief Оператор присваивания с перемещением
        math_vector & operator=(math_vector &&) = default;

        // Размер
        //@{
        /** @brief Размерность вектора
        @return Размерность вектора, заданная конструктором или последним
        присваиванием
        */
        size_type dim() const
        {
            return this->size();
        }

        size_type size() const
        {
            return this->data_.size();
        }
        //@}

        // Доступ к данным
        //@{
        /** @brief Индексированный доступ к данным
        @param index индекс элемента
        @return Ссылка на элемент с индексом @c index
        @throw То же, что <tt>check_policy::check_index(*this, index)</tt>
        */
        value_type & operator[](size_type index)
        {
            return const_cast<value_type&>(grabin::as_const(*this)[index]);
        }

        value_type const & operator[](size_type index) const
        {
            check_policy::check_index(*this, index);

            return this->data_[index];
        }
        //@}

        //@{
        /** @brief Индексированный доступ к данным c проверкой индекса
        @param index индекс элемента
        @return Ссылка на элемент с индексом @c index
        std::out_of_range, если @c index не принадлежит интервалу
        <tt>[0;x.dim())</tt>
        */
        value_type & at(size_type index)
        {
            return const_cast<value_type&>(grabin::as_const(*this).at(index));
        }

        value_type const & at(size_type index) const
        {
            if(index < 0 || this->dim() <= index)
            {
                throw std::out_of_range("math_vector::at - Invalid index");
            }

            return this->data_[index];
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

        /** @brief Деление вектора на скаляр
        @param a скаляр
        @return <tt> *this </tt>
        @post Каждый элемент <tt>*this</tt> делится на @c a
        */
        math_vector & operator/=(value_type const & a)
        {
            check_policy::check_division_by_zero(a);

            for(auto & elem : *this)
            {
                elem /= a;
            }
            return *this;
        }

        /** @brief Прибавление вектора
        @param x вектор
        @pre <tt>x.dim() == this->dim()</tt>
        @return <tt>*this</tt>
        @post К каждому элементу <tt>*this</tt> прибавляется соответствующий
        элемент @c x
        @throws То же, что <tt>check_policy::ensure_equal_dimensions(*this, x)</tt>
        */
        math_vector & operator+=(math_vector const & x)
        {
            check_policy::ensure_equal_dimensions(*this, x);

            assert(x.dim() == this->dim());

            for(auto i = this->begin(); i != this->end(); ++ i)
            {
                *i += *(x.begin() + (i - this->begin()));
            }

            return *this;
        }

        /** @brief Вычитание вектора
        @param x вектор
        @pre <tt>x.dim() == this->dim()</tt>
        @return <tt>*this</tt>
        @post Из каждого элемента <tt>*this</tt> вычитаются соответствующий
        элемент @c x
        @throws То же, что <tt>check_policy::ensure_equal_dimensions(*this, x)</tt>
        */
        math_vector & operator-=(math_vector const & x)
        {
            check_policy::ensure_equal_dimensions(*this, x);

            assert(x.dim() == this->dim());

            for(auto i = this->begin(); i != this->end(); ++ i)
            {
                *i -= *(x.begin() + (i - this->begin()));
            }

            return *this;
        }

    private:
        Container data_;
    };

    // Линейные операции
    //@{
    /** @brief Умножение вектора на скаляр
    @param x вектор
    @param a скаляр
    @return Вектор, размерность которого равна размерности @c x, а элементы
    равны соответствующим элементам вектора @c x, умноженным на скаляр @c a
    */
    template <class T, class Check>
    math_vector<T, Check>
    operator*(math_vector<T, Check> x,
              typename math_vector<T, Check>::value_type const & a)
    {
        x *= a;
        return x;
    }

    template <class T, class Check>
    math_vector<T, Check>
    operator*(typename math_vector<T, Check>::value_type const & a,
              math_vector<T, Check> const & x)
    {
        return x * a;
    }
    //@}

    /** @brief Деление вектора на скаляр
    @param x вектор
    @param a скаляр
    @pre <tt>a != 0</tt>
    @return Вектор, размерность которого равна размерности @c x, а элементы
    равны соответствующим элементам вектора @c x, делённого на скаляр @c a
    */
    template <class T, class Check>
    math_vector<T, Check>
    operator/(math_vector<T, Check> x,
               typename math_vector<T, Check>::value_type const & a)
    {
        x /= a;
        return x;
    }

    /** @brief Оператор сложения двух векторов
    @param x, y слагаемые
    @pre <tt>x.dim() == y.dim()</tt>
    @return Вектор, размерность которого равна размерности слагаемых, а элементы
    равны сумме соответствующих элементов слагаемых.
    @throw То же, что <tt> Check::ensure_equal_dimensions(*this, x) </tt>
    */
    template <class T, class Check>
    math_vector<T, Check>
    operator+(math_vector<T, Check> x, math_vector<T, Check> const & y)
    {
        x += y;
        return x;
    }

    /** @brief Оператор сложения двух векторов с разными типами элементов
    @param x, y слагаемые
    @pre <tt>x.dim() == y.dim()</tt>
    @return Вектор, размерность которого равна размерности операндов, а элементы
    равны сумме соответствующих элементов @c x и @c y.
    @throw То же, что <tt> Check::ensure_equal_dimensions(*this, x) </tt>
    */
    template <class T1, class T2, class Check>
    auto operator+(math_vector<T1, Check> const & x,
                   math_vector<T2, Check> const & y)
    -> math_vector<decltype(x[0] + y[0]), Check>
    {
        math_vector<decltype(x[0] + y[0]), Check> result(x);

        result += y;

        return result;
    }

    /** @brief Оператор вычитания двух векторов
    @param x уменьшаемое
    @param y вычитаемое
    @pre <tt>x.dim() == y.dim()</tt>
    @return Вектор, размерность которого равна размерности операндов, а элементы
    равны разности соответствующих элементов @c x и @c y.
    @throw То же, что <tt> Check::ensure_equal_dimensions(*this, x) </tt>
    */
    template <class T, class Check>
    math_vector<T, Check>
    operator-(math_vector<T, Check> x, math_vector<T, Check> const & y)
    {
        x -= y;
        return x;
    }

    /** @brief Оператор вычитания двух векторов с разными типами элементов
    @param x уменьшаемое
    @param y вычитаемое
    @pre <tt>x.dim() == y.dim()</tt>
    @return Вектор, размерность которого равна размерности операндов, а элементы
    равны разности соответствующих элементов @c x и @c y.
    @throw То же, что <tt> Check::ensure_equal_dimensions(*this, x) </tt>
    */
    template <class T1, class T2, class Check>
    auto operator-(math_vector<T1, Check> const & x,
                   math_vector<T2, Check> const & y)
    -> math_vector<decltype(x[0] - y[0]), Check>
    {
        math_vector<decltype(x[0] - y[0]), Check> result(x);

        result -= y;

        return result;
    }

    /** @brief Специализация класса-характеристики для определения типа среднего
    для векторов
    @tparam T тип элементов
    @tparam Check тип стратегии проверок
    @tparam W тип весов
    */
    template <class T, class Check, class W>
    struct average_type<math_vector<T, Check>, W>
    {
        /// @brief Тип-результат
        using type = math_vector<grabin::average_type_t<T, W>, Check>;
    };
}
// namespace v1
}
// namespace grabin


#endif
// Z_GRABIN_MATH_MATH_VECTOR_HPP_INCLUDED
