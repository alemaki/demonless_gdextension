#ifndef CIRCLE_WAVE_ATTACK_HPP
#define CIRCLE_WAVE_ATTACK_HPP

#include "projectiles/projectile.hpp"
#include "attacks/wave_attack.hpp"

class CircleWaveAttack : public WaveAttack
{
    GDCLASS(CircleWaveAttack, WaveAttack)

protected:
    virtual void _step(double delta) override;

protected:
    static void _bind_methods();
};

#endif