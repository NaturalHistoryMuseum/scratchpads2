#!/bin/bash
for i in $(find . | grep "\.js" | grep -v ".min" | sed "s/\.js//")
do
	yuicompressor $i.js -o $i.min.js
done
