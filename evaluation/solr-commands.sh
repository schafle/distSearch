#Follow instructions from http://stackoverflow.com/questions/31790202/solrcloud-5-and-zookeeper-config-upload/31795397#31795397

#Download SOlr
wget http://apache.mirrors.pair.com/lucene/solr/6.1.0/solr-6.1.0.tgz

#Change the zoo.cfg file

#Add id to myid on each node

#Start the zookeeper
./bin/zkServer.sh start
#Start solr in standalone mode
./bin/solr start

#Start Solr Cloud
bin/solr start -c -z ec2-54-212-144-222.us-west-2.compute.amazonaws.com:2181,ec2-54-185-121-26.us-west-2.compute.amazonaws.com:2181,ec2-52-13-69-89.us-west-2.compute.amazonaws.com:2181

#Check if the solr cluster is up and running
bin/solr status

#Index documents
./bin/post -c wiki-sample -d ../simple/

#Create a collection; should be run in any one node
./bin/solr create -c distSearch -shards 1 -replicationFactor 1

#Delete a collection
./bin/solr delete -c distSearch

#Index the documents
./bin/post -c distSearch /mnt/data/

#Search documents
time curl "http://ec2-54-203-201-231.us-west-2.compute.amazonaws.com:8983/solr/wiki-sample/select?wt=json&indent=true&q=\"United+Nations\"

time curl "http://ec2-52-13-68-8.us-west-2.compute.amazonaws.com:8983/solr/distSearch/select?q=Bigfoot&rows=700&shards=ec2-54-203-201-231.us-west-2.compute.amazonaws.com:8983/solr/distSearch,ec2-54-185-92-198.us-west-2.compute.amazonaws.com:8983/solr/distSearch,ec2-54-203-222-84.us-west-2.compute.amazonaws.com:8983/solr/distSearch,ec2-52-13-68-8.us-west-2.compute.amazonaws.com:8983/solr/distSearch,ec2-54-214-117-179.us-west-2.compute.amazonaws.com:8983/solr/distSearch,ec2-54-184-153-83.us-west-2.compute.amazonaws.com:8983/solr/distSearch,ec2-54-189-118-56.us-west-2.compute.amazonaws.com:8983/solr/distSearch&wt=json&indent=true"

time curl "http://ec2-52-13-68-8.us-west-2.compute.amazonaws.com:8983/solr/distSearch/select?q=Bigfoot&rows=700&shards=ec2-54-203-201-231.us-west-2.compute.amazonaws.com:8983/solr/distSearch,ec2-54-185-92-198.us-west-2.compute.amazonaws.com:8983/solr/distSearch,ec2-54-203-222-84.us-west-2.compute.amazonaws.com:8983/solr/distSearch&wt=json&indent=true"
