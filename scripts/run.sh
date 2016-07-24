b=1
position=`expr $1 - $b`
#echo $position
hostname_cmd="awk NR==$1 filename.txt"
#echo $hostname_cmd
hostname=`$hostname_cmd`
#echo $hostname
./binServer --indexdir /mnt/index --position $position --hostname $hostname --filename filename.txt
