#!/usr/bin/python3
"""
A Python3 client that can be ran on the CI40 in order to use with the examples which use
UDP server
"""
import argparse
parser = argparse.ArgumentParser()
parser.add_argument("-p", "--port", help="Port to bind to", default=3001, type=int)
parser.add_argument("-b", "--buffer", help="Buffer size", default=1024, type=int)
parser.add_argument("-s", "--size", help="Data size", default=64, type=int)
parser.add_argument("ip", metavar="ip", help="Server ip", type=str, nargs=1)
parser.add_argument("-dp", "--dest_port", help="Destination port", default=3000, type=int)
parser.add_argument("-m", "--message_count", help="Amount of messages to send, 0 for infintiy",
                    default=1, type=int)
parser.add_argument("-w", "--wait", help="Wait time between messages in miliseconds", type=int,
                    default=1000)

args = parser.parse_args()

import socket
import time

server_ip = args.ip[0]
server_port = args.dest_port
local_port = args.port
message_count = args.message_count
wait_time = args.wait
size = args.size

sock = socket.socket(socket.AF_INET6, socket.SOCK_DGRAM)
sock.bind(("::", local_port))

if message_count != 1:
    print("Sending {} packets every {} miliseconds to {} port {}, press Ctrl + C to stop".format(
        message_count if message_count > 0 else "infinitely many",
        wait_time, server_ip, server_port))
else:
    print("Sending one packet to {} port {}".format(server_ip, server_port))

while True:
    message = "a" * size
    sock.sendto(message.encode('utf-8'), (server_ip, server_port))

    if message_count > 0:
        message_count = message_count - 1;
        if message_count == 0:
            break

    if wait_time > 0:
        time.sleep(wait_time/1000.0)
