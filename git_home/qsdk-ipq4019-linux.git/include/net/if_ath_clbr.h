#ifndef CLIENTBRIDGE_H
#define CLIENTBRIDGE_H

#include <linux/if_arp.h>
#include <linux/ip.h>
#include <net/ipv6.h>
#include <net/ip6_checksum.h>
#include <net/addrconf.h>
#include <linux/icmpv6.h>
#include <linux/udp.h>
#include <linux/skbuff.h>
#include <linux/if_ether.h>
#include <linux/inetdevice.h>
#include <asm/byteorder.h>
#include <net/arp.h>
#include <linux/version.h>
#include "clbr_queue.h"

/*stone added macro for vlan passthrough*/
//#define VLAN_PASSTHROUGH
/* Constants: */
/* Number of maximum learned entries ('source IP - source MAC'
 * association learned from outgoing ARP request/reply messages) value
 * must be in power of two. This value is used in hashing function to
 * achieve modulo functionality using bit wise 'and' operation.
 */
#define CLBR_MAX_LEARNED_ENTRIES    128
#define SUPPORT_MAX_ETHER_CLIENTS   32

/* Number of maximum tracked entries ('target IP - source MAC
 * association' learned from outgoing ARP probe request messages)
 * value must be in power of two. This value is used to perform modulo
 * using bit wise 'and' operation.
 */
#define CLBR_MAX_TRACKED_ENTRIES      32

/* Offset (in bytes) of Ethernet,ARP and IP packet fields in ethernet
 * 802.3 frame 
 */
#define CLBR_ETH_DST_ADDR_OFFSET             0
#define CLBR_ETH_SRC_ADDR_OFFSET             6
#define CLBR_ETH_TYPE_OFFSET                12
#define CLBR_IP_SRC_ADDR_OFFSET             26
#define CLBR_IP_DST_ADDR_OFFSET             30
#define CLBR_IPV6_SRC_ADDR_OFFSET		22
#define CLBR_IPV6_DST_ADDR_OFFSET 	38
#define CLBR_ARP_OPCODE_OFFSET              20
#define CLBR_ARP_SENDER_HW_ADDR_OFFSET      22
#define CLBR_ARP_SENDER_PROT_ADDR_OFFSET    28
#define CLBR_ARP_TARGET_HW_ADDR_OFFSET      32
#define CLBR_ARP_TARGET_PROT_ADDR_OFFSET    38
#define CLBR_ICMPV6_TARGET_ADDR_OFFSET	62

#ifdef IPV6_PASSTHROUGH
#define CLBR_NDISC_ROUTER_SOLICITATION 133
#define CLBR_NDISC_NEIGHBOUR_SOLICITATION	135
#define CLBR_NDISC_NEIGHBOUR_ADVERTISEMENT	136
#define CLBR_ICMPV6_ECHO_REQUEST		128
#define CLBR_ICMPV6_ECHO_REPLY		129
#define CLBR_ICMPV6_MULTICAST_LISTENER_REPORT 131
#define CLBR_ICMPV6_ROUTER_LINKLAYER_ADDR 64
#define CLBR_ICMPV6_NEIGH_LINKLAYER_ADDR 80
#define CLBR_ICMPV6_NEIGH_OPT_TYPE	78
//pseduo header for ipv6
/*typedef struct sIpv6ph
{
	struct in6_addr saddr;
	struct in6_addr daddr;
	__u32  payload_len;
	__u8   zero[3];
	__u8   nexthdr;	
}tIpv6ph;*/
#endif

/*
 * ARM Endian-ness.  In little endian mode, the data bus is connected such
 * that byte accesses appear as:
 * 0 = d0...d7, 1 = d8...d15, 2 = d16...d23, 3 = d24...d31
 * and word accesses (data or instruction) appear as:
 * d0...d31
 *
 * When in big endian mode, byte accesses appear as:
 * 0 = d24...d31, 1 = d16...d23, 2 = d8...d15, 3 = d0...d7
 * and word accesses (data or instruction) appear as:
 * d0...d31
 */
