let index=1
while read p; do
  ssh -n -i ~/Desktop/Michigan.pem -T ubuntu@$p 'killall -9 binServer'
  ssh -n -i ~/Desktop/Michigan.pem -T ubuntu@$p 'ps -ef | grep defunct | grep -v grep | cut -b8-20 | xargs kill -9'
  #ssh -n -i ~/Desktop/Michigan.pem -T ubuntu@$p 'screen -d -m ./run.sh $index &'
let "index++"
done <filename.txt
