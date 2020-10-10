project('ffcache', 'cpp',
  version : '0.0.12',
  default_options : ['cpp_std=c++17']
)
python = get_option('python')
pymod = import('python')
pyinst = pymod.find_installation(python)
pyinst.extension_module('_ffcache', 'src/pymain.cpp')
