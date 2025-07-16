#include "projectile.hpp"

void Projectile::on_timeout()
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
        this->lifespan_timer->connect("timeout", godot::Callable(this, "on_timeout"));
    }
}
 
void Projectile::_bind_methods()
{
    using namespace godot;

    ClassDB::bind_method(D_METHOD("set_lifespan_timer", "timer"), &Projectile::set_lifespan_timer);
    ClassDB::bind_method(D_METHOD("get_lifespan_timer"), &Projectile::get_lifespan_timer);
    ClassDB::bind_method(D_METHOD("on_timeout"), &Projectile::on_timeout);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "lifespan_timer", PropertyHint::PROPERTY_HINT_NODE_TYPE, "Timer",PropertyUsageFlags::PROPERTY_USAGE_DEFAULT, "Timer"), "set_lifespan_timer", "get_lifespan_timer");
}

void Projectile::set_lifespan_timer(godot::Timer* timer)
{
    this->lifespan_timer = timer;
}
