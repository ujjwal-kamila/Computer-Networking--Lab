
# 🌐 Computer Networks Project Repository

Welcome to my ✨ **Computer Networks Project Repository** ✨!  
This repository includes a set of socket programming projects implemented in C using **TCP** and **UDP** protocols.  
Explore real-time communication, file transfer, concurrent servers, and more — all in one place!

---

## 📁 Repository Structure 📊

| 🔢 No. | 📌 Project Title                           | 🌐 Protocol | ⚙️ Status         |
|-------|---------------------------------------------|-------------|------------------|
| 1️⃣   | 🔁 Echo Server                              | TCP         | Done ✅ |
| 2️⃣   | 🔁 Echo Server                              | UDP         | Done ✅ |
| 3️⃣   | 💬 Chat Server                              | TCP         | Done ✅ |
| 4️⃣   | 💬 Chat Server                              | UDP         | Done ✅ |
| 5️⃣   | 🕓 Day-Time Server                          | TCP & UDP   | Done ✅ |
| 6️⃣   | ➗ Math Server                              | TCP         | Done ✅ |
| 7️⃣   | 🔄 Concurrent Multi-client Chat Server      | TCP         | Done ✅ |
| 8️⃣   | 📂 File Transfer Client-Server              | TCP         | Done ✅ |

---

## 🚀 Features

✅ Socket Programming  
✅ TCP/UDP Protocol Implementations  
✅ Concurrent Client Handling with `pthreads`  
✅ Real-time Messaging  
✅ File Transfer  
✅ Modular and Clean Code  

---

## 🧰 How to Run

1. **Compile the server and client files** using `gcc`:
   ```bash
   gcc server.c -o server -lpthread
   gcc client.c -o client
   ```

2. **Start the server:**
   ```bash
   ./server <port_number>
   ```

3. **Connect the client:**
   ```bash
   ./client <server_ip> <port_number>
   ```

> ⚠️ Replace `<port_number>` with the desired port (e.g., 8080) and `<server_ip>` with the server’s IP address (use `127.0.0.1` for localhost).

---

## 📚 Topics Covered

- 🔗 TCP vs UDP communication
- 📞 Blocking vs Non-blocking sockets
- 🧵 Multi-threading with `pthread`
- 📤 Client-server file transmission
- 🔄 One-to-one and broadcast messaging
- 🕰️ Time synchronization using Day-Time protocol
- ➕ Real-time arithmetic evaluation

---

## 📦 Future Improvements

- ✅ GUI Client using Python/Tkinter
- ✅ Secure communication using SSL
- ✅ Upload/Download file feature with progress bar
- ✅ Chat with usernames & colors

---

## 🙌 Contributing

Feel free to open issues or suggest new features!  
PRs are welcome 🤝






---

## 👨‍💻 Author

**Ujjwal Kamila**  
🖥️ B.Tech CSE | Computer Networking & Systems Enthusiast  
📧 [ujjwalkamila86@gmail.com]  
🔗 GitHub: [ujjwal-kamila](https://github.com/ujjwal-kamila)

---

## ⭐ If you like it...

Leave a ⭐ on the repository and share it with friends interested in networks and socket programming!
