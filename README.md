# prfsswitch
This is a program for the FAT32PRFS file system, see [FAT32PRFS](https://github.com/elbojvv/fat32prfs).

A LED, with a resistor of 330 Ohm, is connected to GPIO23 and ground. A 3 way switch, with a protective resistor of 330 Ohm, is connected to GPIO 24 and 25 and ground (see file Switch_schematics.jpg):
- In the normal mode GPIO 24 is connected to ground.
- In the rPRFS mode GPIO 25 is connected to ground.
- No pins are connected to ground in read only mode.


The program reads the switch, controls the LED and sends the status to /proc/prfs_mode.

 # Compiling
The program needs the pigpio library which is installed with Raspberry Pi OS. The program can be compiled with:
```
make
```

# Installing 
The program should be started as super user:
```
sudo prfs_switch &
```
