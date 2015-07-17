angular.module('fishApp').factory('Sim', function() {
    var sim = {}
    sim.method1 = function() {
        $log.info("Successful call in service");
    }
    return sim;
});
