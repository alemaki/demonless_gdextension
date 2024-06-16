import os

# Include necessary environment
env = SConscript('godot-cpp/SConstruct')

# Add include paths for compialtion
env.Append(CPPPATH=['doctest/doctest'])
env.Append(CPPPATH=['behaviour_tree'])
env.Append(CPPPATH=['src'])

#for libraries
#env.Append(LIB="...")
#env.Append(LIBPATH="...")

def collect_cpp_files(root):
    cpp_files = []
    for dirpath, _, filenames in os.walk(root):
        for filename in filenames:
            if filename.endswith('.cpp'):
                cpp_files.append(os.path.join(dirpath, filename))
    return cpp_files

# Collect all CPP files
src = []
src += collect_cpp_files("doctest/doctest")
src += collect_cpp_files("behaviour_tree")
src += collect_cpp_files("src")

target = ARGUMENTS.get('target', 'template_release')

if target == 'template_debug' or target == 'debug':
    env.Append(CPPDEFINES=['DEBUG'])
    env.Append(CXXFLAGS=['-g', '-O0'])  # Debug flags: No optimization, include debug symbols
    env['suffix'] = '.template_debug'
else:
    env.Append(CPPDEFINES=['NDEBUG'])
    env.Append(CXXFLAGS=['-O3'])  # Release flags: Full optimization
    env['suffix'] = '.template_release'

if env['platform'] == 'linux' or env['platform'] == 'windows':
	libpath = '../libdemonless{}{}'.format(env['suffix'], env['SHLIBSUFFIX'])
	sharedlib = env.SharedLibrary(libpath, src)
	Default(sharedlib)

if env['platform'] == 'android': # TODO
	pass

