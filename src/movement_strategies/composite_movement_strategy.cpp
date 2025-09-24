#include "composite_movement_strategy.hpp"

godot::TypedArray<MovementStrategy> CompositeMovementStrategy::get_movement_strategies() const
{
    godot::TypedArray<MovementStrategy> strategies;
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

bool CompositeMovementStrategy::is_done() const
{
    for (int i = 0; i < get_child_count(); ++i)
    {
        MovementStrategy* strategy = Object::cast_to<MovementStrategy>(get_child(i));
        if (strategy == nullptr)
        {
            ERR_PRINT_ONCE("Child " + godot::itos(i) + " is not a movement strategy: " + this->get_name());
            return true;
        }
        if (!(strategy->is_done()))
        {
            return false;
        }
    }
    return true;
}

void CompositeMovementStrategy::_ready()
{
    ERR_FAIL_COND_MSG(get_child_count() == 0, "No children of CompositeMovementStrategy: " + this->get_name());
}

void CompositeMovementStrategy::_apply(godot::Ref<MovementContext> context, double delta)
{
    ERR_FAIL_NULL(context);
    for (int i = 0; i < get_child_count(); ++i)
    {
        MovementStrategy* strategy = Object::cast_to<MovementStrategy>(get_child(i));
        ERR_CONTINUE(strategy == nullptr);
        if (!(strategy->is_done()))
        {
            strategy->apply(context, delta);
        }
    }
}

void CompositeMovementStrategy::_bind_methods() {
    // Nothing to bind; strategies are children, not a property
}