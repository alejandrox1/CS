#ifndef __PTHRUTILS_H__
#define __PTHRUTILS_H__

#include <pthread.h>


struct params                                                                   
{                                                                               
	pthread_mutex_t mutex;                                                      
	pthread_cond_t done;                                                        
	int commfd;                                                                 
};                                                                              
                                                                                
typedef struct params params_t;

#endif // __PTHRUTILS_H__
