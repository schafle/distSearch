while read p; do
  ssh -o StrictHostKeyChecking=no -n -i ~/Michigan.pem -T ubuntu@$p 'rm -rf logs/*'
done <filename.txt
