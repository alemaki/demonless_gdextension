#ifndef COMPOSITE_MOVEMENT_STRATEGY_HPP
#define COMPOSITE_MOVEMENT_STRATEGY_HPP

#include <godot_cpp/classes/node.hpp>
#include "movement_strategies/movement_strategy.hpp"

class CompositeMovementStrategy : public MovementStrategy
{
    GDCLASS(CompositeMovementStrategy, MovementStrategy);

private:
    godot::TypedArray<MovementStrategy> movement_strategies;

public:
    godot::TypedArray<MovementStrategy> get_movement_strategies() const;

    virtual void apply(godot::Ref<MovementContext> context, double delta) const;

protected:
    static void _bind_methods();
};

#endif // COMPOSITE_MOVEMENT_STRATEGY_HPP