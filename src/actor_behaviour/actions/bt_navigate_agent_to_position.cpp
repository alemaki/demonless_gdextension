#include "bt_navigate_agent_to_position.hpp"
#include "actors/npc/agent.hpp"

BTTask::Status BTNavigateAgentToPosition::_tick(double delta)
{
    Agent* agent = godot::Object::cast_to<Agent>(this->get_actor());

    TASK_FAIL_COND_COMP_MSG(
        agent == nullptr,
        vformat("%s is not an Agent as expected.", this->get_actor())
    );

    TASK_FAIL_COND_COMP_MSG(
        agent->get_navigation_agent() == nullptr,
        vformat("Agent has no NavigationAgent3D.", this->get_actor())
    );

    TASK_SUCCEED_COND(agent->get_navigation_agent()->is_navigation_finished());

    Vector3 direction = agent->get_navigation_agent()->get_next_path_position() - agent->get_global_position();
    /* will not be 0vec */
    direction.normalize();
    agent->set_velocity(direction * speed);
    agent->move_and_slide();

    TASK_RUNNING();
}

void BTNavigateAgentToPosition::_enter()
{
    ERR_FAIL_COND_MSG(
        !(this->get_blackboard()->has_var(this->position_name)),
        vformat("Blackboard has no value named: %s", this->position_name)
    );

    Vector3 position = this->get_blackboard()->get_var(this->position_name);

    Agent* agent = godot::Object::cast_to<Agent>(this->get_actor());

    ERR_FAIL_COND_MSG(
        agent == nullptr,
        vformat("%s is not an Agent as expected.", this->get_actor())
    );

    agent->get_navigation_agent()->set_target_position(position);
}

void BTNavigateAgentToPosition::_bind_methods()
{
    using namespace godot;

    BIND_GETTER_SETTER_PROPERTY_DEFAULT(BTNavigateAgentToPosition, STRING_NAME, position_name);
    BIND_GETTER_SETTER_PROPERTY_DEFAULT(BTNavigateAgentToPosition, FLOAT, speed);
}
