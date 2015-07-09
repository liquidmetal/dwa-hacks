#!/usr/bin/python

import sys
sys.path.append("/work/protobuf/python/")

import os, errno
import fish_sim_pb2
import fish_gl_pb2

filename = "/tmp/fish-pipe"
try:
    os.mkfifo(filename, 0700)
except OSError, e:
    print("The file probably already exists")
    pass

fp = os.open(filename, os.O_RDONLY)
msg_counter = 0
while True:
    buff_size = os.read(fp, 1)

    if buff_size == "":
        print("The stream ended")
        break

    buff = os.read(fp, ord(buff_size))
    fish = fish_sim_pb2.FishSim()
    fish.ParseFromString(buff)

    msg_counter += 1
    if msg_counter % 1000 == 0:
        print(msg_counter)

os.close(fp)


