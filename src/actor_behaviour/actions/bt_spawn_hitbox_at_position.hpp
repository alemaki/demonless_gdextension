#ifndef BT_SPAWN_HITBOX_AT_POSITION_HPP
#define BT_SPAWN_HITBOX_AT_POSITION_HPP

#include "behaviour_tree/tasks/bt_action.hpp"
#include "components/area3d/hitbox.hpp"

class BTSpawnHitboxAtPosition : public BTAction
{
    GDCLASS(BTSpawnHitboxAtPosition, BTAction);

    Hitbox* hitbox = nullptr;

    godot::StringName var_global_position;
    godot::StringName var_look_at_pos;
    godot::String path_to_attack_hitbox;
    godot::StringName hitbox_var;

public:
    CREATE_GETTER_SETTER_DEFAULT(godot::StringName, var_global_position);
    CREATE_GETTER_SETTER_DEFAULT(godot::StringName, var_look_at_pos);
    CREATE_GETTER_SETTER_STRING_DEFAULT(path_to_attack_hitbox);
    CREATE_GETTER_SETTER_DEFAULT(godot::StringName, hitbox_var);

protected:
    virtual void _setup() override;
    virtual BTTask::Status _tick(double delta) override;

public:

protected:
    static void _bind_methods();

};

#endif // BT_SPAWN_HITBOX_AT_POSITION_HPP
