# About this Program

As with a lot name descisions, I took the elitist route and chose one that derives from the latin word for interconnected.  
Of course, this program aims to do just that.  
A computer than runs coniuncti can connect to another computer running coniuncti.  
A set of computers connected to each other is called the cluster and each computer in the cluster is called a node.  
The program can connect a node to any ip address another node is capable of connecting to.  
It can be seen as one step deeper than a virtual lan as it can connect internal ip addresses not broascasted on a node's private lan.  
Using rules given by the user, coniuncti will decide what ips should be shared.  
Because it only shares ips defined by the user, the process is lightweight, fast, and flexible.  

# Program State

As of now, coniuncti is in its very early stages and is not suitable for wide use.  
Coniuncti currently supports unix only but as this is early stages it may grow to include more platforms.  

# Program Build

To build coniuncti, one must simply have the git and make tools and then run:  
```
git clone https://github.com/i-agrawal/coniuncti.git
cd coniuncti
make
```
The executable will be called `coniuncti`
