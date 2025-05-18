#include "damage_profile.hpp"

godot::Ref<DamageInfo> DamageProfile::construct_damage_info(godot::Node *actor_source)
{
    godot::Ref<DamageInfo> damage_info;
    damage_info.instantiate();
    damage_info->set_damage(this->base_damage);
    damage_info->set_source(actor_source);
    return damage_info;
}

void DamageProfile::_bind_methods()
{
    BIND_GETTER_SETTER_PROPERTY_DEFAULT(DamageProfile, FLOAT, base_damage);
}