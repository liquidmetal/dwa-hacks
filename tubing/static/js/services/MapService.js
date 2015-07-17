angular.module('fishApp').factory('Maps', ['$log', 'Upload',
    function($log, Upload) {
        var map = {};
        map.fetch_default_maps = function() {
            return [
                {name: 'aztec'},
                {name: 'michi'},
                {name: 'heaven'},
            ]
        }

        map.upload_map = function(file, success, fail) {
            if(!file) {
                throw "Invalid file received for uploading";
            }

            // Try and upload the file
            Upload.upload({
                url: '/upload_map',
                file: file,
            }).success(function(data, status, headers, config) {
                $log.info("File upload successful!");
                if(success && typeof success === 'function') {
                    success(data, status, headers, config);
                }
            }).error(function(data, status, headers, config) {
                $log.info("File upload failed!");

                if(fail && typeof fail === 'function') {
                    fail(data, status, headers, config);
                }
            });
        }

        return map;
    }
]);
