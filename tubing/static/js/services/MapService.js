angular.module('fishApp').factory('Maps', ['$log', '$http', 'Upload', 
    function($log, $http, Upload) {
        var map = {};
        map.fetch_maps = function(success, fail) {
            $http.get('/map_list').success(function(data, status, headers, config) {
                $log.info("Map list fetched successfull!");
                if(success && typeof success === 'function') {
                    success(data, status, headers, config);
                }
            }).error(function(data, status, headers, config) {
                $log.info("Map list fetching failed!");
                if(fail && typeof fail === 'function') {
                    fail(data, status, headers, config);
                }
            })
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
