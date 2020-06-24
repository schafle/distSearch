# Optimizing Search in Un-Sharded Large-Scale Distributed Systems
Distributed file systems and storage networks are increasingly common due to the need to store large volumes of unstructured data. While these systems support large-scale storage, they create challenges relating to efficiently discovering, accessing, managing, and analyzing distributed data. At the core of these challenges is the need to support scalable discovery of unstructured data. Traditional search methods leverage centralized and globally sharded indexes—a single index distributed based on contents. We present a distributed search framework that does not rely on sharding and can be applied to a range of distributed storage models. Our approach is built on top of Lucene and utilizes search trees to distribute queries and parallelize the search. To further optimize query performance we explore methods to prioritize indexes based on size. Our search framework is evaluated against alternatives like Grep and Solr.

## Setting up a node
- install git 
	`sudo apt-get install git --assume-yes`
- clone the repository
	`git clone https://github.com/wtpoo/distSearch.git`
- Locate scripts directory and run setup.sh; It will install other necessary dependencies and guide you through process of instantiating a node

## Documentation
Find the documentation for this project [here](http://wtpoo.github.io/distSearch/ "documentation").
