sudo apt-get install cmake --assume-yes
sudo apt-get install libboost-all-dev --assume-yes
sudo apt-get install git --assume-yes
mkdir ../ext
git clone https://github.com/luceneplusplus/LucenePlusPlus.git ../ext/
wget https://github.com/easylogging/easyloggingpp/releases/download/v9.80/easyloggingpp_v9.80.tar.gz
mkdir ../ext/logger
tar -xvf easyloggingpp_v9.80.tar.gz -C ../ext/logger/
