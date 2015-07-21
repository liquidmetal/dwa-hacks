angular.module('fishApp').factory('Sim', ['$location', '$log', '$rootScope', function($location, $log, $rootScope) {
    var sim = {}

    sim.ws = new WebSocket('ws://'+$location.host()+':'+$location.port()+'/sim');
    sim._ws_open = false;
    sim._fish_count = 0;
    sim._frame_data = [];
    sim._frame_count = -1;
    sim._current_frame = 0;


    sim.ws.onopen = function() {
        sim._ws_open = true;
    }

    sim.wait_for_connection = function(callback) {
        setTimeout(function() {
            $log.log("Thingy")
            if(sim.ws.readyState === 1) {
                $log.log("Connection to websocket established");
                if(callback!=null && typeof callback==='function') {
                    callback();
                }
                return
            } else {
                sim.wait_for_connection(callback);
            }
        }, 500);

    }

    sim.start_simulation = function(map_name) {
        $log.info("Successful call in service");
        sim.ws.send("S:" + map_name);
    }

    sim.get_fish_stream = function(callback) {
        var return_data = function(evt) {
            sim.ws.send('RA');

            var data = JSON.parse(evt.data);

            if(data.fish_id == 0) {
                sim._frame_count += 1;
            }

            if(sim._frame_data.length <= sim._frame_count) {
                sim._frame_data.push({});
            }

            sim._frame_data[sim._frame_count][data.fish_id] = data;

            var max_fish = data.fish_id + 1;
            if(sim.get_fish_count() < max_fish) {
                sim.set_fish_count(max_fish);
            }

            if(callback && typeof callback === 'function') {
                callback(data)
            }

            $rootScope.$apply();
        }

        sim.ws.onmessage = return_data;
        sim.ws.send('R');
    }

    sim.set_fish_count = function(count) {
        sim._fish_count = count;
    }

    sim.get_fish_count = function() {
        return sim._fish_count;
    }

    sim.get_current_frame = function() {
        return sim._current_frame;
    }

    sim.get_fish_position = function(frame, fish_id) {
        return sim._frame_data[frame][fish_id];
    }

    sim.is_connected = function() {
        return sim._ws_open;
    }

    return sim;
}]);
