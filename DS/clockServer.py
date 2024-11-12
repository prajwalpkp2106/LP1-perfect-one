import socket
import time
import threading

# Shared variable for Lamport Clock
lamport_clock = 0

def start_ntp_server():
    ntp_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    ntp_socket.bind(('localhost', 12346))  # Changed to port 12346

    print("NTP server running...")

    while True:
        data, address = ntp_socket.recvfrom(1024)  # Buffer size
        if data:
            timestamp = int(time.time()) + 2208988800  # Convert to NTP time by adding offset

            # Prepare a 48-byte NTP response packet
            response = b'\x1b' + (b'\0' * 39) + timestamp.to_bytes(4, 'big') + (b'\0' * 4)
            ntp_socket.sendto(response, address)

def start_lamport_server():
    global lamport_clock
    lamport_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    lamport_socket.bind(('localhost', 12347))  # Changed to port 12347

    print("Lamport clock server running...")

    while True:
        data, address = lamport_socket.recvfrom(1024)  # Buffer size
        if data:
            received_time = int(data.decode())
            lamport_clock = max(lamport_clock, received_time) + 1  # Update clock
            lamport_socket.sendto(str(lamport_clock).encode(), address)

def run_servers():
    ntp_thread = threading.Thread(target=start_ntp_server)
    lamport_thread = threading.Thread(target=start_lamport_server)

    ntp_thread.start()
    lamport_thread.start()

    ntp_thread.join()
    lamport_thread.join()   

if __name__ == "__main__":
    run_servers()
