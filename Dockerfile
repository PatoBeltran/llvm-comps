FROM        ubuntu:trusty
MAINTAINER  Patricio Beltran <pbelsal@gmail.com>

# Container setup
RUN apt-get update -qq && apt-get -y install cmake git build-essential vim python gdb

RUN mkdir -p /var/llvm
RUN mkdir -p /var/tests
RUN mkdir -p /var/Debug+Asserts

ENV SRC_ROOT /var/llvm
ENV OBJ_ROOT /var/Debug+Asserts

WORKDIR ${OBJ_ROOT}

ADD src/ /var/llvm
