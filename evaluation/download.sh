while read p; do
	wget https://dumps.wikimedia.org/other/static_html_dumps/current/$p/wikipedia-$p-html.tar.7z
done <data.txt

