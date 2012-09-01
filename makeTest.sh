sharedLib="myLib.so"

echo "make shared library : ${sharedLib}"
sh makeSharedLib.sh
echo "**********************************************************************"
echo "make test"
g++ test.cpp -o test ./${sharedLib} -Wall
