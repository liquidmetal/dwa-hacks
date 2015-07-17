angular.module('fishApp').controller('IndexCtrl', ['$scope', '$log', 'Maps',
    function($scope, $log, Maps) {
        $scope.initialize = function() {
            $scope.default_maps = Maps.fetch_default_maps();
            $scope.selected_map = "custom";
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

        // Execution of code begins here
        $scope.initialize()
    }
]);
