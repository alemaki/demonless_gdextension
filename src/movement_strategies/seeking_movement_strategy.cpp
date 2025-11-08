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

void SeekingMovementStrategy::_bind_methods()
{

}
