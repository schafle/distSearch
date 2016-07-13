sudo apt-get install software-properties-common
sudo add-apt-repository ppa:george-edison55/cmake-3.x
sudo apt-get update
sudo apt-get install g++
sudo apt-get install cmake --assume-yes
sudo apt-get install libboost-all-dev --assume-yes
sudo apt-get install git --assume-yes
sudo apt-get install p7zip-full
git clone https://github.com/luceneplusplus/LucenePlusPlus.git
echo "Next steps:"
echo "cd to LucenePlusPlus"
echo "mkdir build"
echo "cd build"
echo "cmake .."
echo "make"
