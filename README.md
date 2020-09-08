# ffcache

![C/C++ CI](https://github.com/shosatojp/ffcache/workflows/C/C++%20CI/badge.svg)

Firefox cache extractor.

## build

### build on host

```sh
sudo apt-get install -y g++ python3.8-dev make python3-pip
pip3 install pybind11
make
```

### build with docker

```sh
sudo docker-compose up --build --build-arg PYTHON=python3.8
```
