#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <time.h>

#define PORT 8080
// IP Address of the Ubuntu PC on the ethernet network
#define SERVER_IP "192.168.3.171" 

typedef struct {
    int value;
    int slot_index;
    int buffer_sum;
} BufferReply;

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <number_of_children>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    
    for (int i = 0; i < n; i++) {
        pid_t pid = fork();

        if (pid == 0) { 
            // --- CHILD PROCESS ---
            // Seed randomness using PID so each child gets a unique number
            srand(time(NULL) ^ getpid()); 
            
            int sock;
            struct sockaddr_in server;
            
            sock = socket(AF_INET, SOCK_STREAM, 0);
            server.sin_family = AF_INET;
            server.sin_port = htons(PORT);
            server.sin_addr.s_addr = inet_addr(SERVER_IP);

            if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
                perror("Connection failed");
                exit(1);
            }

            int value_to_send = rand() % 100 + 1; // Random number 1-100
            
            send(sock, &value_to_send, sizeof(value_to_send), 0);

            BufferReply reply;
            if (recv(sock, &reply, sizeof(reply), 0) > 0) {
                printf("PID: %5d | Sent: %4d | Slot: %d | Buffer Sum: %5d\n", 
                       getpid(), 
                       reply.value, 
                       reply.slot_index, 
                       reply.buffer_sum);
            }

            close(sock);
            exit(0); 
        }
    }

    while (wait(NULL) > 0);
    printf("\nALL %d data transmission complete.\n", n);

    return 0;
}
