export PREFIX="$HOME/opt/cross"
export TARGET=i686-elf
# Ensure compiler build is able to detect our new binutils once it has been
# built.
export PATH="$PREFIX/bin:$PATH"


# Ubuntu 16.04.3 LTS
# * binutils-2.24
# * gcc-4.8.2


############
#          #
# BINUTILS #
#          #
############

cd $HOME/src

if [ -d build-binutils ]; then rm -rf build-binutils; fi
mkdir build-binutils
cd build-binutils

# disable-nls : disable native language support
# with-sysroot : enables sysroot support 
../binutils-2.24/configure --target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror
make
make install

echo "\t>>> binutils-make"

#######
#     #
# GCC #
#     #
#######

cd $HOME/src

# The $PREFIX/bin dir _must_ be in the PATH. We did that above.
which -- $TARGET-as || echo $TARGET-as is not in the PATH

if [ -d build-gcc ]; then rm -rf build-gcc; fi
mkdir build-gcc
cd build-gcc

# without-headers : tells gcc not to rely on any C library (standard or
# 	runtime) being present for the target.
# enable-languages : tells gcc not to compile all other language frontends it
# 	supports 
../gcc-4.8.2/configure --target=$TARGET --prefix="$PREFIX" --disable-nls --enable-languages=c,c++ --without-headers 
make all-gcc
make all-target-libgcc
make install-gcc
make install-target-libgcc
