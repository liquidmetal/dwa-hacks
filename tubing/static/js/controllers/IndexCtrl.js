angular.module('fishApp').controller('IndexCtrl', ['$scope', '$log', '$location', 'Maps',
    function($scope, $log, $location, Maps) {
        $scope.initialize = function() {
            Maps.fetch_maps($scope._map_fetch_successful, $scope._map_fetch_fail);

            $scope.default_maps = [];
            $scope.selected_map = "custom";
            $scope.custom = {}
            $scope.custom.file = [];
            return;
        }

        $scope._map_fetch_successful = function(data, status, headers, config) {
            $scope.default_maps = data.maps;
            $log.info("Fetched " + data.map_count + " pre-existing maps");
            return;
        }

        $scope._map_fetch_successful_switch = function(data, status, headers, config) {
            // Update the default_maps thing
            $scope._map_fetch_successful(data, status, headers, config);

            // Switch to the map just uploaded
            var mapname = $scope.custom.file[0].name;
            var i=0;
            for(i=0;i<$scope.default_maps.length;i++) {
                if($scope.default_maps[i].name == mapname) {
                    $scope.selected_map = $scope.default_maps[i];
                    break;
                }
            }
        }

        $scope._map_fetch_fail = function(data, status, headers, config) {
            $log.error("Fetching of map list failed");
            return
        }

        // ui_* functions are used in the template
        $scope.ui_select_map = function(map) {
            $scope.selected_map = map;
            return;
        }

        $scope.ui_is_current_map_selected = function(map) {
            return angular.equals($scope.selected_map, map);
        }

        $scope.ui_is_custom_selected = function() {
            return $scope.selected_map == "custom";
        }

        $scope.ui_custom_map_changed = function(input) {
            return
        }

        $scope.ui_map_upload_success = function(data, status, headers, config) {
            $log.info("The map upload was successful!");
            Maps.fetch_maps($scope._map_fetch_successful_switch, $scope._map_fetch_fail);
            return
        }

        $scope.ui_map_upload_fail = function(data, status, headers, config) {
            $log.info("The map upload failed miserably");
        }

        $scope.ui_run_simulation = function() {
            $log.info("Running simulation!");
            $location.path('/simulate/' + $scope.selected_map.name);
            return
        }

        // All watches on elements go here
        $scope.$watch('custom', function(newfile) {
            if(!newfile || !newfile.file || newfile.file.length == 0) {
                // Nothing to do here
                return
            }

            Maps.upload_map(newfile.file[0], $scope.ui_map_upload_success, $scope.ui_map_upload_fail);
        }, true);

        // Execution of code begins here
        $scope.initialize()
    }
]);
