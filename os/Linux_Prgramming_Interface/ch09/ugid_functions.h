/*                                                                              
 * ugid_functions.h                                                             
 *                                                                              
 * Taken from The Linux Programming Interface.                                  
 */                                                                             
#ifndef UGID_FUNCTIONS_H                                                        
#define UGID_FUNCTIONS_H                                                        

#include <pwd.h>
#include <grp.h>

char* groupNameFromId(gid_t gid);

char* userNameFromId(uid_t uid);                                                 
                                                                                
#endif 
