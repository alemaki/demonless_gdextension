#include "enemy.hpp"

void Enemy::_ready()
{
    utils::ensure_node(this->skill_action_manager, this, "SkillActionManager");
}