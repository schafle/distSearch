while read p; do
  scp -o StrictHostKeyChecking=no -i ~/Michigan.pem setup_node.sh ubuntu@$p:/home/ubuntu
done <filename.txt
