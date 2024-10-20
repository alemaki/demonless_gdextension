#include "bt_despawn_hitbox.hpp"
#include "components/area3d/hitbox.hpp"

BTTask::Status BTDespawnHitbox::_tick(double delta)
{
    godot::Variant var = this->get_blackboard()->get_var(this->hitbox_var, Variant(), this->is_complain_enabled());
    Hitbox* hitbox = godot::Object::cast_to<Hitbox>(var);
    TASK_FAIL_COND(hitbox == nullptr);
    this->get_actor()->remove_child(hitbox);
    hitbox->queue_free();
    TASK_SUCCEED();
}

void BTDespawnHitbox::_bind_methods()
{
    BIND_GETTER_SETTER_PROPERTY_DEFAULT(BTDespawnHitbox, STRING_NAME, hitbox_var);
}