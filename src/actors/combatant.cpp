#include "combatant.hpp"

void Combatant::_ready()
{
    utils::ensure_node(this->hurtbox, this, "Hurtbox");
    utils::ensure_node(this->health_component, this, "HealthComponent");

    if (this->hurtbox)
    {
        this->hurtbox->connect("hurtbox_hit", callable_mp(this, &Combatant::_on_hurtbox_hit));
    }
}

void Combatant::_on_hurtbox_hit(const godot::Area3D* _hitbox)
{
    const Hitbox* hitbox = godot::Object::cast_to<Hitbox>(_hitbox);
    ERR_FAIL_NULL(hitbox);
    this->on_hit(hitbox);
}

void Combatant::on_hit(const Hitbox* hitbox)
{
    ERR_FAIL_NULL_MSG(this->health_component, vformat("%s: no health_component to take damage from hurtbox", this->get_name()));
    LOG_DEBUG(vformat("%s: Took damage: %d", this->get_name(), hitbox->get_damage_info()->get_damage()));
    this->health_component->take_damage(hitbox->get_damage_info());
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
