#ifndef BT_CHARACTER_INPUT_PRESSED_HPP
#define BT_CHARACTER_INPUT_PRESSED_HPP

#include "behaviour_tree/tasks/bt_condition.hpp"

class BTCharacterInputPressed : public BTCondition
{
    GDCLASS(BTCharacterInputPressed, BTCondition);

private: 
    bool check_attack_pressed = false;
    bool check_block_pressed = false;
    bool check_direction_pressed = false;

protected:
    virtual BTTask::Status _tick(double delta) override;

public:
    CREATE_GETTER_SETTER_DEFAULT(bool, check_attack_pressed);
    CREATE_GETTER_SETTER_DEFAULT(bool, check_block_pressed);
    CREATE_GETTER_SETTER_DEFAULT(bool, check_direction_pressed);

protected:
    static void _bind_methods();
};

#endif /* BT_CHARACTER_INPUT_PRESSED_HPP */