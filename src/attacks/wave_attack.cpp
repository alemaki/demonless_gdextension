#include "wave_attack.hpp"
#include "godot_cpp/variant/utility_functions.hpp"

void WaveAttack::set_direction(const godot::Vector3 direction)
{
    ERR_FAIL_COND_MSG(direction.is_zero_approx(), "Attack direction cannot be godot::Vector3{0, 0, 0}");
    this->direction = direction;
}

void WaveAttack::_reset()
{
    this->waves_fired = 0;
}


void WaveAttack::_bind_methods()
{
    BIND_GETTER_SETTER_PROPERTY_OBJECT_DEFAULT(WaveAttack, projectile, PROPERTY_HINT_NODE_TYPE, "Projectile", PROPERTY_USAGE_DEFAULT, Projectile);
    BIND_GETTER_SETTER_PROPERTY_DEFAULT(WaveAttack, INT, projectiles_per_wave);
    BIND_GETTER_SETTER_PROPERTY_DEFAULT(WaveAttack, INT, waves);
    BIND_GETTER_SETTER_PROPERTY_DEFAULT(WaveAttack, FLOAT, wave_cooldown);
    BIND_GETTER_SETTER_PROPERTY_DEFAULT(WaveAttack, VECTOR3, direction);
}