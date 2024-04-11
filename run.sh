#!/bin/bash
clear
cd build
cmake ..
make
cp ../rwkv_vocab_v20230424.json .
./test