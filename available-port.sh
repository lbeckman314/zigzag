#!/bin/bash
# getPort() function: Chris Down, Stefan Leisten
#   https://unix.stackexchange.com/questions/55913/whats-the-easiest-way-to-find-an-unused-local-port
#   https://unix.stackexchange.com/questions/74185/how-can-i-prevent-grep-from-showing-up-in-ps-results
#   https://serverfault.com/questions/7503/how-to-determine-if-a-bash-variable-is-empty
#   https://stackoverflow.com/questions/192249/how-do-i-parse-command-line-arguments-in-bash

# control-c will terminate script execution and remove temporary files
trap "echo; echo 'SIGINT received: deleting temp files then exiting'; clean; exit 1" INT


# initial arg values
debug=0
port=-5
silent=0


getPort()
{
    read LOWERPORT UPPERPORT < /proc/sys/net/ipv4/ip_local_port_range
    while :
    do
            port="`shuf -i $LOWERPORT-$UPPERPORT -n 1`"
            ss -lpn | grep -q ":$port " || break

    done
    echo $port
}


main()
{
    getPort
}

# call main function
main
