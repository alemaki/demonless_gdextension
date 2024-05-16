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
    godot::Node2D* actor = godot::Object::cast_to<godot::Node2D>(this->get_actor());
    projectile->set_position(actor->get_position());
    projectile->set_direction(godot::Vector2(1, 0));

    godot::Node* parent = actor->get_parent();

    if (parent == nullptr)
    {
        godot::UtilityFunctions::printerr("Actor has not parent.");
        return BTTask::Status::FAILURE;
    }

    parent->add_child(projectile);

    return BTTask::Status::SUCCESS;
}

void BTShootLinearProjectile::set_linear_projectile_scene_path(const godot::String& linear_projectile_scene_path)
{
    this->linear_projectile_scene_path = linear_projectile_scene_path;
}

void BTShootLinearProjectile::_bind_methods()
{
    using namespace godot;

    ClassDB::bind_method(D_METHOD("set_linear_projectile_scene_path", "linear_projectile_scene_path"), &BTShootLinearProjectile::set_linear_projectile_scene_path);
    ClassDB::bind_method(D_METHOD("get_linear_projectile_scene_path"), &BTShootLinearProjectile::get_linear_projectile_scene_path);

    ADD_PROPERTY(PropertyInfo(Variant::STRING, "projectile_scene"), "set_linear_projectile_scene_path", "get_linear_projectile_scene_path");
}