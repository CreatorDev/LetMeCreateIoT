#!/usr/bin/python3

"""
Test server for the clicker to send a hello and let the server bounce it back
"""
import socket

IP = "::"
PORT = 3000

sock = socket.socket(socket.AF_INET6, socket.SOCK_DGRAM)
sock.bind((IP, PORT))

print("Server initialised, awaiting data. Use Ctrl + C to stop")
while True:
    data, metadata = sock.recvfrom(1024)

    text = data.decode('utf-8')
    address = metadata[0]
    print("Received: {}".format(text))
    if text == "Hello":
        sock.sendto("Hello bounced".encode('utf-8'), (address, 3001))

