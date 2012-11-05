
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

if [ -f ${output} ]
then
    mv ${output} ${output}.bak
fi

compileFlag=1
for file in $files
do
    objFile="obj${i}.o"
    if [ -f ${objFile} ]
    then
	rm ${objFile}
    fi
    echo "g++ -fPIC -c $file -o ${objFile} -I${MYLIB}"
    g++ -fPIC -c $file -o ${objFile} -I${MYLIB}

    if [ -f ${objFile} ]
    then
	objList="${objList} ${objFile}"
    else
	compileFlag=0
    fi
    i=`expr $i + 1`
done

if [ ${compileFlag} -eq 1 ]
then
    echo "g++ -shared ${objList} -o ${output}"
    g++ -shared ${objList} -o ${output}
    echo "making headerfile.h ... "
    sh findHeader.sh
else
    echo ""
    echo "compile failed"
fi

rm ${objList}
cd ${nowDir}

