angular.module('fishApp').factory('Maps', ['$log',
    function($log) {
        var map = {};
        map.fetch_default_maps = function() {
            return [
                {name: 'aztec'},
                {name: 'michi'},
                {name: 'heaven'},
            ]
        }
        return map;
    }
]);
