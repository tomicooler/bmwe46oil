#!/bin/bash

while true
do
    magick import -window root "$(date +%s).png"
    sleep 0.05
done
