#!/usr/bin/python
import websocket
import thread
import time

def on_message(ws, message):
        print message

def on_error(ws, error):
    print error

def on_close(ws):
    print "### closed ###"

def on_open(ws):
    life = 1
    def run(*args):
#        for i in range(10):
        while life == 1 :
            time.sleep(1)
#            ws.send("Hello ")
        time.sleep(1)
        ws.close()
        print "thread terminating..."
    thread.start_new_thread(run, ())


if __name__ == "__main__":
    websocket.enableTrace(True)
#    ws = websocket.WebSocketApp("ws://127.0.0.1:8080/")
    ws = websocket.WebSocketApp("ws://127.0.0.1:8080/",
                                on_message = on_message,
                                on_error = on_error,
                                on_close = on_close)
#    ws = websocket.create_connection("ws://127.0.0.1:8080/",subprotocols=["wamp"])

    ws.on_open = on_open
    ws.on_message = on_message
    ws.on_error = on_error
    ws.on_close = on_close

    ws.run_forever()
