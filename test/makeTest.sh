. ${HOME}/Dropbox/settings/ubuntu/zsh_definition.sh

sharedLib="libmyLib.so"

if [ $# != 0 ]
then
    echo "make shared library : ${sharedLib}"
    cd ..
    sh makeSharedLib.sh
    cd test/
    echo "*******************************************************************"
fi

echo "make test"

MYLIB="${HOME}/github/myCppLib"
echo "g++ -Wall test.cpp -I${MYLIB} -L${MYLIB} -lmylib -o test"
g++ -Wall test.cpp -I${MYLIB} -L${MYLIB} -lmylib -o test

