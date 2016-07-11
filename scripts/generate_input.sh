if [[ $# -ne 2 ]] ; then
    echo 'Usage:'
    echo '     ./generate_input.sh 32 input.txt'
    exit 0
fi
val=`expr $1 - 37`
cat /dev/urandom | tr -dc 'a-zA-Z0-9' | fold -w $val | head -n 1 > $2
