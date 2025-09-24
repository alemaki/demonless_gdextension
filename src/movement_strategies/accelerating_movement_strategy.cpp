#include "accelerating_movement_strategy.hpp"
#include <godot_cpp/variant/utility_functions.hpp>

void AcceleratingMovementStrategy::_apply(godot::Ref<MovementContext> context, double delta)
{
    double speed = context->get_speed();
    speed += this->acceleration_per_second;
    speed = godot::Math::clamp<double>(speed, this->min_speed, this->max_speed);
    context->set_speed(speed);
    context->set_position(context->get_position() + context->get_direction()*context->get_speed()*delta);
    last_speed_call = speed;
}

bool AcceleratingMovementStrategy::is_done() const
{
    if (this->acceleration_per_second < 0)
    {
        return godot::Math::is_equal_approx(this->last_speed_call, this->min_speed);
    }
    return godot::Math::is_equal_approx(this->last_speed_call, this->max_speed);
}

void AcceleratingMovementStrategy::_ready()
{
    ERR_FAIL_COND(!godot::Math::is_zero_approx(this->min_speed - this->max_speed) && this->min_speed > this->max_speed);
    ERR_FAIL_COND_MSG(godot::Math::is_equal_approx(this->acceleration_per_second, 0.), "Acceleration is approx 0: " + this->get_name());
}

void AcceleratingMovementStrategy::_bind_methods()
{
    BIND_GETTER_SETTER_DEFAULT(AcceleratingMovementStrategy, acceleration_per_second);
    BIND_GETTER_SETTER_DEFAULT(AcceleratingMovementStrategy, min_speed);
    BIND_GETTER_SETTER_DEFAULT(AcceleratingMovementStrategy, max_speed);
}
