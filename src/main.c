#include <logging.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <net/ethernet.h>
#include <netinet/if_ether.h>
#include <linux/if_packet.h>
#include <net/if.h>
#include <netinet/ip.h>

int main() {

    const int BUF_SIZE = 65535;

    // prepare the logfile
    FILE *fptr;
    fptr = fopen("log.txt", "w");

    // prepare the socket and bind to the target interface
    int sock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (sock < 0) {
        error("socket error");
        return 1;
    }
    struct sockaddr_ll bindAddress;
    memset(&bindAddress, 0, sizeof(bindAddress));
    bindAddress.sll_family = AF_PACKET;
    bindAddress.sll_protocol = htons(ETH_P_ALL);
    bindAddress.sll_ifindex = if_nametoindex("enp0s3");
    int b = bind(sock, (struct sockaddr*)&bindAddress, sizeof(bindAddress));
    if (b < 0) {
        error("bind error");
        return 1;
    }

    // prepare the data store
    uint8_t *buf = malloc(BUF_SIZE);
    struct sockaddr peerAddr;
    socklen_t peerAddrLen;

    // loop for 500 packets
    for (int packetIdx = 0; packetIdx < 500; packetIdx++) {
        // get one packet
        int len = recvfrom(sock, buf, BUF_SIZE, 0, &peerAddr, &peerAddrLen);
        if (len < 0) {
            error("recvfrom error");
            continue;
        }

        char logStr[256];
        sprintf(logStr, "packet len: %d", len);
        logger(logStr, fptr, true);

        // pull the ip header after ethernet header
        struct iphdr *iph = (struct iphdr*)(buf + sizeof(struct ethhdr));

        uint32_t saddr = ntohl(iph->saddr);
        uint32_t daddr = ntohl(iph->daddr);
        char protocol[5];
        switch (iph->protocol) {
            case 1:
                sprintf(protocol, "ICMP");
                break;
            case 6:
                sprintf(protocol, "TCP");
                break;
            case 17:
                sprintf(protocol, "UDP");
                break;
            default:
                sprintf(protocol, "%d", iph->protocol);
        }

        sprintf(logStr, "source address: %d.%d.%d.%d", saddr>>24, (saddr>>16)&0xff, (saddr>>8)&0xff, saddr&0xff);
        logger(logStr, fptr, false);
        sprintf(logStr, "destination address: %d.%d.%d.%d", daddr>>24, (daddr>>16)&0xff, (daddr>>8)&0xff, daddr&0xff);
        logger(logStr, fptr, false);
        sprintf(logStr, "protocol: %s", protocol);
        logger(logStr, fptr, false);

        // dump out hex bytes to the log file
        for (uint16_t idx1 = 0; idx1 < len; idx1+=16) {
            for (uint16_t idx2 = 0; idx2 < 16 && len > idx1+idx2; idx2++) {
                sprintf(logStr, "%02x ", buf[idx1+idx2]);
                dataLogger(logStr, fptr, false);
            }
            sprintf(logStr, "\n");
            dataLogger(logStr, fptr, false);
        }
    }

    free(buf);
    fclose(fptr);

    return 0;
}