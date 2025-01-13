#ifndef CIRCLE_WAVE_ATTACK_HPP
#define CIRCLE_WAVE_ATTACK_HPP

#include "projectiles/projectile.hpp"
#include "attacks/attack.hpp"

class CircleWaveAttack : public Attack
{
    GDCLASS(CircleWaveAttack, Attack)

private:
    Projectile* projectile; 
    double shot_cooldown = 1;
    int shots = 0;
    int bullets_per_shot = 0;
    double direction = 0;

public:
    CREATE_GETTER_SETTER_DEFAULT(Projectile*, projectile);
    CREATE_GETTER_SETTER_POSITIVE_DEFAULT(double, shot_cooldown);
    CREATE_GETTER_SETTER_POSITIVE_DEFAULT(int, shots);
    CREATE_GETTER_SETTER_POSITIVE_DEFAULT(int, bullets_per_shot);
    CREATE_GETTER_SETTER_POSITIVE_DEFAULT(double, direction);

protected:
    /* virtual void _reset() override; */
    virtual void _step(double delta) override;

protected:
    static void _bind_methods();
};

#endif /* ATTACK_HPP */