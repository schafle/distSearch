#Download SOlr
wget http://apache.mirrors.pair.com/lucene/solr/6.1.0/solr-6.1.0.tgz

#Start solr in standalone mode
./bin/solr start

#Start Solr Cloud
bin/solr -e cloud

#Index documents
./bin/post -c wiki-sample -d ../simple/

#Search documents
time curl "http://ec2-54-203-201-231.us-west-2.compute.amazonaws.com:8983/solr/wiki-sample/select?wt=json&indent=true&q=\"United+Nations\"
