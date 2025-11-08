#include "movement_strategy.hpp"

void MovementStrategy::_apply(godot::Ref<MovementContext> context, double delta)
{
    /* ERR_FAIL_NULL(context); // No need to check this since apply calls it. */

    /* default behaviour is linear */
    context->set_position(context->get_position() + context->get_direction()*context->get_speed()*delta);
}

bool MovementStrategy::_is_done() const
{
    return false;
}

void MovementStrategy::apply(godot::Ref<MovementContext> context, double delta)
{
    ERR_FAIL_NULL(context);
    if (this->is_done())
    {
        this->MovementStrategy::_apply(context, delta);
        return;
    }
    if (this->never_ending)
    {
        this->time_remaining -= delta;
    }
    this->_apply(context, delta);
}

bool MovementStrategy::is_done() const
{
    if (this->never_ending)
    {
        return false;
    }
    else if (!(only_duration)
             && (this->duration != 0
                 || this->time_remaining > 0)
            )
    {
        return this->_is_done();
    }
    else if (only_duration
             && this->time_remaining > 0)
    {
        return false;
    }
    return true;
}

void MovementStrategy::_ready()
{
    this->time_remaining = this->duration;
    this->_init();
}

void MovementStrategy::_bind_methods()
{
    using namespace godot;

    //TODO: switch to godot_cpp 4.3 to be able to expose virtual methods.
    BIND_GETTER_SETTER_PROPERTY_DEFAULT(MovementStrategy, FLOAT, duration);
    BIND_GETTER_SETTER_PROPERTY_BOOL_DEFAULT(MovementStrategy, never_ending);
    BIND_GETTER_SETTER_PROPERTY_BOOL_DEFAULT(MovementStrategy, only_duration);

}