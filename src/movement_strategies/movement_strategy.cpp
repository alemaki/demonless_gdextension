#include "movement_strategy.hpp"

void MovementStrategy::_apply(godot::Ref<MovementContext> context, double delta)
{
    /* ERR_FAIL_NULL(context); // No need to check this since apply calls it. */

    /* default behaviour is linear */
    context->set_position(context->get_position() + context->get_direction()*context->get_speed()*delta);
}

void MovementStrategy::apply(godot::Ref<MovementContext> context, double delta)
{
    ERR_FAIL_NULL(context);
    if (this->is_done())
    {
        /* TODO: leave it frozen? */
        return;
    }
    this->_apply(context, delta);
    this->time_passed += delta;
}

bool MovementStrategy::is_done() const
{
    return this->time_passed > this->duration;
}

void MovementStrategy::_bind_methods()
{
    using namespace godot;

    BIND_GETTER_SETTER_DEFAULT(MovementStrategy, duration);
    //TODO: switch to godot_cpp 4.3 to be able to expose virtual methods.
}