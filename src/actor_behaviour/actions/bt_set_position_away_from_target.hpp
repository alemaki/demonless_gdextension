#ifndef BT_SET_POSITION_AWAY_FROM_TARGET_HPP
#define BT_SET_POSITION_AWAY_FROM_TARGET_HPP

#include "behaviour_tree/tasks/bt_action.hpp"

class BTSetPositionAwayFromTarget : public BTAction
{
    GDCLASS(BTSetPositionAwayFromTarget, BTAction);

    godot::StringName position_name;
    godot::StringName target_name;
    double distance = 1; // TODO: extract to another action to set distance?

public:
    CREATE_GETTER_SETTER_DEFAULT(godot::StringName, position_name);
    CREATE_GETTER_SETTER_DEFAULT(godot::StringName, target_name);
    CREATE_GETTER_SETTER_POSITIVE_DEFAULT(double, distance);

protected:
    virtual BTTask::Status _tick(double delta) override;
    virtual void _enter() override;

public:

protected:
    static void _bind_methods();

};

#endif // BT_SET_POSITION_AWAY_FROM_TARGET_HPP