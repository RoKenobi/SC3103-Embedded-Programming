# Shared Buffer Lab - README

## 1. Lab Setup & Terminal Splitting
To efficiently run this client-server model, use a single monitor split into two views using the tmux terminal multiplexer.
1. Open a terminal on the Ubuntu PC.
2. Start tmux by typing: tmux
3. Split the screen vertically: Press Ctrl+B, let go, then press %.
4. You now have a Left Pane (Ubuntu PC / Server) and a Right Pane.
5. Move to the Right Pane (Press Ctrl+B + Right Arrow) and SSH into the Raspberry Pi: ssh pi@192.168.3.1 (Password: raspberry).

## 2. Compilation and Execution

Step 1: On the Left Pane (Ubuntu PC), compile and start the server.

    gcc -o server server.c -lpthread
    ./server

Step 2: On the Right Pane (Raspberry Pi), compile the client and trigger N concurrent connections (e.g., 8).

    gcc -o client client.c
    ./client 8

## 3. Expected Output

Server View (Left Pane):

    Buffer serverrunning. waiting for data.
    Server stored:   53 | Slot: 0 | New sum:    53
    Server stored:    1 | Slot: 1 | New sum:    54
    Server stored:   51 | Slot: 2 | New sum:   105
    Server stored:   61 | Slot: 3 | New sum:   166
    Server stored:    3 | Slot: 4 | New sum:   169
    Server stored:   85 | Slot: 0 | New sum:   201
    Server stored:   45 | Slot: 1 | New sum:   245
    Server stored:   38 | Slot: 2 | New sum:   232

Client View (Right Pane):

    PID:  1015 | Sent:   53 | Slot: 0 | Buffer Sum:    53
    PID:  1016 | Sent:    1 | Slot: 1 | Buffer Sum:    54
    PID:  1020 | Sent:   61 | Slot: 3 | Buffer Sum:   166
    PID:  1019 | Sent:   51 | Slot: 2 | Buffer Sum:   105
    PID:  1017 | Sent:    3 | Slot: 4 | Buffer Sum:   169
    PID:  1021 | Sent:   45 | Slot: 1 | Buffer Sum:   245
    PID:  1022 | Sent:   85 | Slot: 0 | Buffer Sum:   201
    PID:  1018 | Sent:   38 | Slot: 2 | Buffer Sum:   232
    
    ALL 8 data transmission complete.

## 4. Explanation of Output
The output demonstrates a network race condition being successfully managed by a mutex lock. 

On the Client side, you can see the PIDs are printing out of order (e.g., PID 1020 finishes before PID 1019). This proves that all 8 child processes are executing concurrently and racing across the network.

However, on the Server side, the data is processed linearly. The slot indices proceed perfectly from 0 to 4, and then wrap around back to 0. When the wrap-around occurs at Slot 0 (the 6th transaction), the server correctly subtracts the old value (53) from the running total (169) before adding the new value (85), resulting in the correct new sum of 201.

## 5. Correlation to the Question
This implementation perfectly satisfies the prompt's constraints:
* "Accept command-line argument N, use fork() to create N child processes." The client uses atoi(argv[1]) to capture the input and a for loop with fork() to spawn exactly N distinct processes.
* "Maintain shared circular buffer (size 5)..." The server initializes a global array of size 5. The index is incremented using modulo arithmetic (write_index + 1) % 5, forcing it to endlessly loop through slots 0-4.
* "...enters critical section (protected by mutex...)" The server utilizes pthread_mutex_lock() before reading or altering the shared array and sum. This is the single most critical component of the code; without it, the 8 concurrent connections would overwrite each other's data, corrupting the sum and slot indices.
* "Sends back and prints: value, slot index, buffer sum." Instead of sending raw, messy byte streams, the system utilizes a C struct (BufferReply) to guarantee the data is perfectly packaged by the server and accurately unpacked and printed by the client.
