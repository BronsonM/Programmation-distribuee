var fs = require('fs');
var path = require('path');
var httpServer = require('http').createServer(
    function(request, response) {
        if(request.url != ''){//request.url is the file being requested by the client
            var filePath = '.' + request.url;
            if (filePath == './'){filePath = './MainClient.html';} // Serve index.html if ./ was requested
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
	STREAM_PORT = process.argv[3] || 26,
	WEBSOCKET_PORT = process.argv[4] || 8080,
	STREAM_MAGIC_BYTES = 'jsmp'; // Must be 4 bytes

var width = 320,
	height = 240;

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
		socket.send('something1');
		for( var i in socketServer.clients ) {
			socketServer.clients[i].send('something2');
		}
		console.log('Received Message: ' + message);
        	if (message.type === 'utf8') {
            		console.log('Received Message: ' + message.utf8Data);
            		socket.sendUTF(message.utf8Data);
			socket.send(streamHeader, {binary:true});
       		 }
       		 else if (message.type === 'binary') {
            		console.log('Received Binary Message of ' + message.binaryData.length + ' bytes');
            		socket.sendBytes(message.binaryData);
        	}
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
