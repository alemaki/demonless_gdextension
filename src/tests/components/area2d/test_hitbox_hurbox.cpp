#define DOCTEST_CONFIG_NO_EXCEPTIONS_BUT_WITH_ALL_ASSERTS
#include <doctest.h>

#include "components/area2d/hitbox.hpp"
#include "components/area2d/hurtbox.hpp"
#include "tests/test_utils/signal_watcher.hpp"

TEST_SUITE("HitboxHurtboxInteractionTests")
{

    TEST_CASE("Test hurtbox and hitbox")
    {

        Hitbox* hitbox = memnew(Hitbox);
        Hurtbox* hurtbox = memnew(Hurtbox);

        SUBCASE("Test hitbox and hurtbox interaction")
        {
            /* simulate addition to tree. TODO: find solution. */
            hitbox->_ready();
            hurtbox->_ready();

            REQUIRE(hitbox->has_signal("hit_hurtbox"));
            REQUIRE(hurtbox->has_signal("hurtbox_hit"));

            SignalWatcher::watch_signals(hitbox);
            SignalWatcher::watch_signals(hurtbox);

            hitbox->set_monitoring(true);
            hitbox->emit_signal("area_entered", hurtbox);

            CHECK(SignalWatcher::signal_emitted(hitbox, "hit_hurtbox"));
            CHECK(SignalWatcher::signal_emitted(hurtbox, "hurtbox_hit"));

            godot::Array hitbox_signal;
            hitbox_signal.push_back(hurtbox);
            CHECK(hitbox_signal == SignalWatcher::get_signal_emitted_arguments(hitbox, "hit_hurtbox"));

            godot::Array hurtbox_signal;
            hurtbox_signal.push_back(hitbox);
            CHECK(hurtbox_signal == SignalWatcher::get_signal_emitted_arguments(hurtbox, "hurtbox_hit"));
        }

        SUBCASE("Test hitbox and hurtbox won't emmit signal when touched by other area2D objects")
        {
            godot::Area2D* area2d= memnew(godot::Area2D);

            /* simulate addition to tree. TODO: find solution. */
            area2d->_ready();
            hitbox->_ready();
            hurtbox->_ready();

            REQUIRE(hitbox->has_signal("hit_hurtbox"));
            REQUIRE(hurtbox->has_signal("hurtbox_hit"));

            SignalWatcher::watch_signals(hitbox);
            SignalWatcher::watch_signals(hurtbox);

            hitbox->set_monitoring(true);
            hitbox->emit_signal("area_entered", area2d);
            hurtbox->emit_signal("area_entered", area2d);

            CHECK_FALSE(SignalWatcher::signal_emitted(hitbox, "hit_hurtbox"));
            CHECK_FALSE(SignalWatcher::signal_emitted(hurtbox, "hurtbox_hit"));

            memfree(area2d);
        }

        memfree(hitbox);
        memfree(hurtbox);
    }

}