#ifdef __BIG_ENDIAN
//i.e BIG_ENDIAN
/* Protocols that are supported in client bridge */
#define CLBR_ETH_TYPE_IP_NET_ORDER      0x0800
#define CLBR_ETH_TYPE_IPV6_NET_ORDER    0x86dd
#define CLBR_ETH_TYPE_ARP_NET_ORDER	0x0806
#ifdef VLAN_PASSTHROUGH
#define CLBR_VLAN_ETH_TYPE_IP_NET_ORDER      0x8100
#endif

#define CLBR_ETH_TYPE_IPX_NET_ORDER	0x8137
#define CLBR_ETH_TYPE_AT_NET_ORDER	0x809B /* Apple Talk */

#define CLBR_ETH_TYPE_BOOTPC            0x0044
#define CLBR_ETH_TYPE_BOOTPS            0x0043

#define CLBR_ETH_TYPE_BOOTP_BBIT        0x8000

/* ARP Request and reply opcodes */
#define CLBR_ARP_OP_REQUEST_NET_ORDER   0x0001
#define CLBR_ARP_OP_REPLY_NET_ORDER     0x0002

#else
//i.e LITTLE_ENDIAN
/* Protocols that are supported in client bridge */
#define CLBR_ETH_TYPE_IP_NET_ORDER      0x0008
#define CLBR_ETH_TYPE_IPV6_NET_ORDER      0xdd86
#define CLBR_ETH_TYPE_ARP_NET_ORDER     0x0608
#ifdef VLAN_PASSTHROUGH
#define CLBR_VLAN_ETH_TYPE_IP_NET_ORDER      0x0081
#endif

#define CLBR_ETH_TYPE_IPX_NET_ORDER	0x3781
#define CLBR_ETH_TYPE_AT_NET_ORDER	0x9B80 /* Apple Talk */

#define CLBR_ETH_TYPE_BOOTPC            0x4400
#define CLBR_ETH_TYPE_BOOTPS            0x4300

#define CLBR_ETH_TYPE_BOOTP_BBIT        0x0080

/* ARP Request and reply opcodes */
#define CLBR_ARP_OP_REQUEST_NET_ORDER   0x0100
#define CLBR_ARP_OP_REPLY_NET_ORDER     0x0200
#endif

/* IBM RPL/POS based on Novell 802.2 */
#define RPL_ETH_8022_OFFSET				17
#define RPL_BROADCAST_ADDR				0x030002000000
//#define RPL_FIND						0x0001
//#define RPL_FOUND						0x0002
//#define RPL_SENDFILE					0x0010
#define RPL_EC_2x5_2x6_SUREPOS			"AMD 10-100"
#define RPL_EC_1x4_2x44					"DE220"
#define RPL_EC_2x5_2x6_SUREPOS_LEN		10
#define RPL_EC_1x4_2x44_LEN				5
#define RPL_EC_2x5_2x6_SUREPOS_TO		"SYMBOL0100"
#define RPL_EC_1x4_2x44_TO				"SYMBOL0200"
#define RPL_EC_2x5_2x6_SUREPOS_TO_LEN	10
#define RPL_EC_1x4_2x44_TO_LEN			10

/* Macro */
/* Increments ARP probe tracking buffer index. Incremented index will
 * always be in range of 0 to CLBR_MAX_TRACKED_ENTRIES.
 */
#define CLBR_INCR_TRACK_BUF_INDEX(nIndex) \
        (nIndex) = ((nIndex) + 1) & (~CLBR_MAX_TRACKED_ENTRIES)

