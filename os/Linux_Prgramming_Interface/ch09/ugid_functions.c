/*
 * ugid_functions.h
 *
 * Taken from The Linux Programming Interface.
 */
#include "ugid_functions.h"
#include <ctype.h>
#include <pwd.h>
#include <grp.h>

/******************************************************************************
 *                               Public API                                   *
 *****************************************************************************/
/**
 * userNameFromId - Return name corresponding to 'uid', or NULL on return.
 * @uid: uid.
 */
char* userNameFromId(uid_t uid)
{
    struct passwd *pwd;

    pwd = getpwuid(uid);
    return (pwd == NULL) ? NULL : pwd->pw_name;
}

/******************************************************************************
 *                               Private API                                  *
 *****************************************************************************/
