import os

# Define build directories
if 'target' not in ARGUMENTS:
    ARGUMENTS['target'] = 'template_debug'

if 'debug_symbols' not in ARGUMENTS:
    ARGUMENTS['debug_symbols'] = 'yes'

target = ARGUMENTS.get('target', "template_debug")
debug_symbols = ARGUMENTS.get('debug_symbols', "yes")

variant_dir = os.path.join('..', 'bin')

# Ensure variant directory exists
if not os.path.exists(variant_dir):
    os.makedirs(variant_dir)

# Include necessary environment from godot-cpp
env = SConscript(os.path.join('behaviour_tree_GDE', 'godot-cpp', 'SConstruct'))

# Add include paths for compialtion
include_paths = [
     os.path.join('behaviour_tree_GDE'),
     os.path.join('src')
]

test_utils_dir = os.path.join('behaviour_tree_GDE', 'tests', 'test_utils')

compilation_paths = [
     os.path.join('behaviour_tree_GDE'),
     os.path.join('src')
]

exclude_compilation_paths = [
     os.path.join('godot-cpp'), # built separately as static library
     test_utils_dir, # built separately as its own static library, see test_utils/SConscript
]


for include_path in include_paths:
    env.Append(CPPPATH=[include_path])

# for libraries
# env.Append(LIB="...")
# env.Append(LIBPATH="...")

# Godot doesn't like ".obj"
env['OBJSUFFIX'] = '.o'

if target == 'template_debug':
    env.Append(CPPDEFINES=['DEBUG'])
    if debug_symbols == "yes":
        pass
        #env.Append(CXXFLAGS=['-g', '-O0'])  # Debug flags: No optimization, include debug symbols
else:
    env.Append(CPPDEFINES=['NDEBUG'])


def collect_cpp_files(root, exclude_dirs):
    cpp_files = []
    for dirpath, _, filenames in os.walk(root):
        for filename in filenames:
            if any(excluded in dirpath for excluded in exclude_dirs):
                continue
            if filename.endswith('.cpp'):
                cpp_files.append(os.path.join(dirpath, filename))
    return cpp_files

# Hand it `env` and it registers itself in env['LIBS']. See test_utils/SConscript.
SConscript(os.path.join(test_utils_dir, 'SConscript'), exports='env')

# Collect all CPP files
src = []
for compilation_path in compilation_paths:
    src += collect_cpp_files(compilation_path, exclude_compilation_paths)

print("Compiler: " + env['CC'])

if env['platform'] == 'linux' or env['platform'] == 'windows':
	libpath = os.path.join(variant_dir, 'demonless{}{}'.format(env['suffix'], env['SHLIBSUFFIX']))
	sharedlib = env.SharedLibrary(libpath, src)
	Default(sharedlib)

if env['platform'] == 'android': # TODO
	pass
