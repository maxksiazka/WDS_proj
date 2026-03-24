import socket

# unused for now, will later be used to send fake sensor data to the server
STRUCT_FORMAT="<I6f3ff"

def run_emulator():
    udp_sock = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
    udp_sock.bind(('',12345))
    print("MCU Emulator is running on port 12345...")
    
    data, addr = udp_sock.recvfrom(1024)
    print(f"Received data from {addr}: {data.hex()}")
    message = data.decode()

    if "DISCOVER" in message:
        tcp_port = int(message.split(":")[1])
        print(f"Received DISCOVER message, Connecting to TCP server at {addr[0]}:{tcp_port}...")
        tcp_sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        try:
            tcp_sock.connect((addr[0], tcp_port))
            print("Connected to TCP server.")
        except Exception as e:
            print(f"Failed to connect to TCP server: {e}")
            return

if __name__ == '__main__':
    run_emulator()
