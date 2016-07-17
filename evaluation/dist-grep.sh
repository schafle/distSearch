if [[ $# -ne 4 ]] ; then
    echo 'Usage:'
    echo '     ./dist_grep.sh path_to_filename.txt path_to_aws_key.pem "Text you want to grep for" ~/directory'
    exit 0
fi
#echo parallel-ssh -i -h $1 -t 10000000 -x "-oStrictHostKeyChecking=no -i $2" -l ubuntu "grep -ilr \"$3\" ~/documents"
time parallel-ssh -P -h $1 -t 10000000 -x "-oStrictHostKeyChecking=no -i $2" -l ubuntu "grep -ilr \"$3\" $4"
