#ifndef WAVE_ATTACK_HPP
#define WAVE_ATTACK_HPP

#include "godot_cpp/classes/packed_scene.hpp"
#include "projectiles/projectile.hpp"
#include "skills/skill_action.hpp"

class WaveAttack : public SkillAction
{
    GDCLASS(WaveAttack, SkillAction)

protected:
    godot::Ref<godot::PackedScene> projectile_scene = nullptr;
    Projectile* projectile = nullptr;
    double wave_cooldown = 1;
    int waves = 1;
    int waves_fired = 0;
    int projectiles_per_wave = 0;
    godot::Vector3 direction = {1, 0, 0};

public:
    CREATE_GETTER_SETTER_DEFAULT(godot::Ref<godot::PackedScene>, projectile_scene);
    /* Attack will take ownership of the projectile in question. TODO: is this ok? */
    CREATE_GETTER_SETTER_DEFAULT(Projectile*, projectile);
    CREATE_GETTER_SETTER_POSITIVE_DEFAULT(double, wave_cooldown);
    CREATE_GETTER_SETTER_POSITIVE_DEFAULT(int, waves);
    CREATE_GETTER_SETTER_POSITIVE_DEFAULT(int, projectiles_per_wave);

    void set_direction(const godot::Vector3 direction);
    _FORCE_INLINE_ godot::Vector3 get_direction() const
    {
        return this->direction;
    }

    _FORCE_INLINE_ int get_waves_fired() const
    {
        return this->waves_fired;
    }



protected:
    virtual void _reset() override;

public:
    virtual void _ready() override;
    virtual ~WaveAttack();

protected:
    static void _bind_methods();
};

#endif