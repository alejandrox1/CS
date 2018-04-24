# Build Chain
To recreate the environment simply run:
```
./clean; ./build.sh && ./qemu.sh
```

`build.sh` will create the sysroot, install all headers, and compile libraries
and kernel.
`qemu.sh` will build cdrom image and spin up the emulator.


* Install Headers
  ```
  ./headers.sh
  ```

* Build libraries and kernel
  ```
  ./build.sh
  ```

* Create a bootale CDROM image
  ```
  ./iso.sh
  ```

* Qemu it up
  ```
  ./qemu.sh
  ```

* Clen Up
  ```
  ./clean.sh
  ```
