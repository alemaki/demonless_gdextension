#include "bt_shoot_linear_projectile.hpp"

#include <godot_cpp/classes/scene_tree.hpp>
#include "behaviour_tree/utils/utils.hpp"

void BTShootLinearProjectile::_setup()
{
    LinearProjectile* projectile = utils::load_scene_node<LinearProjectile>(linear_projectile_scene_path);
    ERR_FAIL_COND_EDMSG(projectile == nullptr, "Something went wrong loading: " + linear_projectile_scene_path);
    this->linear_projectile = projectile;
}

BTTask::Status BTShootLinearProjectile::_tick(double delta)
{
    LinearProjectile* projectile = godot::Object::cast_to<LinearProjectile>(this->linear_projectile->duplicate());

    TASK_FAIL_COND_MSG(projectile == nullptr, "Cannot duplicate linear projectile.");

    godot::Node2D* actor = godot::Object::cast_to<godot::Node2D>(this->get_actor());
    projectile->set_position(actor->get_position());
    projectile->set_direction(godot::Vector2(1, 0));

    /* TODO: should bullet be actor's child. */
    actor->add_child(projectile);

    return BTTask::Status::SUCCESS;
}

void BTShootLinearProjectile::_bind_methods()
{
    using namespace godot;

    BIND_GETTER_SETTER_PROPERTY_DEFAULT(BTShootLinearProjectile, STRING, linear_projectile_scene_path);

}