#ifndef AGENT_HPP
#define AGENT_HPP

#include <godot_cpp/classes/animation_player.hpp>
#include <godot_cpp/classes/character_body3d.hpp>
#include <godot_cpp/classes/navigation_agent3d.hpp>

#include "behaviour_tree/behaviour_tree.hpp"
#include "components/area3d/hurtbox.hpp"
#include "components/health/health_component.hpp"

/**
 * @brief Agent class to serve as base for all AI driven npc characters. Fields are either set by adding a
 * child to the node with the default name and proper class or by force setting them as properties.
 * Force setting properties overrides the default child pick.
 */
class Agent : public godot::CharacterBody3D
{
    GDCLASS(Agent, godot::CharacterBody3D)

protected:
    Hurtbox* hurtbox = nullptr;
    HealthComponent* health_component = nullptr;
    godot::AnimationPlayer* animation_player = nullptr;
    godot::NavigationAgent3D* navigation_agent = nullptr;
    BehaviourTree* behaviour_tree = nullptr;
    godot::Ref<Blackboard> blackboard = memnew(Blackboard);

public:
    CREATE_GETTER_SETTER_DEFAULT(Hurtbox*, hurtbox);
    CREATE_GETTER_SETTER_DEFAULT(HealthComponent*, health_component);
    CREATE_GETTER_SETTER_DEFAULT(godot::AnimationPlayer*, animation_player);
    CREATE_GETTER_SETTER_DEFAULT(godot::NavigationAgent3D*, navigation_agent);
    CREATE_GETTER_SETTER_DEFAULT(BehaviourTree*, behaviour_tree);

    void _ready() override;

protected:
    static void _bind_methods();
};

#endif /* AGENT_HPP */