#!/usr/bin/python
import websocket
import thread
import time

import socket

HOST = '127.0.0.1'        # Local host
PORT = 50007              # Arbitrary port
msg = "-1"
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind((HOST, PORT))
s.listen(1)

print 'Waiting for connection...'
conn, addr = s.accept()

print 'Connected by client', addr


#conn.close()
print 'Server closed.'

# Here we just receive a message from the Node.js server and pass it to the socket used for the task CommunicationWebServer
def on_message(ws, message):
	print("INCOMMING: " + message);
	message = message.lower()
	v = message.split (' ')
	if v.__len__() == 1:
		m = "0"
	else:
		m = v[1]
	if v[0] == "fwd" or v[0] == "f":
		print("avancer continuellement de " + m)
		conn.send("0 "+m);
		
	elif v[0] == "bwd" or v[0] == "b":
		print("reculer continuellement de " + m)
		conn.send("1 "+m);
		
	elif v[0] == "cw":
		print("tourner "+ m +" degrees sens horaire")
		conn.send("2 "+m);
		
	elif v[0] == "ccw":
		print("tourner "+ m +" degres dans le sens antihoraire")
		conn.send("3 "+m);
		
	elif v[0] == "cam":
		print("Etat camera: " + m)
		if v[1] == "start":
			conn.send("4 "+m);
		if v[1] == "stop":
			conn.send("5 "+m);
		if v[1] == "up":
			conn.send("6 "+m);
		if v[1] == "down":
			conn.send("7 "+m);
			
	elif v[0] == "armfr":
		conn.send("8 "+m);
	
	elif v[0] == "armfl":
		conn.send("9 "+m);
		
	elif v[0] == "armrr":
		conn.send("a "+m);
		
	elif v[0] == "armrl":
		conn.send("b "+m);
		
	elif v[0] == "spfr":
		conn.send("c "+m);
		
	elif v[0] == "spfl":
		conn.send("d "+m);
		
	elif v[0] == "sprl":
		conn.send("e "+m);
		
	elif v[0] == "sprr":
		conn.send("f "+m);
		
	elif v[0] == "acfr":
		conn.send("g "+m);
		
	elif v[0] == "acfl":
		conn.send("h "+m);
		
	elif v[0] == "acrr":
		conn.send("i "+m);
		
	elif v[0] == "acrl":
		conn.send("j "+m);
		
	elif v[0] == "list":
		if v[1] == "config":
			conn.send("k "+m);

	else:
		print("commande pas reconnus" + v[0])
		msg = "-1"

def on_error(ws, error):
    print error

def on_close(ws):
    print "### closed ###"
	
def on_open(ws):
    life = 1
    ws.send("IamRobot")
    def run(*args):
        while life == 1 :
            time.sleep(1)
            data = conn.recv(1024)
            #print 'Received data from client', repr(data), '...send it back...'
            conn.send(data)
        time.sleep(1)
        ws.close()
        print "thread terminating..."
    thread.start_new_thread(run, ())


if __name__ == "__main__":
    websocket.enableTrace(True)
    ws = websocket.WebSocketApp("ws://127.0.0.1:8080/",
                                on_message = on_message,
                                on_error = on_error,
                                on_close = on_close)


    ws.on_open = on_open
    ws.on_message = on_message
    ws.on_error = on_error
    ws.on_close = on_close

    ws.run_forever()
