#!/usr/bin/python
import websocket
import thread
import time

import socket

HOST = '127.0.0.1'        # Local host
PORT = 50007              # Arbitrary port

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
	v = message.split (' ')

	message = message.lower()
	
	#validate if the user wrote a valid command
	if v[0] == "fwd" or v[0] == "f":
		print("avancer continuellement de " + v[1])
	elif v[0] == "bwd" or v[0] == "b":
		print("reculer continuellement de " + v[1])
	elif v[0] == "cw":
		print("tourner "+ v[1] +" degrees sens horaire")
	elif v[0] == "ccw":
		print("tourner "+ v[1] +" degres dans le sens antihoraire")
	elif v[0] == "cam":
		print("Etat camera: " + v[1])
	else:
		print("commande pas reconnus" + v[0])
	conn.send(message)

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
