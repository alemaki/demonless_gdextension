#include "agent.hpp"

void Agent::_ready()
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

void Agent::_bind_methods()
{
    BIND_GETTER_SETTER_PROPERTY_OBJECT_DEFAULT(
        Agent, animation_player, PROPERTY_HINT_NODE_TYPE, "AnimationPlayer",
        PROPERTY_USAGE_DEFAULT, AnimationPlayer);

    BIND_GETTER_SETTER_PROPERTY_OBJECT_DEFAULT(
        Agent, navigation_agent, PROPERTY_HINT_NODE_TYPE, "NavigationAgent3D",
        PROPERTY_USAGE_DEFAULT, NavigationAgent3D);

    BIND_GETTER_SETTER_PROPERTY_OBJECT_DEFAULT(
        Agent, health_component, PROPERTY_HINT_NODE_TYPE, "HealthComponent",
        PROPERTY_USAGE_DEFAULT, HealthComponent);

    BIND_GETTER_SETTER_PROPERTY_OBJECT_DEFAULT(
        Agent, hurtbox, PROPERTY_HINT_NODE_TYPE, "Hurtbox",
        PROPERTY_USAGE_DEFAULT, Hurtbox);

    BIND_GETTER_SETTER_PROPERTY_OBJECT_DEFAULT(
        Agent, behaviour_tree, PROPERTY_HINT_NODE_TYPE, "BehaviourTree",
        PROPERTY_USAGE_DEFAULT, BehaviourTree);
}