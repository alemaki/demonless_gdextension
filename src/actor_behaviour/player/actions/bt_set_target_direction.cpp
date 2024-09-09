#include "bt_set_target_direction.hpp"
#include "actors/player/player_character.hpp"

BTTask::Status BTSetTargetDirection::_tick(double delta)
{
    PlayerCharacter* player = godot::Object::cast_to<PlayerCharacter>(this->get_actor());
    TASK_FAIL_COND_COMP_MSG(player == nullptr,"Actor is not a player character.");

    CharacterMovementComponent* movement_cp = player->get_movement_component();
    TASK_FAIL_COND_COMP_MSG(movement_cp == nullptr, "Player doesn't have a movement component.");

    movement_cp->set_target_direction(godot::Vector3(this->target_direction.x, 0, this->target_direction.y));
    TASK_SUCCEED();
}

void BTSetTargetDirection::_bind_methods()
{
    using namespace godot;

    BIND_GETTER_SETTER_PROPERTY_DEFAULT(BTSetTargetDirection, VECTOR2, target_direction); 
}