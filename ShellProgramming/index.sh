#! /bin/bash



# Test for read100
gcc read100.c -o read100
content="Lorem ipsum dolor sit amet consectetur adipisicing elit. Corrupti, accusantium quisquam, necessitatibus tenetur alias eaque nobis nemo dolorem similique deleniti asperiores nisi assumenda officiis, minus delectus ipsa quo. Nulla officiis id soluta labore! Enim, rem. Saepe voluptatum ut provident nam veritatis sed fugit suscipit laborum sit eius, est labore. Modi deserunt quo, asperiores numquam aliquam adipisci ipsam." 
touch /tmp/confirmance_file
./read100 /tmp/confirmance_file "$content"
if [ $? -eq 0 -a `wc -c < /tmp/confirmance_file` -eq 100 ]
then
	var1=`echo "$content" | head -c 100`
	var2=`cat /tmp/confirmance_file`
	
	if [ "$var1" == "$var2" ] # Check the content of file is same or not
	then
		echo "read100 pass"
	else
		echo "read100 fail"
	fi
else
	echo "read100 fail"
fi




# Test for read510
gcc read510.c -o read510
out510=`./read510 /tmp/confirmance_file` # Store the output in variable
if [ $? -eq 0 ]
then
	char510=`head -c 10 /tmp/confirmance_file | tail -c 6` # Get the 5th to 10th bytes using head and tail
	if [ "$char510" == "$out510" ] # Check if output is same to expected result
	then
		echo "read510 pass"
	else
		echo "read510 fail"
	fi
else
	echo "read510 fail"
fi



# Test for write10end
gcc write10end.c -o write10end
bytes_before_writing=`wc -c < /tmp/confirmance_file` # Store number of bytes before writing 10 bytes
cp /tmp/confirmance_file /tmp/confirmance_file.bak # create a backup file to test read
./write10end /tmp/confirmance_file > /tmp/confirmance_test_file # Redirect read output in test file
if [ $? -eq 0 ]
then
	bytes_after_writing=`wc -c < /tmp/confirmance_file` # number of bytes after writing 10 bytes
	diff /tmp/confirmance_file.bak /tmp/confirmance_test_file # Check if original file and read output is same
	if [ $? -eq 0 -a `expr $bytes_before_writing + 10` -eq $bytes_after_writing ]
	then
		echo "write10end pass"
	else
		echo "write10end fail"
	fi
else
	echo "write10end fail"
fi




# Test for copy
gcc copy.c -o copy
./copy /tmp/confirmance_file /tmp/confirmance_copy_file # Copy the file using copy program
if [ $? -eq 0 ]
then
	diff /tmp/confirmance_file /tmp/confirmance_copy_file # Check if copied and original file are identical
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
echo "MynameisRohit" > /tmp/confirmance_file 
cp /tmp/confirmance_file /tmp/confirmance_file.bak # Create backup file for current file
./rename /tmp/confirmance_file /tmp/confirmance_renamed_file # Use rename program to rename the file
if [ $? -eq 0 ]
then
	diff /tmp/confirmance_file.bak /tmp/confirmance_renamed_file # Check if backup file and renamed file are identical
	if [ $? -eq 0 ]
	then
		echo "rename pass"
	else
		echo "rename fail"
	fi
else
	echo "rename fail"
fi




# Delete object files
rm read100 read510 write10end copy rename /tmp/confirmance*_file*

