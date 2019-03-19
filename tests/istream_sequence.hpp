#ifndef Z_GRABIN_TEST_ISTREAM_SEQUENCE_HPP_INCLUDED
#define Z_GRABIN_TEST_ISTREAM_SEQUENCE_HPP_INCLUDED

#include <sstream>
#include <iterator>

namespace grabin_test
{
    template <class Value, class Tag = void>
    class istream_sequence
    {
    public:
        // Типы
        /// @brief Тип значения
        using value_type = Value;

        /// @brief Тип итератора
        using iterator = std::istream_iterator<Value>;

        // Создание, копирование, уничтожение
        /** @brief Конструктор на оснвое последовательности, заданной парой
        итераторов
        */
        template <class Iterator>
        istream_sequence(Iterator first, Iterator last)
         : is_(istream_sequence::make_string(std::move(first), std::move(last)))
        {}

        /** @brief Конструктор на оснвое списка ициализации
        @param src список инициализации
        */
        explicit istream_sequence(std::initializer_list<Value> src)
         : istream_sequence(src.begin(), src.end())
        {}

        // Итераторы
        /** @brief Итератор, задающий начало последовательности
        */
        iterator begin()
        {
            return iterator(this->is_);
        }

        /** @brief Итератор, задающий конец последовательности
        */
        iterator end()
        {
            return iterator();
        }

    private:
        template <class Iterator>
        std::string make_string(Iterator first, Iterator last)
        {
            std::ostringstream os;
            for(; first != last; ++ first)
            {
                os << *first << " ";
            }

            return os.str();
        }

        std::istringstream is_;
    };
}
// namespace grabin_test

#endif
// Z_GRABIN_TEST_ISTREAM_SEQUENCE_HPP_INCLUDED
