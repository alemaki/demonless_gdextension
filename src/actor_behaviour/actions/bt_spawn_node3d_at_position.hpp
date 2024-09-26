#ifndef BT_SPAWN_NODE_AT_POSITION_HPP
#define BT_SPAWN_NODE_AT_POSITION_HPP

#include "behaviour_tree/tasks/bt_action.hpp"
#include <godot_cpp/classes/node3d.hpp>

class BTSpawnNode3DAtPosition : public BTAction
{
    GDCLASS(BTSpawnNode3DAtPosition, BTAction);

    Node3D* node3d = nullptr;

    godot::StringName var_global_position;
    godot::String path_to_node3d;
    godot::StringName node3d_var;
    bool child_of_actor = false;

public:
    CREATE_GETTER_SETTER_DEFAULT(godot::StringName, var_global_position);
    CREATE_GETTER_SETTER_STRING_DEFAULT(path_to_node3d);
    CREATE_GETTER_SETTER_DEFAULT(godot::StringName, node3d_var);
    CREATE_GETTER_SETTER_BOOL_DEFAULT(child_of_actor);

protected:
    virtual void _setup() override;
    virtual BTTask::Status _tick(double delta) override;

public:

protected:
    static void _bind_methods();

};

#endif // BT_SPAWN_NODE_AT_POSITION_HPP
