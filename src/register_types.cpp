/* godot-cpp integration testing project.
 *
 * This is free and unencumbered software released into the public domain.
 */

#include "register_types.h"

#include <gdextension_interface.h>

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>

#include "bt_register_types.h"

#include "actors/player/player_character.hpp"

#include "actor_behaviour/actions/bt_execute_attack.hpp"
#include "actor_behaviour/actions/bt_queue_free_node.hpp"
#include "actor_behaviour/actions/bt_look_at_position.hpp"
#include "actor_behaviour/actions/bt_spawn_node3d_at_position.hpp"
#include "actor_behaviour/actions/bt_play_animation.hpp"

#include "actor_behaviour/conditions/bt_node3d_in_range.hpp"

#include "skills/skill_action.hpp"
#include "skills/projectile_time_jump.hpp"

#include "attacks/wave_attack.hpp"
#include "attacks/circle_wave_attack.hpp"
#include "attacks/directional_wave_attack.hpp"

#include "components/area3d/hitbox.hpp"
#include "components/area3d/hitbox_blocker.hpp"
#include "components/area3d/hurtbox.hpp"
#include "components/health/health_component.hpp"
#include "components/input/character_input_component.hpp"
#include "components/movement/character_movement_component.hpp"

#include "movement_strategies/movement_context.hpp"
#include "movement_strategies/movement_strategy.hpp"
#include "movement_strategies/composite_movement_strategy.hpp"
#include "movement_strategies/sequential_movement_strategy.hpp"

#include "projectiles/projectile.hpp"

#include "ui/health_bar.hpp"


using namespace godot;

void initialize_gdextension_module(ModuleInitializationLevel p_level)
{
	::initialize_behaviour_tree_module(p_level);

	if (p_level == MODULE_INITIALIZATION_LEVEL_SCENE)
	{
		ClassDB::register_class<MovementContext>();
		ClassDB::register_class<MovementStrategy>();
		ClassDB::register_class<CompositeMovementStrategy>();
		ClassDB::register_class<SequentialMovementStrategy>();

		ClassDB::register_class<Projectile>();

		ClassDB::register_class<HealthComponent>();
		ClassDB::register_class<Hitbox>();
		ClassDB::register_class<Hurtbox>();
		ClassDB::register_class<HitboxBlocker>();
		ClassDB::register_class<CharacterInputComponent>();
		ClassDB::register_class<CharacterMovementComponent>();

		ClassDB::register_class<PlayerCharacter>();

		/* UI */
		ClassDB::register_class<HealthBar>();

		/* Behaviour */
		ClassDB::register_class<SkillAction>();

		ClassDB::register_class<ProjectileTimeJump>();

		ClassDB::register_class<WaveAttack>();
		ClassDB::register_class<CircleWaveAttack>();
		ClassDB::register_class<DirectionalWaveAttack>();

		ClassDB::register_class<BTExecuteAttack>();
		ClassDB::register_class<BTQueueFreeNode>();
		ClassDB::register_class<BTSpawnNode3DAtPosition>();
		ClassDB::register_class<BTPlayAnimation>();
		ClassDB::register_class<BTLookAtPosition>();

		ClassDB::register_class<BTNode3DInRange>();
	}
	if (p_level == MODULE_INITIALIZATION_LEVEL_EDITOR)
	{

	}
}

void uninitialize_gdextension_module(ModuleInitializationLevel p_level)
{
	::uninitialize_behaviour_tree_module(p_level);
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE)
	{
		return;
	}
}

extern "C"
{
// Initialization.
GDExtensionBool GDE_EXPORT gdextension_library_init(GDExtensionInterfaceGetProcAddress p_get_proc_address, GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization)
{
	godot::GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);

	init_obj.register_initializer(initialize_gdextension_module);
	init_obj.register_terminator(uninitialize_gdextension_module);
	init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

	return init_obj.init();
}
}
