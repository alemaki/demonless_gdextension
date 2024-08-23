#ifndef BT_PLAY_ANIMATION_HPP
#define BT_PLAY_ANIMATION_HPP

#include "behaviour_tree/tasks/bt_action.hpp"

class BTPlayAnimation : public BTAction
{
    GDCLASS(BTPlayAnimation, BTAction);

private: 
    godot::StringName animation_player_name;
    godot::StringName play_animation;

protected:
    virtual BTTask::Status _tick(double delta) override;

public:
    CREATE_GETTER_SETTER_STRING_DEFAULT(animation_player_name);
    CREATE_GETTER_SETTER_STRING_DEFAULT(play_animation);

protected:
    static void _bind_methods();

};

#endif // BT_PLAY_ANIMATION_HPP