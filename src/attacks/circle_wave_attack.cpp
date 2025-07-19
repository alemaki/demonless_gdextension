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

    int shots_to_fire = (this->time_accumulated / this->wave_cooldown) - this->waves_fired;

    if (shots_to_fire <= 0 || this->waves <= this->waves_fired)
    {
        return;
    }
    godot::UtilityFunctions::print("here1");

    double angle_step = 0;
    if (projectiles_per_wave != 0)
    {
        angle_step = 2 * Math_PI / projectiles_per_wave;
    }

    for (int shot = 1; shot <= shots_to_fire; shot++)
    {
        for (int i = 0; i < projectiles_per_wave; ++i)
        {
            double angle = i * angle_step;
            godot::Vector3 dir = this->direction.rotated(VECTOR_UP, angle);

            godot::UtilityFunctions::print("here2");
            Projectile* projectile = Object::cast_to<Projectile>(projectile->duplicate());
            ERR_FAIL_NULL(projectile);
            projectile->set_position(this->actor_source->get_position());
            projectile->set_direction(dir);
            get_tree()->get_current_scene()->add_child(projectile);
            godot::UtilityFunctions::print("here3");
        }
    }

    this->waves_fired += shots_to_fire;
    godot::UtilityFunctions::print("finished");
}

void CircleWaveAttack::_bind_methods()
{
    BIND_GETTER_SETTER_PROPERTY_OBJECT_DEFAULT(CircleWaveAttack, projectile, PropertyHint::PROPERTY_HINT_NODE_TYPE, "Projectile", PropertyUsageFlags::PROPERTY_USAGE_DEFAULT, Projectile);
    BIND_GETTER_SETTER_PROPERTY_DEFAULT(CircleWaveAttack, INT, projectiles_per_wave);
    BIND_GETTER_SETTER_PROPERTY_DEFAULT(CircleWaveAttack, INT, waves);
    BIND_GETTER_SETTER_PROPERTY_DEFAULT(CircleWaveAttack, FLOAT, wave_cooldown);
    BIND_GETTER_SETTER_PROPERTY_DEFAULT(CircleWaveAttack, VECTOR3, direction);
}