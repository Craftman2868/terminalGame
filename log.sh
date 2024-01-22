#!/bin/zsh

p=$$

nostdin()
{
    while true; do
        read -sk 1 c

        case "$c" in
        "q")
            read -t 1 -n 10000 discard
            /bin/kill -SIGHUP $p
            break;
            ;;
        "l")
            clear
            ;;
        esac
    done
}

nostdin &

while true; do
    cat debug/pipe

    echo "\n========================================\n"
done
