//
// Created by Kimihito on 04.10.2024.
//

#include <iostream>
#include <utility>
#include <unistd.h>
#include "Tracerouter.h"

Tracerouter::Tracerouter() {
    Init();
}

Tracerouter::Tracerouter(std::string address): destName(std::move(address)) {
    Init();
    GetIP();
}

Tracerouter::~Tracerouter() {
    printf("Destroyed\n");
}

void Tracerouter::SetDestination(std::string &address) {
    this->destName = std::move(address);
    GetIP();
}

void Tracerouter::PrintDestinationInfo() {
    printf("[Name]\t\t%s\n", destName.c_str());
    printf("[Addresses]\t\n");

    for(auto it = destIPs.begin(); it != destIPs.end(); it++){
        if (it == destIPs.begin())
            printf("\t\t%s*\n", (*it).c_str());
        else
            printf("\t\t%s\n", (*it).c_str());
    }
    printf("\n");
}

void Tracerouter::GetIP() {
    destIPs.clear();
    addrinfo hints{}, *res;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    if(getaddrinfo(this->destName.c_str(), nullptr, &hints, &res) != 0) {
        printf("%s\t[Error]: Destination doesn't exist\n", log.c_str());
    }

    for (; res != nullptr; res=res->ai_next)
        destIPs.emplace_back(inet_ntoa(((struct sockaddr_in *)res->ai_addr)->sin_addr));

    destIP = destIPs.front();
}

void Tracerouter::Trace() {
    int sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sock < 0) {
        printf("%s[Error]: Socket creation failed\n", log.c_str());
        exit(1);
    }

    sockaddr_in destinationAddress{};
    destinationAddress.sin_family = AF_INET; //Setting family to ipv4
    destinationAddress.sin_port = htons(0);
    inet_pton(AF_INET, destIP.c_str(), &destinationAddress.sin_addr);

    icmp icmpPacket{};

    //MaxHops ~ steps
    const int maxHops = 30;

    //Something weird happenes here... Looks like timeout doesnt work at all, lol
    //Due to weird anomaly when i setting the value to 150, the received packet doesn't throw an error.
    timeval timeout;
    timeout.tv_sec = 150;
    timeout.tv_usec = 0;
    setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, (const char*)&timeout, sizeof(timeout));
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout));
    //
    for(int ttl = 1; ttl <= maxHops; ttl++){
        Sleep(1000);

        setsockopt(sock, IPPROTO_IP, IP_TTL, reinterpret_cast<const char *>(&ttl), sizeof(ttl));

        icmpPacket.type = 8; //Echo request
        icmpPacket.code = 0;
        icmpPacket.checksum = 0;
        icmpPacket.info = 0;
        icmpPacket.payload = 0b00000000000000000000000001;
        icmpPacket.checksum = Checksum(&icmpPacket, sizeof(icmpPacket));

        int sendData = sendto(sock, reinterpret_cast<const char *>(&icmpPacket), sizeof(icmpPacket), 0,
                              (struct sockaddr*)&destinationAddress, sizeof(destinationAddress));
        if (sendData < 0) {
            printf("%s\t[Error]: send failed, code: %d\n",log.c_str(), WSAGetLastError());
            continue;
        }

        uint64_t ms_start = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();

        sockaddr_in receiveAddress{};
        socklen_t addressLen = sizeof(destinationAddress);

        //Response buffer
        char buffer[1024] = {0};
        
        int recieveResult = recvfrom(sock, buffer, sizeof(buffer), 0,
                                     (struct sockaddr*)&receiveAddress, &addressLen);

        uint64_t ms_end = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();

        if (recieveResult < 0) {
            int errorCode = WSAGetLastError();
            printf("%s\t[Error]: recvfrom failed, code: %d\n",log.c_str(),errorCode);
            continue;
        } else {
            //Filling first 20 bytes ~ IpHeader
            auto* ipHeader = (iphdr*)buffer;
            //Filling next bytes
            icmp* icmpResp = (icmp*)(buffer+(ipHeader->h_len*4));

            printf("%s\t[%d]\t%llu ms\t->%s\t\n",log.c_str(),ttl, ms_end-ms_start, inet_ntoa(receiveAddress.sin_addr));

            if(icmpResp->type == 0) {
                break;
            }
        }
    }

    closesocket(sock);
}

void Tracerouter::Init() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed." << std::endl;
    }
}

uint16_t Tracerouter::Checksum(void *buffer, uint32_t length) {
    auto* data = reinterpret_cast<uint16_t*>(buffer);
    uint32_t sum = 0;
    for (int i = 0; i < length / 2; i++) {
        sum += data[i];
    }
    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    return ~static_cast<uint16_t>(sum);
}
//Still hope i'd never see this again..
// or till normal documentation existence =)
