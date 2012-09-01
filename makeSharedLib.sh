list_file="include_source.txt"
output="myLib.so"

files=`cat ${list_file}`
arg=""
for file in $files
do
    arg="${arg} ${file}"
#    arg="${arg}-c ${file} "
done

echo "g++ -shared${arg} -o ${output} -fPIC -Wall"
g++ -shared ${arg} -o ${output} -fPIC -Wall