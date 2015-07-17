angular.module('fishApp').factory('Data', ['$log',
    function($log) {
        var data = {};
        data.uploadMap = function() {
            $log.info("Uploading map to server");
        }
        return data;
    }
]);
