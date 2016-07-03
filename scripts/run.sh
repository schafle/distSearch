b=1
position=`expr $1 - $b`
#echo $position
hostname_cmd="awk NR==$1 filename.txt"
#echo $hostname_cmd
hostname=`$hostname_cmd`
#echo $hostname
./binServer . $position $hostname filename.txt
