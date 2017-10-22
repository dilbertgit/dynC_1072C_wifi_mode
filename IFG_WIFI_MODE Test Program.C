#class auto
#use "RCM66xxW.lib"

#define TCPCONFIG 6

#define NET_VERBOSE

#define USE_DHCP
#define WIFI_USE_WPA
#define WIFI_AES_ENABLED

#define IFC_WIFI_SSID "defaultSSID"

#memmap xmem
#use "dcrtcp.lib"

void delay(unsigned long cnt1)
{
  unsigned long intDELAY;
  intDELAY = MS_TIMER;
  while( (intDELAY+cnt1) > MS_TIMER);
}

void main(void)
{
	int 			x;
  word			timer;
  longword 	defaultGateway;
  char 			defaultGatewayStr[16];
  int				wifiMode;

  // initialize the TCP/IP stack
  sock_init();

  // initialize WIFI interface
  ifconfig(IF_WIFI0,
    IFS_DOWN,                    	// bring down WiFi0
    IFS_END);

  ifconfig(IF_WIFI0,
    IFS_DHCP,                     0,
    IFS_END);

  ifconfig(IF_WIFI0,
    IFS_DHCP_TIMEOUT,             60,
    IFS_IPADDR,                   aton("192.168.10.100"),
    IFS_NETMASK,                  aton("255.255.255.0"),
    IFS_ROUTER_SET,               aton("192.168.10.1"),
    IFS_WIFI_SSID,                strlen("defaultSSID"), "defaultSSID",
    IFS_WIFI_WPA_PSK_PASSPHRASE,  "defaultpass",
    IFS_WIFI_REGION,              IFPARAM_WIFI_REGION_AMERICAS,
    IFS_WIFI_MODE,                IFPARAM_WIFI_INFRASTRUCTURE,
    IFS_WIFI_CHANNEL,             0,
    IFS_WIFI_AUTHENTICATION,      IFPARAM_WIFI_AUTH_WPA_PSK,
    IFS_WIFI_ENCRYPTION,          IFPARAM_WIFI_ENCR_CCMP,
    IFS_WIFI_TX_POWER,            15,
   	IFS_END);

	for (x = 0; x < 10; x++)
  {
		// bring up the WiFi interface
    printf(" Bringing up the WiFi interface, this may take up to 30 seconds... \n");
    ifconfig(IF_WIFI0,
      IFS_UP,
      IFS_END);

    ifconfig(IF_WIFI0,
      IFS_DHCP,                     0,
      IFS_END);

	  ifconfig(IF_WIFI0,
	    IFS_DHCP_TIMEOUT,             60,
	    IFS_IPADDR,                   aton("192.168.10.100"),
	    IFS_NETMASK,                  aton("255.255.255.0"),
	    IFS_ROUTER_SET,               aton("192.168.10.1"),
    	IFS_WIFI_SSID,                strlen("defaultSSID"), "defaultSSID",
    	IFS_WIFI_WPA_PSK_PASSPHRASE,  "defaultpass",
	    IFS_WIFI_REGION,              IFPARAM_WIFI_REGION_AMERICAS,
	    IFS_WIFI_MODE,                IFPARAM_WIFI_INFRASTRUCTURE,
	    IFS_WIFI_CHANNEL,             0,
	    IFS_WIFI_AUTHENTICATION,      IFPARAM_WIFI_AUTH_WPA_PSK,
	    IFS_WIFI_ENCRYPTION,          IFPARAM_WIFI_ENCR_CCMP,
	    IFS_WIFI_TX_POWER,            15,
	    IFS_END);

    timer = _SET_SHORT_TIMEOUT(30000);
    while (ifpending(IF_WIFI0) != IF_UP)
    {
      if (_CHK_SHORT_TIMEOUT(timer))
      {
        printf(" failed! \n");
        printf(" Check the connection settings and try again! \n");
        break;
      }

      tcp_tick(NULL);
    }

    if (ifpending(IF_WIFI0) == IF_UP)
      printf(" success! \n");

    // print the wifi mode
    ifconfig(IF_WIFI0, IFG_WIFI_MODE, &wifiMode, IFS_END);
    printf(" WIFI MODE: %i \n", wifiMode);

    // delay
    delay(2000);

    // take the interface back down
    printf(" Bringing down the WiFi interface, please wait...");
    ifconfig(IF_WIFI0, IFS_DOWN, IFS_END);

    timer = _SET_SHORT_TIMEOUT(5000);
    while (ifpending(IF_WIFI0) != IF_DOWN)
    {
      if (_CHK_SHORT_TIMEOUT(timer))
      {
        printf(" failed! exiting... \n");
        exit(1);
      }

      tcp_tick(NULL);
    }

    if (ifpending(IF_WIFI0) == IF_DOWN)
      printf(" success! \n");

    // print the wifi mode
    ifconfig(IF_WIFI0, IFG_WIFI_MODE, &wifiMode, IFS_END);
    printf(" WIFI MODE: %i \n", wifiMode);

    // delay
    delay(2000);
  }
}

