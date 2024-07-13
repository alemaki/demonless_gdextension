#ifndef BT_EVALUATE_MOVEMENT_INPUT
#define BT_EVALUATE_MOVEMENT_INPUT

#include "behaviour_tree/tasks/bt_action.hpp"
#include "projectiles/linear_projectile.hpp"

class BTEvaluateMovementInput : public BTAction
{
    GDCLASS(BTEvaluateMovementInput, BTAction);
    bool complain = true;
protected:
    virtual BTTask::Status _tick(double delta) override;

public:
    void set_complain(bool complain);
    _FORCE_INLINE_ bool get_complain() const
    {
        return this->complain;
    }

protected:
    static void _bind_methods();

};

#endif // BT_EVALUATE_MOVEMENT_INPUT
