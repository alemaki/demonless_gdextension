#include "bt_execute_attack.hpp"
#include "skills/skill_action.hpp"

BTTask::Status BTExecuteAttack::_tick(double delta)
{
    TASK_FAIL_COND_COMP_MSG(
        !(this->get_blackboard()->has_var(this->attack_to_execute)),
        vformat("%s: Blackboard has no value named: %s", this->get_name(), this->attack_to_execute)
    );

    //TODO: make attack only or all skills?
    SkillAction* attack = godot::Object::cast_to<SkillAction>(this->get_blackboard()->get_var(this->attack_to_execute));
    TASK_FAIL_COND_COMP_MSG(attack == nullptr, vformat("%s: %s is not SkillAction as expected", this->get_name(), this->attack_to_execute));
    TASK_SUCCEED_COND(attack->is_finished());

    attack->step(delta);

    TASK_RUNNING();
}


void BTExecuteAttack::_enter()
{
    ERR_FAIL_COND_MSG(
        !(this->get_blackboard()->has_var(this->attack_to_execute)),
        vformat("%s: Blackboard has no value named: %s", this->get_name(), this->attack_to_execute)
    );

    SkillAction* attack = godot::Object::cast_to<SkillAction>(this->get_blackboard()->get_var(this->attack_to_execute));
    ERR_FAIL_COND_MSG(attack == nullptr, vformat("%s: %s is not SkillAction as expected", this->get_name(), this->attack_to_execute));
    attack->reset();
    attack->set_actor_source(godot::Object::cast_to<godot::Node3D>(this->get_actor()));
}

void BTExecuteAttack::_bind_methods()
{
    BIND_GETTER_SETTER_PROPERTY_DEFAULT(BTExecuteAttack, STRING_NAME, attack_to_execute);
}