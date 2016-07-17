nodes=$1
while read p; do
	let index=1
	for i in {1..3}   #Iterate over all nodes
		do
			node_cmd="awk NR==$i filename.txt"
			node=`$node_cmd`
			link_cmd="awk NR==$index links.txt"
			link=`$link_cmd`
			echo "ssh -n -i ~/Michigan.pem -T ubuntu@$node 'cd /mnt; wget $link'"
			let "index++"
		done
done <links.txt
