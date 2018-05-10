FROM ubuntu:18.04
MAINTAINER Jorge Alarcon Ochoa

ARG DEBIAN_FRONTEND=noninteractive

RUN apt-get update -y && \
    apt-get install -y gcc-5

#RUN apt-get install -y software-properties-common ubuntu-drivers-common && \
#    add-apt-repository ppa:graphics-drivers/ppa && \
#    apt-get update -y && \
#    ubuntu-drivers autoinstall

# GM107M [GeForce GTX 960M]
RUN apt-get install -y nvidia-driver-390 nvidia-cuda-toolkit 

CMD ["/bin/bash"]
