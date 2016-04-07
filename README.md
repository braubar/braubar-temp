# braubar-temp

Temperature sensor for Braubar. Needs to be installed on a Arduino Uno with Ethernet Shield 2. Then it sends every second a temperature value and its id as a UDP-Broadcast-Package on port 50505 to the local network.

To upload braubar-temp  to your Arduino first build it and then upload it. 

```bash
make 
make upload
```

