#include "enemy.hpp"

void Enemy::_ready()
{
    this->Agent::_ready();
    utils::ensure_node(this->skill_action_manager, this, "SkillActionManager");

    ERR_FAIL_NULL(this->skill_action_manager);
    ERR_FAIL_NULL(this->blackboard);

    godot::TypedArray<SkillAction> skill_actions = this->skill_action_manager->get_all_skills();
    for (int64_t i = 0, size = skill_actions.size(); i < size; i++)
    {
        SkillAction* skill_action = godot::Object::cast_to<SkillAction>(skill_actions[i]);
        /* skill action manager will return only skills so check is not needed, but just in case */
        ERR_CONTINUE(skill_action == nullptr);
        if (blackboard->has_var(skill_action->get_name()))
        {
            godot::UtilityFunctions::push_warning(vformat("%s: Multiple attacks with the same name detected: %s", this->get_name(), skill_action->get_name()));
        }
        blackboard->set_var(skill_action->get_name(), skill_action);
    }

    ERR_FAIL_NULL(this->blackboard);
    Node* player = get_tree()->get_first_node_in_group("player");
    this->blackboard->set_var("target", player);
}