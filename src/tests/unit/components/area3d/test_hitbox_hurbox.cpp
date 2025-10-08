#define DOCTEST_CONFIG_NO_EXCEPTIONS_BUT_WITH_ALL_ASSERTS
#include <doctest.h>

#include "components/area3d/hitbox.hpp"
#include "components/area3d/hurtbox.hpp"
#include "components/area3d/hitbox_blocker.hpp"
#include "tests/test_utils/test_runner.hpp"
#include "tests/test_utils/signal_watcher.hpp"

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

        godot::Array hurtbox_signal;
        hurtbox_signal.push_back(hitbox);
        CHECK_EQ(hurtbox_signal, SignalObserver::get_signal_emitted_arguments(hurtbox, "hurtbox_hit"));
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
}