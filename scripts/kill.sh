let index=1
if [[ $1 = "star" ]] ; then
	while read p; do
		  ssh -o StrictHostKeyChecking=no -n -i ~/Michigan.pem -T ubuntu@$p 'killall -9 binStarServer'
		  #ssh -o StrictHostKeyChecking=no -n -i ~/Michigan.pem -T ubuntu@$p 'ps -ef | grep defunct | grep -v grep | cut -b8-20 | xargs kill -9'
		  #ssh -n -i ~/Desktop/Michigan.pem -T ubuntu@$p 'screen -d -m ./run.sh $index &'
	let "index++"
	done <filename.txt
	exit 0
fi
if [[ $1 = "tree" ]] ; then
	while read p; do
		  ssh -o StrictHostKeyChecking=no -n -i ~/Michigan.pem -T ubuntu@$p 'killall -9 binServer'
		  ssh -o StrictHostKeyChecking=no -n -i ~/Michigan.pem -T ubuntu@$p 'ps -ef | grep defunct | grep -v grep | cut -b8-20 | xargs kill -9'
		  #ssh -n -i ~/Desktop/Michigan.pem -T ubuntu@$p 'screen -d -m ./run.sh $index &'
	let "index++"
	done <filename.txt
fi

