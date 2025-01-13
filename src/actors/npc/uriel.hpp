#ifndef URIEL_HPP
#define URIEL_HPP

#include <godot_cpp/classes/character_body3d.hpp>

class Uriel : public godot::CharacterBody3D
{
    GDCLASS(Uriel, godot::CharacterBody3D)

private:

public:

protected:
    static void _bind_methods(){};
};

#endif URIEL_HPP