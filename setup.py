import setuptools
import glob
import subprocess
from setuptools import setup
from setuptools.command.install import install
from setuptools.command.build_ext import build_ext
from setuptools.extension import Extension
import sysconfig
import os
import shutil


class MakeExtension(Extension):
    def __init__(self, name, sourcedir=''):
        Extension.__init__(self, name, sources=[])
        self.sourcedir = os.path.abspath(sourcedir)


class my_ext(build_ext):
    def build_extension(self, ext):
        subprocess.run(['make', f'-j{os.cpu_count()}'])
        binname = '_ffcache.cpython-38-x86_64-linux-gnu.so'
        outpath = os.path.join(self.build_lib, binname)
        shutil.move(binname, outpath)


# class my_install(install):
#     def get_outputs(self):
#         return install.get_outputs(self) #+ ['_ffcache.cpython-38-x86_64-linux-gnu.so']


with open('README.md', 'rt', encoding='utf-8') as fh:
    long_description = fh.read()

setup(
    name='ffcache',
    version='0.0.9',
    author='shosatojp',
    author_email='me@shosato.jp',
    description='Firefox cache extractor',
    long_description=long_description,
    long_description_content_type='text/markdown',
    url='https://github.com/shosatojp/ffcache',
    packages=setuptools.find_packages(),
    classifiers=[
        'Programming Language :: Python :: 3',
        'License :: OSI Approved :: MIT License',
        'Operating System :: OS Independent',
    ],
    python_requires='>=3.6',
    scripts=[],
    install_requires=[
        'pybind11',
        'brotli'
    ],
    cmdclass={
        'build_ext': my_ext
    },
    ext_modules=[
        MakeExtension('_ffcache2'),
    ]
)
