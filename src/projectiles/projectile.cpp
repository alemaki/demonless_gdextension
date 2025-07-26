#include "projectile.hpp"

void Projectile::_on_timeout()
{
    this->queue_free();
}

void Projectile::_ready()
{
    if (!this->lifespan_timer)
    {
        this->lifespan_timer = memnew(godot::Timer);
        this->add_child(this->lifespan_timer);
        this->lifespan_timer->set_autostart(true);
        this->lifespan_timer->set_one_shot(true);
    }
    this->lifespan_timer->connect("timeout", godot::Callable(this, "_on_timeout"));
}
 
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
    ClassDB::bind_method(D_METHOD("_on_timeout"), &Projectile::_on_timeout);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "lifespan_timer", PROPERTY_HINT_NODE_TYPE, "Timer", PROPERTY_USAGE_DEFAULT, "Timer"), "set_lifespan_timer", "get_lifespan_timer");
}

void Projectile::set_lifespan_timer(godot::Timer* timer)
{
    this->lifespan_timer = timer;
}
