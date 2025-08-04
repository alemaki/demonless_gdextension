#include "circle_wave_attack.hpp"
#include "godot_cpp/variant/utility_functions.hpp"

void CircleWaveAttack::_step(double delta)
{
    ERR_FAIL_NULL(this->actor_source);
    ERR_FAIL_NULL(this->projectile);
    ERR_FAIL_COND(direction.is_zero_approx());
    ERR_FAIL_COND(this->wave_cooldown == 0);

    int waves_to_fire = (this->time_accumulated / this->wave_cooldown) - this->waves_fired;
    ERR_FAIL_COND(waves_to_fire > 3);
    if (waves_to_fire <= 0 || this->waves <= this->waves_fired)
    {
        return;
    }

    double angle_step = 0;
    if (projectiles_per_wave != 0)
    {
        angle_step = 2 * Math_PI / projectiles_per_wave;
    }

    for (int wave = 0; wave < waves_to_fire; wave++)
    {
        for (int projectile_index = 0; projectile_index < projectiles_per_wave; ++projectile_index)
        {
            double angle = projectile_index * angle_step;
            godot::Vector3 dir = this->direction.rotated(VECTOR_UP, angle);
            Projectile* projectile_cpy = Object::cast_to<Projectile>(this->projectile->duplicate());
            ERR_FAIL_NULL(projectile_cpy);
            projectile_cpy->set_position(this->actor_source->get_position());
            projectile_cpy->get_movement_context()->set_direction(dir);
            get_tree()->get_current_scene()->add_child(projectile_cpy);
        }
    }

    this->waves_fired += waves_to_fire;
}

void CircleWaveAttack::_bind_methods()
{
}