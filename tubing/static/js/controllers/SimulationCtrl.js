angular.module('fishApp').controller('SimulationCtrl', ['$scope', '$log', '$routeParams', '$location', 'Maps', 'Sim',
    function($scope, $log, $routeParams, $location, Maps, Sim) {
        $scope._load_map = function() {
            Maps.fetch_map_data($scope.map_name, $scope._map_load_success, $scope._map_load_fail);
        }
    
        $scope._map_load_success = function(data, status, headers, config) {
            $scope.map_data = data;

            Sim.wait_for_connection(function() {
                // Now that we have all the data, we can start the simulation
                Sim.start_simulation($scope.map_name);
                Sim.get_fish_stream(function(data) {
                });
            });
        }

        $scope._map_load_fail = function(data, status, headers, config) {
        }

        $scope.initialize = function() {
            $scope.map_name = $routeParams.mapname;
            $scope.frame_data = [];
            $scope.frame_count = -1;

            $scope._load_map()
        }

        $scope.ui_back_to_maps = function() {
            $location.path('/');
        }

        $scope.initialize();
    }
]);
