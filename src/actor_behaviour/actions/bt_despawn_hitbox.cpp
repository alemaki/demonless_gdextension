#include "bt_despawn_hitbox.hpp"
#include "components/area3d/hitbox.hpp"

BTTask::Status BTDespawnHitboxAtPosition::_tick(double delta)
{
    godot::Variant var = this->get_blackboard()->get_var(this->var_hitbox, Variant(), this->is_complain_enabled());
    Hitbox* hitbox = godot::Object::cast_to<Hitbox>(var);
    TASK_FAIL_COND(hitbox == nullptr);
    this->get_actor()->remove_child(hitbox);
    hitbox->queue_free();
    TASK_SUCCEED();
}

void BTDespawnHitboxAtPosition::_bind_methods()
{
    BIND_GETTER_SETTER_PROPERTY_DEFAULT(BTDespawnHitboxAtPosition, STRING_NAME, var_hitbox);
}