#include "melee_attack.hpp"

void MeleeAttack::_step(double delta)
{
    ERR_FAIL_NULL(this->actor_source);
    bool cancellable = (this->time_accumulated <= this->early_cancel_endpoint)
                       || (this->time_accumulated >= this->late_cancel_startpoint);
    this->set_cancellable(cancellable);

    bool hitbox_should_be_active = (this->time_accumulated >= this->hit_startpoint) 
                            && (this->time_accumulated <= this->hit_endpoint);

    if (hitbox_should_be_active && !this->is_hitbox_active)
    {
        ERR_FAIL_NULL(this->hitbox);
        ERR_FAIL_COND(direction.is_zero_approx());
        godot::Vector3 source_position = this->actor_source->get_global_position();
        this->hitbox->set_monitoring(true);
        this->hitbox->set_visible(true);
        this->hitbox->set_global_position(source_position);
        this->hitbox->look_at(source_position + this->get_direction());
        this->is_hitbox_active = true;
    }
    else if (this->is_hitbox_active)
    {
        if (this->change_direction_allowed)
        {
            this->hitbox->look_at(this->actor_source->get_global_position() + this->get_direction());
        }
        if (!hitbox_should_be_active)
        {
            this->hitbox->set_visible(false);
            this->hitbox->set_monitoring(false);
        }
    }
}

void MeleeAttack::_reset()
{
    this->set_cancellable(false);
}

void MeleeAttack::_end()
{
    this->set_cancellable(false);
}

void MeleeAttack::_ready()
{
    utils::ensure_node(this->hitbox, this, "Hitbox");
    if (this->hitbox && !(IS_EDITOR))
    {
        this->hitbox->set_monitoring(false);
        this->hitbox->set_visible(false);
    }
}

void MeleeAttack::_bind_methods()
{
    using namespace godot;
    
    BIND_GETTER_SETTER_PROPERTY_DEFAULT(MeleeAttack, FLOAT, early_cancel_endpoint);
    BIND_GETTER_SETTER_PROPERTY_DEFAULT(MeleeAttack, FLOAT, late_cancel_startpoint);
    BIND_GETTER_SETTER_PROPERTY_DEFAULT(MeleeAttack, FLOAT, hit_endpoint);
    BIND_GETTER_SETTER_PROPERTY_DEFAULT(MeleeAttack, FLOAT, hit_startpoint);
    BIND_GETTER_SETTER_PROPERTY_OBJECT_DEFAULT(MeleeAttack, hitbox, PROPERTY_HINT_NODE_TYPE, "Hitbox", PROPERTY_USAGE_DEFAULT, Hitbox);
}
