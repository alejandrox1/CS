/*
 * idshow.c
 *
 * Display all user and group identifiers associated with a process.
 *
 * Note: This program uses Linux-specific calls and the Linux-specific
 * file-system user and group IDs.
 *
 * Taken from The Linux Prgramming Interface.
 */
#define _GNU_SOURCE
#include <sys/fsuid.h>
#include <unistd.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include "error_functions.h"
#include "ugid_functions.h"

#define SG_SIZE (NGROUPS_MAX + 1)

int main(void)
{
    uid_t ruid, euid, suid, fsuid;
    gid_t rgid, egid, sgid, fsgid;
    gid_t suppGroups[SG_SIZE];
    int numGroups, j;
    char *p;

    if (getresuid(&ruid, &euid, &suid) == -1)
        errExit("getresuid");
    if (getresgid(&rgid, &egid, &sgid) == -1)
        errExit("getresgid");

    /* 
     * Attempts to change the filesystem ID are always ignored for unprivileged
     * processes, even so, the following calls return the current filesystem
     * IDs.
     */
    fsuid = setfsuid(0);
    fsgid = setfsgid(0);

    printf("UID: ");
    p = userNameFromId(ruid);
    printf("real=%s (%ld); ", (p == NULL) ? "???" : p, (long)ruid);
    p = userNameFromId(ruid);                                                   
    printf("eff=%s (%ld); ", (p == NULL) ? "???" : p, (long)euid);
    p = userNameFromId(ruid);                                                   
    printf("saved=%s (%ld); ", (p == NULL) ? "???" : p, (long)suid);
    p = userNameFromId(fsuid);
    printf("fs=%s (%ld); ", (p == NULL) ? "???" : p, (long)fsuid);
    printf("\n");

    printf("GID: ");
    p = groupNameFromId(ruid);
    printf("real=%s (%ld); ", (p == NULL) ? "???" : p, (long)rgid);             
    p = groupNameFromId(ruid);                                                   
    printf("eff=%s (%ld); ", (p == NULL) ? "???" : p, (long)egid);              
    p = groupNameFromId(ruid);                                                   
    printf("saved=%s (%ld); ", (p == NULL) ? "???" : p, (long)sgid);            
    p = groupNameFromId(fsuid);                                                  
    printf("fs=%s (%ld); ", (p == NULL) ? "???" : p, (long)fsgid);              
    printf("\n");

    numGroups = getgroups(SG_SIZE, suppGroups);
    if (numGroups == -1)
        errExit("getgroups");

    printf("Supplementary groups (%d): ", numGroups);
    for (j = 0; j < numGroups; ++j)
    {
        p = groupNameFromId(suppGroups[j]);
        printf("%s (%ld) ", (p == NULL) ? "???" : p, (long)suppGroups[j]);
    }
    printf("\n");

    exit(EXIT_SUCCESS);
}
