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
    bool complain = true;

protected:
    virtual BTTask::Status _tick(double delta) override;

public:
    void set_check_attack_pressed(bool check_attack_pressed);
    _FORCE_INLINE_ bool get_check_attack_pressed()
    {
        return this->check_attack_pressed;
    }
    void set_check_block_pressed(bool check_block_pressed);
    _FORCE_INLINE_ bool get_check_block_pressed()
    {
        return this->check_block_pressed;
    }
    void set_check_direction_pressed(bool check_direction_pressed);
    _FORCE_INLINE_ bool get_check_direction_pressed()
    {
        return this->check_direction_pressed;
    }
    void set_complain(bool complain);
    _FORCE_INLINE_ bool get_complain()
    {
        return this->complain;
    }

protected:
    static void _bind_methods();
};

#endif /* BT_CHARACTER_INPUT_PRESSED_HPP */