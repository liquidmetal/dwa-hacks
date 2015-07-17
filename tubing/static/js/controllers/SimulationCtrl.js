angular.module('fishApp').controller('SimulationCtrl', ['$scope', '$log', '$routeParams', 'Maps',
    function($scope, $log, $routeParams, Maps) {
        $scope._load_map = function() {
            Maps.fetch_map_data($scope.map_name, $scope._map_load_success, $scope._map_load_fail);
        }
    
        $scope._map_load_success = function(data, status, headers, config) {
            $scope.map_data = data;
        }

        $scope._map_load_fail = function(data, status, headers, config) {
        }

        $scope.initialize = function() {
            $scope.map_name = $routeParams.mapname;

            $scope._load_map()
        }

        $scope.initialize();
    }
]);
