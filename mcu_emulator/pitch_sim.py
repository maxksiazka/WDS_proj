# pyright: basic
import socket
import struct
import random
import time
import math

# Data frame format: <I Q 3f 3f 3f f f f f i i B B H
STRUCT_FORMAT = "<IQ3f3f3fffff fff BB H"
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
    """Create a mock sensor data packet with a slow pitching maneuver"""
    sigma_accel = 0.2  # m/s²
    sigma_gyro = 0.05  # rad/s noise standard deviation
    timestamp_us = int(time.time() * 1_000_000)

    t = time.time()
    period = 10.0          # Time in seconds to complete a full pitch up/down cycle
    max_pitch_deg = 20.0   # Maximum pitch angle amplitude
    max_pitch_rad = math.radians(max_pitch_deg)

    theta = max_pitch_rad * math.sin(2 * math.pi * t / period)

    pitch_rate_y = max_pitch_rad * (2 * math.pi / period) * math.cos(2 * math.pi * t / period)

    accel_x_g = -9.81 * math.sin(theta)
    accel_y_g = 0.0
    accel_z_g = 9.81 * math.cos(theta)

    accel = (
        accel_x_g + random.gauss(0, sigma_accel),
        accel_y_g + random.gauss(0, sigma_accel),
        accel_z_g + random.gauss(0, sigma_accel),
    )
    
    gyro = (
        0.0 + random.gauss(0, sigma_gyro),
        pitch_rate_y + random.gauss(0, sigma_gyro),
        0.0 + random.gauss(0, sigma_gyro),
    )

    mag = (20.0, 25.0, 30.0)
    pressure = 101325.0
    temperature = 25.5
    altitude = 100.0
    airspeed = 150.0
    gps_ground_speed = 200.0
    gps_lat = 52.0  # 52.0°
    gps_lon = 21.0  # 21.0°
    gps_sats = 12
    gps_fix = 2

    packet_data = struct.pack(
        STRUCT_FORMAT[:-1],
        SYNC_WORD,
        timestamp_us,
        *accel,
        *gyro,
        *mag,
        pressure,
        temperature,
        altitude,
        airspeed,
        gps_ground_speed,
        gps_lat,
        gps_lon,
        gps_sats,
        gps_fix,
    )

    checksum = calculate_crc16(packet_data)

    packet = struct.pack(
        STRUCT_FORMAT,
        SYNC_WORD,
        timestamp_us,
        *accel,
        *gyro,
        *mag,
        pressure,
        temperature,
        altitude,
        airspeed,
        gps_ground_speed,
        gps_lat,
        gps_lon,
        gps_sats,
        gps_fix,
        checksum,
    )

    return packet


def run_emulator():
    udp_sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    udp_sock.bind(("", 12345))
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

            print("Starting to send sensor data...")
            while True:
                packet = create_sensor_packet()
                tcp_sock.sendall(packet)
                time.sleep(0.02)  # Stream smoothly at 50 Hz

        except Exception as e:
            print(f"Error: {e}")
        finally:
            tcp_sock.close()

    udp_sock.close()


if __name__ == "__main__":
    run_emulator()
