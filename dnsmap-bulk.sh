#!/bin/bash
if [[ $# -ne 2 ]]
then
	echo "usage: `basename $0` <domains-file> <results-path>";
	echo "i.e.:";
	echo "`basename $0` domains.txt";
	echo "`basename $0` domains.txt /tmp/";
	exit
else
	for i in `cat $1`
	do
		dnsmap $i -r $2
	done		
fi
