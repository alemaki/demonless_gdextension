#include "accelerating_movement_strategy.hpp"
#include <godot_cpp/variant/utility_functions.hpp>

void AcceleratingMovementStrategy::_apply(godot::Ref<MovementContext> context, double delta)
{
    double speed = context->get_speed();
    speed += this->acceleration_per_second;
    godot::Math::clamp<double>(speed, this->min_speed, this->max_speed);
    context->set_speed(speed);
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
    ERR_FAIL_COND_MSG(godot::Math::is_equal_approx(this->last_speed_call, this->max_speed), "Acceleration is approx 0: " + this->get_name());
}

void AcceleratingMovementStrategy::_bind_methods()
{
    BIND_GETTER_SETTER_DEFAULT(AcceleratingMovementStrategy, acceleration_per_second);
    BIND_GETTER_SETTER_DEFAULT(AcceleratingMovementStrategy, min_speed);
    BIND_GETTER_SETTER_DEFAULT(AcceleratingMovementStrategy, max_speed);
}
