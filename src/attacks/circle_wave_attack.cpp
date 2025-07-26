#include "circle_wave_attack.hpp"
#include "godot_cpp/variant/utility_functions.hpp"

void CircleWaveAttack::set_direction(const godot::Vector3 direction)
{
    ERR_FAIL_COND_MSG(direction.is_zero_approx(), "Attack direction cannot be godot::Vector3{0, 0, 0}");
    this->direction = direction;
}

void CircleWaveAttack::_reset()
{
    this->waves_fired = 0;
}

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
            projectile_cpy->set_direction(dir);
            get_tree()->get_current_scene()->add_child(projectile_cpy);
        }
    }

    this->waves_fired += waves_to_fire;
}

void CircleWaveAttack::_bind_methods()
{
    BIND_GETTER_SETTER_PROPERTY_OBJECT_DEFAULT(CircleWaveAttack, projectile, PROPERTY_HINT_NODE_TYPE, "Projectile", PROPERTY_USAGE_DEFAULT, Projectile);
    BIND_GETTER_SETTER_PROPERTY_DEFAULT(CircleWaveAttack, INT, projectiles_per_wave);
    BIND_GETTER_SETTER_PROPERTY_DEFAULT(CircleWaveAttack, INT, waves);
    BIND_GETTER_SETTER_PROPERTY_DEFAULT(CircleWaveAttack, FLOAT, wave_cooldown);
    BIND_GETTER_SETTER_PROPERTY_DEFAULT(CircleWaveAttack, VECTOR3, direction);
}