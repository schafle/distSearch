sudo apt-get install cmake --assume-yes
sudo apt-get install libboost-all-dev --assume-yes
sudo apt-get install git --assume-yes
mkdir ../ext
git clone https://github.com/luceneplusplus/LucenePlusPlus.git ../ext/
wget https://github.com/easylogging/easyloggingpp/releases/download/v9.80/easyloggingpp_v9.80.tar.gz
mkdir ../ext/logger
tar -xvf easyloggingpp_v9.80.tar.gz -C ../ext/logger/
echo "Next steps:"
mkdir ../bin
echo "cd ../bin"
echo "cmake .."
echo "make"
echo "List DNS of all the nodes in a filename filename.txt and save it in scripts folder"
echo "Transfer binaries with ./move.sh"
echo "Kill running processes with ./kill.sh"
echo "Start running processes with ./start_tree.sh or ./start_star.sh"
echo "Cheers..."
