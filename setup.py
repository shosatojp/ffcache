import setuptools
import glob
import subprocess
from setuptools import setup
from setuptools.command.build_ext import build_ext
from setuptools.extension import Extension
import os
import shutil


class my_ext(build_ext):
    def build_extension(self, _):
        subprocess.run(['make', 'py', f'-j{os.cpu_count()}'])
        bins = glob.glob('*.so')
        for bin in bins:
            outpath = os.path.join(self.build_lib, bin)
            shutil.move(bin, outpath)


with open('README.md', 'rt', encoding='utf-8') as fh:
    long_description = fh.read()

setup(
    name='ffcache',
    version='0.0.12',
    author='shosatojp',
    author_email='me@shosato.jp',
    description='Firefox cache extractor',
    long_description=long_description,
    long_description_content_type='text/markdown',
    url='https://github.com/shosatojp/ffcache',
    packages=setuptools.find_packages(),  # find package `ffcache`
    classifiers=[
        'Programming Language :: Python :: 3',
        'License :: OSI Approved :: MIT License',
        'Operating System :: OS Independent',
    ],
    python_requires='>=3.6',
    install_requires=[
        'pybind11',
        'brotli',
    ],
    cmdclass={
        # custom build script
        'build_ext': my_ext,
    },
    ext_modules=[
        # for c++ extension
        Extension('', []),
    ]
)
