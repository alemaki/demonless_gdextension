#ifndef BT_LOOK_AT_POSITION
#define BT_LOOK_AT_POSITION

#include "behaviour_tree/tasks/bt_action.hpp"

class BTLookAtPosition : public BTAction
{
    GDCLASS(BTLookAtPosition, BTAction);

    godot::StringName look_at_position_var;
    godot::Vector3 up_vector = godot::Vector3(0, 1, 0);

public:
    CREATE_GETTER_SETTER_DEFAULT(godot::StringName, look_at_position_var);
    CREATE_GETTER_SETTER_DEFAULT(godot::Vector3, up_vector);

protected:
    virtual BTTask::Status _tick(double delta) override;

public:

protected:
    static void _bind_methods();

};

#endif // BT_LOOK_AT_POSITION