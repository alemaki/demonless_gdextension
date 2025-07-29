#ifndef MOVEMENT_STRATEGY_HPP
#define MOVEMENT_STRATEGY_HPP

#include <godot_cpp/classes/resource.hpp>
#include "movement_strategies/movement_context.hpp"

class MovementStrategy : public godot::Resource
{
    GDCLASS(MovementStrategy, godot::Resource);

public:
    virtual void apply(godot::Ref<MovementContext> context, double delta) const = 0;

protected:
    static void _bind_methods();
};

#endif // MOVEMENT_STRATEGY_HPP