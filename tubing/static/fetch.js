var socket = new WebSocket("ws://localhost:8080/sim");
var counter = 0;

socket.onopen = function() {
    socket.send("R");
}

socket.onmessage = function(msg) {
    parsed_message = JSON.parse(msg.data)

    counter += 1;
    if(counter % 1000 == 0) {
        console.log(msg)
    }
    socket.send("RA")
}
