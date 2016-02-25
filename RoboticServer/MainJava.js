"use strict";

var socket;
var dPadState = "Move";
var wheel = "TL";
var tlAcc = ["D", "D", "D"];
var trAcc = ["D", "D", "D"];
var blAcc = ["D", "D", "D"];
var brAcc = ["D", "D", "D"];
var tlBras = ["D", "D"];
var trBras = ["D", "D"];
var blBras = ["D", "D"];
var brBras = ["D", "D"];


var acceleration;
var vitesse;
var position;

var moteur;
var bras;

var sendWheel;
var defaultWheel;
var cancelWheel;

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


    moteur = document.getElementById("Moteur");
    bras = document.getElementById("Bras");

    sendWheel = document.getElementById("sendWheel");
    defaultWheel = document.getElementById("defaultWheel");
    cancelWheel = document.getElementById("cancelWheel");

    acceleration = document.getElementById("Acceleration");
    vitesse = document.getElementById("Vitesse");
    position = document.getElementById("Position");

    var message = document.getElementById("message");
    var RobotAvailable = 0;

    status.textContent = "Not Connected";
    url.value = "ws://localhost:8080";
    close.disabled = true;
    send.disabled = true;
    stop.disabled = true;
    moteur.checked = true;

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
        socket.send("stop");
    });

    // Send text to the server when the Send button is clicked
    bras.addEventListener("click", function(event) {
        acceleration.value = "D"
        acceleration.disabled = true;

        if (wheel == "TL") {
            tlAcc[0] = position.value;
            tlAcc[1] = vitesse.value;
            tlAcc[2] = acceleration.value;
        } else if (wheel == "TR") {
            trAcc[0] = position.value;
            trAcc[1] = vitesse.value;
            trAcc[2] = acceleration.value;
        } else if (wheel == "BL") {
            blAcc[0] = position.value;
            blAcc[1] = vitesse.value;
            blAcc[2] = acceleration.value;
        } else if (wheel == "BR") {
            brAcc[0] = position.value;
            brAcc[1] = vitesse.value;
            brAcc[2] = acceleration.value;
        }

        var ac, vi, po;

        ac = "0"
        if (wheel == "TL") {
            po = tlBras[0];
            vi = tlBras[1];
        } else if (wheel == "TR") {
            po = trBras[0];
            vi = trBras[1];
        } else if (wheel == "BL") {
            po = blBras[0];
            vi = blBras[1];
        } else if (wheel == "BR") {
            po = brBras[0];
            vi = brBras[1];
        }

        acceleration.value = ac;
        vitesse.value = vi;
        position.value = po;

    });

    // Send text to the server when the Send button is clicked
    moteur.addEventListener("click", function(event) {
        acceleration.disabled = false;
        if (wheel == "TL") {
            tlBras[0] = position.value;
            tlBras[1] = vitesse.value;
        } else if (wheel == "TR") {
            trBras[0] = position.value;
            trBras[1] = vitesse.value;
        } else if (wheel == "BL") {
            blBras[0] = position.value;
            blBras[1] = vitesse.value;
        } else if (wheel == "BR") {
            brBras[0] = position.value;
            brBras[1] = vitesse.value;
        }

        var ac, vi, po;

        if (wheel == "TL") {
            po = tlAcc[0];
            vi = tlAcc[1];
            ac = tlAcc[2];
        } else if (wheel == "TR") {
            po = trAcc[0];
            vi = trAcc[1];
            ac = trAcc[2];
        } else if (wheel == "BL") {
            po = blAcc[0];
            vi = blAcc[1];
            ac = blAcc[2];
        } else if (wheel == "BR") {
            po = brAcc[0];
            vi = brAcc[1];
            ac = brAcc[2];
        }
        acceleration.value = ac;
        vitesse.value = vi;
        position.value = po;

    });

    sendWheel.addEventListener("click", function(event) {

        if (bras.checked) {
            if (wheel == "TL") {
                tlBras[0] = position.value;
                tlBras[1] = vitesse.value;
            } else if (wheel == "TR") {
                trBras[0] = position.value;
                trBras[1] = vitesse.value;
            } else if (wheel == "BL") {
                blBras[0] = position.value;
                blBras[1] = vitesse.value;
            } else if (wheel == "BR") {
                brBras[0] = position.value;
                brBras[1] = vitesse.value;
            }
        } else {
            if (wheel == "TL") {
                tlAcc[0] = position.value;
                tlAcc[1] = vitesse.value;
                tlAcc[2] = acceleration.value;
            } else if (wheel == "TR") {
                trAcc[0] = position.value;
                trAcc[1] = vitesse.value;
                trAcc[2] = acceleration.value;
            } else if (wheel == "BL") {
                blAcc[0] = position.value;
                blAcc[1] = vitesse.value;
                blAcc[2] = acceleration.value;
            } else if (wheel == "BR") {
                brAcc[0] = position.value;
                brAcc[1] = vitesse.value;
                brAcc[2] = acceleration.value;
            }
        }

        var tl = false;
        var tr = false;
        var bl = false;
        var br = false;

        for (var i = 0; i < 3; i++) {
            if (tlAcc[i] != "D") {
                tl = true;
            }
            if (trAcc[i] != "D") {
                tb = true;
            }
            if (blAcc[i] != "D") {
                bl = true;
            }
            if (brAcc[i] != "D") {
                br = true;
            }
        }

        if (tl || tr) {
            socket.send("FrontMot:" + tlAcc[0] + " " + tlAcc[1] + " " + tlAcc[2] + " " + trAcc[0] + " " + trAcc[1] + " " + trAcc[2]);
        }

        if (bl || br) {
            socket.send("BackMot:" + blAcc[0] + " " + blAcc[1] + " " + blAcc[2] + " " + brAcc[0] + " " + brAcc[1] + " " + brAcc[2]);
        }

        tl = false;
        tr = false;
        bl = false;
        br = false;

        for (i = 0; i < 2; i++) {
            if (tlBras[i] != "D") {
                tl = true;
            }
            if (trBras[i] != "D") {
                tb = true;
            }
            if (blBras[i] != "D") {
                bl = true;
            }
            if (brBras[i] != "D") {
                br = true;
            }
        }

        if (tl || tr) {
            socket.send("FrontBras:" + tlBras[0] + " " + tlBras[1] + " " + tlBras[2] + " " + trBras[0] + " " + trBras[1] + " " + trBras[2]);
        }

        if (bl || br) {
            socket.send("BackBras:" + blBras[0] + " " + blBras[1] + " " + blBras[2] + " " + brBras[0] + " " + brBras[1] + " " + brBras[2]);
        }
    });


    defaultWheel.addEventListener("click", function(event) {
        if (bras.checked) {
            acceleration.value = "D";
            vitesse.value = "D";
            position.value = "D";
        } else {
            acceleration.value = "D";
            vitesse.value = "D";
            position.value = "D";
        }
    });

    cancelWheel.addEventListener("click", function(event) {

        tlAcc = ["D", "D", "D"];
        trAcc = ["D", "D", "D"];
        blAcc = ["D", "D", "D"];
        brAcc = ["D", "D", "D"];
        tlBras = ["D", "D"];
        trBras = ["D", "D"];
        blBras = ["D", "D"];
        brBras = ["D", "D"];

        var ac, vi, po;

        if (bras.checked) {
            ac = "0"
            if (wheel == "TL") {
                po = tlBras[0];
                vi = tlBras[1];
            } else if (wheel == "TR") {
                po = trBras[0];
                vi = trBras[1];
            } else if (wheel == "BL") {
                po = blBras[0];
                vi = blBras[1];
            } else if (wheel == "BR") {
                po = brBras[0];
                vi = brBras[1];
            }
        } else {
            if (wheel == "TL") {
                po = tlAcc[0];
                vi = tlAcc[1];
                ac = tlAcc[2];
            } else if (wheel == "TR") {
                po = trAcc[0];
                vi = trAcc[1];
                ac = trAcc[2];
            } else if (wheel == "BL") {
                po = blAcc[0];
                vi = blAcc[1];
                ac = blAcc[2];
            } else if (wheel == "BR") {
                po = brAcc[0];
                vi = brAcc[1];
                ac = brAcc[2];
            }
        }
        acceleration.value = ac;
        vitesse.value = vi;
        position.value = po;
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
    socket.send(cmd);
}


function point_robo(event) {
    var pos_x = event.offsetX ? (event.offsetX) : event.pageX - document.getElementById("robodiv").offsetLeft;
    var pos_y = event.offsetY ? (event.offsetY) : event.pageY - document.getElementById("robodiv").offsetTop;
    var max_y = document.getElementById('roboimg').offsetWidth;
    var max_x = document.getElementById('roboimg').offsetHeight;

    if (bras.checked) {
        if (wheel == "TL") {
            tlBras[0] = position.value;
            tlBras[1] = vitesse.value;
        } else if (wheel == "TR") {
            trBras[0] = position.value;
            trBras[1] = vitesse.value;
        } else if (wheel == "BL") {
            blBras[0] = position.value;
            blBras[1] = vitesse.value;
        } else if (wheel == "BR") {
            brBras[0] = position.value;
            brBras[1] = vitesse.value;
        }
    } else {
        if (wheel == "TL") {
            tlAcc[0] = position.value;
            tlAcc[1] = vitesse.value;
            tlAcc[2] = acceleration.value;
        } else if (wheel == "TR") {
            trAcc[0] = position.value;
            trAcc[1] = vitesse.value;
            trAcc[2] = acceleration.value;
        } else if (wheel == "BL") {
            blAcc[0] = position.value;
            blAcc[1] = vitesse.value;
            blAcc[2] = acceleration.value;
        } else if (wheel == "BR") {
            brAcc[0] = position.value;
            brAcc[1] = vitesse.value;
            brAcc[2] = acceleration.value;
        }
    }


    if (max_y / 3 > pos_y) {
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

    var ac, vi, po;

    if (bras.checked) {
        ac = "0"
        if (wheel == "TL") {
            po = tlBras[0];
            vi = tlBras[1];
        } else if (wheel == "TR") {
            po = trBras[0];
            vi = trBras[1];
        } else if (wheel == "BL") {
            po = blBras[0];
            vi = blBras[1];
        } else if (wheel == "BR") {
            po = brBras[0];
            vi = brBras[1];
        }
    } else {
        if (wheel == "TL") {
            po = tlAcc[0];
            vi = tlAcc[1];
            ac = tlAcc[2];
        } else if (wheel == "TR") {
            po = trAcc[0];
            vi = trAcc[1];
            ac = trAcc[2];
        } else if (wheel == "BL") {
            po = blAcc[0];
            vi = blAcc[1];
            ac = blAcc[2];
        } else if (wheel == "BR") {
            po = brAcc[0];
            vi = brAcc[1];
            ac = brAcc[2];
        }
    }
    acceleration.value = ac;
    vitesse.value = vi;
    position.value = po;

}

function isNumber(evt) {
    evt = (evt) ? evt : window.event;
    var charCode = (evt.which) ? evt.which : evt.keyCode;
    if (charCode > 31 && (charCode < 48 || charCode > 57)) {
        return false;

    }
    return true;
}