#ifndef PLAYER_CHARACTER_HPP
#define PLAYER_CHARACTER_HPP

#include <godot_cpp/classes/character_body2d.hpp>

class PlayerCharacter : public godot::CharacterBody2D
{
    GDCLASS(PlayerCharacter, CharacterBody2D);

public:
    void _process(double delta) override;
    void _physics_process(double delta) override;

protected: 
    static void _bind_methods();

};

#endif // PLAYER_CHARACTER_HPP