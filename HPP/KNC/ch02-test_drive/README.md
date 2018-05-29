# Test Driving Intel Xeon Phi Systems

## Auto-Vectorization Benchmarks
We provide a simple SAXPY type calculation in the source file `helloflops1.c`.
To build this application simply run the `build.sh` script. 
The script allows the control of the level of optimization for the resulting
binary:
```bash 
 $ ./build.sh -h

Usage:

     ./build.sh [-v|-h] [-i|--input INPUT_FILE]

     -v|--version print to stdout any information related to this script.

     -h|--help print to stdout any help information included in the header
               of the script.

     -no-opt normal compilation (no autovectorization enabled).

     -knl (intended for compilation on KNL nodes) enable the use of the
          AVX-512 instruction set during compilation. For more details see
          documentation about the use of the -xMIC-AVX512 compiler flag.

     -skylake (intended for compilation on Skylake nodes) enable the use of
              the AVX-512 instruction set during compilation. For more details
              see documentation about the use of the -xMIC-AVX512 compiler flag.

     -haswell (inteded for compilation on Haswell nodes) enable the use of the
              AVX instruction set during compilation. For more details see
              documentation about the use of the -xAVX compiler flag.
```
