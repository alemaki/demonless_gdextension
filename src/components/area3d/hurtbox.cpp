#include "hurtbox.hpp"
#include "hitbox.hpp"

void Hurtbox::hitbox_entered(godot::Area3D* _hitbox)
{
    Hitbox* hitbox = godot::Object::cast_to<Hitbox>(_hitbox);
    ERR_FAIL_COND_MSG(hitbox->get_damage_profile().is_null(), vformat("%s: hitbox (%s) has no damage_profile", this->get_path(), hitbox->get_name()));
    godot::Ref<DamageInfo> damage_info = hitbox->get_damage_profile()->construct_damage_info(hitbox->get_actor_source());
    this->emit_signal("hurtbox_hit", damage_info);
}

void Hurtbox::_bind_methods()
{
    using namespace godot;

    ADD_SIGNAL(MethodInfo("hurtbox_hit", PropertyInfo(Variant::OBJECT, "damage_info", PROPERTY_HINT_NONE, "DamageInfo")));
}