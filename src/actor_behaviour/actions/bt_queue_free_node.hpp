#ifndef BT_QUEUE_FREE_NODE_HPP
#define BT_QUEUE_FREE_NODE_HPP

#include "behaviour_tree/tasks/bt_action.hpp"

class BTQueueFreeNode : public BTAction
{
    GDCLASS(BTQueueFreeNode, BTAction);

    godot::StringName var_node;

public:
    CREATE_GETTER_SETTER_DEFAULT(godot::StringName, var_node);

protected:
    virtual BTTask::Status _tick(double delta) override;

public:

protected:
    static void _bind_methods();

};

#endif // BT_QUEUE_FREE_NODE_HPP
