#include "bt_play_animation.hpp"

#include <godot_cpp/classes/animation_player.hpp>

BTTask::Status BTPlayAnimation::_tick(double delta)
{
    godot::AnimationPlayer* animation_player = godot::Object::cast_to<AnimationPlayer>(this->get_blackboard()->get_var(this->animation_player_name, godot::Variant(), this->is_complain_enabled()));
    TASK_FAIL_COND_COMP_MSG(animation_player == nullptr, "Animation player is not set properly");
    TASK_FAIL_COND_COMP_MSG(!(animation_player->has_animation(this->play_animation)), "Animation player doesn't have animation " + this->play_animation);

    animation_player->play(this->play_animation);

    TASK_SUCCEED();
}


void BTPlayAnimation::_bind_methods()
{
    using namespace godot;

    BIND_GETTER_SETTER_PROPERTY_DEFAULT(BTPlayAnimation, STRING_NAME, animation_player_name);
    BIND_GETTER_SETTER_PROPERTY_DEFAULT(BTPlayAnimation, STRING_NAME, play_animation);
}