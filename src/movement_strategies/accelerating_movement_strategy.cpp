#include "accelerating_movement_strategy.hpp"
#include <godot_cpp/variant/utility_functions.hpp>

void AcceleratingMovementStrategy::_apply(godot::Ref<MovementContext> context, double delta)
{
    double speed = context->get_speed();
   /*
    * Integrate the velocity over the current frame.
    * Starting from the current speed `s`, compute the displacement by
    * integrating (acceleration * x + s) over the interval [0, delta].
    * which is 1/2*(acceleration * delta^2) + delta*s
    * Take into account that max_speed/min_speed can be reached in the meantime so split integral.
    */
    double time_at_extreme = 0;
    double distance_at_extreme = 0;
    if (this->acceleration_per_second > 0)
    {
        time_at_extreme = ((speed + delta*this->acceleration_per_second) - this->max_speed)/this->acceleration_per_second;
    }
    else
    {
        time_at_extreme = (this->min_speed - (speed + delta*this->acceleration_per_second))/this->acceleration_per_second;
    }
    time_at_extreme = time_at_extreme > 0 ? time_at_extreme : 0;
    time_at_extreme = godot::Math::clamp<double>(time_at_extreme, 0, delta);
    double time_accelerating = delta - time_at_extreme;
    context->set_position(
        context->get_position()
        + context->get_direction()*speed*time_accelerating
        + 0.5*context->get_direction()*this->acceleration_per_second*time_accelerating*time_accelerating
        + context->get_direction()*time_at_extreme*(this->acceleration_per_second > 0 ? this->max_speed : this->min_speed)
    );
    speed += this->acceleration_per_second*delta;
    speed = godot::Math::clamp<double>(speed, this->min_speed, this->max_speed);
    context->set_speed(speed);
    last_speed_call = speed;
}

bool AcceleratingMovementStrategy::_is_done() const
{
    if (this->acceleration_per_second < 0)
    {
        return godot::Math::is_equal_approx(this->last_speed_call, this->min_speed);
    }
    return godot::Math::is_equal_approx(this->last_speed_call, this->max_speed);
}

void AcceleratingMovementStrategy::_init()
{
    ERR_FAIL_COND(!godot::Math::is_zero_approx(this->min_speed - this->max_speed) && this->min_speed > this->max_speed);
    ERR_FAIL_COND_MSG(godot::Math::is_equal_approx(this->acceleration_per_second, 0.), "Acceleration is approx 0: " + this->get_name());
}

void AcceleratingMovementStrategy::_bind_methods()
{
    BIND_GETTER_SETTER_PROPERTY_DEFAULT(AcceleratingMovementStrategy, FLOAT, acceleration_per_second);
    BIND_GETTER_SETTER_PROPERTY_DEFAULT(AcceleratingMovementStrategy, FLOAT, min_speed);
    BIND_GETTER_SETTER_PROPERTY_DEFAULT(AcceleratingMovementStrategy, FLOAT, max_speed);
}
