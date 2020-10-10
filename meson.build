project('ffcache', 'cpp',
  version : '0.0.12',
  default_options : ['cpp_std=c++17']
)
python = get_option('python')
pymod = import('python')
pyinst = pymod.find_installation(python)
pybind11_dep = dependency('pybind11', fallback: ['pybind11','pybind11_dep'])
pyinst.extension_module('_ffcache', 
            'src/util.cpp',
            'src/ffcacheentry.cpp',
            'src/ffcacheindex.cpp',
            'src/httpheader.cpp',
            'src/structs.cpp',
            'src/ffcache.cpp',
            'src/pymain.cpp',
		    dependencies: [pybind11_dep])
