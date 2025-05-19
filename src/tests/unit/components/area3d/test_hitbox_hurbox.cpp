#define DOCTEST_CONFIG_NO_EXCEPTIONS_BUT_WITH_ALL_ASSERTS
#include <doctest.h>

#include "components/area3d/hitbox.hpp"
#include "components/area3d/hurtbox.hpp"
#include "components/area3d/hitbox_blocker.hpp"
#include "tests/test_utils/test_runner.hpp"
#include "tests/test_utils/signal_observer.hpp"

struct HitboxFixture
{
    Hitbox* hitbox = memnew(Hitbox);
    Hurtbox* hurtbox = memnew(Hurtbox);
    HitboxBlocker* hitbox_blocker = memnew(HitboxBlocker);

    HitboxFixture()
    {
        godot::Node* scene_root = ::get_scene_root();
        scene_root->add_child(hitbox);
        scene_root->add_child(hurtbox);
        scene_root->add_child(hitbox_blocker);
        hitbox->set_monitoring(true);
    }

    ~HitboxFixture()
    {
        memdelete(hitbox);
        memdelete(hurtbox);
        memdelete(hitbox_blocker);
    }
};

TEST_SUITE("HitboxHurtboxInteractionTests")
{
    TEST_CASE_FIXTURE(HitboxFixture, "Test hitbox and hurtbox interaction.")
    {
        REQUIRE(hitbox->has_signal("hit_hurtbox"));
        REQUIRE(hurtbox->has_signal("hurtbox_hit"));

        SignalObserver::watch_signals(hitbox);
        SignalObserver::watch_signals(hurtbox);

        hitbox->emit_signal("area_entered", hurtbox);

        CHECK(SignalObserver::signal_emitted(hitbox, "hit_hurtbox"));
        CHECK(SignalObserver::signal_emitted(hurtbox, "hurtbox_hit"));

        godot::Array hitbox_signal;
        hitbox_signal.push_back(hurtbox);
        CHECK_EQ(hitbox_signal, SignalObserver::get_signal_emitted_arguments(hitbox, "hit_hurtbox"));

        godot::Array hurtbox_signal_args = SignalObserver::get_signal_emitted_arguments(hurtbox, "hurtbox_hit");
        REQUIRE_EQ(hurtbox_signal_args.size(), 1);
        godot::Ref<DamageInfo> emitted_damage_info = hurtbox_signal_args[0];
        REQUIRE_FALSE(emitted_damage_info.is_null());
        CHECK_EQ(emitted_damage_info->get_damage(), hitbox->get_damage_profile()->get_base_damage());
        CHECK_EQ(emitted_damage_info->get_source(), hitbox->get_actor_source());
    }

    TEST_CASE_FIXTURE(HitboxFixture, "Test hitbox and hurtbox won't emmit signal when touched by other area3D objects.")
    {
        godot::Area3D* area3d = memnew(godot::Area3D);
        ::get_scene_root()->add_child(area3d);

        REQUIRE(hitbox->has_signal("hit_hurtbox"));
        REQUIRE(hurtbox->has_signal("hurtbox_hit"));

        SignalObserver::watch_signals(hitbox);
        SignalObserver::watch_signals(hurtbox);

        hitbox->emit_signal("area_entered", area3d);
        hurtbox->emit_signal("area_entered", area3d);

        CHECK_FALSE(SignalObserver::signal_emitted(hitbox, "hit_hurtbox"));
        CHECK_FALSE(SignalObserver::signal_emitted(hurtbox, "hurtbox_hit"));

        memdelete(area3d);
    }

    TEST_CASE_FIXTURE(HitboxFixture, "Test hitbox and hitbox_blocker interaction.")
    {
        REQUIRE(hitbox->has_signal("hit_blocker"));
        REQUIRE(hitbox_blocker->has_signal("hitbox_blocked"));

        SignalObserver::watch_signals(hitbox);
        SignalObserver::watch_signals(hitbox_blocker);

        hitbox->emit_signal("area_entered", hitbox_blocker);

        CHECK(SignalObserver::signal_emitted(hitbox, "hit_blocker"));
        CHECK(SignalObserver::signal_emitted(hitbox_blocker, "hitbox_blocked"));

        godot::Array hitbox_signal;
        hitbox_signal.push_back(hitbox_blocker);
        CHECK_EQ(hitbox_signal, SignalObserver::get_signal_emitted_arguments(hitbox, "hit_blocker"));

        godot::Array hitbox_blocker_signal;
        hitbox_blocker_signal.push_back(hitbox);
        CHECK_EQ(hitbox_blocker_signal, SignalObserver::get_signal_emitted_arguments(hitbox_blocker, "hitbox_blocked"));
    }

    TEST_CASE_FIXTURE(HitboxFixture, "Hitbox has a non-null damage_profile by default and it is settable.")
    {
        REQUIRE_FALSE(hitbox->get_damage_profile().is_null());

        godot::Ref<DamageProfile> damage_profile;
        damage_profile.instantiate();
        damage_profile->set_base_damage(42);

        hitbox->set_damage_profile(damage_profile);
        CHECK_EQ(hitbox->get_damage_profile(), damage_profile);
        CHECK_EQ(hitbox->get_damage_profile()->get_base_damage(), 42);
    }

    TEST_CASE("A hitbox without a DamageProfile reports a configuration warning until it enters the tree.")
    {
        Hitbox* hitbox = memnew(Hitbox);
        CHECK(hitbox->get_damage_profile().is_null());
        CHECK_FALSE(hitbox->_get_configuration_warnings().is_empty());

        ::get_scene_root()->add_child(hitbox);

        CHECK_FALSE(hitbox->get_damage_profile().is_null());
        CHECK(hitbox->_get_configuration_warnings().is_empty());

        memdelete(hitbox);
    }

    TEST_CASE("Duplicated hitboxes each get their own damage_profile, not a shared one.")
    {
        Hitbox* original = memnew(Hitbox);
        original->set_damage_profile(memnew(DamageProfile));
        original->get_damage_profile()->set_base_damage(10);

        Hitbox* copy = godot::Object::cast_to<Hitbox>(original->duplicate());
        REQUIRE(copy != nullptr);

        copy->get_damage_profile()->set_base_damage(99);

        CHECK_EQ(original->get_damage_profile()->get_base_damage(), 10);
        CHECK_EQ(copy->get_damage_profile()->get_base_damage(), 99);
        CHECK_NE(original->get_damage_profile(), copy->get_damage_profile());

        memdelete(original);
        memdelete(copy);
    }

    TEST_CASE("A hitbox without a damage_profile errors instead of crashing when it hits a hurtbox.")
    {
        Hitbox* hitbox = memnew(Hitbox);
        Hurtbox* hurtbox = memnew(Hurtbox);
        ::get_scene_root()->add_child(hitbox);
        ::get_scene_root()->add_child(hurtbox);
        hitbox->set_damage_profile(nullptr);

        CHECK_GODOT_ERROR(hitbox->emit_signal("area_entered", hurtbox));

        memdelete(hitbox);
        memdelete(hurtbox);
    }
}