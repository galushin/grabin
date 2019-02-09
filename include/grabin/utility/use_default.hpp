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

#ifndef Z_GRABIN_UTILITY_USE_DEFAULT_HPP_INCLUDED
#define Z_GRABIN_UTILITY_USE_DEFAULT_HPP_INCLUDED

/** @file grabin/utility/use_default.hpp
 @brief Вспомогательный тип, используемый для обозначения "использовать тип по-умолчанию"
*/

namespace grabin
{
inline namespace v1
{
    /// @brief Вспомогательный тип-тэг "использовать тип по-умолчанию"
    struct use_default {};

    template <class T, class Default>
    using replace_use_default_t = std::conditional_t<std::is_same<T, use_default>::value, Default, T>;
}
// namespace v1
}
// namespace grabin



#endif
// Z_GRABIN_UTILITY_USE_DEFAULT_HPP_INCLUDED
