angular.module('fishApp').directive('stats', ['$log', '$window', function($log, $window) {
    return {
        restrict: 'E',
        link: function(scope, element, attrs) {
            scope.stats = new Stats();
            element.append( scope.stats.domElement );

            scope._update_stats = function() {
                requestAnimationFrame( scope._update_stats );
                scope.stats.update();
            }

            // Get the stat update chain started
            scope._update_stats();
        }
    };
}]);
