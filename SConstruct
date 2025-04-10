import os
from SCons.Script import SConscript, Default
env = None

variant_dir = os.path.join('..', 'bin')

# Ensure variant directory exists
if not os.path.exists(variant_dir):
    os.makedirs(variant_dir)

env = SConscript(os.path.join('behaviour_tree_GDE', 'SConscript'))

# Add include paths for compialtion
include_paths = [
     os.path.join('src')
]

test_utils_dir = os.path.join('behaviour_tree_GDE', 'tests', 'test_utils')

compilation_paths = [
     os.path.join('behaviour_tree_GDE', 'tests'), # the rest of behaviour_tree_GDE is its own static library
     os.path.join('src')
]

exclude_compilation_paths = [
     test_utils_dir, # built separately as its own static library, see test_utils/SConscript
]


for include_path in include_paths:
    env.Append(CPPPATH=[include_path])

# for libraries
# env.Append(LIB="...")
# env.Append(LIBPATH="...")


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
