!#/bin/bash


for file in test/sm83/v1/*.json; do
    ./gameboy "$file"
    if [ $? -ne 0 ]; then
        echo "-- Instruction failed --"
        exit 1
    fi

done




