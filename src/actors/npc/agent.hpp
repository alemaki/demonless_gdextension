#ifndef AGENT_HPP
#define AGENT_HPP

#include <godot_cpp/classes/animation_player.hpp>
#include <godot_cpp/classes/navigation_agent3d.hpp>

#include "actors/combatant.hpp"
#include "behaviour_tree/behaviour_tree.hpp"

/**
 * @brief Agent class to serve as base for all AI driven npc characters. Fields are either set by adding a
 * child to the node with the default name and proper class or by force setting them as properties.
 * Force setting properties overrides the default child pick.
 */
class Agent : public Combatant
{
    GDCLASS(Agent, Combatant)

protected:
    godot::AnimationPlayer* animation_player = nullptr;
    godot::NavigationAgent3D* navigation_agent = nullptr;
    BehaviourTree* behaviour_tree = nullptr;
    godot::Ref<Blackboard> blackboard = memnew(Blackboard);
    godot::Ref<BTTask> beh_task = nullptr;

public:
    CREATE_GETTER_SETTER_DEFAULT(godot::AnimationPlayer*, animation_player);
    CREATE_GETTER_SETTER_DEFAULT(godot::NavigationAgent3D*, navigation_agent);
    CREATE_GETTER_SETTER_DEFAULT(BehaviourTree*, behaviour_tree);

    void _ready() override;
    void _physics_process(double delta) override;

protected:
    static void _bind_methods();
};

#endif /* AGENT_HPP */