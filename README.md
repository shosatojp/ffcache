# ffcache

![C/C++ CI](https://github.com/shosatojp/ffcache/workflows/C/C++%20CI/badge.svg)

Firefox cache extractor.

## Usage

### Get list of cache

```sh
ffcache --cache ~/.cache/mozilla/firefox/hoge.default/cache2 --list
```

### Save cached file

```sh
ffcache --cache ~/.cache/mozilla/firefox/hoge.default/cache2 --key https://example.com/image.png --out myimage.png
```

### Options

```sh
[usage]
ffcache [OPTIONS]
--list  -l      list all keys
--cache -c      cache2 directory
--key   -k      key
--out   -o      output path
```

### Python Binding

```py
import ffcache
```

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
