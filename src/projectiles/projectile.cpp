#include "projectile.hpp"

void Projectile::set_direction(godot::Vector3 direction)
{
    this->direction = direction.normalized();
}


void Projectile::_bind_methods()
{
    using namespace godot;

    ClassDB::bind_method(D_METHOD("set_direction", "direction"), &Projectile::set_direction);
    ClassDB::bind_method(D_METHOD("get_direction"), &Projectile::get_direction);

    ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "direction"), "set_direction", "get_direction");

    ClassDB::bind_method(D_METHOD("set_lifespan_timer", "timer"), &Projectile::set_lifespan_timer);
    ClassDB::bind_method(D_METHOD("get_lifespan_timer"), &Projectile::get_lifespan_timer);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "lifespan_timer", PropertyHint::PROPERTY_HINT_NODE_TYPE, "Timer",PropertyUsageFlags::PROPERTY_USAGE_DEFAULT, "Timer"), "set_lifespan_timer", "get_lifespan_timer");
}

void Projectile::set_lifespan_timer(godot::Timer* timer)
{
    this->lifespan_timer = timer;
}
