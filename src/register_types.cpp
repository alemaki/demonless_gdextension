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

#include "behaviour/actions/bt_shoot_linear_projectile.hpp"

#include "behaviour/conditions/bt_node2d_in_range.hpp"

#include "behaviour/player/actions/bt_evaluate_movement_input.hpp"

#include "components/area2d/hitbox.hpp"
#include "components/area2d/hurtbox.hpp"
#include "components/health/health_component.hpp"
#include "components/input/character_input_component.hpp"
#include "projectiles/projectile.hpp"
#include "projectiles/linear_projectile.hpp"
#include "ui/health_bar.hpp"


using namespace godot;

void initialize_gdextension_module(ModuleInitializationLevel p_level)
{
	::initialize_behaviour_tree_module(p_level);
	::initialize_test_module(p_level);
	
	if (p_level == MODULE_INITIALIZATION_LEVEL_SCENE)
	{
		ClassDB::register_class<Projectile>();
		ClassDB::register_class<HealthComponent>();
		ClassDB::register_class<Hitbox>();
		ClassDB::register_class<Hurtbox>();
		ClassDB::register_class<LinearProjectile>();
		ClassDB::register_class<CharacterInputComponent>();

		ClassDB::register_class<PlayerCharacter>();

		/* UI */
		ClassDB::register_class<HealthBar>();
		
		/* Behaviour */
		ClassDB::register_class<BTShootLinearProjectile>();

		ClassDB::register_class<BTNode2DInRange>();

		/* Player behavior */
		ClassDB::register_class<BTEvaluateMovementInput>();
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
