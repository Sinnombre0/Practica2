#include <sys/socket.h>
#include <sys/ioctl.h>
#include <linux/if_packet.h>
#include <net/if.h>
#include <net/ethernet.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>

int main(void) {
    //Creamos el socket para simular la capa 2
    int socket_emisor = socket(AF_PACKET, SOCK_RAW, htons(0x88B5)); // 0x88B5 es el Ethertype para el protocolo personalizado
    struct ifreq ifr;
    strcpy(ifr.ifr_name, "eth0");
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

    const char *payload = "Capa 2";
    size_t payload_len = strlen(payload);

    // Buffer completo
    

}