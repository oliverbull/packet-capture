# packet-capture
Simple packet capture application 

Run this command to run without the need for su priviledges:
```
sudo setcap cap_net_admin,cap_net_raw=ep packetcapture
```
To run with gdb debugger, also apply this to gdb:
```
sudo setcap cap_net_admin,cap_net_raw=ep /usr/bin/gdb
```
Ensure you configure gdb to not run in shell mode:
```
-gdb-set startup-with-shell off
```
