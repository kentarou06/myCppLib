
nowDir=`pwd`
cd ${HOME}/lib/myCppLib

list_file="include_source.txt"
output="libmylib.so"
MYLIB="${HOME}/lib/myCppLib"

files=`cat ${list_file}`
arg=""
objList=""
i=0;
MYLIB="${HOME}/lib/myCppLib"
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

cd ${nowDir}

echo "making headerfile.h ... "
sh findHeader.sh