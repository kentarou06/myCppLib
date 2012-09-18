
files=`find -name "*.h"`

output="headerfile.h"

echo "" > ${output}

for file in ${files}
do
    echo "  ${file}"
    if [ ${file} != "./${output}" ]
    then
	echo "/*   filename : ${file}   */" >> ${output}
	cat ${file} >> ${output}
	echo "" >> ${output}
	echo "" >> ${output}
	echo "/*************************************************************/" >> ${output}
	echo "" >> ${output}
	echo "" >> ${output}
    fi
done

echo "output : ${output}"