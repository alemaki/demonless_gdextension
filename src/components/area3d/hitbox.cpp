#include "hitbox.hpp"
#include "components/area3d/hitbox_blocker.hpp"
#include "components/area3d/hurtbox.hpp"

void Hitbox::_on_area_entered(godot::Area3D* area3d)
{
    Hurtbox* hurtbox = godot::Object::cast_to<Hurtbox>(area3d);
    if (hurtbox != nullptr)
    {
        hurtbox->hitbox_entered(this);
        this->emit_signal("hit_hurtbox", hurtbox);
        return;
    }

    HitboxBlocker* hitbox_blocker = godot::Object::cast_to<HitboxBlocker>(area3d);
    if (hitbox_blocker != nullptr)
    {
        hitbox_blocker->hitbox_entered(this);
        this->emit_signal("hit_blocker", hitbox_blocker);
        return;
    }
}

void Hitbox::_ready()
{
    this->connect("area_entered", callable_mp(this, &Hitbox::_on_area_entered));
}

void Hitbox::_bind_methods()
{
    using namespace godot;
    ClassDB::bind_method(D_METHOD("_on_area_entered", "area3d"), &Hitbox::_on_area_entered);

    BIND_GETTER_SETTER_BOOL_DEFAULT(Hitbox, blockable);

    ADD_SIGNAL(MethodInfo("hit_hurtbox", PropertyInfo(Variant::OBJECT, "hitbox", PROPERTY_HINT_NONE, "Area3D")));
    ADD_SIGNAL(MethodInfo("hit_blocker", PropertyInfo(Variant::OBJECT, "hitbox_blocker", PROPERTY_HINT_NONE, "Area3D")));
}