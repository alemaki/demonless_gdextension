#ifndef ATTACK_HPP
#define ATTACK_HPP

#include <godot_cpp/classes/character_body3d.hpp>
#include "utils/utils.hpp"

class Attack : public godot::Node
{
    GDCLASS(Attack, godot::Node)

private:
    double attack_duration;

protected:
    virtual void _reset(){};

public:
    CREATE_GETTER_SETTER_POSITIVE_DEFAULT(double, attack_duration);

    void reset();

protected:
    static void _bind_methods(){};
};

#endif ATTACK_HPP