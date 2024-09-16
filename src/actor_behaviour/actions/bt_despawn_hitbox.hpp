#ifndef BT_DESPAWN_HITBOX_AT_POSITION
#define BT_DESPAWN_HITBOX_AT_POSITION

#include "behaviour_tree/tasks/bt_action.hpp"

class BTDespawnHitboxAtPosition : public BTAction
{
    GDCLASS(BTDespawnHitboxAtPosition, BTAction);

    godot::StringName var_hitbox;

protected:
    CREATE_GETTER_SETTER_DEFAULT(godot::StringName, var_hitbox);

    virtual BTTask::Status _tick(double delta) override;

public:

protected:
    static void _bind_methods();

};

#endif // BT_DESPAWN_HITBOX_AT_POSITION
