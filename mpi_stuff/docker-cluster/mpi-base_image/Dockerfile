FROM alpine:3.7
MAINTAINER Jorge Alarcon Ochoa <alarcj137@gmail.com>
# Based off nlknguyen/alpine-mpich https://github.com/NLKNguyen/alpine-mpich/blob/master/base/Dockerfile

RUN apk update && apk upgrade && apk add --no-cache gfortran build-base

# Install MPICH.
ARG MPICH_VERSION="3.2"
ARG MPICH_CONFIGURE_OPTIONS
ARG MPICH_MAKE_OPTIONS

RUN mkdir /tmp/mpich-src
WORKDIR /tmp/mpich-src
RUN wget http://www.mpich.org/static/downloads/${MPICH_VERSION}/mpich-${MPICH_VERSION}.tar.gz && \
    tar xfz mpich-${MPICH_VERSION}.tar.gz  && \
    cd mpich-${MPICH_VERSION} && \
    ./configure ${MPICH_CONFIGURE_OPTIONS} && \
    make ${MPICH_MAKE_OPTIONS} && make install && \
    rm -rf /tmp/mpich-src

# Test MPI installation.
RUN mkdir /tmp/mpich_test
WORKDIR /tmp/mpich_test
ADD mpich_test .
RUN sh mpich_test.sh && rm -rf /tmp/mpich_test

# Add default user.
ARG USER=mpi
ENV USER ${USER}
ENV USER_HOME /home/${USER}

RUN adduser -D ${USER} && \
    echo "${USER}   ALL=(ALL) NOPASSWD:ALL" >> /etc/sudoers && \
    chown -R ${USER}:${USER} ${USER_HOME}

# Create working directory for user.
ARG WORKDIR=/project
ENV WORKDIR ${WORKDIR}

RUN mkdir ${WORKDIR} && chown -R ${USER}:${USER} ${WORKDIR}


WORKDIR ${WORKDIR}
USER ${USER}

CMD ["/bin/ash"]
