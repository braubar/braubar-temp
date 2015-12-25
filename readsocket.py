import socket
import sys

# Create a TCP/IP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
# Bind the socket to the port
server_address = ('192.168.1.9', 80)
sock.bind(server_address)
# Listen for incoming connections
sock.listen(1)

try:
    while True:
        # Wait for a connection
        print('waiting for a connection...')
        connection, client_address = sock.accept()
        print('connection from %s:%d' % client_address)
        try:
            while True:
                # Receive the data one byte at a time
                data = connection.recv(1)
                sys.stdout.write(data)
                if data:
                    # Send back in uppercase
                    connection.sendall(data.upper())
                else:
                    print('no more data, closing connection.')
                    break
        finally:
            # Clean up the connection
            connection.close()
except KeyboardInterrupt:
    print('exiting.')
finally:
    sock.shutdown(socket.SHUT_RDWR)
    sock.close()
