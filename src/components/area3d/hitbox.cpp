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
    if (this->damage_profile.is_null() && !IS_EDITOR)
    {
        this->damage_profile.instantiate();
    }
    this->connect("area_entered", callable_mp(this, &Hitbox::_on_area_entered));
}

godot::PackedStringArray Hitbox::_get_configuration_warnings() const
{
    godot::PackedStringArray warnings = Area3D::_get_configuration_warnings();
    if (this->damage_profile.is_null())
    {
        warnings.push_back("No DamageProfile assigned. A default one will be created at runtime.");
    }
    return warnings;
}

void Hitbox::_bind_methods()
{
    using namespace godot;
    ClassDB::bind_method(D_METHOD("_on_area_entered", "area3d"), &Hitbox::_on_area_entered);

    BIND_GETTER_SETTER_DEFAULT(Hitbox, damage_profile);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "damage_profile", PROPERTY_HINT_RESOURCE_TYPE, "DamageProfile", PROPERTY_USAGE_DEFAULT | PROPERTY_USAGE_ALWAYS_DUPLICATE, "DamageProfile"), "set_damage_profile", "get_damage_profile");
    ADD_SIGNAL(MethodInfo("hit_hurtbox", PropertyInfo(Variant::OBJECT, "hurtbox", PROPERTY_HINT_NONE, "Area3D")));
    ADD_SIGNAL(MethodInfo("hit_blocker", PropertyInfo(Variant::OBJECT, "hitbox_blocker", PROPERTY_HINT_NONE, "Area3D")));
}