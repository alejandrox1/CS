# MPI Base Image

The [Dockerfile](Dockerfile) in this directory dictates how to build a
container image from `alpine:3.7` with `MPICH-3.2`.

The container image build MPI with C, C++, and Fortran support; creates and 
adds a default user, `USER`, to the list of suddoers as well as creating a
workspace, `WORKDIR=/project` for said user.

The directory [`mpich_test`](mpich_test) is used while building the container
image to test for the installation of the C and Fortran MPI compiler wrappers.

This image is based off [nlknguyen/alpine-mpich's base image](https://github.com/NLKNguyen/alpine-mpich/blob/master/base/Dockerfile).
