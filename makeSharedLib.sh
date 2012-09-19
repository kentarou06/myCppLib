
nowDir=`pwd`
cd ${HOME}/github/myCppLib

list_file="include_source.txt"
output="libmylib.so"
MYLIB="${HOME}/github/myCppLib"

files=`cat ${list_file}`
arg=""
objList=""
i=0;
MYLIB="${HOME}/github/myCppLib"
for file in $files
do
    objFile="obj${i}.o"
    echo "g++ -fPIC -c $file -o ${objFile} -I${MYLIB}"
    g++ -fPIC -c $file -o ${objFile} -I${MYLIB}

    objList="${objList} ${objFile}"
    i=`expr $i + 1`
done

echo "g++ -shared ${objList} -o ${output}"
g++ -shared ${objList} -o ${output}
#rm ${objList}

cd ${nowDir}

echo "making headerfile.h ... "
sh findHeader.sh