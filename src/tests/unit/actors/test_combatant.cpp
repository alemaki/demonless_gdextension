#define DOCTEST_CONFIG_NO_EXCEPTIONS_BUT_WITH_ALL_ASSERTS
#include <doctest.h>

#include "actors/combatant.hpp"
#include "utils/utils.hpp"

struct CombatantFixture
{
    Combatant* combatant = memnew(Combatant);
    Hurtbox* hurtbox = memnew(Hurtbox);
    HealthComponent* health_component = memnew(HealthComponent);

    CombatantFixture()
    {
        hurtbox->set_name("Hurtbox");
        health_component->set_name("HealthComponent");
        combatant->add_child(hurtbox);
        combatant->add_child(health_component);
        health_component->set_max_hp(100);
        health_component->set_current_hp(100);

        ::get_scene_root()->add_child(combatant);
    }

    ~CombatantFixture()
    {
        memdelete(combatant);
    }
};

TEST_SUITE("TestCombatant")
{
    TEST_CASE_FIXTURE(CombatantFixture, "Combatant finds Hurtbox/HealthComponent children by name")
    {
        CHECK_EQ(combatant->get_hurtbox(), hurtbox);
        CHECK_EQ(combatant->get_health_component(), health_component);
    }

    TEST_CASE_FIXTURE(CombatantFixture, "A hurtbox hit applies damage to the health_component")
    {
        godot::Ref<DamageInfo> damage_info;
        damage_info.instantiate();
        damage_info->set_damage(30);

        hurtbox->emit_signal("hurtbox_hit", damage_info);

        CHECK_EQ(health_component->get_current_hp(), 70);
    }
}

TEST_SUITE("[errors] TestCombatant")
{
    TEST_CASE("A hurtbox hit without a health_component errors instead of crashing")
    {
        Combatant* combatant = memnew(Combatant);
        Hurtbox* hurtbox = memnew(Hurtbox);
        hurtbox->set_name("Hurtbox");
        combatant->add_child(hurtbox);

        /* No HealthComponent child present, so _ready() itself fails to find one. */
        CHECK_GODOT_ERROR(::get_scene_root()->add_child(combatant));

        godot::Ref<DamageInfo> damage_info;
        damage_info.instantiate();
        damage_info->set_damage(10);

        CHECK_GODOT_ERROR(hurtbox->emit_signal("hurtbox_hit", damage_info));

        memdelete(combatant);
    }

    TEST_CASE_FIXTURE(CombatantFixture, "A hurtbox hit with a null damage_info errors instead of crashing")
    {
        CHECK_GODOT_ERROR(hurtbox->emit_signal("hurtbox_hit", godot::Ref<DamageInfo>()));
    }
}
