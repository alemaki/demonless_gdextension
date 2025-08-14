#include "projectile_time_jump.hpp"
#include "projectiles/projectile.hpp"

void ProjectileTimeJump::_step(double delta)
{
    godot::Node* scene_root = ::get_scene_root();
    godot::TypedArray<godot::Node> projectiles = get_all_children_of_type<Projectile>(scene_root);
    for (int i = 0, size = projectiles.size(); i < size; i++)
    {
        Projectile* projectile = godot::Object::cast_to<Projectile>(projectiles[i]);
        MovementStrategy* move_strat = projectile->get_movement_strategy();
        godot::Ref<MovementContext> move_ctx = projectile->get_movement_context();
        move_strat->apply(move_ctx, this->time_skip); // TODO: is movement strategy responsible for moving the projectile? - probably yes.
        projectile->set_movement_context(move_ctx);
    }
    /* End the skill instantly */
    this->duration = 0;
}

void ProjectileTimeJump::_bind_methods()
{
    BIND_GETTER_SETTER_PROPERTY_DEFAULT(ProjectileTimeJump, FLOAT, time_skip);
}