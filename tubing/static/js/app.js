angular.module("fishApp", ['ngRoute', 'ngFileUpload']).config(['$routeProvider',
    function($routeProvider) {
        $routeProvider.when('/', {
            templateUrl: '/static/template/home.html',
            controller: 'IndexCtrl'
        }).when('/simulate', {
            templateUrl: '/static/template/simulate.html',
            controller: 'SimulationCtrl'
        }).otherwise({
            redirectTo: '/'
        });
    }
])
