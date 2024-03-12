#pragma once

#include <iostream>
#include <string>
#include <vector>

namespace network
{
using namespace std;

class systemd
{
public:
    bool start(void);
    bool stop(void);
    bool restart(void);
};

class resolv_conf
{
public:
    bool add(std::vector<std::string> servers);
};

class route
{
private:
    bool add(std::string port, std::string ip);
    bool del(std::string port, std::string ip);
};

class netplan
{
public:
    netplan(string file);
    ~netplan();

    bool eth_add(string port, vector<string> dns); // dhcp
    bool eth_add(string port, vector<string> ip, string gw, vector<string> dns); // static
    bool eth_del(string port);
    
    bool wifi_add(string port, string ssid, string psk); // dhcp
    bool wifi_del(string port);

    bool is_dhcp(string port);

    bool apply(void); // exec

private:
    std::string m_file;
    std::string get_tag(std::string port);
};

class interface
{
public:
    interface(std::string port);
    ~interface();
private:
    std::string m_port;
};


} // namespace network


