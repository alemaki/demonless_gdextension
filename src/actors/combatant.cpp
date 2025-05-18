#include "combatant.hpp"

void Combatant::_ready()
{
    utils::ensure_node(this->hurtbox, this, "Hurtbox");
    utils::ensure_node(this->health_component, this, "HealthComponent");

    if (this->hurtbox)
    {
        this->hurtbox->connect("hurtbox_hit", callable_mp(this, &Combatant::on_hit));
    }
}

void Combatant::on_hit(const godot::Ref<DamageInfo>& damage_info)
{
    ERR_FAIL_NULL_MSG(this->health_component, vformat("%s: no health_component to take damage from hurtbox", this->get_name()));
    ERR_FAIL_COND_MSG(damage_info.is_null(), vformat("%s: damage_info is null", this->get_name()));
    LOG_DEBUG(vformat("%s: Took %d damage.", this->get_name(), damage_info->get_damage()));
    this->health_component->take_damage(damage_info);
}

void Combatant::_bind_methods()
{
    using namespace godot;

    BIND_GETTER_SETTER_PROPERTY_OBJECT_DEFAULT(
        Combatant, hurtbox, PROPERTY_HINT_NODE_TYPE, "Hurtbox",
        PROPERTY_USAGE_DEFAULT, Hurtbox);

    BIND_GETTER_SETTER_PROPERTY_OBJECT_DEFAULT(
        Combatant, health_component, PROPERTY_HINT_NODE_TYPE, "HealthComponent",
        PROPERTY_USAGE_DEFAULT, HealthComponent);
}
