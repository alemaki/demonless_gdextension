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

bool CompositeMovementStrategy::_is_done() const
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

void CompositeMovementStrategy::_init()
{
    ERR_FAIL_COND_MSG(get_child_count() == 0, "No children of CompositeMovementStrategy: " + this->get_name());
}

void CompositeMovementStrategy::_apply(godot::Ref<MovementContext> context, double delta)
{
    ERR_FAIL_NULL(context);

    if (this->child_contexts.size() != get_child_count())
    {
        this->child_contexts.resize(get_child_count());
    }

    godot::Vector3 start_position = context->get_position();
    godot::Vector3 blended_displacement;
    double total_weight = 0.0;

    for (int i = 0; i < get_child_count(); ++i)
    {
        MovementStrategy* strategy = Object::cast_to<MovementStrategy>(get_child(i));
        ERR_CONTINUE(strategy == nullptr);
        if (strategy->is_done())
        {
            continue;
        }

        /* Get reference for this Ref since it might change value */
        godot::Ref<MovementContext>& child_context = this->child_contexts.write[i];
        if (child_context.is_null())
        {
            /* First tick for this child */
            child_context = godot::Ref<MovementContext>(memnew(MovementContext));
            child_context->set_speed(context->get_speed());
        }
        /* Ground truth shared with every child every tick. 
         * Speed is deliberately left at 1, so child can ramp up on it's own if it does. */
        child_context->set_position(start_position);
        child_context->set_direction(context->get_direction());
        child_context->set_target(context->get_target());

        strategy->apply(child_context, delta);

        double weight = strategy->get_blend_weight();
        blended_displacement += (child_context->get_position() - start_position) * weight;
        total_weight += weight;
    }

    if (total_weight != 0.0)
    {
        context->set_position(start_position + blended_displacement);
        if (!blended_displacement.is_zero_approx())
        {
            context->set_direction(blended_displacement);
        }
        context->set_speed(blended_displacement.length() / delta);
    }
}

void CompositeMovementStrategy::_bind_methods() {
    // Nothing to bind; strategies are children, not a property
}