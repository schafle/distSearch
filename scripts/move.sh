echo -n > start_tree.sh
echo -n > start_star.sh
client=`awk NR==1 filename.txt`
scp -o StrictHostKeyChecking=no -i ~/Michigan.pem generate_input.sh ubuntu@$client:/home/ubuntu
ssh -o StrictHostKeyChecking=no -n -i ~/Michigan.pem -T ubuntu@$client 'killall -9 binClient'
let index=1
while read p; do
  ssh -o StrictHostKeyChecking=no -n -i ~/Michigan.pem -T ubuntu@$p 'killall -9 binServer'
 # ssh -o StrictHostKeyChecking=no -n -i ~/Michigan.pem -T ubuntu@$p 'killall -9 binStarServer'
  scp -o StrictHostKeyChecking=no -i ~/Michigan.pem setup_node.sh ../bin/binServer ../bin/binStarServer ../bin/binClient run.sh star.sh extract.sh filename.txt ubuntu@$p:/home/ubuntu
  echo "ssh -o StrictHostKeyChecking=no -n -i ~/Michigan.pem -T ubuntu@$p \" sh -c 'nohup ./run.sh $index > /dev/null 2>&1 &'\"" >> start_tree.sh
 # echo "ssh -o StrictHostKeyChecking=no -n -i ~/Michigan.pem -T ubuntu@$p 'screen -d -m ./star.sh $index &'" >> start_star.sh
let "index++"
done <filename.txt
chmod 700 start_tree.sh
chmod 700 start_star.sh
