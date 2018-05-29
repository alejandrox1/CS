help (
[[
This module is for a locally-installed version of tophat.

Version 2.1.1

]])

whatis("Name: tophat")
whatis("Version: 2.1.1")
whatis("Category: computational biology, genomics")
whatis("Keywords: Biology, Genomics, Alignment, Sequencing")
whatis("URL: https://ccb.jhu.edu/software/tophat/tutorial.shtml")
whatis("Description: A spliced read mapper for RNA-seq")

setenv("TACC_TOPHAT_DIR", "/work/05692/jochoa/lonestar/apps/tophat/v2.1.1")
prepend_path("PATH",      "/work/05692/jochoa/lonestar/apps/tophat/v2.1.1/bin")

prereq("bowtie/2.2.6")
prereq("boost/1.59")
prereq("intel/16.0.1")
