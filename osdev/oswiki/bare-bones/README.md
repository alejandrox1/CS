# Notes

# `size_t`
* `size_t` is defined by the compiler (`stddef.h:213`) as a: 
   ```
   #define __SIZE_TYPE__ long unsigned int
   typedef __SIZE_TYPE__ size_t;
   ```

* The Linux kernel (2.6.26) defines instead a type `__kernel_size_t` (`include/asm-alpha/posix_types.h:19`)
   not wanting to assume that GCC us being used,
   ```
   typedef unsigned long __kernel_size_t;
   ```
