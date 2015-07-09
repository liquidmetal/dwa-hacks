var socket = new WebSocket("ws://localhost:8080/sim");
var counter = 0;

socket.onopen = function() {
    socket.send("R");
}

socket.onmessage = function(msg) {
    counter += 1;
    console.log(counter)
    if(counter % 1000 == 0) {
        console.log(msg)
    }
}
