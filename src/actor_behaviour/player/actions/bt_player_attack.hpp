#ifndef BT_PLAYER_ATTACK_HPP
#define BT_PLAYER_ATTACK_HPP

#include "behaviour_tree/tasks/bt_action.hpp"

class BTEvaluateMovementInput : public BTAction
{
    GDCLASS(BTEvaluateMovementInput, BTAction);

protected:
    virtual BTTask::Status _tick(double delta) override;

public:

protected:
    static void _bind_methods();

};

#endif // BT_PLAYER_ATTACK_HPP
