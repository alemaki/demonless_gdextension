import os

# Define build directories
target = ARGUMENTS.get('target', "release")
variant_dir = os.path.join('..', 'bin')

# Ensure variant directory exists
if not os.path.exists(variant_dir):
    os.makedirs(variant_dir)

# Include necessary environment
env = SConscript('behaviour_tree_GDE/godot-cpp/SConstruct')

# Add include paths for compialtion
include_paths = [
     'behaviour_tree_GDE/tests/test_utils/doctest',
     'behaviour_tree_GDE',
     'src'
]

compilation_paths = [
     'behaviour_tree_GDE',
     'src'
]

for include_path in include_paths:
    env.Append(CPPPATH=[include_path])

# for libraries
# env.Append(LIB="...")
# env.Append(LIBPATH="...")

def collect_cpp_files(root):
    cpp_files = []
    for dirpath, _, filenames in os.walk(root):
        for filename in filenames:
            if filename.endswith('.cpp'):
                cpp_files.append(os.path.join(dirpath, filename))
    return cpp_files

# Collect all CPP files
src = []
for compilation_path in compilation_paths:
    src += collect_cpp_files(compilation_path)

# Godot doesn't like ".obj"
env['OBJSUFFIX'] = '.o'

if target == 'template_debug' or target == 'debug':
    env.Append(CPPDEFINES=['DEBUG'])
    env.Append(CXXFLAGS=['-g', '-O0'])  # Debug flags: No optimization, include debug symbols
else:
    env.Append(CPPDEFINES=['NDEBUG'])

if env['platform'] == 'linux' or env['platform'] == 'windows':
	libpath = os.path.join(variant_dir, 'demonless{}{}'.format(env['suffix'], env['SHLIBSUFFIX']))
	sharedlib = env.SharedLibrary(libpath, src)
	Default(sharedlib)

if env['platform'] == 'android': # TODO
	pass

