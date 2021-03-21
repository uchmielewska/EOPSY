#!/bin/sh
#shell programing - task "modify" for EOPSY course at WUT
#author: Urszula Chmielewska

filename=`basename $0`


lower()
{
	newfilename=${filename tr a-z A-Z}
	if [ $newfilename == $filename ]
	then
		if [ ! "$2" = -r ]
		then
			echo "The filename was already lowerized"
		fi
	else
		mv $filename $newfilename
	fi
}

upper()
{
	newfilename=${filename tr A-Z a-z}
	if [ $newfilename == $filename ]
	then
		if [ ! "$2" = -r ]
		then
			echo "The filename was already uppercased"
		fi
	else
		mv $filename $newfilename
	fi
}


#-h or no argument: print help
usage () {
cat<<EOT

$filename is a script which modifies file names. It can lowerize and uppercase file names or call sed command with the given sed pattern.
Script can be used with the following syntax:

	$filename [-r] [-l|-u] <dir/file names...>
  	$filename [-r] <sed pattern> <dir/file names...>
  	$filename [-h]

options:
  	-r             turns on recursion
  	-l|-u          lowercase or uppercase
  	<sed pattern>  sed patter to modify the names (instead of lower/uppercasing)
  	-h             shows help

$filename correct syntax examples:
  	$filename -l file1 file2
  	$filename -r -u directory
  	$filename 's/x/Y/' bla.txt bla2 bla3
  	
$filename incorrect syntax examples:
  	$filename -d
  	$filename -u -l file
  	$filename -r -u
EOT
return
}


#last argument
if [ $# == 3 ]
then
	path=$3
else
	path=$2
fi

#test if the path exist
if [ ! -d $path ] && [ ! -f $path ]
then
	echo "The path : $path does not exist"
	exit
fi




case "$1" in
	h | -z)
		usage 
		exit
		;;
	-l)
		lower $path
		exit
		;;
	-u)
		upper $path
		exit
		;;
esac


