#include "agent.hpp"

void Agent::_ready()
{
    DISABLE_EDITOR_PROCESSING();

    utils::ensure_node(this->animation_player, this, "AnimationPlayer");
    utils::ensure_node(this->navigation_agent, this, "NavigationAgent3D");
    utils::ensure_node(this->health_component, this, "HealthComponent");
    utils::ensure_node(this->hurtbox, this, "Hurtbox");
    utils::ensure_node(this->behaviour_tree, this, "BehaviourTree");

    if (this->behaviour_tree)
    {
        this->beh_task = this->behaviour_tree->instantiate(this, this->blackboard);
    }
}

void Agent::_physics_process(double delta)
{
    ERR_FAIL_NULL(this->beh_task);
    this->beh_task->execute(delta);
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