#!/usr/bin/python3
"""
Generic Python3 server that can be ran on the CI40 in order to use with the examples which use
UDP client code.
"""
import argparse
parser = argparse.ArgumentParser()
parser.add_argument("-p", "--port", help="Port to bind to", default=3000, type=int)
parser.add_argument("-b", "--buffer", help="Buffer size", default=1024, type=int)
args = parser.parse_args()

import socket

# No IP to connect to needed for a server
IP = "::"
PORT = args.port

# Creates a socket using IPV6 and accepting datagrams
sock = socket.socket(socket.AF_INET6, socket.SOCK_DGRAM)
sock.bind((IP, PORT))

print("Server initialised, awaiting data. Use Ctrl + C to stop")
while True:
    data, address = sock.recvfrom(args.buffer)
    # Print client data as it arrives. For unpacking the data or converting to a UTF-8 string see below:
    # https://docs.python.org/3.5/library/struct.html
    # https://docs.python.org/3/howto/unicode.html#the-string-type
    print(data)
