#ifndef DIRECTIONAL_WAVE_ATTACK_HPP
#define DIRECTIONAL_WAVE_ATTACK_HPP

#include "projectiles/projectile.hpp"
#include "attacks/wave_attack.hpp"

class DirectionalWaveAttack : public WaveAttack
{
    GDCLASS(DirectionalWaveAttack, WaveAttack)

    double angle_step = 0;

public:
    CREATE_GETTER_SETTER_POSITIVE_DEFAULT(double, angle_step);

protected:
    virtual void _step(double delta) override;
    static void _bind_methods();
};

#endif