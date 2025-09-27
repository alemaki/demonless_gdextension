#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "actors/npc/agent.hpp"
#include "skills/skill_action.hpp"

/**
 * @brief Enemy class to serve as base for all enemies in the game. Registers all SkillActions that
 * are children of the node in the blackboard of the Agent Base class for useage of the behavour tree.
 */
class Enemy : public Agent
{
    GDCLASS(Enemy, Agent)

protected:
    godot::TypedArray<SkillAction> attacks; /* currently SkillAction describes all npc attacks. */

public:
    void _ready() override;

protected:
    static void _bind_methods();
};

#endif /* ENEMY_HPP */