#ifdef CLBR_NDEBUG
#  define assert(expr) do {} while (0)
#else
#  define assert(expr) \
        if(!(expr)) {					\
        printk( "Assertion failed! %s,%s,%s,line=%d\n",	\
        #expr,__FILE__,__FUNCTION__,__LINE__);		\
        }
#endif

/* Copy MAC value character by character (size 6) */
#define CLBR_COPY_MAC(aDestMac, aSrcMac)                               \
{                                                                      \
	*((unsigned char *)(aDestMac)) = *((unsigned char *)(aSrcMac));    \
	*((unsigned char *)(aDestMac)+1) = *((unsigned char *)(aSrcMac)+1);\
	*((unsigned char *)(aDestMac)+2) = *((unsigned char *)(aSrcMac)+2);\
	*((unsigned char *)(aDestMac)+3) = *((unsigned char *)(aSrcMac)+3);\
	*((unsigned char *)(aDestMac)+4) = *((unsigned char *)(aSrcMac)+4);\
	*((unsigned char *)(aDestMac)+5) = *((unsigned char *)(aSrcMac)+5);\
}

#ifdef __BIG_ENDIAN
//i.e BIG_ENDIAN

/* Copy integer value character by character(size 4) */
#define CLBR_COPY_INT(pDestInt, pSrcInt)             \
{                                                    \
	*((unsigned char *)(pDestInt)) = *((unsigned char *)(pSrcInt));    \
	*((unsigned char *)(pDestInt)+1) = *((unsigned char *)(pSrcInt)+1);\
	*((unsigned char *)(pDestInt)+2) = *((unsigned char *)(pSrcInt)+2);\
	*((unsigned char *)(pDestInt)+3) = *((unsigned char *)(pSrcInt)+3);\
}

/* Get short value character by character(size 2) */
#define CLBR_COPY_SHORT(pDestShort, pSrcShort)           \
{                                                        \
	*((unsigned char *)(pDestShort)) = *((unsigned char *)(pSrcShort));    \
	*((unsigned char *)(pDestShort)+1) = *((unsigned char *)(pSrcShort)+1);\
}
#else
//i.e. LITTLE_ENDIAN

/* Copy integer value character by character(size 4) */
#define CLBR_COPY_INT(pDestInt, pSrcInt)             \
{                                                    \
	*((unsigned char *)(pDestInt)+3) = *((unsigned char *)(pSrcInt));  \
	*((unsigned char *)(pDestInt)+2) = *((unsigned char *)(pSrcInt)+1);\
	*((unsigned char *)(pDestInt)+1) = *((unsigned char *)(pSrcInt)+2);\
	*((unsigned char *)(pDestInt)) = *((unsigned char *)(pSrcInt)+3);  \
}

/* Get short value character by character(size 2) */
#define CLBR_COPY_SHORT(pDestShort, pSrcShort)           \
{                                                        \
	*((unsigned char *)(pDestShort)+1) = *((unsigned char *)(pSrcShort));\
	*((unsigned char *)(pDestShort)) = *((unsigned char *)(pSrcShort)+1);\
}
#endif

typedef struct in6_addr tIn6_addr;

/* Structures: */
/* IP MAC association structure */
typedef struct sIpMacAssociation
{
   u_int32_t        u32Ip;
   tIn6_addr	ipv6addr;
   unsigned char    acMac[ETH_ALEN];
   unsigned char    pad[2];
   /* Record the traffic time */
   u_int32_t        tstamp;
   /* Used to record the free index in the pool */
   int		    myFreeIndex;
   /* Marked it as preferred ether client or not */
   int		    preferred;
}tIpMacAssociation;

/* IP MAC association link list node */
typedef struct sIpMacAssociationNode
{
    struct sIpMacAssociationNode    *psNext;
    tIpMacAssociation               sIpMac;
    u_int32_t                       routerIP;
    u_int32_t                       transid;
    int                             dhcprenew_flag;
}tIpMacAssociationNode;

#define ETHERNET_II_PROTOCOL_NUM_MIN   0x05F7
/*
 *  Following list the Ethernet headers for supporting IP and IPX packets.
 */
struct EthernetIIhdr
{
    unsigned char  dst[6];
    unsigned char  src[6];
    unsigned short proto;
}__attribute__((packed));

struct Novellwo8022
{
    unsigned char  dst[6];
    unsigned char  src[6];
    unsigned short length;
    unsigned short chksum;    
}__attribute__((packed));

struct Novellw8022
{
    unsigned char  dst[6];
    unsigned char  src[6];
    unsigned short length;
    unsigned char  dsap;
    unsigned char  ssap;
    unsigned char  ctrl;
}__attribute__((packed));

struct NovellwSnap
{
    unsigned char  dst[6];
    unsigned char  src[6];
    unsigned short length;
    unsigned char  dsap;
    unsigned char  ssap;
    unsigned char  ctrl;
    unsigned char  orgcode[3];
    unsigned short type;
}__attribute__((packed));

typedef union uEthernetFrameFormat
{
    struct EthernetIIhdr *eIIhdr;
    struct Novellwo8022  *nvlwo8022;
    struct Novellw8022   *nvlw8022;
    struct NovellwSnap   *nvlwsnap;
    unsigned char *raw;
}EthernetFrameFormat;

# define DHCP_OPTIONS_BUFSIZE	308
# define SLACK_FOR_BUGGY_SERVERS	80

enum {
        OPTION_IP=1,
        OPTION_IP_PAIR,
        OPTION_IP_COMP,
        OPTION_STRING,
        OPTION_BOOLEAN,
        OPTION_U8,
        OPTION_U16,
        OPTION_S16,
        OPTION_U32,
        OPTION_S32
};

#define OPTION_REQ      0x10 /* have the client request this option */
#define OPTION_LIST     0x20 /* There can be a list of 1 or more of these */

struct dhcp_option {
        char name[16];
        char flags;
        unsigned char code;
};

struct dhcp_option options[] = {
        /* name[16]     flags                                   code */
        {"subnet",      OPTION_IP | OPTION_REQ,                 0x01},
        {"timezone",    OPTION_S32,                             0x02},

#ifdef RFC3442_121_SUPPORT
        /* RFC 3442: The Classless Static Routes option code MUST appear in the
         * parameter request list prior to both the Router option code and the
         * Static Routes option code, if present. */
        {"csroute",     OPTION_IP_COMP | OPTION_REQ,            0x79},
#endif
        {"router",      OPTION_IP | OPTION_LIST | OPTION_REQ,   0x03},
        {"timesvr",     OPTION_IP | OPTION_LIST,                0x04},
        {"namesvr",     OPTION_IP | OPTION_LIST,                0x05},
        {"dns",         OPTION_IP | OPTION_LIST | OPTION_REQ,   0x06},
        {"logsvr",      OPTION_IP | OPTION_LIST,                0x07},
        {"cookiesvr",   OPTION_IP | OPTION_LIST,                0x08},
        {"lprsvr",      OPTION_IP | OPTION_LIST,                0x09},
        {"hostname",    OPTION_STRING | OPTION_REQ,             0x0c},
        {"bootsize",    OPTION_U16,                             0x0d},
        {"domain",      OPTION_STRING | OPTION_REQ,             0x0f},
        {"swapsvr",     OPTION_IP,                              0x10},
        {"rootpath",    OPTION_STRING,                          0x11},
        {"ipttl",       OPTION_U8,                              0x17},
        {"mtu",         OPTION_U16,                             0x1a},
        {"broadcast",   OPTION_IP | OPTION_REQ,                 0x1c},
#ifdef RFC2132_33_SUPPORT
       {"sroute",      OPTION_IP_PAIR | OPTION_REQ,            0x21},
#endif
        {"ntpsrv",      OPTION_IP | OPTION_LIST,                0x2a},
#ifdef SUPPORT_OPTION_43
        {"vendor_specific",     OPTION_STRING | OPTION_REQ,     0x2b},
#endif
        {"wins",        OPTION_IP | OPTION_LIST,                0x2c},
        {"requestip",   OPTION_IP,                              0x32},
        {"lease",       OPTION_U32,                             0x33},
        {"dhcptype",    OPTION_U8,                              0x35},
        {"serverid",    OPTION_IP,                              0x36},
        {"message",     OPTION_STRING,                          0x38},
        {"tftp",        OPTION_STRING,                          0x42},
        {"bootfile",    OPTION_STRING,                          0x43},
        {"",            0x00,                           0x00}
};


/* Lengths of the different option types */
int option_lengths[] = {
        [OPTION_IP] =           4,
        [OPTION_IP_PAIR] =      8,
        [OPTION_IP_COMP] =      5,
        [OPTION_BOOLEAN] =      1,
        [OPTION_STRING] =       1,
        [OPTION_U8] =           1,
        [OPTION_U16] =          2,
        [OPTION_S16] =          2,
        [OPTION_U32] =          4,
        [OPTION_S32] =          4
};

/* miscellaneous defines */
#define MAC_BCAST_ADDR          (unsigned char *) "\xff\xff\xff\xff\xff\xff"
#define OPT_CODE 0
#define OPT_LEN 1
#define OPT_DATA 2

/* DHCP option codes (partial list) */
#define DHCP_PADDING            0x00
#define DHCP_SUBNET             0x01
#define DHCP_TIME_OFFSET        0x02
#define DHCP_ROUTER             0x03
#define DHCP_TIME_SERVER        0x04
#define DHCP_NAME_SERVER        0x05
#define DHCP_DNS_SERVER         0x06
#define DHCP_LOG_SERVER         0x07
#define DHCP_COOKIE_SERVER      0x08
#define DHCP_LPR_SERVER         0x09
#define DHCP_HOST_NAME          0x0c
#define DHCP_BOOT_SIZE          0x0d
#define DHCP_DOMAIN_NAME        0x0f
#define DHCP_SWAP_SERVER        0x10
#define DHCP_ROOT_PATH          0x11
#define DHCP_IP_TTL             0x17
#define DHCP_MTU                0x1a
#define DHCP_BROADCAST          0x1c
#define DHCP_NTP_SERVER         0x2a

#ifdef SUPPORT_OPTION_43
#define DHCP_VENDOR_SPECIFIC    0x2b
#endif

#define DHCP_WINS_SERVER        0x2c
#define DHCP_REQUESTED_IP       0x32
#define DHCP_LEASE_TIME         0x33
#define DHCP_OPTION_OVER        0x34
#define DHCP_MESSAGE_TYPE       0x35
#define DHCP_SERVER_ID          0x36
#define DHCP_PARAM_REQ          0x37
#define DHCP_MESSAGE            0x38
#define DHCP_MAX_SIZE           0x39
#define DHCP_T1                 0x3a
#define DHCP_T2                 0x3b
#define DHCP_VENDOR             0x3c
#define DHCP_CLIENT_ID          0x3d

#define DHCP_END                0xFF


#define BOOTREQUEST             1
#define BOOTREPLY               2

#define ETH_10MB                1
#define ETH_10MB_LEN            6

#define DHCPDISCOVER            1
#define DHCPOFFER               2
#define DHCPREQUEST             3
#define DHCPDECLINE             4
#define DHCPACK                 5
#define DHCPNAK                 6
#define DHCPRELEASE             7
#define DHCPINFORM              8

#define BROADCAST_FLAG          0x8000


#define OPTION_FIELD            0
#define FILE_FIELD              1
#define SNAME_FIELD             2

#define IS_TX 0
#define IS_RX 1
#define BEFORE_HACK 0
#define AFTER_HACK 1

typedef struct sBootpHeader
{
    u_int8_t opcode;
    u_int8_t hw_type;
    u_int8_t hwaddr_len;
    u_int8_t hop_count;
    u_int32_t trans_id;
    u_int16_t num_sec;
    u_int16_t flags;
    u_int32_t client_ip;
    u_int32_t your_ip;
    u_int32_t sv_ip;
    u_int32_t gw_ip;
    u_int8_t chaddr[16];
    u_int8_t sname[64];
    u_int8_t file[128];
    u_int32_t cookie;
    u_int8_t options[DHCP_OPTIONS_BUFSIZE + SLACK_FOR_BUGGY_SERVERS]; /* 312 - cookie */
}BootpHeader;

struct IBM_RPL
{
    unsigned char  PROGRAM_LENGTH[2];
    unsigned char  PROGRAM_COMMAND[2];
    unsigned char  CORR_HDR[4];
    unsigned char  CORRELATOR[4];
    unsigned char  INFO_HDR[4];
    unsigned char  FRAME_HDR[4];
    unsigned char  MAX_FRAME[2];
    unsigned char  CLASS_HDR[4];
    unsigned char  CONN_CLASS[2];
    unsigned char  SOURCE_HDR[4];
    unsigned char  SOURCE_ADDR[6]; //Requesting Devicve MAC address
    unsigned char  LSAP_HDR[4];
    unsigned char  RSAP[1];
    unsigned char  SEARCH_HDR[4];
    unsigned char  LOADER_HDR[4];
    unsigned char  MACH_CONF[8];
    unsigned char  EQUIP_FLAGS[2];
    unsigned char  MEMORY_SIZE[2];
    unsigned char  REMOTE_PROGRAM_LOAD_EC[8];
    unsigned char  ADAPTER_ID[2];
    unsigned char  ADAPTER_EC[10];
}__attribute__((packed));

/* Client bridge context structure contains hash table. Hash table is
 * array of link lists. The alternative data structure that could be
 * used for hash table was one dimension array. The reasons for using
 * array of link list based hash table are discussed below.
 * One dimension array based hash table is a good option only when
 * there is surety that no collision will occur. If there is collision
 * then addition operation becomes very costly and complex affair as
 * in worst case a free location need to be searched in complete array
 * to add new entry. It makes hash table approach worse than linear
 * table where at the time of addition it is known that at what place
 * new entry will be added. Similarly after collision it is the linear
 * search that is used to find the desired entry which is the same
 * scheme that is used in linear table.
 * Array of link list based hash table is a good option in all cases.
 * Array index is generated using hashing function and entry will be
 * added in the link list whose head pointer is stored at that
 * location. In that case when there is no collision all link lists
 * will contain single entry. If collision occurs then new entry will
 * be added at head of the link list. This operation costs same in both
 * cases (collision and non collision). At the time of search maximum
 * it need to traverse complete link list to find desired entry. 
 * In typical case there would be more than one link lists in the hash
 * table so entries would be distributed among all link lists. Hence
 * in typical case when there is collision, searching operation will
 * always cost lower in array of link list based approach than one
 * dimension array based approach where it could be the searching of
 * complete table.
 */

#ifndef INDEX_BIT
#define INDEX_BIT(x) (1 << (x)) 
#endif

#define FREE_INDEX_00   INDEX_BIT(0) //1
#define FREE_INDEX_01   INDEX_BIT(1) //2
#define FREE_INDEX_02   INDEX_BIT(2) //4
#define FREE_INDEX_03   INDEX_BIT(3) //8
#define FREE_INDEX_04   INDEX_BIT(4) //16
#define FREE_INDEX_05   INDEX_BIT(5) //32
#define FREE_INDEX_06   INDEX_BIT(6) //64
#define FREE_INDEX_07   INDEX_BIT(7)//128
#define FREE_INDEX_08   INDEX_BIT(8) //256
#define FREE_INDEX_09   INDEX_BIT(9) //512
#define FREE_INDEX_10   INDEX_BIT(10) //1024
#define FREE_INDEX_11   INDEX_BIT(11) //2048
#define FREE_INDEX_12   INDEX_BIT(12) //4096
#define FREE_INDEX_13   INDEX_BIT(13) //8192
#define FREE_INDEX_14   INDEX_BIT(14) //16384
#define FREE_INDEX_15   INDEX_BIT(15) //32768
#define FREE_INDEX_ALL  0xffffffff

struct proc_dir_entry {
	unsigned int low_ino;
	umode_t mode;
	nlink_t nlink;
	kuid_t uid;
	kgid_t gid;
	loff_t size;
	const struct inode_operations *proc_iops;
	const struct file_operations *proc_fops;
	struct proc_dir_entry *next, *parent, *subdir;
	void *data;
	atomic_t count;		/* use count */
	atomic_t in_use;	/* number of callers into module in progress; */
			/* negative -> it's going away RSN */
	struct completion *pde_unload_completion;
	struct list_head pde_openers;	/* who did ->open, but not ->release */
	spinlock_t pde_unload_lock; /* proc_fops checks and pde_users bumps */
	u8 namelen;
	char name[];
};

/* Client bridge context structure. */
typedef struct sClbrCtxt
{
  /* Array of IP-MAC association structures that forms a pool.*/
   tIpMacAssociationNode asIpMacPool[SUPPORT_MAX_ETHER_CLIENTS];

   /* Index of next free IP-MAC association node in pool. */
   int                   nFreeIndex;
   
   /* Number of ethernet clients */
   int			 nClients;
   
   /* Index bit of free IP-MAC association node in pool. */
   u_int32_t	 	 nFreeIndexBit;
   
   /* Array of pointers that shall be used for making hash table to 
    * store learned IP-MAC associations.
    */
   tIpMacAssociationNode *apHashTbl[CLBR_MAX_LEARNED_ENTRIES];

   /* Array that shall be used as circular buffer to track ARP probe
    * request
    */
   tIpMacAssociation     asTrackBuf[CLBR_MAX_TRACKED_ENTRIES];
   
   /* Index of next available entry in circular buffer */
   int                   nTrackBufIndex;

   /* Own Router IP address */
   u_int32_t             own_routerIP;

   /* Wireless port MAC address */
   unsigned char	     acWirelessMac[ETH_ALEN];
   unsigned char	     pad[2];
}tClbrCtxt;

#define MAC_ADDR_EQ(a1,a2)    (memcmp(a1,a2,ETH_ALEN) == 0)

struct clbr_acl_entry {
    TAILQ_ENTRY(clbr_acl_entry) clbr_acl_list;
    u_int8_t mac_addr[ETH_ALEN];
};

typedef struct sclbr_obj{
    tClbrCtxt *pCtxt;
    struct net_device *dev;
    rwlock_t clbr_acl_lock;
    TAILQ_HEAD(, clbr_acl_entry) clbr_acl;
    struct timer_list clbr_acl_timo;
}clbr_obj_t;

/* This function allocates memory for context structure and initializes
 * client bridge library instance. It returns context structure pointer
 * to caller.
 *
 * pMac: pointer to wireless port MAC. Client bridge uses this MAC 
 * address for address translation of incoming and outgoing traffic.
 *
 * return: pointer to client bridge context structure
 */
void* ath_cb_attach(char *pcMac);

/* This function deallocates client bridge context structure memory.
 *
 * ppsCtxt: pointer to pointer to client bridge context structure. 
 */
void ath_cb_detach(void** ppvCtxt);

/* This function gets invoked for outgoing traffic. It learns IP-MAC
 * association from outgoing ARP request/reply messages, tracks ARP 
 * probe requests using tracking database and translates outgoing frames
 * by substituting source MAC address with wireless port MAC. It 
 * modifies "Sender Hardware Address" field of outgoing ARP messages by
 * copying wireless port MAC in this field.
 * 
 * pvCtxt: pointer to client bridge context structure
 * skb: pointer to buffer that keeps data to transmit.
 */
u_int8_t ath_cb_xmit(struct net_device *dev, struct sk_buff *skb);

/* This function gets invoked for incoming traffic. It looks for MAC
 * associated with destination IP for incoming ARP reply messages and
 * IP packets and translates these packets by substituting destination
 * MAC address with MAC address of real destination. It modifies
 * "Target Hardware Address" field of incoming ARP reply messages by
 * copying MAC address of real destination in this field. In case of
 * ARP probe response it looks for MAC associated with source IP in 
 * its tracking database and substitutes destination MAC address and
 * "Target Hardware Address" with MAC address of real destination. 
 * After translation of ARP probe response it removes IP-MAC entry 
 * (used in translation) from tracking database.
 * 
 * pvCtxt: pointer to client bridge context structure
 * skb: pointer to buffer that keeps received data.
 */
u_int8_t ath_cb_recv(struct net_device *dev, struct sk_buff *skb);

void ath_cb_replaceMac(void *pvCtxt, uint8_t *data, int flg);

int ath_cb_read_ether(char *, int, void*, void*);

void ath_cb_ether_timeout_node(unsigned long);

struct ieee80211com;

int ath_cb_check_preferred(clbr_obj_t*, struct sk_buff*);
#ifdef DHCP_BLOCK
int hack_bootp_packet(struct sk_buff*,tClbrCtxt* psCtxt);
#else
void hack_bootp_packet(struct sk_buff*,tClbrCtxt* psCtxt);
#endif

tIpMacAssociationNode*
ath_cb_deletelastnode(tIpMacAssociationNode **ppsList);
extern u_int8_t clbr_recv(struct net_device *dev, struct sk_buff *skb);
extern u_int8_t clbr_xmit(struct net_device *dev, struct sk_buff *skb);
extern int register_clbr_device(struct net_device *dev);
extern int unregister_clbr_device(struct net_device *dev);
extern void cb_force_clean_node(void);

#define IPV6_ADDR_TF(addr) ntohl((addr).s6_addr32[0]),   \
        ntohl((addr).s6_addr32[1]),                      \
        ntohl((addr).s6_addr32[2]),                      \
        ntohl((addr).s6_addr32[3])

#endif

#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,22))
#define MAC_HEADER(_skb)         ((_skb)->mac.raw)
#define MAC_HEADER_SRC(_skb)     (((struct ethhdr*)(_skb)->mac.raw)->h_source)
#define MAC_HEADER_DST(_skb)     (((struct ethhdr*)(_skb)->mac.raw)->h_dest)
#define MAC_HEADER_PROTO(_skb)   (((struct ethhdr*)(_skb)->mac.raw)->h_proto)
#define IP_HEADER(_skb)          ((_skb)->data + sizeof(struct ethhdr))
#define IP_HEADER_SRC(_skb)      ((_skb)->mac.raw + CLBR_IP_SRC_ADDR_OFFSET)
#define IP_HEADER_DST(_skb)      ((_skb)->mac.raw + CLBR_IP_DST_ADDR_OFFSET)
#define IPV6_HEADER_SRC(_skb)    ((_skb)->mac.raw + CLBR_IPV6_SRC_ADDR_OFFSET)
#define RESET_MAC_HEADER(_skb)   ((_skb)->mac.raw = (_skb)->data)
#define ARP_HEADER(_skb)         ((struct arphdr *)(_skb)->nh.arph)
#define DEV_GET_BY_NAME(_name)   (dev_get_by_name(_name))
#else  /* LINUX_VERSION_CODE < KERNEL_VERSION(2,6,22) */
#include <net/net_namespace.h>
#define MAC_HEADER(_skb)         (skb_mac_header(_skb))
#define MAC_HEADER_SRC(_skb)     (((struct ethhdr*)skb_mac_header(_skb))->h_source)
#define MAC_HEADER_DST(_skb)     (((struct ethhdr*)skb_mac_header(_skb))->h_dest)
#define MAC_HEADER_PROTO(_skb)   (((struct ethhdr*)skb_mac_header(_skb))->h_proto)
#define IP_HEADER(_skb)          (skb_network_header(_skb))
#define IP_HEADER_SRC(_skb)      (skb_mac_header(_skb) + CLBR_IP_SRC_ADDR_OFFSET)
#define IP_HEADER_DST(_skb)      (skb_mac_header(_skb) + CLBR_IP_DST_ADDR_OFFSET)
#define IPV6_HEADER_SRC(_skb)    (skb_mac_header(_skb) + CLBR_IPV6_SRC_ADDR_OFFSET)
#define RESET_MAC_HEADER(_skb)   (skb_reset_mac_header(_skb))
#define ARP_HEADER(_skb)         (arp_hdr(_skb))
#define DEV_GET_BY_NAME(_name)   (dev_get_by_name(&init_net,(_name)))
#endif  /* LINUX_VERSION_CODE < KERNEL_VERSION(2,6,22) */
