#include "seeking_movement_strategy.hpp"

void SeekingMovementStrategy::_apply(godot::Ref<MovementContext> context, double delta)
{
    ERR_FAIL_NULL_MSG(context->get_target(), "%d: SeeekingMovementStrategy has no target.");
    godot::Node3D* target = context->get_target();
    godot::Vector3 pos = context->get_position();
    godot::Vector3 direction = pos.direction_to(target->get_position());
    direction.y = 0;
    context->set_direction(direction);
    pos = pos + direction*context->get_speed()*delta;
    context->set_position(pos);
    if (!(this->never_ending))
    {
        this->time_remaining -= delta;
    }
}

bool SeekingMovementStrategy::is_done() const
{
    if (this->never_ending)
    {
        return false;
    }
    else if (this->time_remaining <= 0)
    {
        return true;
    }
    return false;
}

void SeekingMovementStrategy::_ready()
{
    this->time_remaining = this->duration;
}

void SeekingMovementStrategy::_bind_methods()
{
    BIND_GETTER_SETTER_PROPERTY_BOOL_DEFAULT(SeekingMovementStrategy, never_ending);
    BIND_GETTER_SETTER_PROPERTY_DEFAULT(SeekingMovementStrategy, FLOAT, duration);
}
