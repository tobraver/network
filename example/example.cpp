#include <iostream>
#include "../network.h"

int main(int argc, char const *argv[])
{
    network::netplan net("99_config.yaml");

    if(net.is_dhcp("eth0"))
    {
        printf("net is dhcp!\n");
    }
    else {
        printf("net is static!\n");

    }
    return 0;
}

