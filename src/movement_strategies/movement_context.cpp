#include "movement_context.hpp"


void MovementContext::_bind_methods()
{
    BIND_GETTER_SETTER_PROPERTY_DEFAULT(MovementContext, VECTOR3, position);
    BIND_GETTER_SETTER_PROPERTY_DEFAULT(MovementContext, VECTOR3, direction);
    BIND_GETTER_SETTER_PROPERTY_DEFAULT(MovementContext, VECTOR3, speed);
}