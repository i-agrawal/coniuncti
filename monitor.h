#ifndef __monitor_h__
#define __monitor_h__

//-------------------------STRUCTS-----------------------------
/*
  struct node
    -- ip: node's director's ip
       port: node's director's port
       subnet: node's subnet base
       mask: node's subnet mask
*/
struct
node
{
  unsigned int ip;
  unsigned short pport;
  unsigned short cport;
  unsigned int subnet;
  unsigned int mask;
};
/*
  struct proxy
    -- src: fd of requesting client
       dst: fd of our connection to target
*/
struct
proxy
{
  int src;
  int dst;
};
/*
  struct endpoint
    -- fd: listening on to redirect to endpoint
       ip: endpoint's local ip
       port: endpoint's local port
       nodeip: endpoint's node's ip
       nodeport: endpoint's node's port
*/
struct
endpoint
{
  int fd;
  unsigned int ip;
  unsigned int port;
  unsigned int nodeip;
  unsigned int nodeport;
};
/*
  struct monitor
    -- host: information about this node
       nodelist: list of nodes in cluster
       eplist: list of endpoints in cluster
       proxylist: list of proxies in cluster
       sockets: set of open file descriptors
       fdlock: lock used for atomic operation
*/
struct
monitor
{
  struct node* host;

  int nodes;
  int nodecap;
  struct node* nodelist;

  int endpoints;
  int epcap;
  struct endpoint* eplist;

  int proxies;
  int proxycap;
  struct proxy* proxylist;

  void* sockets;
  int fdlock;

  int running;
};
//-----------------------------END STRUCTS-----------------------------

//------------------------------FUNCTIONS------------------------------
/*
  func addnode
    -- purpose: create pointer to monitor
*/
struct monitor* monitorcreate();
/*
  func addnode
    -- purpose: add a node to monitor's node list
*/
void addnode(struct monitor*, unsigned int, unsigned short, unsigned int, unsigned int);
/*
  func delnode
    -- purpose: delete a node from monitor's node list
*/
void delnode(struct monitor*, int);
/*
  func addep
    -- purpose: add an endpoint to monitor's endpoint list
*/
void addep(struct monitor*, unsigned int, unsigned short);
/*
  func delep
    -- purpose: delete an endpoint from monitor's endpoint list
*/
void delep(struct monitor*, int);
//----------------------------END FUNCTIONS----------------------------


#endif
