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
    }
    // namespace detail
    /// @endcond

    /** @brief Необязательный объект
    @tparam T Тип объекта, который может содержаться в <tt>optional<T></tt>
    @todo Проверить спецификации constexpr
    @todo стратегия проверок
    @todo монадический интерфейс
    @todo Синтаксис optional_value = {}
    @todo constexpr
    */
    template <class T>
    class optional
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
        optional() noexcept
         : has_value_(false)
        {}

        optional(nullopt_t) noexcept
         : optional()
        {}
        //@}

        // @todo Конструктор копий и перемещения

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

        // @todo Реализовать все конструкторы

        /// @brief Деструктор
        ~optional()
        {
            this->reset();
        }

        // @todo Реализовать Присваивание

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

            this->reset();

            new(std::addressof(this->value_))T(std::forward<Args>(args)...);
            this->has_value_ = true;

            return *(*this);
        }

        // @todo Реализовать Обмен

        // @todo Реализовать все Немодифицирующие операции
        //@{
        /** @brief Содержит ли <tt>*this</tt> значение
        @return @b true тогда и только тогда, когда <tt>*this</tt> содержит значение
        */
        bool has_value() const noexcept
        {
            return this->has_value_;
        }

        explicit operator bool() const noexcept
        {
            return this->has_value();
        }
        //@}

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

            return this->value_;
        }

        T & operator*() &
        {
            // @todo Проверка через стратегию
            assert(this->has_value());

            return this->value_;
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
            if(!this->has_value())
            {
                throw grabin::bad_optional_access{};
            }
            else
            {
                return *(*this);
            }
        }

        T & value() &
        {
            if(!this->has_value())
            {
                throw grabin::bad_optional_access{};
            }
            else
            {
                return *(*this);
            }
        }
        //@}

        // Модифицирующие операции
        /** @brief Если <tt>*this</tt> содержит значение, то вызывает для него деструктор, в
        противном случае ничего не делает.
        @post <tt>this->has_value() == false</tt>
        */
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

    // @todo Реализовать прочую функциональность
}
// namespace v1
}
// namespace grabin

#endif
// Z_GRABIN_OPTIONAL_HPP_INCLUDED
