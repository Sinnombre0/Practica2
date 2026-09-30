#include <sys/socket.h>
#include <sys/ioctl.h>
#include <linux/if_packet.h>
#include <net/if.h>
#include <net/ethernet.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

int main(){
    // Creamos el socket que recibe el mensaje
    int socket_receptor = socket(AF_PACKET, SOCK_RAW, htons(0x88B5));

    struct ifreq ifr;
    memset(&ifr, 0, sizeof(ifr));
    strcpy(ifr.ifr_name, "eth0");

    int ifindex = ifr.ifr_ifindex;

    struct sockaddr_ll direccion;
    memset(&direccion, 0, sizeof(direccion));
    direccion.sll_family  = AF_PACKET;
    direccion.sll_protocol = htons(0x88B5);
    direccion.sll_ifindex = ifindex;

    unsigned char buffer[2048];

    while (1) {
        ssize_t n = recvfrom(socket_receptor, buffer, sizeof(buffer), 0, NULL, NULL);

        unsigned short ethertype = (buffer[12] << 8) | buffer[13];

        // Extraemos MACs y payload
        unsigned char *mac_destino = buffer;        
        unsigned char *mac_origen  = buffer + 6;     
        unsigned char *payload     = buffer + 14;   
        size_t payload_len = n - 14;

        //Imprimir info de la trama
        printf("\n=== Mensaje recibido (%zd bytes) ===\n", n);
        printf("MAC destino: %02X:%02X:%02X:%02X:%02X:%02X\n",
               mac_destino[0], mac_destino[1], mac_destino[2],
               mac_destino[3], mac_destino[4], mac_destino[5]);
        printf("MAC origen : %02X:%02X:%02X:%02X:%02X:%02X\n",
               mac_origen[0], mac_origen[1], mac_origen[2],
               mac_origen[3], mac_origen[4], mac_origen[5]);
        printf("Ethertype  : 0x%04X\n", ethertype);
        printf("Payload (%zu bytes): ", payload_len);

        // Imprimir mensaje
        for (size_t i = 0; i < payload_len; i++) {
            unsigned char c = payload[i];
            putchar((c >= 32 && c < 127) ? c : '.');
        }
        putchar('\n');

    close(socket_receptor);
    return 0;
    }
}