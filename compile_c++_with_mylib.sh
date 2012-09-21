
if [ $# -ne 2 ]
then
    echo "sh $0 [target] [output]"
    exit
fi

TARGET=$1
OUTPUT=$2

MYLIB="${HOME}/lib/myCppLib"
echo "g++ -Wall ${TARGET} -I${MYLIB} -L${MYLIB} -lmylib -o ${OUTPUT}"
g++ -Wall ${TARGET} -I${MYLIB} -L${MYLIB} -lmylib -o ${OUTPUT}
