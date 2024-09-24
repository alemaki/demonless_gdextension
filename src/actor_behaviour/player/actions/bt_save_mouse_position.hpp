#ifndef BT_SAVE_MOUSE_POSITION_HPP
#define BT_SAVE_MOUSE_POSITION_HPP

#include "behaviour_tree/tasks/bt_action.hpp"

class BTSaveMousePosition : public BTAction
{
    GDCLASS(BTSaveMousePosition, BTAction);

private:
    godot::StringName mouse_position_var;

public:
    CREATE_GETTER_SETTER_DEFAULT(godot::StringName, mouse_position_var);

protected:
    virtual BTTask::Status _tick(double delta) override;

protected:
    static void _bind_methods();

};

#endif // BT_SAVE_MOUSE_POSITION_HPP
