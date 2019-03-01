v1=1
v2=0
v3=0
v4=1

if test $# -lt 1
then

printf "Usage: cv.sh [a.b.c.d]
Generate version information using version number \"a.b.c.d\". If the numbers
were missed. Default version \"%d.%d.%d.%d\" will be used.
Example: cv.sh 1.0.0.1" $v1 $v2 $v3 $v4

else
    str=$1

    var=0
    IFS='.' read -ra ADDR <<< $str
    for i in "${ADDR[@]}"; do
        var=$((var+1))
        # echo $var

        if [ $var -eq 1 ]
        then
            v1=$i
        elif [ $var -eq 2 ]
        then
            v2=$i
        elif [ $var -eq 3 ]
        then
            v3=$i
        elif [ $var -eq 4 ]
        then
            v4=$i
        fi
    done
fi

printf "#define VER_FILE \"%d.%d.%d.%d\"\n"  $v1 $v2 $v3 $v4  > VersionInfo.h
git log --format="#define GIT_HASH \"%H\"" -1 >> VersionInfo.h
