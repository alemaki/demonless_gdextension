#include "bt_execute_attack.hpp"
#include "attacks/wave_attack.hpp"

BTTask::Status BTExecuteAttack::_tick(double delta)
{
    TASK_FAIL_COND_COMP_MSG(
        !(this->get_blackboard()->has_var(this->attack_to_execute)),
        vformat("%s: Blackboard has no value named: %s", this->get_name(), this->attack_to_execute)
    );

    TASK_FAIL_COND_COMP_MSG(
        !(this->get_blackboard()->has_var("target")),
        vformat("%s: No target in blackboard", this->get_name())
    );

    //TODO: make attack, not wave attack, doesn't make sense?
    WaveAttack* attack = godot::Object::cast_to<WaveAttack>(this->get_blackboard()->get_var(this->attack_to_execute));
    Node3D* target = godot::Object::cast_to<Node3D>(this->get_blackboard()->get_var("target"));
    Node3D* actor = godot::Object::cast_to<Node3D>(this->get_actor());

    TASK_FAIL_COND_COMP_MSG(attack == nullptr, vformat("%s: %s is not Attack as expected", this->get_name(), this->attack_to_execute));
    TASK_FAIL_COND_COMP_MSG(target == nullptr, vformat("%s: Target is not Node3D as expected", this->get_name()));
    TASK_FAIL_COND_COMP_MSG(actor == nullptr, vformat("%s: Actor is not Node3D as expected", this->get_name()));

    TASK_SUCCEED_COND(attack->is_finished());

    godot::Vector3 direction = actor->get_global_position().direction_to(target->get_global_position());
    direction.y = 0; //TODO, fix this so it wont move upon the UP_VECTOR
    attack->set_target(target);
    attack->set_direction(direction);
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