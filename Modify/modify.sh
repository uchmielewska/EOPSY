#!/bin/bash
#shell programing - task "modify" for EOPSY course at WUT
#author: Urszula Chmielewska

filename=`basename $0`

#function to change name from low letters to uppercased letters
low()
{
	newname="$(echo "$1" | tr A-Z a-z)"
	if test "$1" != "$newname"
	then
		mv "$1" "$newname"
	fi
}

#function to change name from uppercased letters to low letters 
up()
{
	newname="$(echo "$1" | tr a-z A-Z)"
	if test "$1" != "$newname"
	then
		mv "$1" "$newname"
	fi
}

#function to change name using sed pattern
sedCommand()
{
	sedPath=$(echo "$1" | sed "$2")
	if test "$1" != "$sedPath"
	then
		mv "$1" "$sedPath"
	fi
}

#function which resursively changes file names from uppercased letters to low letters
recurrenceLow()
{
	cd $1
	for el in *
	do
		if test -f "$el";
		then
			low $el
		fi
		
		if test -d "$el";
		then
	  		recurrenceLow $el 
		fi
	done	
	cd ..
}

#function which resursively changes file names from low letters to uppercased letters
recurrenceUp()
{
	cd $1
	for el in *
	do
		if test -f "$el";
		then
			up $el
		fi
		
		if test -d "$el";
		then
	  		recurrenceUp $el 
		fi
	done	
	cd ..
}

#function which resursively changes file names using sed pattern
recurrenceSed()
{
	cd $1
	for el in *
	do
		if test -f "$el";
		then
			newname=$(echo "$el" | sed "$2")
			if test "$el" != "$newname"
			then
	  			mv "$el" "$newname"
	  		fi
		fi
		
		if test -d "$el";
		then
	  		recurrenceSed $el "$2"
		fi
	done	
	cd ..
}

#function which prints help if used enters -h or there is wrong calling of the script
usage () 
{
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
  	$filename -l file
  	$filename -r -u directory
  	$filename 's/x/Y/' bla.txt
  	
$filename incorrect syntax examples:
  	$filename -d
  	$filename -u -l file
  	$filename -r -u
EOT
return
}


#set last argument as path
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
	usage
	exit
fi


#run script using reccurence
for argument in $*
do
	case $argument in
		-r)
			for argument2 in $*
			do
				case $argument2 in
				-l)
					recurrenceLow $path
					exit
					;;
				-u)
					recurrenceUp $path
					exit
					;;
				*/*/*/*)
					recurrenceSed $path $argument2
					exit
					;;
				*)
					echo "Option unavailable"
					usage
					exit
					;;
				esac
			done
			;;
		-rl | -lr)
			recurrenceLow $path
			exit
			;;
		-ru | -ur)
			recurrenceUp $path
			exit
			;;
	esac
done

#test if path given by a user is file
if test -d "$path"
	then
		echo "Option unavailable"
		usage
		exit	
	fi
	
#all other cases than recurrence
case "$1" in
	-h | -z)
		usage 
		exit
		;;
	-l)
		low $path
		exit
		;;
	-u)
		up $path
		exit
		;;
	*/*/*/*) 	
		sedArg=$1
		sedCommand $path $sedArg
		exit
		;;
	-* | *)
		echo "Option unavaliable"
		usage
		exit 1
		;;
esac
