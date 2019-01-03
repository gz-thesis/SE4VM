#!/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

if [ ${1:0:1} == "/" ] 
then
    TESTCASE=$1
else
    TESTCASE=$DIR"/"$1
fi

if [ ${2:0:1} == "/" ] 
then
    EXE1=$2
else
    EXE1=$DIR"/"$2
fi

if [ ${3:0:1} == "/" ] 
then
    EXE2=$3
else
    EXE2=$DIR"/"$3
fi

# prepare the tmp dir for testing results
tmp=/tmp/$(/bin/date +%s)/
/bin/mkdir -p $tmp
result=$DIR"/result.txt"
touch $result

# for each testcase
for f in $TESTCASE"/"*.ktest ;
do
    fname=${f##*\/}
    fname=${fname%%\.*}
    output1=$tmp$fname".1.out"
    output2=$tmp$fname".2.out"
    KTEST_FILE=f $EXE1 &> $output1
    KTEST_FILE=f $EXE2 &> $output2

    if diff $output1 $output2 > /dev/null ;
    then
        echo "identical" > /dev/null
    else
        echo "TESTCASE $fname HAVE DIFFERENT PATH" >> $result
        echo -e "path before\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>" >> $result
        cat $output1 >> $result
        echo -e "path after\n<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" >> $result
        cat $output2 >> $result
        echo "==================================================" >>$result
        echo 
        echo
    fi
done
