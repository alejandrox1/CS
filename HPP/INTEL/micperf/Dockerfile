FROM centos:7.2.1511
MAINTAINER alarcj137@gmail.com

ENV MKLROOT="/ot/mkl"
ENV LD_LIBRARY_PATH="/opt/intel/psxe_runtime/linux/compiler/lib/intel64_lin${LD_LIBRARY_PATH}"

# MKL runtime filename.
ARG MKL_RUNTIME_FILENAME="l_mklb_p_2017.1.132"

# Memkind and micperf RPM names.
ARG XPPSL_PATH="xppsl-1.5.3/rhel7.2/x86_64"
ARG MEMKIND="xppsl-memkind-1.5.3-4380.x86_64.rpm"
ARG MICPERF="xppsl-micperf-1.5.3-4380.x86_64.rpm"

# Install mkl runtime and benchmarks.
#ADD ${MKL_RUNTIME_FILENAME}.tgz ${MKLROOT}

# Install Intel parallel studio runtime from a repository. This will install
# MPI and OpenMP runtimes.
RUN rpm --import http://yum.repos.intel.com/2017/setup/RPM-GPG-KEY-intel-psxe-runtime-2017 && \
    rpm -Uhv http://yum.repos.intel.com/2017/setup/intel-psxe-runtime-2017-reposetup-1-0.noarch.rpm && \
    yum -y install intel-mpi-runtime-64 && \
    yum -y install intel-openmp-runtime-64

# Install Memkind from local RPM.
ADD ${XPPSL_PATH}/${MEMKIND} .
RUN yum install -y ./${MEMKIND} && \
    rm -f ${MEMKIND}

# Install micperf from the local RPM.
ADD ${XPPSL_PATH}/${MICPERF} .
RUN yum install -y ./${MICPERF} && \
    rm -f ./${MICPERF}

# Set Intel MPI enviroment variables and run command shell.
CMD source /opt/intel/psxe_runtime/linux/bin/psxevars.sh && /bin/sh
