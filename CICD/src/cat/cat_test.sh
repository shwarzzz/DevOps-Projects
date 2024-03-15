#!/bin/bash

SUCCESS=0
FAIL=0
COUNTER=0
DIFF_RES=""

declare -a tests=(
"VAR testt.txt"
)

declare -a extra=(
"-s test"
"-b -e -n -s -t -v test"
"-t test1"
"-n test3" 
"-n -b test1"
"-s -n -e test1"
"-n test1"
"-n test1"
"-n test1 test3"
"-v test4"
)

testing()
{
    t=$(echo $@ | sed "s/VAR/$var/")
    ./s21_cat $t > test_s21_cat.log
    cat $t > test_sys_cat.log
    DIFF_RES="$(diff -s test_s21_cat.log test_sys_cat.log)"
    (( COUNTER++ ))
    if [ "$DIFF_RES" == "Files test_s21_cat.log and test_sys_cat.log are identical" ]
    then
      (( SUCCESS++ ))
        echo "Fail: $FAIL Success: $SUCCESS Number: $COUNTER cat $t"
    else
      (( FAIL++ ))
        echo "Fail: $FAIL Success: $SUCCESS Number: $COUNTER cat $t"
    fi
    rm test_s21_cat.log test_sys_cat.log
}

# специфические тесты
for i in "${extra[@]}"
do
    var="-"
    testing $i
done

# 1 параметр
for var1 in b e n s t v
do
    for i in "${tests[@]}"
    do
        var="-$var1"
        testing $i
    done
done

echo "FAIL: $FAIL"
echo "SUCCESS: $SUCCESS"
echo "ALL: $COUNTER"

if [ $FAIL != 0 ]
then
    exit 1
else
    exit 0
fi
