#include "hitbox_blocker.hpp"
#include "components/area3d/hitbox.hpp"

void HitboxBlocker::hitbox_entered(godot::Area3D* hitbox)
{
    this->emit_signal("hitbox_blocked", hitbox);
}

void HitboxBlocker::set_blocker_enabled(bool enabled)
{
    this->set_visible(enabled);
    this->call_deferred("set_monitoring", enabled);
    for (int i = 0; i < this->get_child_count(); i++)
    {
        godot::Node *child = this->get_child(i);
        if (godot::CollisionShape3D* shape = Object::cast_to<godot::CollisionShape3D>(child))
        {
            shape->call_deferred("set_disabled", !enabled);
            shape->set_visible(enabled);
        }
        else if (godot::CollisionPolygon3D* poly = Object::cast_to<godot::CollisionPolygon3D>(child))
        {
            poly->call_deferred("set_disabled", !enabled);
            poly->set_visible(enabled);
        }
    }
}

void HitboxBlocker::_bind_methods()
{
    using namespace godot;

    ADD_SIGNAL(MethodInfo("hitbox_blocked", PropertyInfo(Variant::OBJECT, "hitbox", PROPERTY_HINT_NONE, "Area3D")));
}