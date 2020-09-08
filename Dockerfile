# FROM ubuntu:18.04

# RUN apt-get update && \
#     apt-get install -y g++ libpython3.8-dev pybind11-dev make python3.8 python3-pip
# RUN pip3 install pybind11

# WORKDIR /home

# CMD make

FROM alpine:edge
RUN apk add g++ python3-dev py3-pip make
RUN apk add make
RUN pip3 install pybind11
WORKDIR /home
CMD make