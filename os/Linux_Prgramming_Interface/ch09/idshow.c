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
#define __GNU_SOURCE
#include <sys/fsuid.h>
#include <unistd.h>
#include <limits.h>
#include <stdio.h>
#include "error_functions.h"
#include "ugid_funcstions.h"

#define SG_SIZE (NGROUPS_MAX + 1)

int main(int argc, char* argv[])
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
    p = userNameFromID(ruid);
    printf("real=%s (%ld); ", (p == NULL) ? "???" : p, (long)ruid);
    p = userNameFromID(ruid);                                                   
    printf("eff=%s (%ld); ", (p == NULL) ? "???" : p, (long)euid);
    p = userNameFromID(ruid);                                                   
    printf("saved=%s (%ld); ", (p == NULL) ? "???" : p, (long)suid);

    exit(EXIT_SUCCESS);
}
