#!/bin/bash

echo "timestamp,temperature" > data.csv

for f in *.png
do
    magick convert $f -crop 70x24+447+232 /tmp/cropped-$f
    tesseract /tmp/cropped-$f /tmp/ocred nobatch digits quiet

    datetime=${f%.*}
    text=$(cat /tmp/ocred.txt)

    if ! echo $text | grep -qE '[ "]'; then
	# hack: tesseract sometimes messes up the "." character and no space between the digits
        text="${text:0:2} ${text:2:2}"
    fi

    text=$(echo $text | tr ' ' .)
    echo "ocred [$datetime] - [$text]"
    echo "$datetime,$text" >> data.csv
done

