parallel-ssh -i -h filename.txt -t 10000000 -x "-oStrictHostKeyChecking=no -i Michigan.pem" -l ubuntu "$1"
