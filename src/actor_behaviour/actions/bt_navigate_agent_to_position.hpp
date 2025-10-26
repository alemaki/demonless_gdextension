#ifndef BT_NAVIGATE_AGENT_TO_POSITON_HPP
#define BT_NAVIGATE_AGENT_TO_POSITON_HPP

#include "behaviour_tree/tasks/bt_action.hpp"

class BTNavigateAgentToPosition : public BTAction
{
    GDCLASS(BTNavigateAgentToPosition, BTAction);

    godot::StringName position_name;
    double speed = 1;

public:
    CREATE_GETTER_SETTER_DEFAULT(godot::StringName, position_name);
    CREATE_GETTER_SETTER_DEFAULT(double, speed);

protected:
    virtual BTTask::Status _tick(double delta) override;
    virtual void _enter() override;

public:

protected:
    static void _bind_methods();

};

#endif // BT_NAVIGATE_AGENT_TO_POSITON_HPP