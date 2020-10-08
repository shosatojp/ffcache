rm -rf dist/
python3 setup.py sdist
pip3 install $(find dist -type f)
