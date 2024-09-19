#include "bt_spawn_hitbox_at_position.hpp"
#include "utils/utils.hpp"

void BTSpawnHitboxAtPosition::_setup()
{

    this->hitbox = utils::load_scene_node<Hitbox>(this->path_to_attack_hitbox);
}

BTTask::Status BTSpawnHitboxAtPosition::_tick(double delta)
{
    TASK_FAIL_COND(this->hitbox == nullptr);
    TASK_COMPLAIN_COND(this->hitbox_var == godot::String(""), "Hitbox_var not set.");

    Hitbox* hitbox = godot::Object::cast_to<Hitbox>(this->hitbox->duplicate());
    ERR_FAIL_COND_V(hitbox == nullptr, BTTask::Status::FAILURE);

    this->get_actor()->add_child(hitbox);

    if (this->get_blackboard()->has_var(this->var_global_position))
    {
        hitbox->set_global_position(this->get_blackboard()->get_var(this->var_global_position));
    }
    if (this->get_blackboard()->has_var(this->var_look_at_pos))
    {
        hitbox->look_at(this->get_blackboard()->get_var(this->var_look_at_pos));
    }

    if (!(this->hitbox_var.is_empty()))
    {
        this->get_blackboard()->set_var(this->hitbox_var, hitbox);
    }

    TASK_SUCCEED();
}

void BTSpawnHitboxAtPosition::_bind_methods()
{
    BIND_GETTER_SETTER_PROPERTY_DEFAULT(BTSpawnHitboxAtPosition, STRING, path_to_attack_hitbox);
    BIND_GETTER_SETTER_PROPERTY_DEFAULT(BTSpawnHitboxAtPosition, STRING_NAME, var_global_position);
    BIND_GETTER_SETTER_PROPERTY_DEFAULT(BTSpawnHitboxAtPosition, STRING_NAME, var_look_at_pos);
    BIND_GETTER_SETTER_PROPERTY_DEFAULT(BTSpawnHitboxAtPosition, STRING_NAME, hitbox_var);
}