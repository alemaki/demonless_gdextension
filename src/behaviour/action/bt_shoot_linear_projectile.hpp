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
    void set_linear_projectile_scene_path(const godot::String& linear_projectile_scene_path);
    _FORCE_INLINE_ godot::String get_linear_projectile_scene_path()
    {
        return linear_projectile_scene_path;
    }

protected:
    static void _bind_methods();

};

#endif // BT_SHOOT_LINEAR_PROJECTILE_HPP