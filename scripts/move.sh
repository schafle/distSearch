while read p; do
  ssh -n -i ~/Desktop/Michigan.pem -T ubuntu@$p 'killall -9 binServer'
  scp -i ~/Desktop/Michigan.pem binServer binStarServer binClient run.sh filename.txt ubuntu@$p:/home/ubuntu
done <filename.txt