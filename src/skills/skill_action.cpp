#include "skill_action.hpp"

void SkillAction::set_animation(const godot::StringName &value)
{ 
    this->animation = value;
}

godot::StringName SkillAction::get_animation() const
{
    return this->animation;
}

void SkillAction::set_direction(const godot::Vector3 direction)
{
    ERR_FAIL_COND_MSG(direction.is_zero_approx(), "Attack direction cannot be godot::Vector3{0, 0, 0}");
    if (this->change_direction_allowed || this->phase == SkillAction::SkillPhase::None)
    {
        this->direction = direction.normalized();
    }
}

void SkillAction::reset()
{
    this->time_accumulated = 0;
    this->phase = SkillPhase::None;
    this->_reset();
}

void SkillAction::step(double delta)
{
    if (is_done())
    {
        return;
    }

    phase = SkillPhase::Active;
    time_accumulated += delta;
    _step(delta);

    if (this->time_accumulated >= this->duration)
    {
        this->end();
    }

}
void SkillAction::end()
{
    this->phase = SkillPhase::Ended;
    this->_end();
}

void SkillAction::_bind_methods()
{
    BIND_GETTER_SETTER_PROPERTY_DEFAULT(SkillAction, FLOAT, duration);
    BIND_GETTER_SETTER_PROPERTY_DEFAULT(SkillAction, VECTOR3, direction);
    BIND_GETTER_SETTER_PROPERTY_OBJECT_NO_HINT(SkillAction, actor_source);
    BIND_GETTER_SETTER_PROPERTY_BOOL_DEFAULT(SkillAction, change_direction_allowed);
    BIND_GETTER_SETTER_PROPERTY_DEFAULT(SkillAction, STRING_NAME, animation);
}