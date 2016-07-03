while read p; do
  scp -i ~/Desktop/Michigan.pem binServer binStarServer binClient run.sh filename.txt ubuntu@$p:/home/ubuntu
done <filename.txt
