#!/bin/bash
for i in $(find . | grep "\.js" | grep -v ".min" | sed "s/\.js//")
do
	echo $i.js
	yuicompressor $i.js -o $i.min.js
done
