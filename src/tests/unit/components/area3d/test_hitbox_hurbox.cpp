#define DOCTEST_CONFIG_NO_EXCEPTIONS_BUT_WITH_ALL_ASSERTS
#include <doctest.h>

#include "components/area3d/hitbox.hpp"
#include "components/area3d/hurtbox.hpp"
#include "tests/test_utils/test_runner.hpp"
#include "tests/test_utils/signal_watcher.hpp"

struct HitboxHurtboxFixture
{
    Hitbox* hitbox = memnew(Hitbox);
    Hurtbox* hurtbox = memnew(Hurtbox);

    HitboxHurtboxFixture()
    {
        godot::Node* scene_root = ::get_scene_root();
        scene_root->add_child(hitbox);
        scene_root->add_child(hurtbox);
        hitbox->set_monitoring(true);
    }

    ~HitboxHurtboxFixture()
    {
        memdelete(hitbox);
        memdelete(hurtbox);
    }
};

TEST_SUITE("HitboxHurtboxInteractionTests")
{
    TEST_CASE_FIXTURE(HitboxHurtboxFixture, "Test hitbox and hurtbox interaction.")
    {
        REQUIRE(hitbox->has_signal("hit_hurtbox"));
        REQUIRE(hurtbox->has_signal("hurtbox_hit"));

        SignalWatcher::watch_signals(hitbox);
        SignalWatcher::watch_signals(hurtbox);

        hitbox->emit_signal("area_entered", hurtbox);

        CHECK(SignalWatcher::signal_emitted(hitbox, "hit_hurtbox"));
        CHECK(SignalWatcher::signal_emitted(hurtbox, "hurtbox_hit"));

        godot::Array hitbox_signal;
        hitbox_signal.push_back(hurtbox);
        CHECK_EQ(hitbox_signal, SignalWatcher::get_signal_emitted_arguments(hitbox, "hit_hurtbox"));

        godot::Array hurtbox_signal;
        hurtbox_signal.push_back(hitbox);
        CHECK_EQ(hurtbox_signal, SignalWatcher::get_signal_emitted_arguments(hurtbox, "hurtbox_hit"));
    }

    TEST_CASE_FIXTURE(HitboxHurtboxFixture, "Test hitbox and hurtbox won't emmit signal when touched by other area3D objects.")
    {
        godot::Area3D* area3d = memnew(godot::Area3D);
        ::get_scene_root()->add_child(area3d);

        REQUIRE(hitbox->has_signal("hit_hurtbox"));
        REQUIRE(hurtbox->has_signal("hurtbox_hit"));

        SignalWatcher::watch_signals(hitbox);
        SignalWatcher::watch_signals(hurtbox);

        hitbox->emit_signal("area_entered", area3d);
        hurtbox->emit_signal("area_entered", area3d);

        CHECK_FALSE(SignalWatcher::signal_emitted(hitbox, "hit_hurtbox"));
        CHECK_FALSE(SignalWatcher::signal_emitted(hurtbox, "hurtbox_hit"));

        memdelete(area3d);
    }

    TEST_CASE_FIXTURE(HitboxHurtboxFixture, "Test hitbox emits 'block_triggered'.")
    {
        REQUIRE(hitbox->has_signal("block_triggered"));

        SignalWatcher::watch_signals(hitbox);
        hitbox->set_blockable(true);
        hitbox->set_monitoring(true);
        hitbox->trigger_block();

        CHECK(SignalWatcher::signal_emitted(hitbox, "block_triggered"));
    }
}

TEST_SUITE("[errors] HitboxHurtboxInteractionTests")
{
    TEST_CASE_FIXTURE(HitboxHurtboxFixture, "Test hitbox doesn't emit 'block_triggered', when prohibited.")
    {
        REQUIRE(hitbox->has_signal("block_triggered"));

        SignalWatcher::watch_signals(hitbox);
        hitbox->set_blockable(false);
        hitbox->set_monitoring(true);
        CHECK_GODOT_ERROR(hitbox->trigger_block());

        CHECK_FALSE(SignalWatcher::signal_emitted(hitbox, "block_triggered"));
    }
}