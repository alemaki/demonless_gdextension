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
}