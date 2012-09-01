
list_file="include_source.txt"
output="myLib.o"

files=`cat ${list_file}`
arg=""
for file in $files
do
    arg="${arg} ${file}"
done

echo "g++ -c ${arg} -o ${output}"
g++ -c ${arg} -o ${output}