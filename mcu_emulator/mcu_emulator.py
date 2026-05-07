#pyright: basic
import socket
import struct
import time

# Data frame format: <I Q 3f 3f 3f f f f f i i B B H
# I = uint32_t (sync_word)
# Q = uint64_t (timestamp_us)
# 3f = 3x float (accel)
# 3f = 3x float (gyro)
# 3f = 3x float (mag)
# f = float (pressure)
# f = float (temperature)
# f = float (altitude)
# f = float (airspeed)
# i = int32_t (gps_lat)
# i = int32_t (gps_lon)
# B = uint8_t (gps_sats)
# B = uint8_t (gps_fix)
# H = uint16_t (checksum)
STRUCT_FORMAT = "<IQ3f3f3fffff ii BB H"

SYNC_WORD = 0xDEADBEEF

def calculate_crc16(data):
    """Calculate CRC-16-CCITT checksum"""
    crc = 0xFFFF
    for byte in data:
        crc ^= byte << 8
        for _ in range(8):
            crc = (crc << 1) ^ 0x1021 if (crc & 0x8000) else crc << 1
            crc &= 0xFFFF
    return crc

def create_sensor_packet():
    """Create a mock sensor data packet"""
    timestamp_us = int(time.time() * 1_000_000)
    accel = (9.81, 0.0, 0.0)
    gyro = (0.1, 0.2, 0.3)
    mag = (20.0, 25.0, 30.0)
    pressure = 101325.0
    temperature = 25.5
    altitude = 100.0
    airspeed = 15.0
    gps_lat = 520000000  # 52.0°
    gps_lon = 210000000  # 21.0°
    gps_sats = 12
    gps_fix = 3
    
    # Pack data without checksum
    packet_data = struct.pack(STRUCT_FORMAT[:-1],
        SYNC_WORD, timestamp_us,
        *accel, *gyro, *mag,
        pressure, temperature, altitude, airspeed,
        gps_lat, gps_lon, gps_sats, gps_fix)
    
    # Calculate checksum
    checksum = calculate_crc16(packet_data)
    
    # Pack final packet with checksum
    packet = struct.pack(STRUCT_FORMAT,
        SYNC_WORD, timestamp_us,
        *accel, *gyro, *mag,
        pressure, temperature, altitude, airspeed,
        gps_lat, gps_lon, gps_sats, gps_fix, checksum)
    
    return packet

def run_emulator():
    udp_sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    udp_sock.bind(('', 12345))
    print("MCU Emulator is running on port 12345...")
    
    data, addr = udp_sock.recvfrom(1024)
    print(f"Received data from {addr}: {data.decode()}")
    message = data.decode()

    if "DISCOVER" in message:
        tcp_port = int(message.split(":")[1])
        print(f"Received DISCOVER message, Connecting to TCP server at {addr[0]}:{tcp_port}...")
        tcp_sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        try:
            tcp_sock.connect((addr[0], tcp_port))
            print("Connected to TCP server.")
            
            # Send sensor data continuously
            print("Starting to send sensor data...")
            while True:
                packet = create_sensor_packet()
                tcp_sock.sendall(packet)
                print(f"Sent sensor packet ({len(packet)} bytes)")
                time.sleep(0.1)  # Send at 10 Hz
                
        except Exception as e:
            print(f"Error: {e}")
        finally:
            tcp_sock.close()
    
    udp_sock.close()

if __name__ == '__main__':
    run_emulator()
