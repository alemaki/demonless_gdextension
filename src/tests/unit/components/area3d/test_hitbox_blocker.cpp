#define DOCTEST_CONFIG_NO_EXCEPTIONS_BUT_WITH_ALL_ASSERTS
#include <doctest.h>

#include "components/area3d/hitbox_blocker.hpp"
#include "components/area3d/hitbox.hpp"
#include "tests/test_utils/test_runner.hpp"
#include "tests/test_utils/signal_watcher.hpp"

struct HitboxBlockerFixture
{
    Hitbox* hitbox = memnew(Hitbox);
    HitboxBlocker* hitbox_blocker = memnew(HitboxBlocker);

    HitboxBlockerFixture()
    {
        godot::Node* scene_root = ::get_scene_root();
        scene_root->add_child(hitbox);
        scene_root->add_child(hitbox_blocker);
        hitbox->set_blockable(true);
        hitbox->set_monitoring(true);
        hitbox_blocker->set_monitoring(true);
    }

    ~HitboxBlockerFixture()
    {
        memdelete(hitbox);
        memdelete(hitbox_blocker);
    }
};

TEST_SUITE("HitboxBlockerHitboxTests")
{
    TEST_CASE_FIXTURE(HitboxBlockerFixture, "Test hitbox and hitbox_blocker default interaction.")
    {
        REQUIRE(hitbox->has_signal("block_triggered"));
        REQUIRE(hitbox_blocker->has_signal("hitbox_blocked"));

        SignalWatcher::watch_signals(hitbox);
        SignalWatcher::watch_signals(hitbox_blocker);

        hitbox_blocker->emit_signal("area_entered", hitbox);

        CHECK(SignalWatcher::signal_emitted(hitbox, "block_triggered"));
        CHECK(SignalWatcher::signal_emitted(hitbox_blocker, "hitbox_blocked"));

        godot::Array hitbox_signal;
        CHECK_EQ(hitbox_signal, SignalWatcher::get_signal_emitted_arguments(hitbox, "block_triggered"));

        godot::Array hitbox_blocker_signal;
        CHECK_EQ(hitbox_blocker_signal, SignalWatcher::get_signal_emitted_arguments(hitbox_blocker, "hitbox_blocked"));
    }

    TEST_CASE_FIXTURE(HitboxBlockerFixture, "Test hitbox and hitbox_blocker don't emit if hitbox is not blockable.")
    {
        REQUIRE(hitbox->has_signal("block_triggered"));
        REQUIRE(hitbox_blocker->has_signal("hitbox_blocked"));
        hitbox->set_blockable(false);

        SignalWatcher::watch_signals(hitbox);
        SignalWatcher::watch_signals(hitbox_blocker);

        hitbox_blocker->emit_signal("area_entered", hitbox);

        CHECK_FALSE(SignalWatcher::signal_emitted(hitbox, "block_triggered"));
        CHECK_FALSE(SignalWatcher::signal_emitted(hitbox_blocker, "hitbox_blocked"));
    }

    TEST_CASE_FIXTURE(HitboxBlockerFixture, "Test hitbox_blocker won't emmit signal when touched by other area3D objects.")
    {
        godot::Area3D* area3d = memnew(godot::Area3D);
        ::get_scene_root()->add_child(area3d);

        REQUIRE(hitbox_blocker->has_signal("hitbox_blocked"));

        SignalWatcher::watch_signals(hitbox_blocker);

        hitbox_blocker->emit_signal("area_entered", area3d);

        CHECK_FALSE(SignalWatcher::signal_emitted(hitbox_blocker, "hitbox_blocked"));

        memdelete(area3d);
    }
}