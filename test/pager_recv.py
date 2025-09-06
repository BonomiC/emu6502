#!/opt/homebrew/bin/python3

import os
import socket

def clear_console():
    """Clears the console screen based on the operating system."""
    if os.name == 'nt':  # For Windows
        os.system('cls')
    else:  # For macOS and Linux
        os.system('clear')

def print_page(data: bytes):
    i = 0
    for x in range(16):
        print('06' + f'{x:02x}: ', end='')
        for y in range(16):
            print(f'{data[i]:02x} ', end='')
            i += 1
        print()

# Create a UDP socket
server_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
# server_socket.setblocking(False)

# Bind the socket to an address and port
server_address = ('', 6502)  # Listen on all interfaces, port 12000
server_socket.bind(server_address)

print(f"Listening for UDP packets on port {server_address[1]}...")

# Receive data in a loop
buffer_size = 256  # Maximum bytes to receive
try:
    while True:
        data, client_address = server_socket.recvfrom(buffer_size)
        clear_console()
        print_page(data)

except KeyboardInterrupt:
    print("Server stopped.")
finally:
    server_socket.close()