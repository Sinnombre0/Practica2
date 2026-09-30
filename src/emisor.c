#include <sys/socket.h>
#include <sys/ioctl.h>
#include <linux/if_packet.h>
#include <net/if.h>
#include <net/ethernet.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

// Funcion para agregar ruido
void aplicar_ruido(unsigned char *datos, size_t len, int probabilidad) {
    for (size_t i = 0; i < len; i++)
        if (rand() % 100 < probabilidad)
            datos[i] ^= (1 << (rand() % 8));
}

int main(void) {
    srand(time(NULL));  
    //Creamos el socket para simular la capa 2
    int socket_emisor = socket(AF_PACKET, SOCK_RAW, htons(0x88B5)); // 0x88B5 para el protocolo personalizado
    if(socket_emisor < 0){
        perror("socket");
        return 1;
    }
    // Declaramos ifr para pasar la informacion e inicalizamos memset con 0
    struct ifreq ifr;
    memset(&ifr, 0, sizeof(ifr));
    strncpy(ifr.ifr_name, "eth0", IFNAMSIZ - 1);
    
    // Obtenemos el índice de la interfaz de red 
    ioctl(socket_emisor, SIOCGIFINDEX, &ifr);
    int ifindex = ifr.ifr_ifindex;

    // Obtenemos la dirección MAC de la interfaz de red
    ioctl(socket_emisor, SIOCGIFHWADDR, &ifr);
    unsigned char mac_origen[6];

    // Armamos el buffer de bytes con memcpy
    memcpy(mac_origen, ifr.ifr_hwaddr.sa_data, 6);
    // Mac destino
    unsigned char mac_destino[6] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};

    const char *payload = "Hola capa 2";
    size_t payload_len = strlen(payload);

    // Buffer completo
    unsigned char trama_Ethernet[14 + 1500];  // 14 bytes cabecera + payload
    size_t offset = 0;

     //Copiar MAC destino al inicio
    memmove(trama_Ethernet + offset, mac_destino, 6);
    offset += 6;

    //Copiar MAC origen después
    memmove(trama_Ethernet + offset, mac_origen, 6);
    offset += 6;

    // Copiar Ethertype
    uint16_t ethertype = htons(0x88B5);
    memmove(trama_Ethernet + offset, &ethertype, 2);
    offset += 2;

    //Copiar payload
    memmove(trama_Ethernet + offset, payload, payload_len);
    offset += payload_len;

    // Enviamos la trama por la interfaz
    struct sockaddr_ll sa;
    memset(&sa, 0, sizeof(sa));
    sa.sll_family   = AF_PACKET;
    sa.sll_ifindex  = ifindex;
    sa.sll_halen    = 6;
    memcpy(sa.sll_addr, mac_destino, 6);

    aplicar_ruido(trama_Ethernet + 14, offset - 14, 5); 

    sendto(socket_emisor, trama_Ethernet, offset, 0,(struct sockaddr*)&sa, sizeof(sa));

    printf("Trama enviada (%zu bytes)\n", offset);
    close(socket_emisor);
    return 0;
}