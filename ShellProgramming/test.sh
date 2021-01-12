#! /bin/bash

# Test for read100
gcc read100.c -o read100
touch /tmp/write_file
./read100 /tmp/write_file

if [ $? -eq 0 ]
then 
	echo "read100 pass"
else 
	echo "read100 fail"
fi

# Test for read510
gcc read510.c -o read510
echo "MynameisRohit." > /tmp/read_file
./read510 /tmp/read_file
if [ $? -eq 0 ]
then
	echo "read510 pass"
else
	echo "read510 fail"
fi


# Test for copy
gcc copy.c -o copy
./copy /tmp/read_file /tmp/read_file_copy
if [ $? -eq 0 ]
then
	diff /tmp/read_file /tmp/read_file_copy
	if [ $? -eq 0 ]
	then
		echo "copy pass"
	else
		echo "copy fail"
	fi
else
	echo "copy fail"
fi


# Test for rename
gcc rename.c -o rename
touch /tmp/file1
./rename /tmp/file1 /tmp/file2

if [ $? -eq 0 ]
then
	echo "rename pass"
else
	echo "rename fail"
fi


rm read100 read510 copy rename
