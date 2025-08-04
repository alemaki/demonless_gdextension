#include "movement_context.hpp"

void MovementContext::set_direction(const godot::Vector3 direction)
{
    this->direction = direction.normalized();
}

void MovementContext::_bind_methods()
{
    BIND_GETTER_SETTER_PROPERTY_DEFAULT(MovementContext, VECTOR3, position);
    BIND_GETTER_SETTER_PROPERTY_DEFAULT(MovementContext, VECTOR3, direction);
    BIND_GETTER_SETTER_PROPERTY_DEFAULT(MovementContext, VECTOR3, speed);
}