#!/usr/local/bin/python

import sys
sys.path.append("/work/td/usinha/protobuf/protobuf/python/")

import os, errno
import fish_sim_pb2
import fish_gl_pb2

import tornado.web
import tornado.websocket
import tornado.ioloop

import simplejson, tempfile, subprocess

from codecs import utf_8_encode

MAP_PATH = '/tmp/maps'

class SimHandler(tornado.websocket.WebSocketHandler):
    """
    """
    def execute(self, command):
        popen = subprocess.Popen(command.split(), stdout=subprocess.PIPE)
        lines_iterator = iter(popen.stdout.readline, b"")
        for line in lines_iterator:
            print(line)
        return

    def _open_pipe(self):

        self.fp = open(self.filename, 'rb')
        #self.fp = os.open(self.filename, os.O_RDONLY, 0)
        self.message_counter = 0
        return

    def open(self):
        tmpdir = tempfile.mkdtemp()
        self.filename = os.path.join(tmpdir, 'fish-pipe')

        try:
            os.mkfifo(self.filename, 0700)
        except OSError, e:
            print("The file probably already exists")

        print("WebSocket opened")
        return

    def on_message(self, message):
        if message == "RA" or message == "R":
            print("Waiting for something to happen")
            # Request for a fish
            buff_size = self.fp.read(1)

            if buff_size == "":
                print("The stream ended")
                return

            buff_size = ord(buff_size)

            #buff = os.read(self.fp, ord(buff_size))
            buff = self.fp.read(buff_size)
            fish = fish_sim_pb2.FishSim()
            fish.ParseFromString(buff)

            to_send = {}
            to_send['fish_id'] = fish.fish_id
            to_send['pos_x'] = fish.pos_x
            to_send['pos_y'] = fish.pos_y

            self.write_message(simplejson.dumps(to_send))
            print("Received message")

            self.message_counter += 1
            if self.message_counter % 1000 == 0:
                print(self.message_counter)
        elif message == "A":
            # Acknowledged
            return
        elif message.startswith('S'):
            # Start the simulator
            map_file = message.split(':')[1]
            print("Trying to run command")
            #subprocess.Popen(['../flock-solve', '%s/%s' % (MAP_PATH, map_file), self.filename])
            proc = ['../test.sh',
                 '%s/%s' % (MAP_PATH, map_file), 
                 10000,    # sleeptime
                 10,       # Fish count
                 5,        
                 0.5,
                 0.1,
                 1.5,
                 1,
                 1,
                 1,
                 10,
                 15,
                 15,
                 0.5,
                 123,
                 self.filename]
            # Convert all parameters to string
            proc = [str(x) for x in proc]
            subprocess.Popen(proc)
            print("spanwed command")
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

        convert_cmd = ['vdb_map.sh', '%s/%s' % (MAP_PATH, file_data['filename'])]
        print(convert_cmd)
        try:
	    subprocess.Popen(convert_cmd, cwd=os.path.abspath('../pipeline/'), stdout=subprocess.PIPE)
        except OSError, e:
            import pdb; pdb.set_trace()
            

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

        map_filepath = '%s/%s.map' % (MAP_PATH, map_name)
        if not os.path.exists(map_filepath):
            # The file doesn't exist
            self.set_status(404)
            return

        with open(map_filepath, 'r') as fp:
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


