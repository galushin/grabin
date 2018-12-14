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

#ifndef Z_GRABIN_STATISTICS_MEAN_HPP_INCLUDED
#define Z_GRABIN_STATISTICS_MEAN_HPP_INCLUDED

// @todo Документация

#include <cstdint>
#include <type_traits>

namespace grabin
{
inline namespace v1
{
namespace statistics
{
    /** @brief Класс-характеристика для определения типа среднего значения
    @tparam T тип элементов
    @tparam W тип весов
    */
    template <class T, class W>
    struct average_type
    {
    private:
        using R_T = decltype(std::declval<T>() * std::declval<W>() / std::declval<W>());
        using R_double = decltype(std::declval<double>() * std::declval<T>() / std::declval<W>());

    public:
        /// @brief Тип среднего значения
        using type = std::conditional_t<std::is_integral<T>::value, R_double, R_T>;
    };

    /** @brief Тип-синоним для типа среднего значения
    @tparam T тип элементов
    @tparam W тип весов
    */
    template <class T, class W>
    using average_type_t = typename average_type<T, W>::type;

    /** @brief Накопитель для вычисления математического ожидания
    @tparam T тип значений, для которых вычисляется среднее
    */
    template <class T, class Count = std::size_t>
    class mean_accumulator
    {
    public:
        // Типы
        /// @brief Тип значений
        using value_type = T;

        /// @brief Тип для представления количества элементов
        using count_type = Count;

        /// @brief Тип для представления среднего значения
        using mean_type = average_type_t<T, Count>;

        // Создание, копирование, уничтожение
        /** @brief Конструктор без аргументов
        @post <tt> this->count() == 0 </tt>
        @post <tt> this->mean() == mean_type(0) </tt>
        */
        mean_accumulator() = default;

        // Свойства
        /** @brief Количество обработанных элементов
        @return Количество обработанных элементов, равное количеству вызовов <tt>operator()</tt>
        */
        count_type const & count() const
        {
            return this->count_;
        }

        /** @brief Среднее значение
        @return Среднее значение обработанных к данному моменту значений
        */
        mean_type const & mean() const
        {
            return this->mean_;
        }

        // Обновление
        /** @brief Обработка нового значения
        @param value новое значение
        @return <tt> *this </tt>
        */
        mean_accumulator & operator()(value_type const & value)
        {
            ++ this->count_;
            this->mean_ += (value - this->mean_) / static_cast<mean_type>(this->count_);
            return *this;
        }

    private:
        count_type count_ = count_type(0);
        mean_type mean_ = T(0);
    };
}
// namespace accumulator
}
// namespace v0
}
// namespace grabin

#endif
// Z_GRABIN_STATISTICS_MEAN_HPP_INCLUDED
