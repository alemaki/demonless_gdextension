#include "movement_strategy.hpp"

void MovementStrategy::apply(godot::Ref<MovementContext> context, double delta)
{
    /* default behaviour is linear */
    context->set_position(context->get_position() + context->get_direction()*context->get_speed()*delta);
}

void MovementStrategy::_bind_methods()
{
    using namespace godot;

    //TODO: switch to godot_cpp 4.3 to be able to expose virtual methods.
}