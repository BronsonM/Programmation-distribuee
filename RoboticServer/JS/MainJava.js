"use strict";

var dPadState = "Move";
var wheel = "";

// Initialize everything when the window finishes loading
window.addEventListener("load", function(event) {
    var status = document.getElementById("status");
    var url = document.getElementById("url");
    var open = document.getElementById("open");
    var close = document.getElementById("close");
    var send = document.getElementById("send");
    var stop = document.getElementById("Stop");
    var text = document.getElementById("text");
    var move = document.getElementById("Mouvement");
    var cam = document.getElementById("Camera");
    var zoom = document.getElementById("Zoom");
    var lift = document.getElementById("Lift");
    var moteur = document.getElementById("Moteur");
    var bras = document.getElementById("Bras");
    var acceleration = document.getElementById("Acceleration");
	

    var message = document.getElementById("message");
    var socket;
    var RobotAvailable = 0;

    status.textContent = "Not Connected";
    url.value = "ws://localhost:8080";
    close.disabled = true;
    send.disabled = true;
    stop.disabled = true;

    // Create a new connection when the Connect button is clicked
    open.addEventListener("click", function(event) {
        open.disabled = true;
        socket = new WebSocket(url.value, "wamp"); //use a websocket to communicate with the node.js server


        socket.addEventListener("open", function(event) {

            //Check to see if the robot available
            socket.send("RobotAvailable?"); // send the message to ask ther server if the robot is already connected


        });

        // Display messages received from the server
        socket.addEventListener("message", function(event) {
            if (event.data == "RobotAvailable") { // if the received message means that the robot is available, then set a state variable
                RobotAvailable = 1;
                close.disabled = false;
                send.disabled = false;
                stop.disabled = false;
                document.getElementById('title').style.background = "green"
                status.textContent = "Connected";

            }
            message.textContent = "Server Says: " + event.data;
        });

        // Display any errors that occur
        socket.addEventListener("error", function(event) {
            document.getElementById('title').style.background = "Yellow"
            message.textContent = "Error: " + event;
        });

        socket.addEventListener("close", function(event) {
            open.disabled = false;
            status.textContent = "Not Connected";
        });
    });

    // Close the connection when the Disconnect button is clicked
    close.addEventListener("click", function(event) {
        close.disabled = true;
        send.disabled = true;
        message.textContent = "";
        socket.close();
    });

    // Send text to the server when the Send button is clicked
    send.addEventListener("click", function(event) {
        alert(text.value);
        socket.send(text.value);
        text.value = "";
    });

    // Send text to the server when the Send button is clicked
    move.addEventListener("click", function(event) {
        dPadState = "Move";
    });

    // Send text to the server when the Send button is clicked
    cam.addEventListener("click", function(event) {
        dPadState = "Cam";
    });

    // Send text to the server when the Send button is clicked
    zoom.addEventListener("click", function(event) {
        dPadState = "Zoom";
    });

    // Send text to the server when the Send button is clicked
    lift.addEventListener("click", function(event) {
        dPadState = "Lift";
    });

    // Send text to the server when the Send button is clicked
    stop.addEventListener("click", function(event) {
        alert("STOPPED");
        socket.send("stop");
    });
	
    // Send text to the server when the Send button is clicked
    bras.addEventListener("click", function(event) {
        acceleration.disabled = true;
    });
	
    // Send text to the server when the Send button is clicked
    moteur.addEventListener("click", function(event) {
        acceleration.disabled = false;
    });

    $(document).keyup(function(e) {
        if (e.keyCode == 27) {
            socket.send("stop");
        }
    });
});

function point_it(event) {
    var pos_x = event.offsetX ? (event.offsetX) : event.pageX - document.getElementById("pointer_div").offsetLeft;
    var pos_y = event.offsetY ? (event.offsetY) : event.pageY - document.getElementById("pointer_div").offsetTop;
    var max_y = document.getElementById('cross').offsetWidth;
    var max_x = document.getElementById('cross').offsetHeight;

    var cmd = ""
    var amount = 5;

    if (pos_x / max_x * max_y > pos_y) {

        if ((max_y - pos_y) / max_y * max_x > pos_x) {
            if (dPadState == "Cam") {
                cmd = "camup " + amount;
            } else if (dPadState == "Zoom") {
                cmd = "zoomin " + amount;
            } else if (dPadState == "Lift") {
                cmd = "liftup " + amount;
            } else if (dPadState == "Move") {
                cmd = "f " + amount;
            }
        } else {
            if (dPadState == "Cam") {
                cmd = "camright " + amount;
            } else if (dPadState == "Move") {
                cmd = "cw " + amount;
            } else {
                return;
            }
        }
    } else {
        if ((max_y - pos_y) / max_y * max_x > pos_x) {
            if (dPadState == "Cam") {
                cmd = "camleft " + amount;
            } else if (dPadState == "Move") {
                cmd = "ccw " + amount;
            } else {
                return;
            }
        } else {
            if (dPadState == "Cam") {
                cmd = "camdown " + amount;
            } else if (dPadState == "Zoom") {
                cmd = "zoomout " + amount;
            } else if (dPadState == "Lift") {
                cmd = "liftdown " + amount;
            } else if (dPadState == "Move") {
                cmd = "b " + amount;
            }
        }
    }
    alert(cmd);
    socket.send(cmd);
}


function point_robo(event) {
    var pos_x = event.offsetX ? (event.offsetX) : event.pageX - document.getElementById("robodiv").offsetLeft;
    var pos_y = event.offsetY ? (event.offsetY) : event.pageY - document.getElementById("robodiv").offsetTop;
    var max_y = document.getElementById('roboimg').offsetWidth;
    var max_x = document.getElementById('roboimg').offsetHeight;


    if (max_y / 2 > pos_y) {
        if (max_x / 2 > pos_x) {
            document.getElementById('roboimg').src = "IMG/topLeft.jpg"
			wheel = "TL";
        } else {
            document.getElementById('roboimg').src = "IMG/topRight.jpg"
			wheel = "TR";
        }
    } else {
        if (max_x / 2 > pos_x) {
            document.getElementById('roboimg').src = "IMG/botLeft.jpg"
			wheel = "BL";
        } else {
            document.getElementById('roboimg').src = "IMG/botRight.jpg"
			wheel = "BR";
        }
    }

}

function isNumber(evt) {
    evt = (evt) ? evt : window.event;
    var charCode = (evt.which) ? evt.which : evt.keyCode;
    if (charCode > 31 && (charCode < 48 || charCode > 57)) {
        if (charCode == 46) {
            return true;
        }
        return false;

    }
    return true;
}