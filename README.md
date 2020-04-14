# BMW E46 Oil

Unfortunately non ///M3 models of the BMW E46 series does not have oil temperature gauge
in the instrument cluster. On BMW forums a lot of guys ask for some way to see the
oil temperature, so I tried to come up with a solution.

If you are familiar with BMW diagnostic tools (e.g: INPA), you might have already
seen that for BMW E46 330i (MS 43.0 for M54) the oil temperature is available in the
live data view.
This means: it is possible to query this data, and make an android application
to visualize it real-time. We have sensor for oil temperature, not for the oil
pressure, just a switch.

![alt text](scripts/oilpics/1523787182.png?raw=true "INPA screenshot")

<a href="https://play.google.com/store/apps/details?id=com.tomicooler.bmw.e46oil">Available on Google Play</a>

I have made a new Real-Time Telemetry application for BMW E46 M3 owners, if you are interested check out that project
on my <a href="https://github.com/tomicooler/E46Track">GitHub</a>.

**How hard can it be?**

## General

This project is meant to be a guide about how BMW diagnostic protocol works on BMW E46,
and how to develop an application that uses the DS2 protocol to extract data from ECUs.

## Mythbusting

When I started this project I had not much idea about where to start. My first search
attempts lead to OBDII, and apps like Torque, DashCommand. They use the standard OBDII commands
to query data (e.g RPM, speed, coolant temperature), data must be requested by the clients.
There are also custom PID for the oil temperature (0x5C), unfortunately BMW did not implemented
that for the E46.

My next finding was that these data is available on the CAN bus. The OBDII bluetooth/wifi readers
use ELM327 or STN1100 chipset. These support CAN protocols as well.
There are two problems with this, first, the BMW E46 models do not have the CAN high/low cables in the
OBDII port. It can be fixed easily by a car mechanic (15-30 minutes of work: can high (yellow-red) goes to PIN 6,
can low (yellow-brown) goes to PIN 14. CAN wire is a twisted pair).
The second problem: there are lot of messages flowing on the CAN bus, even when the
car is idling, and these bluetooth devices run out of buffer easily. I haven't got too much experience
with this stuff, messages can be filtered so It might work with that.

You can monitor the CAN messages with an ELM327 device, like this:
```
atz
at sp 6
at ma
```

BMW E46 uses ISO 15765-4 CAN (11 bit ID, 500 kbaud) which is selected using the at sp 6 command.

## K-BUS and DS2 protocol

So I had learned a lot about this stuff, but my original goal was to create a solution where the car does not have
to be modified, and the deployment is easy (e.g: an android application available in the play store).
So I started to search how INPA works, and I found out that our cars have K-BUS hooked in the OBDII port.
INPA and the other BMW diagnostic tools use it. The protocol is called DS2, it is a command and query protocol,
where you can get that from the ECUs, clear error codes etc.

## Reverse engineering the oil temperature

Ok, so we know the protocol, all we need to do is to record an INPA session where the engine is running,
and the oil temperature increases, record the screenshots with timestamps and record the traffic on the
serial port. I had a K+DCAN USB serial cable, and I used Device Monitoring Studio to record the traffic
on the COM port.

In the scripts folder you can see some scripts and programs, one is for recording screenshots in Linux,
(my INPA runs in a virtualbox environment). There is also a script which OCRs the oil temperature from
the screenshots (I was too lazy to do it manually...), and I made a small application which parses the
output of the Device Monitoring Studio software. It prints the output in a CSV which can be imported in
google sheet or excel.

The car was running for 30-40 minutes, I made screenshots at every half minutes, so I got like 78 measurements.
The motronic ECU sent 45 bytes at each request, which is quite small. So my guess was that the oil temperature
must take up one byte, and we need to apply some conversion to get the float precision value.

**request**
```
12050b031f
```

 - 12: motronic ECU
 - 05: 5 bytes long
 - 0b03: data
 - 1f: checksum

