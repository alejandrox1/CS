# General
* To print a random number between `1` and `X` do:
  ```
  $ echo $[RANDOM%X+1]
  ```

* When you need to match all `.*` files except for the current directory `.` or
  the parent directory `..`, use the `.??*` file globbing pattern. 
  For example: 
  ```
  cp .??* dot.orig
  ```
  copies all the dot files in a directory named `dot.orig`.

# File System
## Diagnostics
* Use the command `du -h *` to see disk usage for all directories in the current directory.

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

# Building
* If your build process is failing and you know that you need some extra library
  directories. Try setting `LIBRARY_PATH` to be colon separated list of directories.
  Both the intel and gcc compilers will use the list as library directories (`-L/foo/bar`).


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

## Diagnostics
* Did you know that job resource utilization reports are available via TACC's 
  [remora](https://github.com/TACC/remora) tool? Try it:
  ```
  module load remora
  module help remora
  ```


When running a for loop in multiple directories, it may be helpful to use parentheses to create a sub-shell:
      $ for d in d1 d2 d3 ; do ( cd $d; do_some_command_here; ); done

Linking to TACC packages? You don't have to remember their locations. Just use $TACC_THATPACKAGE_DIR, $TACC_THATPACKAGE_INC, and $TACC_THATPACKAGE_LIB. These
   variables will be defined after you load the module.

Want to install your own python modules? Do "module help python".

Does rank 0 need more memory than your other MPI tasks? If you have 64 tasks, for example, allocate 5 nodes and launch with:
     $ ibrun -n 64 -o 15 # puts rank 0 on a node by itself.

When you need to match all ".*" files except for the current directory "." or the parent directory "..", use the ".??*" file globbing pattern. For example: "cp
   .??* dot.orig" copies all the dot files in a directory named "dot.orig".

You can get the status of the last command with "$?". It is 0 for success and non-zero for failure:
       $ echo "$?"

If you have an MPI application with threads, don't forget to use tacc_affinity. See the system user guide for more details.

Curious about the network topology in your job? Try TACC's remora tool:
     $ module load remora
     $ module help remora

When running a for loop in multiple directories, it may be helpful to use parentheses to create a sub-shell:
      $ for d in d1 d2 d3 ; do ( cd $d; do_some_command_here; ); done

