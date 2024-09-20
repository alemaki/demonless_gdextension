#ifndef BT_DESPAWN_HITBOX
#define BT_DESPAWN_HITBOX

#include "behaviour_tree/tasks/bt_action.hpp"

class BTDespawnHitbox : public BTAction
{
    GDCLASS(BTDespawnHitbox, BTAction);

    godot::StringName var_hitbox;

public:
    CREATE_GETTER_SETTER_DEFAULT(godot::StringName, var_hitbox);

protected:
    virtual BTTask::Status _tick(double delta) override;

public:

protected:
    static void _bind_methods();

};

#endif // BT_DESPAWN_HITBOX
