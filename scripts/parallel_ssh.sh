parallel-ssh -i -h filename.txt -t 10000000 -x "-oStrictHostKeyChecking=no -i ~/Desktop/Michigan.pem" -l ubuntu 'ls'
