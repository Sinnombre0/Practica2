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

int main(void) {
    srand(time(NULL));  
    //Creamos el socket para simular la capa 2
    int socket_emisor = socket(AF_PACKET, SOCK_RAW, htons(0x88B5)); // 0x88B5 para el protocolo personalizado
  
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

     //Copiamos la MAC de destino
    memmove(trama_Ethernet + offset, mac_destino, 6);
    offset += 6;

    //Copiamos la MAC de origen
    memmove(trama_Ethernet + offset, mac_origen, 6);
    offset += 6;

    uint16_t ethertype = htons(0x88B5);
    memmove(trama_Ethernet + offset, &ethertype, 2);
    offset += 2;

    memmove(trama_Ethernet + offset, payload, payload_len);
    offset += payload_len;

    // Enviamos el mensaje  por la interfaz
    struct sockaddr_ll sa;
    memset(&sa, 0, sizeof(sa));
    sa.sll_family   = AF_PACKET;
    sa.sll_ifindex  = ifindex;
    sa.sll_halen    = 6;
    memcpy(sa.sll_addr, mac_destino, 6);

    sendto(socket_emisor, trama_Ethernet, offset, 0,(struct sockaddr*)&sa, sizeof(sa));

    printf("Trama enviada (%zu bytes)\n", offset);
    close(socket_emisor);
    return 0;
}