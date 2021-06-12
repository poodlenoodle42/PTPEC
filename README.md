# PTPEC
Peer to Peer Encryptet Chat Client 

## What is it
A little project to get more familiar with sockets and C that is sophisticated that one could justify to use it in some rare circumstance. It is a little peer to peer chat client that encrypts all send text using AES-256 and the password specified.

## How to use it
```
PTPEC - Peer to peer encrypted chat client

Commandline options:
         -n / --nickname <Nickname>      Your Nickname in the conversasion
         -p / --port <Port>              The port used to connect to others and listend on for others to connect to you
         -a / --address <ip address>     Address of first peer to connect to. If omitted new session is started
         --password <pwd>                Password to protect the conversaion, if omitted you are promted for the password. 
         -h / --help                     Show this help message
```
## Known Problems
When having two peers connected to each other and a third joins using the ip address of the first one, than he is not able to connect to second one, when using the ip of the second peer every thing is fine. :man_shrugging: