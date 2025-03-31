#include "accelerating_movement_strategy.hpp"
#include <godot_cpp/variant/utility_functions.hpp>

void AcceleratingMovementStrategy::_apply(godot::Ref<MovementContext> context, double delta)
{
    double v0 = context->get_speed();
    double a = this->acceleration_per_second;
    double bound = (a > 0) ? this->max_speed : this->min_speed;
    /* time (within this step) at which speed would reach min/max_speed, ignoring the clamp */
    double time_to_bound = (bound - v0) / a;

    double distance;
    double final_speed;

    if (time_to_bound >= delta)
    {
        /* Never reaches the bound this step: distance is the integral of v(t) = v0 + a*t over [0, delta]. */
        distance = v0*delta + 0.5*a*delta*delta;
        final_speed = v0 + a*delta;
    }
    else
    {
        /* Reaches min/max_speed partway through the step: integrate the accelerating part up to
         * time_to_bound, then the remainder of the step moves at a constant `bound` speed. */
        double acceleration_time = time_to_bound > 0.0 ? time_to_bound : 0.0;
        distance = v0*acceleration_time + 0.5*a*acceleration_time*acceleration_time + bound*(delta - acceleration_time);
        final_speed = bound;
    }

    final_speed = godot::Math::clamp<double>(final_speed, this->min_speed, this->max_speed);
    context->set_speed(final_speed);
    context->set_position(context->get_position() + context->get_direction()*distance);
    last_speed_call = final_speed;
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
