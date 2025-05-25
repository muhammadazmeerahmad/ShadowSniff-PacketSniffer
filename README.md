# ShadowSniff Packet Sniffer

🔍 A **lightweight C++ packet sniffer and analyzer**, capturing live network traffic 🎯 and logging packets for detailed inspection 📄. Comes with a Python-based analyzer for easy review 📊. Fully CLI-driven and designed to run smoothly on Linux systems 💻.

---

## ⚠️ Important Note

This project runs **locally on Linux** 💻 and requires root privileges to capture packets.  
It is **not designed for deployment on servers** 🌐 and is intended for practical network monitoring and learning.

🛠️ **Feel free to:**
- 📥 Clone the repository  
- 🧑‍💻 Explore and modify the code  
- 🧪 Test and analyze network traffic  
- 🚀 Use it as a base for packet sniffing or analysis projects

✅ Great for developers, cybersecurity enthusiasts, and network admins!

---

## 🚀 Features

- **Live Packet Capture**: Capture network traffic in real-time from interfaces.  
- **Packet Logging**: Logs raw packets to files for later analysis.  
- **Python Analyzer**: Analyze logged data with an easy-to-use Python script.  
- **CLI Interface**: Simple command-line control with clear options.  
- **Lightweight & Fast**: Minimal dependencies, focusing on performance.

---

## 📦 Clone & Run Locally

### 1️⃣ Clone the repository:
```bash
git clone https://github.com/yourusername/shadowsniff.git
cd shadowsniff
```

### 2️⃣ Compile the source code (only if binary not found):
```bash
gcc src/main.cpp src/packet_sniffer.cpp src/protocol_parser.cpp -o src/a.out
```

### 3️⃣ Make the run script executable:
```bash
chmod +x run.sh
```

### 4️⃣ Run the script:
```bash
./run.sh
```
## 🛠️ Manual Compilation 💻⚙️

If the **binary** is missing or you wanna **recompile** the program, just run this command:

```bash
gcc src/main.cpp src/packet_sniffer.cpp src/protocol_parser.cpp -o src/a.out
```

## 🗂️ Project Structure

```bash
ShadowSniff/
│
├── run.sh                     # Main bash script to compile & run
├── logs/                      # Folder to store captured packet logs
│   └── packet.log             # Packet capture log file
│
├── src/                       # Source code & analyzer script
│   ├── main.cpp               # Main C++ program
│   ├── packet_sniffer.cpp     # Packet sniffing logic
│   ├── protocol_parser.cpp    # Packet parsing logic
│   ├── packet_sniffer.h       # Header file for packet sniffer
│   └── analyzer.py            # Python script to analyze logs 📊🐍
│
└── README.md                  # Project overview & instructions 📘
```

## ⚙️ Requirements

- 🐧 Linux OS (recommended for raw socket support)  
- 🛠️ GCC compiler (`gcc`)  
- 🐍 Python 3 (for log analysis)  
- 🔐 Root privileges (run with `sudo` to sniff packets)  

---

Feel free to open issues or contribute! Let's make network monitoring fun & easy! 🚀🎯

Happy sniffing! 🎉🕵️‍♂️📡
