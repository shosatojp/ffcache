set -e
python3 setup.py sdist
rm -rf build/ *.egg-info/
pip3 install $(find dist -type f)
rm -rf dist/
