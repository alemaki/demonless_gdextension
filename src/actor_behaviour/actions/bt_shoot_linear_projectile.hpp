#ifndef BT_SHOOT_LINEAR_PROJECTILE_HPP
#define BT_SHOOT_LINEAR_PROJECTILE_HPP

#include "behaviour_tree/tasks/bt_action.hpp"
#include "projectiles/linear_projectile.hpp"

class BTShootLinearProjectile : public BTAction
{
    GDCLASS(BTShootLinearProjectile, BTAction);

private: 
    godot::String linear_projectile_scene_path;
    LinearProjectile* linear_projectile;

protected:
    virtual BTTask::Status _tick(double delta) override;
    virtual void _setup() override;
public:
    CREATE_GETTER_SETTER_DEFAULT(const godot::String&, linear_projectile_scene_path);

protected:
    static void _bind_methods();

};

#endif // BT_SHOOT_LINEAR_PROJECTILE_HPP