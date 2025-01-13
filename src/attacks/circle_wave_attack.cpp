#include "circle_wave_attack.hpp"

void CircleWaveAttack::_step(double delta)
{

}

void CircleWaveAttack::_bind_methods()
{
    BIND_GETTER_SETTER_PROPERTY_OBJECT_DEFAULT(CircleWaveAttack, projectile, PropertyHint::PROPERTY_HINT_NODE_TYPE, "Projectile", PropertyUsageFlags::PROPERTY_USAGE_DEFAULT, Projectile);
    BIND_GETTER_SETTER_PROPERTY_DEFAULT(CircleWaveAttack, Variant::INT, bullets_per_shot);
    BIND_GETTER_SETTER_PROPERTY_DEFAULT(CircleWaveAttack, Variant::INT, shots);
    BIND_GETTER_SETTER_PROPERTY_DEFAULT(CircleWaveAttack, Variant::FLOAT, shot_cooldown);
}