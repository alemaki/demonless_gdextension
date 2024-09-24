#ifndef BT_EVALUATE_MOVEMENT_INPUT_HPP
#define BT_EVALUATE_MOVEMENT_INPUT_HPP

#include "behaviour_tree/tasks/bt_action.hpp"

class BTEvaluateMovementInput : public BTAction
{
    GDCLASS(BTEvaluateMovementInput, BTAction);

protected:
    virtual BTTask::Status _tick(double delta) override;

protected:
    static void _bind_methods();

};

#endif // BT_EVALUATE_MOVEMENT_INPUT_HPP