**response**
```
122da002c30000000038003899bbb5a7c80188fc4e64359c79887c2f7c55fefefefe1447097605050d2e9086c2
```

 - 12: motronic ECU
 - 2d: 45 bytes long
 - a0: acknowledged
 - ..: data
 - c2: checksum

From the 45 bytes I visually checked each column in the google sheet, and I choose the
4 best alternative. (byte pos 10, 11, 12, 13)


![alt text](scripts/temp-inpa.png?raw=true "Actual oil temperature (screenshots)")

![alt text](scripts/raw-data.png?raw=true "Raw data from INPA")


|timestamp   | temperature (from INPA) | byte pos 12 | difference | calculated |
| :--------: | :---------------------: | :---------: | :--------: | :--------: |
| 1523786940 | 30.8	                   | 99          | 68.2       | 30.800002  |
| 1523787030 | 32.4	                   | 101         | 68.6	      | 32.392198 |
| 1523787061 | 33.99	               | 103         | 69.01      |	33.984394 |
| 1523787091 | 36.38	               | 106         | 69.62      |	36.372688 |
| 1523787121 | 38.76	               | 109         | 70.24      |	38.760982 |
| 1523787151 | 40.36	               | 111         | 70.64      |	40.353178 |
| 1523787182 | 41.95	               | 113         | 71.05      |	41.945374 |
| 1523787212 | 44.34	               | 116         | 71.66      |	44.333668 |
| 1523787242 | 45.93	               | 119         | 73.07      |	46.721962 |
| 1523787272 | 48.32	               | 121         | 72.68      |	48.314158 |
| 1523787302 | 49.91	               | 123         | 73.09      |	49.906354 |
| 1523787333 | 51.5	                   | 125         | 73.5       | 51.49855 |
| 1523787363 | 53.89	               | 128         | 74.11      |	53.886844 |
| 1523787393 | 55.48	               | 130         | 74.52      |	55.47904 |
| 1523787423 | 57.07	               | 132         | 74.93      |	57.071236 |
| 1523787454 | 58.66	               | 134         | 75.34      |	58.663432 |
| 1523787484 | 61.05	               | 137         | 75.95      |	61.051726 |
| 1523787514 | 62.64	               | 139         | 76.36      |	62.643922 |
| 1523787544 | 65.03	               | 142         | 76.97      |	65.032216 |


[Google sheet](https://drive.google.com/open?id=1seDWL7pUR_3i3hBAskou56WUeok1vXNB)

My original guess was the byte pos 10, but when I substracted the measured data from the raw data the
result was not an increasing value, but the byte pos 12 was a perfect fit.

Solving the following equation:
```
99 * multiplier + offset = 30.8
181 * multiplier + offset = 96.08
```

 - **multiplier: 0.796098**
 - **offset: -48.0137**

And that's it we have everything to calculate the oil temperature.

## Serial port interface

 - 9600 baud
 - 8 data bit
 - one stop bit
 - even parity

## WiFi K+DCAN interface

I use the following interface:

 - [ebay](https://www.ebay.com/usr/catastefy)

It has some additional framing for the requests, I need to ask the vendor maybe he/shel will tell me.
Also if you use the android application, you must disable your mobil data, because otherwise you
won't be able to connect to the device from the app (routing issue).

## Credits

I read and reasearched a lot to get this working, and a lot of knowledge and inspiration comes from
Thaniel (CAN bus project) and f0xy (E36 Track day) project. Without those guys this could not be made,
or it would take ages longer.

## Knowledge base

- [OBDII port](https://www.scantool.net/blog/which-obd-ii-protocol-is-supported-by-my-vehicle/)
- [CAN bus project](https://www.bimmerforums.com/forum/showthread.php?1887229-E46-Can-bus-project)
- [E46 CAN wires](https://wiki.autosportlabs.com/BMW_E46_CAN)
- [E36 Track day](https://www.cliosport.net/threads/e36-track-day-hack-m52-m54-3-0l-engine-build.717773/page-15#post-10524146)
- [DS2 protocol](http://markgardnergibson.com/BMW/protocol.html)