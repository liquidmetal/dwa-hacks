angular.module('fishApp').controller('IndexCtrl', ['$scope', '$log', 'Maps',
    function($scope, $log, Maps) {
        $scope.initialize = function() {
            $scope.default_maps = Maps.fetch_default_maps();
            $scope.selected_map = "custom";

            $scope.custom = {}
            $scope.custom.file = [];
            return;
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
            return
        }

        $scope.ui_map_upload_fail = function(data, status, headers, config) {
            $log.info("The map upload failed miserably");
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
