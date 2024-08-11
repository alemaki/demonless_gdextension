#include "linear_projectile.hpp"
#include "godot_cpp/variant/utility_functions.hpp"
void LinearProjectile::set_direction(godot::Vector2 direction)
{
    this->direction = direction.normalized();
}

void LinearProjectile::_physics_process(double delta)
{
    Projectile::_physics_process(delta);
    godot::Vector2 vector_travelled = this->direction * this->speed * delta;
    this->set_position(this->get_position() + vector_travelled);
}


void LinearProjectile::_bind_methods()
{
    using namespace godot;
    ClassDB::bind_method(D_METHOD("set_speed"), &LinearProjectile::set_speed);
    ClassDB::bind_method(D_METHOD("get_speed"), &LinearProjectile::get_speed);
    ClassDB::bind_method(D_METHOD("set_direction"), &LinearProjectile::set_direction);
    ClassDB::bind_method(D_METHOD("get_direction"), &LinearProjectile::get_direction);

    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "speed"), "set_speed", "get_speed");
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "direction"), "set_direction", "get_direction");
}
