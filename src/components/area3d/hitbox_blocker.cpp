#include "projectile_detector.hpp"
#include "components/area3d/hitbox.hpp"

void HitboxDetector::_on_area_entered(godot::Area3D* area3d)
{
    Hitbox* hitbox = godot::Object::cast_to<Hitbox>(area3d);
    if (hitbox != nullptr)
    {
        hitbox->trigger_block();
        this->emit_signal("hitbox_blocked");
    }
}

void HitboxDetector::_bind_methods()
{
    ADD_SIGNAL(MethodInfo("hitbox_blocked"));
}