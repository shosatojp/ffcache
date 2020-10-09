set -e
python3 setup.py sdist
rm -rf build/ *.egg-info/
twine upload dist/*
rm -rf dist/
