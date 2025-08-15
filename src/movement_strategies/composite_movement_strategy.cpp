#include "composite_movement_strategy.hpp"

using namespace godot;

TypedArray<MovementStrategy> CompositeMovementStrategy::get_movement_strategies() const
{
    TypedArray<MovementStrategy> strategies;
    for (int i = 0; i < get_child_count(); ++i)
    {
        MovementStrategy* strategy = Object::cast_to<MovementStrategy>(get_child(i));
        if (strategy)
        {
            strategies.push_back(strategy);
        }
    }
    return strategies;
}

void CompositeMovementStrategy::apply(Ref<MovementContext> context, double delta) const
{
    ERR_FAIL_NULL(context);
    for (int i = 0; i < get_child_count(); ++i)
    {
        MovementStrategy* strategy = Object::cast_to<MovementStrategy>(get_child(i));
        ERR_CONTINUE(strategy == nullptr);
        strategy->apply(context, delta);
    }
}

void CompositeMovementStrategy::_bind_methods() {
    // Nothing to bind; strategies are children, not a property
}