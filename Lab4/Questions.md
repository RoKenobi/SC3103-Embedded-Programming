Questions | Shared Buffer

    Client: Write a C client program for RPi. Accept command-line
    argument N, use fork() to create N child processes. Each child
    connects to server via TCP socket, sends one integer, receives
    reply, and prints: PID, value sent, buffer slot index, current buffer
    sum.

    Server: Write a C server program for PC. Listen on TCP port, create
    one thread per client connection. Maintain shared circular buffer
    (size 5), write index, and sum. Each thread receives integer, enters
    critical section (protected by mutex/semaphore), inserts value into
    buffer, updates index and sum, sends back and prints: value, slot
    index, buffer sum.
