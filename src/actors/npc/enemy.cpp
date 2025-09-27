#include "enemy.hpp"

void Enemy::_ready()
{
    Agent::_ready();
    ERR_FAIL_NULL(blackboard);
    this->attacks = get_all_children_of_type<SkillAction>(this);
    for (int i = 0, size = attacks.size(); i < size; i++)
    {
        SkillAction* attack = Object::cast_to<SkillAction>(attacks[i]);
        ERR_CONTINUE(attack == nullptr);
        ERR_CONTINUE_MSG(this->blackboard->has_var(attack->get_name()), "Attack with name \"" + attack->get_name() + "\" already exists in blackboard.");
        this->blackboard->set_var(attack->get_name(), attack);
    }
}