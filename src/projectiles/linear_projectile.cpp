#include "linear_projectile.hpp"
#include "godot_cpp/variant/utility_functions.hpp"

void LinearProjectile::_physics_process(double delta)
{
    Projectile::_physics_process(delta);
    godot::Vector3 vector_travelled = this->direction * this->speed * delta;
    this->set_position(this->get_position() + vector_travelled);
}


void LinearProjectile::_bind_methods()
{
    using namespace godot;
    ClassDB::bind_method(D_METHOD("set_speed", "speed"), &LinearProjectile::set_speed);
    ClassDB::bind_method(D_METHOD("get_speed"), &LinearProjectile::get_speed);

    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "speed"), "set_speed", "get_speed");
}
