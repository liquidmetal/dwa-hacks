#!/usr/bin/python

import sys
sys.path.append("/work/protobuf/python/")

import os, errno
import fish_sim_pb2
import fish_gl_pb2

import tornado.web
import tornado.websocket
import tornado.ioloop

import simplejson

from codecs import utf_8_encode

class SimHandler(tornado.websocket.WebSocketHandler):
    """
    """
    def open(self):
        filename = "/tmp/fish-pipe"
        try:
            os.mkfifo(filename, 0700)
        except OSError, e:
            print("The file probably already exists")
            pass

        self.fp = os.open(filename, os.O_RDONLY)
        self.message_counter = 0
        print("WebSocket opened")
        return

    def on_message(self, message):
        if message == "RA" or message == "R":
            # Request for a fish
            buff_size = os.read(self.fp, 1)

            if buff_size == "":
                print("The stream ended")
                return

            buff = os.read(self.fp, ord(buff_size))
            fish = fish_sim_pb2.FishSim()
            fish.ParseFromString(buff)

            to_send = {}
            to_send['pos_x'] = fish.pos_x
            to_send['pos_y'] = fish.pos_y
            to_send['pos_z'] = fish.pos_z
            to_send['orient_x'] = fish.orient_x
            to_send['orient_y'] = fish.orient_y
            to_send['orient_z'] = fish.orient_z

            self.write_message(simplejson.dumps(to_send))

            self.message_counter += 1
            if self.message_counter % 1000 == 0:
                print(self.message_counter)
        elif message == "A":
            # Acknowledged
            return

    def close(self):
        os.close(self.fp)
        return

class MainHandler(tornado.web.RequestHandler):
    def get(self):
        fp = open("./static/index.html")
        self.write(fp.read())
        fp.close()

def start_server():
    """
    """
    application = tornado.web.Application([
            (r"/", MainHandler),
            (r"/sim", SimHandler),
            (r"/static/(.*)", tornado.web.StaticFileHandler, {"path": "./static/"}),
        ])

    application.listen(8080)
    tornado.ioloop.IOLoop.instance().start()

if __name__ == "__main__":
    start_server()


