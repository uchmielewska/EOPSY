#!/bin/sh
#shell programing - task "modify" for EOPSY course at WUT
#author: Urszula Chmielewska

filename=`basename $0`

#functions


L



#if user choses -h or there is no argument chosen: print help
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



if [ -z "$1" ] || [ "$1" = "-h" ]
then
	usage
	exit 0
fi
