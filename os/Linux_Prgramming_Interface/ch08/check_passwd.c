/*
 * check_passwd.c
 *
 * Read a user name and password and check if these are valid.
 * This program assumes use os shadow passwords.
 *
 * Taken from The Linux programming Interface.
 */
/* Compile with -lcrypt */
#if ! defined(__sun)
# define _BSD_SOURCE   /* getpass() from unistd.h */
# ifndef _XOPEN_SOURCE 
# define _XOPEN_SOURCE /* crypt() from unistd.h   */
# endif
#endif
#include <unistd.h>
#include <limits.h>
#include <pwd.h>
#include <shadow.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "error_functions.h"
#include "get_pass.h"

#ifdef TRUE                                                                     
#undef TRUE  
#endif                                                                               
#ifdef FALSE                                                                    
#undef FALSE                                                                    
#endif                                                                          
                                                                                
typedef enum { FALSE, TRUE } Boolean;

int main(void)
{
    char *username, *password, *encrypted, *p;
    struct passwd *pwd;
    struct spwd *spwd;
    Boolean authOk;
    size_t len;
    long lnmax;

    /* Determine the size of a buffer to hold a username and allocate it. */
    lnmax = sysconf(_SC_LOGIN_NAME_MAX);
    if (lnmax == -1)
        lnmax = 256;

    username = malloc(lnmax);
    if (username == NULL)
        errExit("malloc");

    printf("Username: ");
    fflush(stdout);
    if (fgets(username, lnmax, stdin) == NULL)
        exit(EXIT_SUCCESS);

    len = strlen(username);
    if (username[len-1] == '\n')
        username[len-1] = '\0';

    /* Look up password and shadow password records for the username provided. */
    pwd = getpwnam(username);
    if (pwd == NULL)
        fatal("Couldn't get password record");
    spwd = getspnam(username);
    if (spwd == NULL && errno == EACCES)
        fatal("no permission to read shadow password file");
    
    if(spwd != NULL)
        pwd->pw_passwd = spwd->sp_pwdp;

    password = get_pass("Password: ");
    /* Encrypt password and erase cleartext version inmediately. */
    encrypted = crypt(password, pwd->pw_passwd);
    for (p = password; *p != '\0'; ++p)
        *p = '\0';
    free(password);

    if (encrypted == NULL)
        errExit("crypt");

    authOk = strcmp(encrypted, pwd->pw_passwd) == 0;
    if (!authOk)
    {
        printf("Incorrect password\n");
        exit(EXIT_FAILURE);
    }


    printf("Successfully authenticated: UID=%ld\n", (long)pwd->pw_uid);
    
    exit(EXIT_SUCCESS);
}
