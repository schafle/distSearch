while read p; do
  ssh -n -i ~/Michigan.pem -T ubuntu@$p 'killall -9 binServer'
  ssh -n -i ~/Michigan.pem -T ubuntu@$p 'killall -9 binStarServer'
  scp -i ~/Michigan.pem ../bin/binServer ../bin/binStarServer ../bin/binClient run.sh star.sh filename.txt ubuntu@$p:/home/ubuntu
done <filename.txt
