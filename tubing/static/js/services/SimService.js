angular.module('fishApp').factory('Sim', ['$location', '$log', function($location, $log) {
    var sim = {}

    sim.ws = new WebSocket('ws://'+$location.host()+':'+$location.port()+'/sim');
    sim._ws_open = false;


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

    sim.is_connected = function() {
        return sim._ws_open;
    }

    return sim;
}]);
