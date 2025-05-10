#include "damage_info.hpp"

void DamageInfo::_bind_methods()
{
    BIND_GETTER_SETTER_PROPERTY_DEFAULT(DamageInfo, FLOAT, damage);
    BIND_GETTER_SETTER_PROPERTY_OBJECT_NO_HINT(DamageInfo, source);
}