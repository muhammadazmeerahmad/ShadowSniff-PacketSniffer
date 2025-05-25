import os
import re
from collections import Counter

LOG_FILE = "../logs/packet.log"
LOG_PATTERN = re.compile(
    r"Protocol: (\w+) \| Source: ([\d.]+):(\d+) -> Dest: ([\d.]+):(\d+) \| Packet Size: (\d+) bytes"
)

def read_log():
    if not os.path.exists(LOG_FILE):
        print("❌ Log file not found. Make sure 'packet.log' exists.")
        return []

    with open(LOG_FILE, "r") as f:
        lines = f.readlines()

    parsed = []
    for line in lines:
        match = LOG_PATTERN.search(line)
        if match:
            protocol, src_ip, src_port, dst_ip, dst_port, size = match.groups()
            parsed.append({
                "protocol": protocol,
                "src_ip": src_ip,
                "src_port": int(src_port),
                "dst_ip": dst_ip,
                "dst_port": int(dst_port),
                "size": int(size)
            })
    return parsed

def protocol_summary(packets):
    counter = Counter(pkt['protocol'] for pkt in packets)
    print("\n📊 Protocol Summary:")
    for proto, count in counter.items():
        print(f"  - {proto}: {count} packets")

def top_ips(packets, key, label):
    counter = Counter(pkt[key] for pkt in packets)
    print(f"\n📌 Top 5 {label} IPs:")
    for ip, count in counter.most_common(5):
        print(f"  - {ip}: {count} packets")

def largest_packets(packets, n=5):
    top_packets = sorted(packets, key=lambda x: x['size'], reverse=True)[:n]
    print(f"\n🚀 Top {n} Largest Packets:")
    for pkt in top_packets:
        print(f"  - {pkt['protocol']} | {pkt['src_ip']}:{pkt['src_port']} -> {pkt['dst_ip']}:{pkt['dst_port']} | {pkt['size']} bytes")

def main():
    packets = read_log()
    if not packets:
        return

    while True:
        print("\n===== Packet Log Analyzer =====")
        print("1. View Protocol Summary")
        print("2. Top 5 Source IPs")
        print("3. Top 5 Destination IPs")
        print("4. Show Largest Packets")
        print("5. Exit")
        choice = input("Choose an option (1-5): ")

        if choice == '1':
            protocol_summary(packets)
        elif choice == '2':
            top_ips(packets, 'src_ip', 'Source')
        elif choice == '3':
            top_ips(packets, 'dst_ip', 'Destination')
        elif choice == '4':
            largest_packets(packets)
        elif choice == '5':
            print("👋 Exiting analyzer.")
            break
        else:
            print("❗ Invalid choice. Please try again.")

if __name__ == "__main__":
    main()
