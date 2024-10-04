//
// Created by Kimihito on 04.10.2024.
//
#ifndef TRACEROUTE_TRACEROUTER_H
#define TRACEROUTE_TRACEROUTER_H

#include <winsock2.h>
#include <ws2tcpip.h>

#include <string>
#include <chrono>
#include <vector>
#include <iomanip>

using namespace std::chrono;

inline std::string CurrentTime() {
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    std::tm* now_tm = std::localtime(&now_time);
    std::ostringstream oss;
    oss << std::put_time(now_tm, "[%H:%M:%S] ");
    return oss.str();
}

#define log CurrentTime()

class Tracerouter {
    //Destination Info
    std::string destName;
    std::vector<std::string> destIPs;
    //Main IPv4
    std::string destIP;

    void GetIP();
    static void Init();

    static uint16_t Checksum(void* buffer, uint32_t length);

    //https://en.wikipedia.org/wiki/Internet_Control_Message_Protocol
    //8 bytes
    struct icmp {
        uint8_t type;
        uint8_t code;
        uint16_t checksum;
        uint32_t info;
        //
        uint64_t payload;
    };

    //20 bytes
    struct iphdr {
        //Two fields of 4 bits
        uint8_t h_len:4;
        uint8_t version:4;
        //
        uint8_t tos;
        uint16_t total_len;
        uint16_t ident;
        uint16_t frag_and_flags;
        uint8_t ttl;
        uint8_t proto;
        uint16_t checksum;
        uint32_t sourceIP;
        uint32_t destIP;
    };
public:
    Tracerouter();
    Tracerouter(std::string address);
    ~Tracerouter();
    //
    void SetDestination(std::string address);
    void PrintDestinationInfo();
    //
    void Trace();

    enum class ErrorCodes {
        UnexistedDestionationErr = 1,
        SocketCreationErr,
        WSAStartupErr,
    };
};

#endif //TRACEROUTE_TRACEROUTER_H
