#ifndef ACTOR_HPP
#define ACTOR_HPP

#include <godot_cpp/classes/animation_player.hpp>
#include <godot_cpp/classes/character_body3d.hpp>
#include <godot_cpp/classes/navigation_agent3d.hpp>

#include "behaviour_tree/behaviour_tree.hpp"
#include "components/area3d/hurtbox.hpp"
#include "components/health/health_component.hpp"

/**
 * @brief Actor class to serve as base for all npc characters. Fields are either set by adding a
 * child to the node with the default name and proper class or by force setting them as properties.
 * Force setting properties overrides the default child pick.
 */
class Actor : public godot::CharacterBody3D
{
    GDCLASS(Actor, godot::CharacterBody3D)

protected:
    Hurtbox* hurtbox = nullptr;
    HealthComponent* health_component = nullptr;
    godot::AnimationPlayer* animation_player = nullptr;
    godot::NavigationAgent3D* navigation_agent = nullptr;
    BehaviourTree* behaviour_tree = nullptr;

public:
    CREATE_GETTER_SETTER_DEFAULT(Hurtbox*, hurtbox);
    CREATE_GETTER_SETTER_DEFAULT(HealthComponent*, health_component);
    CREATE_GETTER_SETTER_DEFAULT(godot::AnimationPlayer*, animation_player);
    CREATE_GETTER_SETTER_DEFAULT(godot::NavigationAgent3D*, navigation_agent);
    CREATE_GETTER_SETTER_DEFAULT(BehaviourTree*, behaviour_tree);

    void _ready() override
    {
        if (!animation_player)
        {
            animation_player = godot::Object::cast_to<godot::AnimationPlayer>(
                get_node_or_null("AnimationPlayer"));
        }
        if (!navigation_agent)
        {
            navigation_agent = godot::Object::cast_to<godot::NavigationAgent3D>(
                get_node_or_null("NavigationAgent3D"));
        }
        if (!health_component)
        {
            health_component = godot::Object::cast_to<HealthComponent>(
                get_node_or_null("HealthComponent"));
        }
        if (!hurtbox)
        {
            hurtbox = godot::Object::cast_to<Hurtbox>(
                get_node_or_null("Hurtbox"));
        }
        if (!behaviour_tree)
        {
            behaviour_tree = godot::Object::cast_to<BehaviourTree>(
                get_node_or_null("BehaviourTree"));
        }
    }

protected:
    static void _bind_methods()
    {
        BIND_GETTER_SETTER_PROPERTY_OBJECT_DEFAULT(
            Actor, animation_player, PROPERTY_HINT_NODE_TYPE, "AnimationPlayer",
            PROPERTY_USAGE_DEFAULT, AnimationPlayer);

        BIND_GETTER_SETTER_PROPERTY_OBJECT_DEFAULT(
            Actor, navigation_agent, PROPERTY_HINT_NODE_TYPE, "NavigationAgent3D",
            PROPERTY_USAGE_DEFAULT, NavigationAgent3D);

        BIND_GETTER_SETTER_PROPERTY_OBJECT_DEFAULT(
            Actor, health_component, PROPERTY_HINT_NODE_TYPE, "HealthComponent",
            PROPERTY_USAGE_DEFAULT, HealthComponent);

        BIND_GETTER_SETTER_PROPERTY_OBJECT_DEFAULT(
            Actor, hurtbox, PROPERTY_HINT_NODE_TYPE, "Hurtbox",
            PROPERTY_USAGE_DEFAULT, Hurtbox);

        BIND_GETTER_SETTER_PROPERTY_OBJECT_DEFAULT(
            Actor, behaviour_tree, PROPERTY_HINT_NODE_TYPE, "BehaviourTree",
            PROPERTY_USAGE_DEFAULT, BehaviourTree);
    }
};

#endif /* ACTOR_HPP */