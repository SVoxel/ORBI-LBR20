/* dniconfig.h for dni's macro definition.
 * If you have your own dniconfig.h in project for ntpclient,
 * you can replace with yours.
 */
#define NET_IFNAME              DGC_IF_WAN_IFNAME  /* The router's wan side interface name. */
#define PPP_IFNAME              DGC_IF_PPP_IFNAME
#define NET_MOBILE_IFNAME       DGC_IF_MOBILE_WAN_IFNAME  /* The router's wan side interface name for mobile network. */

#define NETGEAR_DAYLIGHT_SAVING_TIME
#define SUPPORT_ORBI_BASE

#define FIX_BUG_28601
#define WLAN_COMMON_SUUPPORT 1 //fix bug 34896
