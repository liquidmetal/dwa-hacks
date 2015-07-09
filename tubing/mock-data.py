#!/usr/bin/python

import sys
sys.path.append("/work/protobuf/python")

import os, time, random
import errno

import fish_sim_pb2

filename = "/tmp/fish-pipe"
try:
    os.mkfifo(filename, 0700)
except OSError, e:
    print "The file probably already exists"
    pass

print("Waiting for reader to connect before sending data")
fp = os.open(filename, os.O_WRONLY)

msg_counter = 0
while True:
    fish = fish_sim_pb2.FishSim()
    fish.pos_x = random.random() * 100
    fish.pos_y = random.random() * 100
    fish.pos_z = random.random() * 100
    fish.orient_x = random.random() * 100
    fish.orient_y = random.random() * 100
    fish.orient_z = random.random() * 100

    buff = fish.SerializeToString()
    message_size = len(buff)
    size_size = os.write(fp, chr(message_size))
    os.write(fp, buff)

    msg_counter += 1
    if msg_counter % 1000 == 0:
        print(msg_counter)

os.close(fp)
os.remove(filename)


