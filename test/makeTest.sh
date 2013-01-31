#. ${HOME}/Dropbox/settings/ubuntu/zsh_definition.sh

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
sh ../compile_c++_with_mylib.sh test.cpp test


