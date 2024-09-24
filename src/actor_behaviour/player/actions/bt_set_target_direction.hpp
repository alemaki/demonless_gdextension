#ifndef BT_SET_TARGET_DIRECTION_HPP
#define BT_SET_TARGET_DIRECTION_HPP

#include "behaviour_tree/tasks/bt_action.hpp"

class BTSetTargetDirection : public BTAction
{
    GDCLASS(BTSetTargetDirection, BTAction);

private:
    godot::Vector2 target_direction = godot::Vector2(0,0);

protected:
    virtual BTTask::Status _tick(double delta) override;

public:
    CREATE_GETTER_SETTER_DEFAULT(godot::Vector2, target_direction);

protected:
    static void _bind_methods();

};

#endif // BT_SET_TARGET_DIRECTION_HPP