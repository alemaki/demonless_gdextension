#include "hitbox_blocker.hpp"
#include "components/area3d/hitbox.hpp"

void HitboxBlocker::hitbox_entered(godot::Area3D* hitbox)
{
    this->emit_signal("hitbox_blocked", hitbox);
}

void HitboxBlocker::_bind_methods()
{
    using namespace godot;

    ADD_SIGNAL(MethodInfo("hitbox_blocked", PropertyInfo(Variant::OBJECT, "hitbox", PROPERTY_HINT_NONE, "Area3D")));
}