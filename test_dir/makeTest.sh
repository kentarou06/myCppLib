. ${HOME}/Dropbox/settings/ubuntu/zsh_definition.sh

echo ${CPP_OPT}

sharedLib="myLib.so"

if [ $# != 0 ]
then
    echo "make shared library : ${sharedLib}"
    sh makeSharedLib.sh
    echo "*******************************************************************"
fi

echo "make test"
echo "g++ test.cpp -o test ./${sharedLib} -Wall"
#g++ test.cpp -o test ./${sharedLib} -Wall # success

#g++ test.cpp -o test ${CPP_OPT} -Wall
#g++ ${CPP_OPT} test.cpp -o test -Wall # failed


#g++ -I/home/kyamada/github/myCppLib/ /home/kyamada/github/myCppLib/myLib.so test.cpp -o test -Wall
g++ test.cpp -o test /home/kyamada/github/myCppLib/myLib.so -Wall