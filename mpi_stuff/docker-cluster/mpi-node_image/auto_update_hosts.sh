#!/bin/sh
#
# auto_update_hosts.
#
# Pass as argument the system's host file. Use this value to update the hosts
# obtained from `get_host`.
#

hosts=$(get_hosts)
echo "$hosts" > "$1"

while sleep 2; do
    current_hosts=$(get_hosts)
    [ "$hosts" != "$current_hosts" ] && echo "$current_hosts" > "$1"
    hosts=$current_hosts
done
