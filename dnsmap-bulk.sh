#!/bin/bash

if [[ $# -ne 1  && $# -ne 2 ]]
then
	echo "usage: $0 <domains-file> [dictionary-file]";
	echo "i.e.:";
	echo "$0 domains.txt";
	echo "$0 domains.txt mywordlist.txt";
	exit
fi

if [[ $# -eq 1 ]]
then
	for DOMAIN in `cat $1`
	do
		./dnsmap $DOMAIN
	done
elif [[ $# -eq 2 ]]
then
	for DOMAIN in `cat $1`
	do
		./dnsmap $DOMAIN $2
	done
fi
