#ifndef BT_EXECUTE_ATTACK_HPP
#define BT_EXECUTE_ATTACK_HPP

#include "behaviour_tree/tasks/bt_action.hpp"

class BTExecuteAttack : public BTAction
{
    GDCLASS(BTExecuteAttack, BTAction);

    godot::StringName attack_to_execute;

public:
    CREATE_GETTER_SETTER_DEFAULT(godot::StringName, attack_to_execute);

protected:
    virtual BTTask::Status _tick(double delta) override;

public:

protected:
    static void _bind_methods();

};

#endif // BT_EXECUTE_ATTACK_HPP