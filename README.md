# About this Program

As with a lot name descisions, I took the elitist route and chose one that derives from the latin word for interconnected.  
Of course, this program aims to do just that. Every computer than runs the program can connect to another computer running the program.  
All the computers connected to each other is called the cluster and each computer is called a node.  
The program can connect a node to any ip address another node is capable of connecting to.  
It can be seen as one step deeper than a virtual lan as it can connect internal ip addresses not broascasted on a node's private lan.  

# Program State

As of now, coniuncti is in its very early stages and is not suitable for wide use.  

# Program Build

Coniuncti is currently a unix only program.  
To build coniuncti, one must simply have the git and make tools and then run:  
```
git clone https://github.com/i-agrawal/coniuncti.git
cd coniuncti
make
```
The executable will be called `coniuncti`
