#ifndef Z_GRABIN_OPTIMIZATION_LOCAL_SEARCH_HPP_INCLUDED
#define Z_GRABIN_OPTIMIZATION_LOCAL_SEARCH_HPP_INCLUDED

#include <utility>

namespace grabin
{
namespace optimization
{
    template <class Point, class ObjectiveValue>
    struct optimization_result
    {
        Point point;
        ObjectiveValue objective_value;
    };

    /** @brief Локальный спуск по первому улучшению для псевдо-булевых функций с шагом равным 1
    @todo Обобщить
    @todo Возможность трассировки (в начале поиска, в конце и на каждой итерации)
    */
    template <class BooleanVector, class Objective>
    auto local_search_boolean(BooleanVector x_current, Objective const & objective)
    -> optimization_result<BooleanVector, decltype(objective(x_current))>
    {
        auto const dim = x_current.size();

        auto y_current = objective(x_current);

        auto failures = 0*dim;
        auto index = 0*dim;

        for(; failures < dim; index = (index + 1) % dim)
        {
            x_current[index] = !x_current[index];

            auto const y_new = objective(x_current);

            if(y_new <= y_current)
            {
                ++ failures;
                x_current[index] = !x_current[index];
            }
            else
            {
                y_current = y_new;
                failures = 0;
            }
        }

        return {std::move(x_current), std::move(y_current)};
    }
}
// namespace optimization
}
// namespace grabin

#endif
// Z_GRABIN_OPTIMIZATION_LOCAL_SEARCH_HPP_INCLUDED
