FROM ubuntu:18.04
ARG PYTHON=python3.8
ENV PYTHON ${PYTHON}

RUN apt-get update && \
    apt-get install -y g++ ${PYTHON}-dev make python3-pip
RUN ${PYTHON} -m pip install pybind11

WORKDIR /home

CMD make PYTHON=${PYTHON}
