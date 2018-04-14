#ifdef _ERRNO_H

# undef EDOM
# undef EILSEQ
# undef ERANGE
# include <linux/errno.h>

/* Linux has no ENOTSUP error code.  */                                         
# define ENOTSUP EOPNOTSUPP                                                     
                                                                                
/* Older Linux versions also had no ECANCELED error code.  */                   
# ifndef ECANCELED                                                              
#  define ECANCELED 125                                                         
# endif                                                                         
                                                                                
/* Support for error codes to support robust mutexes was added later, too.  */  
# ifndef EOWNERDEAD                                                             
#  define EOWNERDEAD        130                                                 
#  define ENOTRECOVERABLE   131                                                 
# endif                                                                         
                                                                                
# ifndef ERFKILL                                                                
#  define ERFKILL       132                                                     
# endif                                                                         
                                                                                
# ifndef EHWPOISON                                                              
#  define EHWPOISON     133                                                     
# endif

#endif /* ifndef _ERRNO_H */


#if !defined _ERRNO_H && defined __need_Emath
/* In case __need_Emath is defined. */
# define EDOM   33  /* Math argument out of domain of function.  */
# define EILSEQ 84  /* Illegal byte sequence.                    */
# define ERANGE 34  /* Math result not representable.            */
#endif /* !_ERRNO_H && __need_Emath */
