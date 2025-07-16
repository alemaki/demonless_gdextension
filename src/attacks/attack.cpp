#include "attack.hpp"

void Attack::reset()
{
    this->elapsed = 0;
    this->_reset();
}

void Attack::step(double delta)
{
    this->_step(delta);
}

void Attack::_bind_methods()
{
    BIND_GETTER_SETTER_PROPERTY_DEFAULT(Attack, INT, duration);
}