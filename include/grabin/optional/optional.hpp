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

#ifndef Z_GRABIN_OPTIONAL_HPP_INCLUDED
#define Z_GRABIN_OPTIONAL_HPP_INCLUDED

/** @file grabin/optional/optional.hpp
 @brief Необязательный объект
 @todo Можно ли вы разить через < все остальные операторы неравенства (в том числе -- гетерогенные)?
*/

#include <grabin/detail/config.hpp>
#include <grabin/utility/in_place.hpp>

#include <cassert>
#include <exception>
#include <type_traits>
#include <utility>

namespace grabin
{
inline namespace v1
{
    /// @brief Тип-тэг для обозначения того, что optional должен быть создан без значения
    struct nullopt_t
    {
        struct tag{};

        nullopt_t() = delete;

        constexpr explicit nullopt_t(tag)
        {}
    };

#if GRABIN_HAS_INLINE_VARIABLES()
// @todo Реализовать для этого случая
#else
    namespace detail
    {
        template <class>
        struct nullopt_holder
        {
            static constexpr nullopt_t nullopt{nullopt_t::tag{}};
        };

        template <class T>
        constexpr nullopt_t nullopt_holder<T>::nullopt;
    }
    // namespace detail

    namespace
    {
        constexpr auto const & nullopt = detail::nullopt_holder<void>::nullopt;
    }
#endif

    /** @brief Класс исключения, порождаемого, когда производится попытка получить доступ к значению
    объекта optional, который не содержит значения
    */
    class bad_optional_access
     : public std::exception
    {
    private:
        virtual char const * what() const noexcept override
        {
            return "Accessing grabin::optional, which has no value";
        }
    };

    /// @cond false
    namespace detail
    {
        struct optional_dummy_type
        {};

        template <class T>
        class optional_storage
        {
        public:
            // Конструкторы
            optional_storage() noexcept
             : has_value_(false)
            {}

            optional_storage(optional_storage const & x)
             : optional_storage()
            {
                if(auto * p = x.get_pointer())
                {
                    this->emplace(*p);
                }
            }

            optional_storage(optional_storage && rhs) noexcept(std::is_nothrow_move_constructible<T>::value)
             : optional_storage()
            {
                if(auto * p = rhs.get_pointer())
                {
                    this->emplace(std::move(*p));
                }
            }

            // Деструктор
            ~optional_storage()
            {
                // @todo Если деструктор тривиальный, то ничего не делать
                this->reset();
            }

            // Присваивание
            optional_storage & operator=(optional_storage const &);
            optional_storage & operator=(optional_storage &&);

            template <class... Args>
            T & emplace(Args && ... args)
            {
                this->reset();

                new(std::addressof(this->value_))T(std::forward<Args>(args)...);
                this->has_value_ = true;

                return *(this->get_pointer());
            }

            // Немодифицирующие операции
            bool has_value() const noexcept
            {
                return this->has_value_;
            }

            T const * get_pointer() const
            {
                return this->has_value() ? std::addressof(this->value_) : nullptr;
            }

            T * get_pointer()
            {
                return this->has_value() ? std::addressof(this->value_) : nullptr;
            }

            // Модифицирующие операции
            void reset() noexcept
            {
                // @todo Не вызывать деструктор, если он тривиальный
                if(this->has_value_)
                {
                    this->value_.T::~T();
                    this->has_value_ = false;
                }
            }

        private:
            bool has_value_ = false;

            union
            {
                nullopt_t::tag dummy_;
                std::remove_const_t<T> value_;
            };
        };

        template <bool DeleteCopyCtor, bool DeleteMoveCtor>
        struct copy_move_ctor_deleter;

        // @todo Покрыть тесами так, чтобы требовались все 4 специализации

        template <>
        struct copy_move_ctor_deleter<false, false>
        {
            copy_move_ctor_deleter() = default;
            copy_move_ctor_deleter(copy_move_ctor_deleter const & ) = default;
            copy_move_ctor_deleter(copy_move_ctor_deleter && ) noexcept = default;
        };

        template <>
        struct copy_move_ctor_deleter<true, false>
        {
            copy_move_ctor_deleter() = default;
            copy_move_ctor_deleter(copy_move_ctor_deleter const & ) = delete;
            copy_move_ctor_deleter(copy_move_ctor_deleter && ) noexcept = default;
        };

        template <>
        struct copy_move_ctor_deleter<true, true>
        {
            copy_move_ctor_deleter() = default;
            copy_move_ctor_deleter(copy_move_ctor_deleter const & ) = delete;
            copy_move_ctor_deleter(copy_move_ctor_deleter && ) noexcept = delete;
        };

        template <class T>
        using copy_move_ctor_deleter_for
            = copy_move_ctor_deleter<!std::is_copy_constructible<T>::value,
                                     !std::is_move_constructible<T>::value>;
    }
    // namespace detail
    /// @endcond

    /** @brief Необязательный объект
    @tparam T Тип объекта, который может содержаться в <tt>optional<T></tt>
    @todo Проверить спецификации constexpr
    @todo стратегия проверок
    @todo монадический интерфейс
    @todo Синтаксис optional_value = {}
    @todo Убедиться, что размер optional соответствует размеру struct {bool, T};
    */
    template <class T>
    class optional
     : private detail::copy_move_ctor_deleter_for<T>
    {
    public:
        // Типы
        /// @brief Тип объекта, который может содержаться в optional
        using value_type = T;

        // Конструкторы
        /** @brief Создание объекта, не содержащего значения
        @post <tt>this->has_value() == false</tt>
        */
        //@{
        optional() noexcept = default;

        optional(nullopt_t) noexcept
         : optional()
        {}
        //@}

        /** @brief Конструктор копий
        @param rhs копируемый объект
        Если @c rhs содержит значение, то инициализирует значение <tt>*this</tt> копией этого
        значения.
        @post <tt>this->has_value() == rhs.has_value()</tt>
        @throw Любые исключения, порождаемые выбранным конструктором типа @c T
        */
        optional(optional const & rhs) = default;

        /** @brief Конструктор перемещения. Если @c rhs содержит значение, то инициализирует
        значение <tt>*this</tt> выражением <tt>std::move(*rhs)</tt>
        @param rhs перемещаемый объект
        @post <tt>this->has_value() == rhs.has_value()</tt>
        @throw Любые исключения, порождаемые выбранным конструктором типа @c T
        @todo Исключать из перегрузки, если не is_move_constructible_v<T>
        */
        optional(optional && rhs) = default;

        /** @brief Инициализирует хранимое значение как объект типа @c T аргументами
        <tt>std::forward<Args>(args)...</tt>
        @post <tt>this->has_value() == true</tt>
        @post <tt>this->value() == T(std::forward<Args>(args)...)</tt>
        */
        template <class... Args, class = std::enable_if_t<std::is_constructible<T, Args...>::value>>
        explicit optional(in_place_t, Args&& ... args)
         : optional()
        {
            this->emplace(std::forward<Args>(args)...);
        }

        // @todo Конструктор со списком инициализации и дополнительными аргументами
        // @todo Конструктор с одним значением
        // @todo Конструктор на основе optional с другим типом
        // @todo Конструктор на основе временного optional с другим типом

        /// @brief Деструктор
        ~optional() = default;

        // Присваивание
        /** @brief Если <tt>*this</tt> не содержит значения, то ничего не делает, иначе уничтожает
        хранимое значение
        @post <tt>this->has_value() == false</tt>
        @return <tt>*this</tt>
        */
        optional & operator=(nullopt_t ) noexcept
        {
            this->reset();
            return *this;
        }

        // @todo Реализовать
        optional & operator=(optional const & x);

        // @todo Реализовать (покрыть тестом, в том числе noexcept)
        optional & operator=(optional && x);

        // @todo Присваивание значения
        // @todo Присваивание optional с другим типом
        // @todo Присваивание временного optional с другим типом

        /** @brief Уничтожает текущее значение (если оно имеется), а затем создаёт новое значение
        с помощью вызова конструктора <tt>T(std::forward<Args>(args)...)</tt>.

        Если во время выполнение конструктора @c T возникло исключение, то <tt>*this</tt> не
        содержит значения, а предыдущее значение (если оно имелось) уничтожается.
        Требует <tt>std::is_constructible<T, Args...>::value == true</tt>.

        @post <tt>this->has_value() == true</tt>
        @post <tt>this->value() == T(std::forward<Args>(args)...)</tt>
        @return Ссылка на вновь созданное значение
        @throw Любые исключения, порождаемые выбранным конструктором
        */
        template <class... Args>
        T & emplace(Args &&... args)
        {
            static_assert(std::is_constructible<T, Args...>::value, "");

            return this->storage_.emplace(std::forward<Args>(args)...);
        }

        // @todo emplace со списком инициализации и дополнительными аргументами

        // @todo Реализовать Обмен

        // @todo Реализовать все Немодифицирующие операции
        //@{
        /** @brief Нестандартная функция доступа к указателю на хранимое значение (если оно имеется)
        @return Если <tt>*this</tt> содержит значение, то возвращает указатель на это значение, в
        противном случае -- @c nullptr.
        */
        T const * get_pointer() const
        {
            return this->storage_.get_pointer();
        }

        T * get_pointer()
        {
            return this->storage_.get_pointer();
        }
        //@}

        // @todo operator->

        //@{
        /** @brief Доступ к значению без проверки его наличия
        @return <tt>*(*this)</tt>
        @throw bad_optional_access, если <tt>this->has_value() == false</tt>
        @todo Покрыть тестами, требующими constexpr
        @todo Другие перегрузки этого оператора
        @todo Унифицировать перегрузки
        */
        T const & operator*() const &
        {
            // @todo Проверка через стратегию
            assert(this->has_value());

            return *(this->get_pointer());
        }

        T & operator*() &
        {
            // @todo Проверка через стратегию
            assert(this->has_value());

            return *(this->get_pointer());
        }
        //@}

        //@{
        /** @brief Содержит ли <tt>*this</tt> значение
        @return @b true тогда и только тогда, когда <tt>*this</tt> содержит значение
        */
        explicit operator bool() const noexcept
        {
            return this->has_value();
        }

        bool has_value() const noexcept
        {
            return this->storage_.has_value();
        }
        //@}

        //@{
        /** @brief Доступ к значению с проверкой его наличия
        @return <tt>*(*this)</tt>
        @throw bad_optional_access, если <tt>this->has_value() == false</tt>
        @todo Покрыть тестами, требующими constexpr
        @todo Другие перегрузки value
        @todo Унифицировать перегрузки
        */
        T const & value() const &
        {
            if(auto * p = this->get_pointer())
            {
                return *p;
            }
            else
            {
                throw grabin::bad_optional_access{};
            }
        }

        T & value() &
        {
            if(auto * p = this->get_pointer())
            {
                return *p;
            }
            else
            {
                throw grabin::bad_optional_access{};
            }
        }
        //@}

        // @todo value_or

        // Модифицирующие операции
        /** @brief Если <tt>*this</tt> содержит значение, то вызывает для него деструктор, в
        противном случае ничего не делает.
        @post <tt>this->has_value() == false</tt>
        */
        void reset() noexcept
        {
            this->storage_.reset();
        }

    private:
        detail::optional_storage<T> storage_;
    };

    // @todo Указание для вывода шаблонного параметра из конструктора

    // Сравнение с nullopt
    //@{
    /** @brief Проверка на равенство с @c nullopt_t
    @param obj объект, сравниваемый с nullopt
    @return <tt>!obj->has_value()</tt>
    */
    template <class T>
    bool operator==(optional<T> const & obj, nullopt_t) noexcept
    {
        return !obj.has_value();
    }

    template <class T>
    bool operator==(nullopt_t, optional<T> const & obj) noexcept
    {
        return !obj.has_value();
    }
    //@}

    //@{
    /** @brief Проверка на несовпадение с @c nullopt_t
    @param obj объект, сравниваемый с nullopt
    @return <tt>obj->has_value()</tt>
    @todo Автоматическое определение
    */
    template <class T>
    bool operator!=(optional<T> const & obj, nullopt_t) noexcept
    {
        return obj.has_value();
    }

    template <class T>
    bool operator!=(nullopt_t, optional<T> const & obj) noexcept
    {
        return obj.has_value();
    }
    //@}

    /** @brief Оператор "меньше" для @c optional и @c nullopt_t
    @return @b false
    */
    template <class T>
    bool operator<(optional<T> const &, nullopt_t) noexcept
    {
        return false;
    }

    /** @brief Оператор меньше для @c nullopt_t и @c optional
    @param obj объект, сравниваемый с @c nullopt
    @return <tt>obj.has_value()</tt>
    */
    template <class T>
    bool operator<(nullopt_t, optional<T> const & obj) noexcept
    {
        return obj.has_value();
    }

    /** @brief Оператор "меньше или равно" для @c optional и @c nullopt_t
    @param obj объект, сравниваемый с @c nullopt
    @return <tt>!obj.has_value()</tt>
    */
    template <class T>
    bool operator<=(optional<T> const & obj, nullopt_t) noexcept
    {
        return !obj.has_value();
    }

    /** @brief Оператор "меньше или равно" для @c nullopt_t и @c optional
    @return @b true
    */
    template <class T>
    bool operator<=(nullopt_t, optional<T> const &) noexcept
    {
        return true;
    }

    /** @brief Оператор "больше" для @c optional и @c nullopt_t
    @param obj объект, сравниваемый с @c nullopt
    @return <tt>obj.has_value()</tt>
    */
    template <class T>
    bool operator>(optional<T> const & obj, nullopt_t) noexcept
    {
        return obj.has_value();
    }

    /** @brief Оператор "больше" для @c nullopt_t и @c optional
    @return @b false
    */
    template <class T>
    bool operator>(nullopt_t, optional<T> const &) noexcept
    {
        return false;
    }

    /** @brief Оператор "больше или равно" для @c optional и @c nullopt_t
    @return @b true
    */
    template <class T>
    bool operator>=(optional<T> const &, nullopt_t) noexcept
    {
        return true;
    }

    /** @brief Оператор "больше или равно" для @c nullopt_t и @c optional
    @param obj объект, сравниваемый с @c nullopt
    @return <tt>!obj.has_value()</tt>
    */
    template <class T>
    bool operator>=(nullopt_t, optional<T> const & obj) noexcept
    {
        return !obj.has_value();
    }

    // Сравнение с объектом
    /** @brief Сравнение @c optional и значения
    @param lhs объект с необязательным значением
    @param rhs значение
    @return Эквивалентно <tt>lhs.has_value() ? lhs.value() == rhs : false</tt>
    */
    template <class T, class U>
    bool operator==(optional<T> const & lhs, U const & rhs)
    {
        if(auto * p = lhs.get_pointer())
        {
            return *p == rhs;
        }
        else
        {
            return false;
        }
    }

    /** @brief Сравнение @c optional и значения
    @param lhs значение
    @param rhs объект с необязательным значением
    @return Эквивалентно <tt>rhs.has_value() ? lhs == rhs.value() : false</tt>
    */
    template <class T, class U>
    bool operator==(T const & lhs, optional<U> const & rhs)
    {
        if(auto * p = rhs.get_pointer())
        {
            return lhs == *p;
        }
        else
        {
            return false;
        }
    }

    /** @brief Оператор "не равно" для @c optional и значений
    @param lhs объект с необязательным значением
    @param rhs значение
    @return <tt>!(lhs == rhs)</tt>
    */
    template <class T, class U>
    bool operator!=(optional<T> const & lhs, U const & rhs)
    {
        return !(lhs == rhs);
    }

    /** @brief Оператор "не равно" для значения и @c optional
    @param lhs значение
    @param rhs объект с необязательным значением
    @return <tt>!(lhs == rhs)</tt>
    */
    template <class T, class U>
    bool operator!=(T const & lhs, optional<U> const & rhs)
    {
        return !(lhs == rhs);
    }

    // @todo Оператор "меньше" с значением
    // @todo Оператор "меньше или равно" с значением
    // @todo Оператор "больше" с значением
    // @todo Оператор "больше или равно" с значением

    // Операторы сравнения
    /** @brief Оператор "равно"
    @param lhs, rhs аргументы
    @return Если <tt>lhs.has_value() != rhs.has_value()</tt>, возвращает @b false, в противном
    случае, если <tt>bool(lhs) == false</tt>, возвращает @b true,
    иначе --- <tt>lhs.value() == rhs.value()</tt>
    */
    template <class T, class U>
    bool operator==(optional<T> const & lhs, optional<U> const & rhs)
    {
        if(auto * p = rhs.get_pointer())
        {
            return lhs == *p;
        }
        else
        {
            return lhs == nullopt;
        }
    }

    /** @brief Оператор "не равно"
    @param lhs, rhs аргументы
    @return <tt>!(lhs == rhs)</tt>
    @todo Автоматическое определение
    */
    template <class T, class U>
    bool operator!=(optional<T> const & lhs, optional<U> const & rhs)
    {
        return !(lhs == rhs);
    }

    // @todo Оператор "меньше"
    // @todo Оператор "меньше или равно"
    // @todo Оператор "больше"
    // @todo Оператор "больше или равно"

    // Специализированные алгоритмы
    // @todo swap

    // @todo make_optional - один аргумент
    // @todo make_optional - несколько аргументов
    // @todo make_optional - список инициализации и несколько аргументов

    // @todo Поддержка hash
}
// namespace v1
}
// namespace grabin

#endif
// Z_GRABIN_OPTIONAL_HPP_INCLUDED
