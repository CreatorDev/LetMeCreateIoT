#!/usr/bin/python3
import socket

# No IP to connect to needed for a server
IP = "::"
PORT = 3000

# Creates a socket using IPV6 and accepting datagrams
sock = socket.socket(socket.AF_INET6, socket.SOCK_DGRAM)
sock.bind((IP, PORT))

while True:
    data, address = sock.recvfrom(2)
    # Decode the client data
    temperature = data[0] + data[1]/100
    print("Address:", address[0])
    print("Temperature:", temperature)
    print("============")
