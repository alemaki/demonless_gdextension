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
    ERR_FAIL_COND_V_MSG(get_child_count() == 0, true, "No children of SequentialMovementStrategy: " + this->get_name());
    int strat = 0;
    MovementStrategy* strategy = Object::cast_to<MovementStrategy>(get_child(strat));
    ERR_FAIL_COND_V_MSG(strategy == nullptr, true, "Child " + godot::itos(strat) + " is not a movement strategy.");

    while (strategy->is_done())
    {
        strat++;
        if (get_child_count() <= strat)
        {
            break;
        }
        strategy = Object::cast_to<MovementStrategy>(get_child(strat));
        ERR_FAIL_COND_V_MSG(strategy == nullptr, true, "Child " + godot::itos(strat) + " is not a movement strategy.");
    }

    return (get_child_count() <= strat);
}

void SequentialMovementStrategy::_apply(godot::Ref<MovementContext> context, double delta)
{
    ERR_FAIL_NULL(context);
    ERR_FAIL_COND(get_child_count() <= this->current_strategy);

    MovementStrategy* strategy = Object::cast_to<MovementStrategy>(get_child(this->current_strategy));
    ERR_FAIL_COND_MSG(strategy == nullptr, "Child " + godot::itos(this->current_strategy) + " is not a movement strategy.");

    while (strategy->is_done())
    {
        this->current_strategy++;
        if (get_child_count() <= this->current_strategy)
        {
            return;
        }
        strategy = Object::cast_to<MovementStrategy>(get_child(this->current_strategy));
        ERR_FAIL_COND_MSG(strategy == nullptr, "Child " + godot::itos(this->current_strategy) + " is not a movement strategy.");
    }

    strategy->apply(context, delta);
}

void SequentialMovementStrategy::_bind_methods() {
    // Nothing to bind; strategies are children, not a property
}