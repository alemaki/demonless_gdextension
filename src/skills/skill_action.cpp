#include "skill_action.hpp"

void SkillAction::reset()
{
    this->time_accumulated = 0;
    this->phase = SkillPhase::None;
    this->_reset();
}

void SkillAction::step(double delta)
{
    if (is_finished())
    {
        return;
    }

    phase = SkillPhase::Active;
    time_accumulated += delta;
    _step(delta);

    if (this->time_accumulated >= this->duration)
    {
        this->phase = SkillPhase::Ended;
        this->_end();
    }

}

void SkillAction::_bind_methods()
{
    BIND_GETTER_SETTER_PROPERTY_DEFAULT(SkillAction, FLOAT, duration);
}