if [[ $# -ne 2 ]] ; then
    echo 'Usage:'
    echo '     ./generate_input.sh 32 input.txt'
    exit 0
fi
cat /dev/urandom | tr -dc 'a-zA-Z0-9' | fold -w $1 | head -n 1 > $2
