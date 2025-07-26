#include "hitbox_blocker.hpp"
#include "components/area3d/hitbox.hpp"

void HitboxBlocker::_ready()
{
    this->connect("area_entered", callable_mp(this, &HitboxBlocker::_on_area_entered));
}

void HitboxBlocker::_on_area_entered(godot::Area3D* area3d)
{
    Hitbox* hitbox = godot::Object::cast_to<Hitbox>(area3d);
    if (hitbox != nullptr && hitbox->is_blockable())
    {
        hitbox->trigger_block();
        this->emit_signal("hitbox_blocked");
    }
}

void HitboxBlocker::_bind_methods()
{
    using namespace godot;
    
    ADD_SIGNAL(MethodInfo("hitbox_blocked"));
}