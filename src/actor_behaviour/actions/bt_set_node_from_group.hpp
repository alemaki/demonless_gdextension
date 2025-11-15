#ifndef BT_SET_NODE_FROM_GROUP_HPP
#define BT_SET_NODE_FROM_GROUP_HPP

#include "behaviour_tree/tasks/bt_action.hpp"

class BTSetNodeFromGroup : public BTAction
{
    GDCLASS(BTSetNodeFromGroup, BTAction);

    godot::StringName group_name;
    godot::StringName node_name;
    godot::StringName set_name;

public:
    CREATE_GETTER_SETTER_DEFAULT(godot::StringName, group_name);
    CREATE_GETTER_SETTER_DEFAULT(godot::StringName, node_name);
    CREATE_GETTER_SETTER_DEFAULT(godot::StringName, set_name);

protected:
    virtual BTTask::Status _tick(double delta) override;

public:

protected:
    static void _bind_methods();

};

#endif // BT_SET_NODE_FROM_GROUP_HPP