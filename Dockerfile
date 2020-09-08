FROM ubuntu:18.04

RUN apt-get update && \
    apt-get install -y g++ python3.8-dev make python3-pip
RUN pip3 install pybind11

WORKDIR /home

CMD make
