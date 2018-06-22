# Life Advice

A ton of life advice (some of it from the 
[Stampede2 supercomputer](https://portal.tacc.utexas.edu/user-guides/stampede2)).


<!--========================================================================-->
<!-- Tips and tricks for general administration, searching, compiling, etc. -->
<!--========================================================================-->
# General
## Bash functions
* To print a random number between `1` and `X` do:
  ```
  $ echo $[RANDOM%X+1]
  ```

## Searching
* When you need to match all `.*` files except for the current directory `.` or
  the parent directory `..`, use the `.??*` file globbing pattern. 
  For example: 
  ```
  cp .??* dot.orig
  ```
  copies all the dot files in a directory named `dot.orig`.

* Commands like `ls`, `mv` and `cp` support file globbing pattern matching:
  ```bash
  ls t*     # -> list all files that start with t
  ls [tx]*  # -> list all files that do start with t or x
  ls [^tx]* # -> list all files that do NOT start with t or x
  ls t?.sh  # -> list files like t1.sh t2.sh but not t22.sh
  ```

## Administration
* You can get the status of the last command with `$?`. It is 0 for success
  and non-zero for failure:
  ```
  echo "$?"
  ```

* Since bash sources `~/.bashrc` on every sub-shell you want to prevent things
  like path duplication by putting in guard if statements:
  ```
  if [ -z "$__INIT" ]; then
      export __INIT=1
      export PATH=$HOME/bin:$PATH
  fi
  ```

* To see how to control an ssh session type: `<enter>~?`

## Compilation
* Are you having trouble figuring out why your link went bad? 
  Try adding `-Wl,--trace` to your link line and capture the output.





<!--========================================================================-->
<!-- More File system Administration specific commands                      -->
<!--========================================================================-->
# File System
## Diagnostics
* Use the command `du -h *` to see disk usage for all directories in the current directory.

* The command `ls -F` adds an extra character to mark file type: 
  `/` for directories `*` executables and `@` for symbolic links.

##  Navigating aroud the File system
* If you set `shopt -s autocd` in the bash shell, then you'll be able to change
  directories by just typing the name of the directory.

* You are writing too fast and misspelled the path of your directory while working 
  in an interactive bash session? Try the following command `shopt -s cdspell` 
  and bash will try several minor spelling corrections to see if it can find 
  the actual directory.

* This up shell function can change directory quickly up the directory tree: `up 2` 
  is the same as `cd ../..`. Extra bonus points if you understand how the printf trick works.
  ```
  up() { cd $(eval "printf '../'%.0s {1..$1}") && pwd }
  ```

* When running a for loop in multiple directories, it may be helpful to use     
  parentheses to create a sub-shell:                                            
  ```                                                                           
  for d in d1 d2 d3 ; do ( cd $d; do_some_command_here; ); done                 
  ``` 





<!--========================================================================-->
<!-- Building and throuble shooting cool applications                       -->
<!--========================================================================-->
# Building
* If your build process is failing and you know that you need some extra library
  directories. Try setting `LIBRARY_PATH` to be colon separated list of directories.
  Both the intel and gcc compilers will use the list as library directories (`-L/foo/bar`).

## Building Scientific Stuff
* To see `MVAPICH2`'s process mapping, `export MV2_SHOW_CPU_BINDING=1` inside
  your script or before launching your job.






# Running
* Does rank 0 need more memory than your other `MPI` tasks?
  If you have 64 tasks, for example, allocate 5 nodes and launch with:
  ```
  ibrun -n 64 -o 15 # puts rank 0 on a node by itself.
  ```

# TACC
* You can find all installed bio codes by executing
  ```
  module keyword bio
  ```

## Software Building
* Add the following include statements in your Makefile to compile PETSc code:
  ```
  include ${PETSC_DIR}/conf/variables
  include ${PETSC_DIR}/conf/rules
  ```

* Linking to TACC packages? You don't have to remember their locations. 
  Just use `$TACC_THATPACKAGE_DIR`, `$TACC_THATPACKAGE_INC`, and `$TACC_THATPACKAGE_LIB`. 
  These variables will be defined after you load the module.

* Want to install your own python modules? Do `module help python`.

## Software Running
* If you have an MPI application with threads, don't forget to use 
  `tacc_affinity`. See the system user guide for more details.

## Diagnostics
* Looking for a file? On a `Lustre file system` like `$WORK`, and `$SCRATCH`, 
  use `lfs find` instead of `find` to search for files. It is much faster:
  ```bash
  lfs find <directory> -name '*.c'
  ```

* Curious about:
  - The network topology in your job? 
  - Job resource utilization reports
  - Would you like to know how much memory your code uses during execution?
  
  Try TACC's [remora](https://github.com/TACC/remora) tool:
  ```
  module load remora
  module help remora
  ```

# Cool Command-line Utilities
* [Reshaping JSON with jq](https://programminghistorian.org/en/lessons/json-and-jq)
