# ffcache

![C/C++ CI](https://github.com/shosatojp/ffcache/workflows/C/C++%20CI/badge.svg)

Firefox cache extractor.

## install

download artifacts from [here](https://github.com/shosatojp/ffcache/actions) or manually build.

## build

### build on host

```sh
sudo apt-get install -y g++ python3.8-dev make python3-pip
pip3 install pybind11
make
```

### build with docker

```sh
sudo docker-compose up --build
```

* specify python version in `docker-compose.yml` (default python3.8)
