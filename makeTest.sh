sharedLib="myLib.so"

if [ $# != 0 ]
then
    echo "make shared library : ${sharedLib}"
    sh makeSharedLib.sh
    echo "*******************************************************************"
fi

echo "make test"
echo "g++ test.cpp -o test ./${sharedLib} -Wall"
g++ test.cpp -o test ./${sharedLib} -Wall
