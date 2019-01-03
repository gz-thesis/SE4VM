#!/bin/bash

IFS=$'\n';
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

if [ ${1:0:1} == "/" ] 
then
    FILE=$1
else
    FILE=$DIR"/"$1
fi

FULLNAME=${FILE##*\/}
SHORTNAME=${FULLNAME%\.*}
NEWFILE=$SHORTNAME"_new.c"

annotate()
{
    tagName=$1
    fileName=$2
    lineNumber=${3%\;\"}
    tagType=${4##kind\:}

    if [ $tagType == 'function' ]; then
        # if it is a function, we insert a printf into the code with function name
        echo $lineNumber
        info="\tprintf(\"entered $tagName \\\n\");"
        sed -i "$(($lineNumber+2)) i\ $info" $NEWFILE
    fi
}

# create a new file 
cp $FILE $NEWFILE

for LINE in $(ctags -f - -R --sort=no -n --fields=+afikKmsSzt --extra=+fq "$FILE" | tac)
do
    IFS=$'\t'
    annotate $LINE
done

