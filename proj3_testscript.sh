#!/bin/bash

pwd
ls &
ls -l &
cd /
sleep 20 &
ls &
pid
tty
/bin/ps -lfu nadiakiamilev
cd
pwd
ls -l
rm -f mail1 mail2
touch mail1
watchmail mail1
echo hi >> mail1
echo HiThere > mail2
watchmail mail2
echo there >> mail1
echo Subject: >> mail2
cat mail1
cat mail2
watchmail mail1 off
echo bye >> mail1
echo bye >> mail2
rm -f test1 test2 test3 test4 test5 test6 test7 test8
test-1+2 > test1
test-1+2 >> test2
test-1+2 >& test3
test-1+2 >>& test4
cat test1 test2 test3 test4
test-1+2 > test1
test-1+2 >> test2
test-1+2 >& test3
test-1+2 >>& test4
cat test1 test2 test3 test4
noclobber
test-1+2 > test5
test-1+2 >> test6
test-1+2 >& test7
test-1+2 >>& test8
cat test5 test6 test7 test8
test-1+2 > test5
test-1+2 >> test6
test-1+2 >& test7
test-1+2 >>& test8
cat test5 test6 test7 test8
grep hello < test8
grep error < test8
rm -f test9 test10 test11 test12
noclobber
test-1+2 > test9
test-1+2 >> test10
test-1+2 >& test11
test-1+2 >>& test12
cat test9 test10 test11 test12
ls | fgrep .c
./test-1+2 | grep  hello
./test-1+2 |& grep hello
./test-1+2 | grep output
./test-1+2 |& grep output
./test-1+2 |& grep error
pid
/bin/ps -lfu nadiakiamilev | grep defunct
