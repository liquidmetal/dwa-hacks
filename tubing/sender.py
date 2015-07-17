#!/usr/bin/python

import sys
sys.path.append("/work/protobuf/python/")

import os, errno
import fish_sim_pb2
import fish_gl_pb2

import tornado.web
import tornado.websocket
import tornado.ioloop

import simplejson, tempfile

from codecs import utf_8_encode

MAP_PATH = '/tmp/maps'

class SimHandler(tornado.websocket.WebSocketHandler):
    """
    """
    def _open_pipe(self):
        try:
            os.mkfifo(self.filename, 0700)
        except OSError, e:
            print("The file probably already exists")
            pass

        self.fp = os.open(self.filename, os.O_RDONLY)
        self.message_counter = 0
        return

    def open(self):
        print("WebSocket opened")
        tmpdir = tempfile.mkdtemp()
        self.filename = os.path.join(tmpdir, 'fish-pipe')
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
        elif message.startswith('S'):
            # Start the simulator
            map_file = message.split(':')[1]
            cmd = "../flock-solve %s/%s %s" % (MAP_PATH, map_file, self.filename)
            os.spawnl(os.P_NOWAIT, cmd)
            print("spanwed command: %s" % cmd)
            self._open_pipe()
            print("Connected to pipe")

    def close(self):
        os.close(self.fp)
        return

class MapUploadHandler(tornado.web.RequestHandler):
    def post(self):
        if os.path.exists(MAP_PATH):
            if not os.path.isdir(MAP_PATH):
                os.remove(MAP_PATH)
                os.mkdir(MAP_PATH)
        else:
            os.mkdir(MAP_PATH)
        file_data = self.request.files['file'][0]

        with open('%s/%s' % (MAP_PATH, file_data['filename']), 'w') as fp:
            fp.write(file_data['body'])

        return

class MainHandler(tornado.web.RequestHandler):
    def get(self):
        fp = open("./index.html")
        self.write(fp.read())
        fp.close()
        return

class MapListHandler(tornado.web.RequestHandler):
    def get(self):
        maps = os.listdir(MAP_PATH)

        ret = {}
        ret['maps'] = []
        ret['map_count'] = len(maps)

        for m in maps:
            with open("%s/%s" % (MAP_PATH, m), 'r') as fp:
                map_width = len(fp.readline())
                map_height = sum(1 for line in fp) + 1

            ret['maps'].append({'name': m, 'width': map_width, 'height': map_height})

        self.set_header('Content-Type', 'application/json')
        self.write(simplejson.dumps(ret))
        return

class MapFetchHandler(tornado.web.RequestHandler):
    def get(self):
        map_name = self.get_argument('name')

        ret = {}
        map_data = []

        map_filepath = '%s/%s' % (MAP_PATH, map_name)
        if not os.path.exists(map_filepath):
            # The file doesn't exist
            self.set_status(404)
            return

        with open('%s/%s' % (MAP_PATH, map_name), 'r') as fp:
            map_data = [line.strip() for line in fp]

        ret['map'] = map_data
        ret['yoyo'] = 1
        self.set_header('Content-Type', 'application/json')
        self.write(simplejson.dumps(ret))
        return
                

def start_server():
    """
    """
    routes = [(r"/", MainHandler),
              (r"/sim", SimHandler),
              (r"/map_list", MapListHandler),
              (r"/upload_map", MapUploadHandler),
              (r'/fetch_map', MapFetchHandler),
              (r"/static/(.*)", tornado.web.StaticFileHandler, {"path": "./static/"}),
             ]

    application = tornado.web.Application(routes, debug=True)

    application.listen(8080)
    tornado.ioloop.IOLoop.instance().start()

if __name__ == "__main__":
    start_server()


