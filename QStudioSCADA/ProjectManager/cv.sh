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

git log --format="#define GIT_HASH \"%H\\0\"" -1 > VersionInfo.rc

printf "\
VS_VERSION_INFO VERSIONINFO\n\
 FILEVERSION %d,%d,%d,%d\n\
 PRODUCTVERSION %d,%d,%d,%d\n\
 FILEFLAGSMASK 0x3fL\n\
#ifdef _DEBUG\n\
 FILEFLAGS 0x1L\n\
#else\n\
 FILEFLAGS 0x0L\n\
#endif\n\
 FILEOS 0x40004L\n\
 FILETYPE 0x1L\n\
 FILESUBTYPE 0x0L\n\
BEGIN\n\
    BLOCK \"StringFileInfo\"\n\
    BEGIN\n\
        BLOCK \"080403A8\"\n\
        BEGIN\n\
            VALUE \"CompanyName\"      , \"SHENZHEN ZHILAI SCI AND TECH CO., LTD\\\\0\"\n\
            VALUE \"FileDescription\"  , \"zhilaibox\\\\0\"\n\
            VALUE \"ProductName\"      , \"zhilaibox\\\\0\"\n\
            VALUE \"LegalTrademarks\"  , \"zhilaibox\\\\0\"\n\
            VALUE \"InternalName\"     , \"zhilaibox.exe\\\\0\"\n\
            VALUE \"OriginalFilename\" , \"zhilaibox.exe\\\\0\"\n\
            VALUE \"PrivateBuild\"     , GIT_HASH\n\
            VALUE \"FileVersion\"      , \"%d.%d.%d.%d\\\\0\"\n\
            VALUE \"ProductVersion\"   , GIT_HASH\n\
        END\n\
    END\n\
    BLOCK \"VarFileInfo\"\n\
    BEGIN\n\
        VALUE \"Translation\", 0x0804, 0x03A8\n\
    END\n\
END\n\
"  \
$v1 $v2 $v3 $v4 \
$v1 $v2 $v3 $v4 \
$v1 $v2 $v3 $v4 \
>> VersionInfo.rc

printf "#define VER_FILE \"%d.%d.%d.%d\"\n"  $v1 $v2 $v3 $v4  > VersionInfo.h
git log --format="#define GIT_HASH \"%H\"" -1 >> VersionInfo.h
