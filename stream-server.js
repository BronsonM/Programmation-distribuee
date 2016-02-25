
// Proxy Server: should connect on localhost:8082 for receiving the web page MainClient.html
var fs = require('fs'); 
var path = require('path');
var httpServer = require('http').createServer( //creation of the http proxy server
    function(request, response) {
        if(request.url != ''){//request.url is the file being requested by the client
            var filePath = '.' + request.url;
            if (filePath == './'){filePath = './MainClient.html';} // Serve MainClient.html if ./ was requested
            var filename = path.basename(filePath);
            var extname = path.extname(filePath);
            var contentType = 'text/html';
            fs.readFile(filePath, function(error, content) {
                response.writeHead(200, { 'Content-Type': contentType });
                response.end(content, 'utf-8');
            });
        }
    }
).listen(8082);


var STREAM_SECRET = process.argv[2],
	STREAM_PORT = process.argv[3] || 26, //port used for the remote web cam
	WEBSOCKET_PORT = process.argv[4] || 8080,
	STREAM_MAGIC_BYTES = 'jsmp'; // Must be 4 bytes

var width = 320, height = 240; //used for the dimension of the video

var RobotPresence =0; //state variable for the robot presence

// Websocket Server
var socketServer = new (require('ws').Server)({port: WEBSOCKET_PORT});
socketServer.on('connection', function(socket) {
	// Send magic bytes and video size to the newly connected socket
	// struct { char magic[4]; unsigned short width, height;}
	var streamHeader = new Buffer(8);
	streamHeader.write(STREAM_MAGIC_BYTES);
	streamHeader.writeUInt16BE(width, 4);
	streamHeader.writeUInt16BE(height, 6);
	//socket.send(streamHeader, {binary:true});

	console.log( 'New WebSocket Connection ('+socketServer.clients.length+' total)' );
	
	socket.on('close', function(code, message){
		console.log( 'Disconnected WebSocket ('+socketServer.clients.length+' total)' );
	});

	socket.on('message', function(message) {
                if (message=="IamRobot"){ //if the message received is IamRobot, it means that the MainController Python module was able
					  // to communicate. Therefore the robot is present and the state variable RobotPresent has to be set
			RobotPresence=1;
                        console.log('Robot connected! ');
		}
                else if (message=="RobotAvailable?"){ //If the message received is RobotAvailable?, it means the web page try to connect
			if (RobotPresence){
                         socketServer.clients[1].send('RobotAvailable');//If the robot is present, then send the message RobotAvailable on the second client channel clients[1]
                        }
                        else
                        {
                         socketServer.clients[0].send('RobotNotAvailable');// otherwise send the message RobotNotavailable on the first channel clients[0]
                        }
		}
                else if (message=="TestLife"){//Test all connections from Web user page up to sockets in Maincontroller.py
			if (RobotPresence){
                         socketServer.clients[0].send('TestLife');
                        }
                        else
                        {
                         socketServer.clients[0].send('RobotNotAvailable');
                        }
		}
		else
			{
                         if (RobotPresence){
                         socketServer.clients[0].send(message); //if any message other than the robot presence !!!, just pass it to the Python module
                        }
                        else
                        {
                         socketServer.clients[0].send('RobotNotAvailable');// if the robot not available, just send RobotNotAvailable
                        }
                        }
		
		console.log('Received Message: ' + message);
        	
    });
});




// HTTP Server to accept incomming MPEG Stream
/* var streamServer = require('http').createServer( function(request, response) {
	var params = request.url.substr(1).split('/');

	if( params[0] == STREAM_SECRET ) {
		width = (params[1] || 320)|0;
		height = (params[2] || 240)|0;
		
		console.log(
			'Stream Connected: ' + request.socket.remoteAddress + 
			':' + request.socket.remotePort + ' size: ' + width + 'x' + height
		);
		request.on('data', function(data){
			socketServer.broadcast(data, {binary:true});
		});
	}
	else {
		console.log(
			'Failed Stream Connection: '+ request.socket.remoteAddress + 
			request.socket.remotePort + ' - wrong secret.'
		);
		response.end();
	}
}).listen(STREAM_PORT); */

console.log('Listening for MPEG Stream on http://127.0.0.1:'+STREAM_PORT+'/<secret>/<width>/<height>');
console.log('Awaiting WebSocket connections on ws://127.0.0.1:'+WEBSOCKET_PORT+'/');
