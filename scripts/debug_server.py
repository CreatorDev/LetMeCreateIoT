#!/usr/bin/python3
"""
A debug server which prints data received over 6lowpan from the PIC32MX clicker.
Can be run on a CI40 with Python3 available.
"""
import argparse
parser = argparse.ArgumentParser()
parser.add_argument("-t", "--terminal", 
		    help="Terminal mode. Shows only received data, doesn't show any network output",
                    action="store_true")
parser.add_argument("-b", "--buffer", help="Recv buffer size", default=2048, type=int)
parser.add_argument("-p", "--port", help="Port to bind to", default=9999, type=int)
args = parser.parse_args()

import socket
import struct

class __Color:
    INFO = "\033[93m"    
    DEFAULT = '\033[0m'

def print_debug(*print_args, **kargs):
    if(not args.terminal):
        print(__Color.INFO + print_args[0] + __Color.DEFAULT, **kargs)

# No IP to connect to needed for a server
IP = "::"
PORT = args.port

# Creates a socket using IPV6 and accepting datagrams
sock = socket.socket(socket.AF_INET6, socket.SOCK_DGRAM)
sock.bind((IP, PORT))

index = None
address = None

print_debug("Server initialised")

while True:
    data, new_address = sock.recvfrom(args.buffer)
    new_address = new_address[0]
    new_index = struct.unpack("<I", data[0:4])[0]

    message = data[4:].decode("utf-8")
    if(address is None or address != new_address):
        address = new_address
        index = new_index
        print_debug("Connected for debug to {0}".format(address))
        if(index != 0):
            print_debug("First message is out of order, {0} messages are missing".format(index))
    elif(new_index == 0):
        print_debug("Clicker has been rebooted")
    elif(new_index != index + 1):
        print_debug("Missed {0} messages".format(new_index - index - 1))

    index = new_index
    print(message, end="", flush=True)
