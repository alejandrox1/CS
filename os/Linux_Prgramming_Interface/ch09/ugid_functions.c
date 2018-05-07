/*
 * ugid_functions.h
 *
 * Taken from The Linux Programming Interface.
 */
#include "ugid_functions.h"
#include <stdio.h>
#include <pwd.h>
#include <grp.h>

/******************************************************************************
 *                               Public API                                   *
 *****************************************************************************/
/**
 * groupNameFromId - Return name corresponding to 'gid', or NULL or return.
 * @gid: gid.
 */
char* groupNameFromId(gid_t gid)
{
    struct group *grp;

    grp = getgrgid(gid);
    return (grp == NULL) ? NULL : grp->gr_name;
}


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
