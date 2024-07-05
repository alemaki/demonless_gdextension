#include "player_character.hpp"

void PlayerCharacter::set_health_component(HealthComponent* health_component)
{
    this->health_component = health_component;
}

void PlayerCharacter::set_input_component(CharacterInputComponent* input_component)
{
    this->input_component = input_component;
}


void PlayerCharacter::_process(double delta)
{

}

void PlayerCharacter::_physics_process(double delta)
{
    
}

void PlayerCharacter::_bind_methods()
{
    
}