let index=1
while read p; do
  link_cmd="awk NR==$index $1"
  link=`$link_cmd`
  echo "ssh -n -i ~/Desktop/Michigan.pem -T ubuntu@$p 'wget $link'"
let "index++"
done <filename.txt
