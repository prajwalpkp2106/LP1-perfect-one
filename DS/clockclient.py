import socket
import time

NTP_OFFSET = 2208988800  # Offset in seconds between 1900 and 1970

def get_ntp_time():
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    
    # Send a request to the NTP server
    # client_socket.sendto(b'\x1b' + bytes(47), ('localhost', 12346))
    client_socket.sendto(b'\x1b' + (b'\0'*47), ('localhost', 12346))

    
    # Receive response from the NTP server
    data, _ = client_socket.recvfrom(1024)
    
    # Debug: print received data
    # print("Received data from NTP server:", data)

    # Check if data has enough length to extract the timestamp
    # Extract timestamp (4 bytes from byte 40)
    timestamp = int.from_bytes(data[40:44], 'big') - NTP_OFFSET
    print(f"NTP Server Time: {time.ctime(timestamp)}")

def get_lamport_time():
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    
    # Initialize local Lamport clock
    local_clock = 0
    
    # Send local clock value to server
    client_socket.sendto(str(local_clock).encode(), ('localhost', 12347))
    
    # Wait for server response
    data, _ = client_socket.recvfrom(1024)
    lamport_time = int(data.decode())
    
    print(f"Lamport Server Time: {lamport_time}")

if __name__ == "__main__":
    # Get NTP Time
    get_ntp_time()

    # Get Lamport Time
    get_lamport_time()
