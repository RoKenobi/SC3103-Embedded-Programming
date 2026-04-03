#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8080
#define BUFFER_SIZE 5

// Global shared variables
int buffer[BUFFER_SIZE] = {0, 0, 0, 0, 0};
int write_index = 0;
int buffer_sum = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; 

// The communication struct
typedef struct {
    int value;
    int slot_index;
    int buffer_sum;
} BufferReply;

void *handle_client(void *socket_desc) {
    int client_sock = *(int*)socket_desc;
    free(socket_desc); 
    
    int received_val;
    BufferReply reply;

    if (recv(client_sock, &received_val, sizeof(received_val), 0) > 0) {
        
        // --- ENTER CRITICAL SECTION ---
        pthread_mutex_lock(&mutex); 
        
        buffer_sum -= buffer[write_index];   // Remove old value
        buffer[write_index] = received_val;  // Insert new value
        buffer_sum += received_val;          // Update sum
        
        reply.value = received_val;
        reply.slot_index = write_index;
        reply.buffer_sum = buffer_sum;
        
        printf("Server stored: %4d | Slot: %d | New sum: %5d\n", reply.value, reply.slot_index, reply.buffer_sum);
        
        write_index = (write_index + 1) % BUFFER_SIZE; // Advance circular index
        
        // --- EXIT CRITICAL SECTION ---
        pthread_mutex_unlock(&mutex); 

        send(client_sock, &reply, sizeof(reply), 0);
    }
    
    close(client_sock);
    pthread_exit(NULL);
}

int main() {
    int server_sock, client_sock, c;
    struct sockaddr_in server, client;

    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY; 
    server.sin_port = htons(PORT);

    bind(server_sock, (struct sockaddr *)&server, sizeof(server));
    listen(server_sock, 5); 

    printf("Buffer server running. waiting for data.\n");

    c = sizeof(struct sockaddr_in);
    while ((client_sock = accept(server_sock, (struct sockaddr *)&client, (socklen_t*)&c))) {
        pthread_t client_thread;
        int *new_sock = malloc(sizeof(int)); 
        *new_sock = client_sock;
        
        pthread_create(&client_thread, NULL, handle_client, (void*) new_sock);
        pthread_detach(client_thread); 
    }
    
    return 0;
}
