angular.module('fishApp').directive('fishViewer', ['$log', function($log) {
    return {
        restrict: 'E',
        templateUrl: '/static/template/fish_viewer.html',
        scope: {
            something: '@'
        },
        link: function(scope, element, attrs) {
            $log.info("The directive is linking!");
            $log.info("The something = " + scope.something)
        }
    };
}]);
