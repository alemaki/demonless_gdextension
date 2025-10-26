#ifndef BT_SET_RANDOM_POSITION_HPP
#define BT_SET_RANDOM_POSITION_HPP

#include "behaviour_tree/tasks/bt_action.hpp"

class BTSetRandomPosition : public BTAction
{
    GDCLASS(BTSetRandomPosition, BTAction);

    godot::StringName position_name;
    godot::Vector3 up_vector = godot::Vector3(0, 1, 0);

public:
    CREATE_GETTER_SETTER_DEFAULT(godot::StringName, position_name);
    CREATE_GETTER_SETTER_DEFAULT(godot::Vector3, up_vector);

protected:
    virtual BTTask::Status _tick(double delta) override;

public:

protected:
    static void _bind_methods();

};

#endif // BT_LOOK_AT_POSITION_HPP