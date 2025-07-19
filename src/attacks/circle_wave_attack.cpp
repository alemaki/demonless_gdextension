#include "circle_wave_attack.hpp"

void CircleWaveAttack::set_direction(const godot::Vector3 direction)
{
    ERR_FAIL_COND_MSG(direction.is_zero_approx(), "Attack direction cannot be godot::Vector3{0, 0, 0}");
    this->direction = direction;
}

void CircleWaveAttack::_reset()
{
    this->Attack::reset();
    this-> shots_fired = 0;
}

void CircleWaveAttack::_step(double delta)
{
    ERR_FAIL_NULL(this->actor_source);
    ERR_FAIL_NULL(this->projectile);
    ERR_FAIL_COND(direction.is_zero_approx());

    int shots_to_fire = this->shots - (this->time_accumulated/this->shot_cooldown) - this->shots_fired;
    if (shots_to_fire <= 0)
        return;

    double angle_step = 2 * Math_PI / bullets_per_shot;
    for (int shot = 1; shot <= shots_to_fire; shot++)
    {
        for (int i = 0; i < bullets_per_shot; ++i)
        {
            double angle = i * angle_step;
            godot::Vector3 dir = this->direction.rotated(VECTOR_UP, angle);

            Projectile* projectile = Object::cast_to<Projectile>(projectile->duplicate());
            ERR_FAIL_NULL(projectile);
            projectile->set_position(this->actor_source->get_position());
            projectile->set_direction(dir);
            get_tree()->get_current_scene()->add_child(projectile);
        }
    } 
}

void CircleWaveAttack::_bind_methods()
{
    BIND_GETTER_SETTER_PROPERTY_OBJECT_DEFAULT(CircleWaveAttack, projectile, PropertyHint::PROPERTY_HINT_NODE_TYPE, "Projectile", PropertyUsageFlags::PROPERTY_USAGE_DEFAULT, Projectile);
    BIND_GETTER_SETTER_PROPERTY_DEFAULT(CircleWaveAttack, Variant::INT, bullets_per_shot);
    BIND_GETTER_SETTER_PROPERTY_DEFAULT(CircleWaveAttack, Variant::INT, shots);
    BIND_GETTER_SETTER_PROPERTY_DEFAULT(CircleWaveAttack, Variant::FLOAT, shot_cooldown);
    BIND_GETTER_SETTER_PROPERTY_DEFAULT(CircleWaveAttack, Variant::VECTOR2, direction);
}