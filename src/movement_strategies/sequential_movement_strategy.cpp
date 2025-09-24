#include "sequential_movement_strategy.hpp"

godot::TypedArray<MovementStrategy> SequentialMovementStrategy::get_movement_strategies() const
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

bool SequentialMovementStrategy::is_done() const
{
    for (int i = this->current_strategy; i < get_child_count(); ++i)
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

void SequentialMovementStrategy::_ready()
{
    ERR_FAIL_COND_MSG(get_child_count() == 0, "No children of SequentialMovementStrategy: " + this->get_name());
}

void SequentialMovementStrategy::_apply(godot::Ref<MovementContext> context, double delta)
{
    ERR_FAIL_NULL(context);
    ERR_FAIL_COND(get_child_count() <= this->current_strategy);

    MovementStrategy* strategy = Object::cast_to<MovementStrategy>(get_child(this->current_strategy));
    ERR_FAIL_COND(strategy == nullptr);

    while (strategy->is_done())
    {
        this->current_strategy++;
        if (get_child_count() <= this->current_strategy)
        {
            return;
        }
        strategy = Object::cast_to<MovementStrategy>(get_child(this->current_strategy));
        ERR_FAIL_COND(strategy == nullptr);
    }

    strategy->apply(context, delta);
}

void SequentialMovementStrategy::_bind_methods() {
    // Nothing to bind; strategies are children, not a property
}