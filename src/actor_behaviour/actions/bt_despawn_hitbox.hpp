#ifndef BT_DESPAWN_HITBOX_HPP
#define BT_DESPAWN_HITBOX_HPP

#include "behaviour_tree/tasks/bt_action.hpp"

class BTDespawnHitbox : public BTAction
{
    GDCLASS(BTDespawnHitbox, BTAction);

    godot::StringName hitbox_var;

public:
    CREATE_GETTER_SETTER_DEFAULT(godot::StringName, hitbox_var);

protected:
    virtual BTTask::Status _tick(double delta) override;

public:

protected:
    static void _bind_methods();

};

#endif // BT_DESPAWN_HITBOX_HPP
