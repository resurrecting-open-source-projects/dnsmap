#!/bin/bash

# Copyright 2015-2019 Joao Eriberto Mota Filho <eriberto@eriberto.pro.br>
# Create a manpage using txt2man command.
#
# This script can be used under BSD-3-Clause license.

# Don't change the following lines. Change MANPAGE 1 and 2 below.
function make-man {
TEST=$(txt2man -h 2> /dev/null)
[ "$TEST" ] || { echo -e "\nYou need to install txt2man, from https://github.com/mvertes/txt2man.\n"; exit 1; }
txt2man -d "$T2M_DATE" -t $T2M_NAME -r $T2M_NAME-$T2M_VERSION -s $T2M_LEVEL -v "$T2M_DESC" $T2M_NAME.txt > $T2M_NAME.$T2M_LEVEL
}

# MANPAGE 1
T2M_DATE="18 Nov 2019"
T2M_NAME=dnsmap
T2M_VERSION=0.35
T2M_LEVEL=1
T2M_DESC="scan for subdomains using bruteforcing techniques"
make-man

# MANPAGE 2
T2M_DATE="18 Nov 2019"
T2M_NAME=dnsmap-bulk
T2M_VERSION=0.1
T2M_LEVEL=1
T2M_DESC="mass scan using dnsmap"
make-man
