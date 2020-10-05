FROM ubuntu:18.04
ARG PYTHON=python3.8
ENV PYTHON ${PYTHON}

RUN apt-get update && \
    apt-get install -y g++ ${PYTHON}-dev make python3-pip git
RUN ${PYTHON} -m pip install pybind11

WORKDIR /home
RUN git clone https://github.com/mapbox/gzip-hpp.git && \
    cp -r gzip-hpp/include/gzip /usr/include
CMD make PYTHON=${PYTHON}
