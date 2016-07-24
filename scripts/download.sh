nodes=$1
let index=1
while read p; do
	for i in {1..31}   #Iterate over all nodes
		do
			node_cmd="awk NR==$i filename.txt"
			node=`$node_cmd`
			link_cmd="awk NR==$index links.txt"
			link=`$link_cmd`
			echo ssh -n -i ~/Michigan.pem -T ubuntu@$node "cd /mnt; wget wget https://dumps.wikimedia.org/other/static_html_dumps/current/$link/wikipedia-$link-html.tar.7z"
			ssh -n -i ~/Michigan.pem -T ubuntu@$node "cd /mnt; wget https://dumps.wikimedia.org/other/static_html_dumps/current/$link/wikipedia-$link-html.tar.7z"
			let "index++"
		done
done <links.txt
