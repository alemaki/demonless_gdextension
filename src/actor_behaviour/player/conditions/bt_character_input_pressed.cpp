#include "bt_character_input_pressed.hpp"

#include "actors/player/player_character.hpp"

BTTask::Status BTCharacterInputPressed::_tick(double delta)
{
    bool is_attack_pressed = this->get_blackboard()->get_var("is_attack_pressed", godot::Variant(), this->complain);
    bool is_block_pressed = this->get_blackboard()->get_var("is_block_pressed", godot::Variant(), this->complain);
    godot::Vector2 direction_input =  this->get_blackboard()->get_var("direction_input", godot::Variant(), this->complain);

    TASK_SUCCEED_COND((is_attack_pressed && this->check_attack_pressed)             
                      || (is_block_pressed && this->check_block_pressed)
                      || ((direction_input != godot::Vector2(0, 0)) && this->check_direction_pressed));

    TASK_FAIL();
}

void BTCharacterInputPressed::set_check_attack_pressed(bool check_attack_pressed)
{
    this->check_attack_pressed = check_attack_pressed;
}

void BTCharacterInputPressed::set_check_block_pressed(bool check_block_pressed)
{
    this->check_block_pressed = check_block_pressed;
}

void BTCharacterInputPressed::set_check_direction_pressed(bool check_direction_pressed)
{
    this->check_direction_pressed = check_direction_pressed;
}

void BTCharacterInputPressed::set_complain(bool complain)
{
    this->complain = complain;
}

void BTCharacterInputPressed::_bind_methods()
{
    using namespace godot;

    ClassDB::bind_method(D_METHOD("set_check_attack_pressed", "check_attack_pressed"), &BTCharacterInputPressed::set_check_attack_pressed);
    ClassDB::bind_method(D_METHOD("get_check_attack_pressed"), &BTCharacterInputPressed::get_check_attack_pressed);
    ClassDB::bind_method(D_METHOD("set_check_block_pressed", "check_block_pressed"), &BTCharacterInputPressed::set_check_block_pressed);
    ClassDB::bind_method(D_METHOD("get_check_block_pressed"), &BTCharacterInputPressed::get_check_block_pressed);
    ClassDB::bind_method(D_METHOD("set_check_direction_pressed", "check_direction_pressed"), &BTCharacterInputPressed::set_check_direction_pressed);
    ClassDB::bind_method(D_METHOD("get_check_direction_pressed"), &BTCharacterInputPressed::get_check_direction_pressed);
    ClassDB::bind_method(D_METHOD("set_complain", "complain"), &BTCharacterInputPressed::set_complain);
    ClassDB::bind_method(D_METHOD("get_complain"), &BTCharacterInputPressed::get_complain);


    ADD_PROPERTY(PropertyInfo(Variant::STRING, "check_attack_pressed"), "set_check_attack_pressed", "get_check_attack_pressed");
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "check_block_pressed"), "set_check_block_pressed", "get_check_block_pressed");
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "check_direction_pressed"), "set_check_direction_pressed", "get_check_direction_pressed");
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "complain"), "set_complain", "get_complain");
}