#include "combo_attack_stage.hpp"

void ComboAttackStage::set_direction(const godot::Vector3 direction)
{
    ERR_FAIL_COND_MSG(direction.is_zero_approx(), "Attack direction cannot be godot::Vector3{0, 0, 0}");
    if (this->change_direction_allowed || this->phase == SkillAction::SkillPhase::None)
    {
        this->direction = direction.normalized();
    }
}

void ComboAttackStage::_step(double delta)
{
    ERR_FAIL_NULL(this->actor_source);
    ERR_FAIL_COND(direction.is_zero_approx());
    bool cancellable = this->time_accumulated <= this->until_cancel_window
                       || this->time_accumulated >= this->after_cancel_window;
    this->set_cancellable(cancellable);
}

void ComboAttackStage::_reset()
{
    this->set_cancellable(false);
}

void ComboAttackStage::_end()
{
    this->set_cancellable(false);
}

void ComboAttackStage::_bind_methods()
{
    using namespace godot;
    
    BIND_GETTER_SETTER_PROPERTY_DEFAULT(ComboAttackStage, FLOAT, until_cancel_window);
    BIND_GETTER_SETTER_PROPERTY_DEFAULT(ComboAttackStage, FLOAT, after_cancel_window);
    BIND_GETTER_SETTER_PROPERTY_BOOL_DEFAULT(ComboAttackStage, change_direction_allowed);
}
