#!/bin/bash

export reset="\e[0m"                                                            
export blue="\e[1;34m"                                                          
export green="\e[1;42m"                                                           
export red="\e[1;31m"

#export ASCII_ART_HEADER="                                                              
#         __v_                                                                   
#        (.___\/{                                                                
#~^~^~^~^~^~^~^~^~^~^~^~^~"

export ASCII_ART_HEADER_BASE='                                                      
                    ##        .                                                 
              ## ## ##       ==                                                 
           ## ## ## ##      ===                                                 
       /""""""""""""""""\___/ ===                                               
  ~~~ {~~ ~~~~ ~~~ ~~~~ ~~ ~ /  ===- ~~~                                        
       \______ o          __/                                                   
         \    \        __/                                                      
          \____\______/                                                         
                                                                                
          |          |                                                          
       __ |  __   __ | _  __   _                                                
      /  \| /  \ /   |/  / _\ |                                                 
      \__/| \__/ \__ |\_ \__  |                                            
'

export ASCII_ART_UP1=${ASCII_ART_HEADER_BASE}'    Bootsraping MPI Docker Cluster...'
export ASCII_ART_UP2=${ASCII_ART_HEADER_BASE}'       MPI Docker Cluster Ready...'
export ASCII_ART_DOWN=${ASCII_ART_HEADER_BASE}'    Tearing down MPI Docker Cluster...'

export ASCII_ART_HEADER1='
                    ##        .
              ## ## ##       ==
           ## ## ## ##      ===
       /""""""""""""""""\___/ ===
  ~~~ {~~ ~~~~ ~~~ ~~~~ ~~ ~ /  ===- ~~~
       \______ o          __/
         \    \        __/
          \____\______/

          |          |
       __ |  __   __ | _  __   _
      /  \| /  \ /   |/  / _\ |
      \__/| \__/ \__ |\_ \__  |

    Tearing down MPI Docker Cluster...
'
