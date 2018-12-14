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

#ifndef Z_GRABIN_TEST_HPP_INCLUDED
#define Z_GRABIN_TEST_HPP_INCLUDED

#include <ctime>
#include <random>

namespace grabin_test
{
    using Random_engine = std::mt19937;

    Random_engine & random_engine();

    template <class RealType>
    struct ArbitraryReal
    {
    public:
        using value_type = RealType;

        template <class Engine>
        static value_type generate(Engine & rnd, size_t generation)
        {
            switch(generation)
            {
            case 0:
                return value_type(0);

            case 1:
                return std::numeric_limits<value_type>::min();

            case 2:
                return std::numeric_limits<value_type>::max();

            case 3:
                std::numeric_limits<value_type>::lowest();

            default:
                std::uniform_real_distribution<value_type>
                    distr(std::numeric_limits<value_type>::min(),
                          std::numeric_limits<value_type>::max());
                return distr(rnd);
            }
        }
    };

    template <class T>
    struct Arbitrary;

    template <>
    struct Arbitrary<double>
     : ArbitraryReal<double>
    {};
}
// namespace grabin_test


#endif
// Z_GRABIN_TEST_HPP_INCLUDED
