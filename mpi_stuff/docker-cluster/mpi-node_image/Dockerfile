FROM alejandrox1/alpine-mpich-base:latest
MAINTAINER Jorge Alarcon Ochoa <alarcj137@gmail.com>
# Based off nlknguyen/alpine-mpich https://github.com/NLKNguyen/alpine-mpich/blob/master/onbuild/Dockerfile
#
# ENV variables frombase image:
#   USER - mpi (defined by ARG).
#   USER_HOME - home dir for USER.
#   WORKDIR - /projects (defined by ARG).

USER root

RUN apk update && apk add --no-cache openssh

###############################################################################
##                                                                           ##
##                               User Setup                                  ##
##                                                                           ##
###############################################################################
# Set a welcome message for when a user sshs into the container.
ADD welcome_msg.txt /etc/mtod
# Auto go to default working directory when user ssh login 
RUN echo "cd $WORKDIR" >> ${USER_HOME}/.profile

# Default hostfile location for mpirun. This file will be updated
# automatically.
ENV HYDRA_HOST_FILE /etc/opt/hosts
RUN echo "export HYDRA_HOST_FILE=${HYDRA_HOST_FILE}" >> /etc/profile
RUN touch ${HYDRA_HOST_FILE}
RUN chown ${USER}:${USER} ${HYDRA_HOST_FILE}


###############################################################################
##                                                                           ##
##                           SSH Server Setup                                ##
##                                                                           ##
###############################################################################
# Add host keys.
RUN cd /etc/ssh/ && ssh-keygen -A -N ''

# Config SSH Daemon.
RUN sed -i "s/#PasswordAuthentication.*/PasswordAuthentication no/g" /etc/ssh/sshd_config && \ 
    sed -i "s/#PermitRootLogin.*/PermitRootLogin no/g" /etc/ssh/sshd_config && \
    sed -i "s/#AuthorizedKeysFile/AuthorizedKeysFile/g" /etc/ssh/sshd_config

# Unlock non-password USER to enable SSH login.
RUN passwd -u ${USER}

# Set up user's public and private keys.
ENV SSHDIR ${USER_HOME}/.ssh
RUN mkdir -p ${SSHDIR}

# Default ssh config file that skips (yes/no) question when first login to the
# host. This file can be overwritten by the following onbuild step if ssh/ 
# directory has config file.
RUN echo "StrictHostKeyChecking no" > ${SSHDIR}/config

###############################################################################
##                                                                           ##
##                           General Setup                                   ##
##                                                                           ##
###############################################################################
#                                                                               
ADD get_hosts.sh /usr/local/bin/get_hosts                                          
RUN chmod +x /usr/local/bin/get_hosts

#
ADD auto_update_hosts.sh /usr/local/bin/auto_update_hosts
RUN chmod +x /usr/local/bin/auto_update_hosts

# 
ADD mpi_bootstrap.sh /usr/local/bin/mpi_bootstrap
RUN chmod +x /usr/local/bin/mpi_bootstrap


# Switch back to default user
USER ${USER}

