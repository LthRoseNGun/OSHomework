#!/bin/bash

SCRIPT=substitue.sh

for f in *
do
	if [ $f = $SCRIPT ]
	then 
		echo "skip $f"
		continue
	fi

	echo -n "substitute myext2 to mymyext2 in $f..."
	cat $f | sed 's/myext2/mymyext2/g' > ${f}_tmp
	mv ${f}_tmp $f
	echo "done"

	echo -n "substitute MYEXT2 to MYMYEXT2 in $f..."
	cat $f | sed 's/MYEXT2/MYMYEXT2/g' > ${f}_tmp
	mv ${f}_tmp $f
	echo "done"
done
