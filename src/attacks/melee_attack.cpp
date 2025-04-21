#include "melee_attack.hpp"

void MeleeAttack::_step(double delta)
{
    ERR_FAIL_NULL(this->actor_source);
    ERR_FAIL_COND(direction.is_zero_approx());
    bool cancellable = this->time_accumulated <= this->early_cancel_endpoint
                       || this->time_accumulated >= this->late_cancel_startpoint;
    this->set_cancellable(cancellable);
}

void MeleeAttack::_reset()
{
    this->set_cancellable(false);
}

void MeleeAttack::_end()
{
    this->set_cancellable(false);
}

void MeleeAttack::_bind_methods()
{
    using namespace godot;
    
    BIND_GETTER_SETTER_PROPERTY_DEFAULT(MeleeAttack, FLOAT, early_cancel_endpoint);
    BIND_GETTER_SETTER_PROPERTY_DEFAULT(MeleeAttack, FLOAT, late_cancel_startpoint);
}
