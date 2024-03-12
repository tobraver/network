#include <unistd.h>
#include <fstream>

#include "network.h"

#include "yaml-cpp/yaml.h"

#define YAML_TRY_START  try {
#define YAML_TRY_END    } \
                        catch (const YAML::Exception& e) { std::cerr << "YAML EXCEPTION: " << e.what() << "\n"; } \
                        catch (...) { std::cerr << "UNKNOW YAML EXCEPTION!" << "\n"; }




namespace network
{

bool network_available(std::string port)
{
    std::string file = "/sys/class/net/";
    file += port + "/operstate";
    std::ifstream f(file);
    if(f)
    {
        std::istreambuf_iterator<char> beg(f), end;
        std::string str(beg, end);
        if(str.find("up") != std::string::npos)
        {
            return true;
        }
    }
    return false;
}

bool systemd::start(void)
{
    std::string cmd = "systemctl start systemd-networkd";
    system(cmd.c_str());
    return true;
}

bool systemd::stop(void)
{
    std::string cmd = "systemctl stop systemd-networkd";
    system(cmd.c_str());
    return true;
}

bool systemd::restart(void)
{
    std::string cmd = "systemctl restart systemd-networkd";
    system(cmd.c_str());
    return true;
}

bool resolv_conf::add(std::vector<std::string> servers)
{
    std::string file = "/etc/resolv.conf";
    std::ofstream f(file);
    if(f)
    {
        std::string content;
        for(auto it : servers)
        {
            content += "nameserver " + it + "\n";
        }
        f << content;
        return true;
    }
    return false;
}

bool route::add(std::string port, std::string ip)
{
    std::string cmd = "route add default gw " + ip + " dev " + port;
    system(cmd.c_str());
    return true;
}

bool route::del(std::string port, std::string ip)
{
    std::string cmd = "route del default gw " + ip + " dev " + port;
    system(cmd.c_str());
    return true;
}

netplan::netplan(std::string file)
{
    m_file = file;
}

netplan::~netplan()
{

}

std::string netplan::get_tag(std::string port)
{
    std::string tag;
    if(port.find("wlan") != std::string::npos)
    {
        tag = "wifis";
    }
    else if(port.find("eth") != std::string::npos)
    {
        tag = "ethernets";
    }
    return tag;
}

// add port, dhcp
bool netplan::eth_add(string port, vector<string> dns)
{
    std::string tag = get_tag(port);
    if(tag.empty())
    {
        return false; // tag is unknown!
    }

YAML_TRY_START
    YAML::Node root = YAML::LoadFile(m_file);

    if(!root["network"])
    {
        return false;
    }

    YAML::Node ethx;

    ethx["dhcp4"] = "yes";

    for(auto it : dns)
    {
        ethx["nameservers"]["addresses"].push_back(it);
    }

    root["network"][tag][port] = ethx;

    std::cout << "--------### netplan ###---------" << std::endl;
    std::cout << root << std::endl;

    std::ofstream f(m_file);
    if(f)
    {
        f << root << "\n";
        return true;
    }
    return false;
YAML_TRY_END
    std::cout << "[netplan,config], yaml error!" << std::endl;
    return false;
}

bool netplan::eth_add(string port, vector<string> ip, string gw, vector<string> dns)
{
    std::string tag = get_tag(port);
    if(tag.empty())
    {
        return false; // tag is unknown!
    }

YAML_TRY_START
    YAML::Node root = YAML::LoadFile(m_file);

    if(!root["network"])
    {
        return false;
    }

    YAML::Node ethx;
    
    for(auto it : ip)
    {
        ethx["addresses"].push_back(it);
    }

    ethx["gateway4"] = gw;

    for(auto it : dns)
    {
        ethx["nameservers"]["addresses"].push_back(it);
    }

    root["network"][tag][port] = ethx;

    std::cout << "--------### netplan ###---------" << std::endl;
    std::cout << root << std::endl;

    std::ofstream f(m_file);
    if(f)
    {
        f << root << "\n";
        return true;
    }
    return false;
YAML_TRY_END
    std::cout << "[netplan,config], yaml error!" << std::endl;
    return false;
}

bool netplan::eth_del(string port)
{
    std::string tag = get_tag(port);
    if(tag.empty())
    {
        return false; // tag is unknown!
    }

YAML_TRY_START
    YAML::Node root = YAML::LoadFile(m_file);

    if(!root["network"])
    {
        return false;
    }
   
    if(root["network"][tag][port])
    {
        root["network"][tag].remove(port);
    }

     if(root["network"][tag].IsMap())
    {
        if(root["network"][tag].size() == 0)
        {
            root["network"].remove(tag);
        }
    }

    std::cout << "--------### netplan ###---------" << std::endl;
    std::cout << root << std::endl;

    std::ofstream f(m_file);
    if(f)
    {
        f << root << "\n";
        return true;
    }
    return false;
YAML_TRY_END
    std::cout << "[netplan,config], yaml error!" << std::endl;
    return false;
}

bool netplan::wifi_add(string port, string ssid, string psk)
{
    std::string tag = get_tag(port);
    if(tag.empty())
    {
        return false; // tag is unknown!
    }

YAML_TRY_START
    YAML::Node root = YAML::LoadFile(m_file);

    if(!root["network"])
    {
        return false;
    }

    YAML::Node wifix;
    YAML::Node info;

    wifix["dhcp4"] = "yes";
    wifix["access-points"][ssid]["password"] = psk;

    root["network"][tag][port] = wifix;

    std::cout << "--------### netplan ###---------" << std::endl;
    std::cout << root << std::endl;

    std::ofstream f(m_file);
    if(f)
    {
        f << root << "\n";
        return true;
    }
    return false;
YAML_TRY_END
    std::cout << "[netplan,config], yaml error!" << std::endl;
    return false;
}

bool netplan::wifi_del(string port)
{
    return netplan::eth_del(port);
}

bool netplan::is_dhcp(string port)
{
   std::string tag = get_tag(port);
    if(tag.empty())
    {
        return false; // tag is unknown!
    }

YAML_TRY_START
    YAML::Node root = YAML::LoadFile(m_file);

    if(!root["network"])
    {
        return false;
    }

    if(root["network"][tag][port]["dhcp4"])
    {
        return true;
    }

    return false;
YAML_TRY_END
    std::cout << "[netplan,config], yaml error!" << std::endl;
    return false;
}

bool netplan::apply(void)
{
    std::string cmd = "killall wpa_supplicant";
    system(cmd.c_str());

    cmd = "netplan apply";
    system(cmd.c_str());
    return true;
}

interface::interface(std::string port)
{
    m_port = port;
}

interface::~interface()
{
}



} // namespace network
