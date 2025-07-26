#include "bt_execute_attack.hpp"
#include "attacks/attack.hpp"

BTTask::Status BTExecuteAttack::_tick(double delta)
{
    TASK_FAIL_COND_COMP(!(this->get_blackboard()->has_var(this->attack_to_execute)));

    Attack* attack = godot::Object::cast_to<Attack>(this->get_blackboard()->get_var(this->attack_to_execute));

    TASK_SUCCEED_COND(attack->is_finished());

    attack->step(delta);

    TASK_RUNNING();
}

void BTExecuteAttack::_bind_methods()
{
    BIND_GETTER_SETTER_PROPERTY_DEFAULT(BTExecuteAttack, STRING_NAME, attack_to_execute);
}