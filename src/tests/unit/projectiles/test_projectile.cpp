#define DOCTEST_CONFIG_NO_EXCEPTIONS_BUT_WITH_ALL_ASSERTS
#include <doctest.h>
#include <godot_cpp/godot.hpp>

#include "projectiles/projectile.hpp"
#include "tests/test_utils/signal_watcher.hpp"

TEST_SUITE("Lifespan tests.")
{
    TEST_CASE("Projectile lifespan timer emmits signals only on timeout.")
    {
        Projectile* projectile = memnew(Projectile);
        godot::Timer* timer = memnew(godot::Timer);
        SignalWatcher::watch_signals(timer);
        timer->start(5);
        projectile->set_lifespan_timer(timer);
        CHECK_FALSE(SignalWatcher::signal_emitted(projectile->get_lifespan_timer(), godot::String("timeout")));
        projectile->get_lifespan_timer()->_process(1);
        CHECK_FALSE(SignalWatcher::signal_emitted(projectile->get_lifespan_timer(), godot::String("timeout")));
        projectile->get_lifespan_timer()->_process(4);
        CHECK(SignalWatcher::signal_emitted(projectile->get_lifespan_timer(), godot::String("timeout")));
        CHECK_FALSE(projectile->is_queued_for_deletion());
    }
};