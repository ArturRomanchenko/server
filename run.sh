#!/bin/bash

cd build
rm -rf *
cmake ..
make 

echo "All logs are located in the project root in the folder logs/server.log"

./server