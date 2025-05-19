#define DOCTEST_CONFIG_NO_EXCEPTIONS_BUT_WITH_ALL_ASSERTS
#include <doctest.h>

#include "attacks/damage_profile.hpp"

TEST_SUITE("TestDamageProfile")
{
    TEST_CASE("DamageProfile basic getters/setters")
    {
        godot::Ref<DamageProfile> damage_profile;
        damage_profile.instantiate();

        CHECK_EQ(damage_profile->get_base_damage(), 1);

        damage_profile->set_base_damage(15);
        CHECK_EQ(damage_profile->get_base_damage(), 15);
    }

    TEST_CASE("DamageProfile clamps base_damage to be non-negative")
    {
        godot::Ref<DamageProfile> damage_profile;
        damage_profile.instantiate();

        damage_profile->set_base_damage(10);
        damage_profile->set_base_damage(-5);
        CHECK_EQ(damage_profile->get_base_damage(), 0);
    }

    TEST_CASE("construct_damage_info builds a DamageInfo from the profile's base_damage and given source")
    {
        godot::Ref<DamageProfile> damage_profile;
        damage_profile.instantiate();
        damage_profile->set_base_damage(25);

        godot::Node* source = memnew(godot::Node);

        godot::Ref<DamageInfo> damage_info = damage_profile->construct_damage_info(source);
        REQUIRE_FALSE(damage_info.is_null());
        CHECK_EQ(damage_info->get_damage(), 25);
        CHECK_EQ(damage_info->get_source(), source);

        memdelete(source);
    }

    TEST_CASE("construct_damage_info returns a fresh DamageInfo each call")
    {
        godot::Ref<DamageProfile> damage_profile;
        damage_profile.instantiate();
        damage_profile->set_base_damage(5);

        godot::Ref<DamageInfo> first = damage_profile->construct_damage_info(nullptr);
        godot::Ref<DamageInfo> second = damage_profile->construct_damage_info(nullptr);

        CHECK_NE(first, second);
        first->set_damage(99);
        CHECK_EQ(second->get_damage(), 5);
    }
}
