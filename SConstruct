import os

# Include necessary environment
env = SConscript('godot-cpp/SConstruct')

# Add include paths for compialtion
env.Append(CPPPATH=["doctest/doctest"])
env.Append(CPPPATH=["behaviour_tree"])
env.Append(CPPPATH=["src"])

#for libraries
#env.Append(LIB="...")
#env.Append(LIBPATH="...")

# Function to collect CPP files recursively from a directory
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

if env['platform'] == 'linux':
	libpath = '../libdemonless{}{}'.format(env['suffix'], env['SHLIBSUFFIX'])
	sharedlib = env.SharedLibrary(libpath, src)
	Default(sharedlib)

if env['platform'] == 'windows': # TODO
	libpath = '../libdemonless{}{}'.format(env['suffix'], env['SHLIBSUFFIX'])
	sharedlib = env.SharedLibrary(libpath, src)
	Default(sharedlib)
if env['platform'] == 'android': # TODO
	pass

