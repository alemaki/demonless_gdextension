#define DOCTEST_CONFIG_NO_EXCEPTIONS_BUT_WITH_ALL_ASSERTS
#include <doctest.h>

#include "attacks/damage_info.hpp"

TEST_SUITE("TestDamageInfo")
{
    TEST_CASE("DamageInfo basic getters/setters")
    {
        godot::Ref<DamageInfo> damage_info;
        damage_info.instantiate();

        CHECK_EQ(damage_info->get_damage(), 0);
        CHECK_EQ(damage_info->get_source(), nullptr);

        damage_info->set_damage(15);
        CHECK_EQ(damage_info->get_damage(), 15);

        godot::Node* source = memnew(godot::Node);
        damage_info->set_source(source);
        CHECK_EQ(damage_info->get_source(), source);

        memdelete(source);
    }

    TEST_CASE("DamageInfo clamps damage to be non-negative")
    {
        godot::Ref<DamageInfo> damage_info;
        damage_info.instantiate();

        damage_info->set_damage(10);
        damage_info->set_damage(-5);
        CHECK_EQ(damage_info->get_damage(), 0);
    }
}
