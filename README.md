cantcoap
========

CoAP implementation that focuses on simplicity by offering a minimal set of functions and straightforward interface.

```C++
	CoapPDU *pdu = new CoapPDU();
	pdu->setVersion(1);
	pdu->setType(CoapPDU::COAP_CONFIRMABLE);
	pdu->setCode(CoapPDU::COAP_GET);
	pdu->setToken((uint8_t*)"\3\2\1\0",4);
	pdu->setMessageID(0x0005);
	pdu->setURI((char*)"test",4);

	// send packet 
	ret = send(sockfd,pdu->getPDUPointer(),pdu->getPDULength(),0);
```
...

```C++
	// receive packet
	ret = recvfrom(sockfd,&buffer,BUF_LEN,0,(sockaddr*)&recvAddr,&recvAddrLen);
	CoapPDU *recvPDU = new CoapPDU((uint8_t*)buffer,ret);
	if(recvPDU->validate()) {
		recvPDU->getURI(uriBuffer,URI_BUF_LEN,&recvURILen);
		...
	}
```

# Compilation

## Dependencies

### CUnit 

See: http://cunit.sourceforge.net/

This is a testing framework for c. 
On Debian based Linux you need libcunit1 and libcunit1-dev

#### Debian (Ubuntu, Mint etc...)

\#apt-get update && apt-get install libcunit1 libcunit1-dev

Or for newbie types:

$sudo apt-get update

$sudo apt-get install libcunit1 libcunit1-dev


#### SuSE Linux - openSUSE 12.1 (x86_64)

Note there is different package naming convention here for packages.

$sudo zypper install libcunit-dev

$sudo zypper install cunit-devel


#### BSD 

Probably some variation, but for FreeBSD, CUnit is in the ports collection at /usr/ports/devel/cunit.

## Build

Type make (Note, build with GNU make on BSD).

#Long description

This is a CoAP implementation with a focus on simplicity. The library only provides PDU construction and de-construction.
The user is expected to deal with retransmissions, timeouts, and message ID matching themselves. This isn’t as arduous as it sounds and makes a lot more sense on a constrained device.
Imagine for example a simple microcontroller sensor that only reports readings once every 15 minutes, and only sends a few packets each time. Do you really need a complicated framework to deal with acknowledgements and re-transmissions?

Since CoAP recommends you only send one packet at at time, this means you only need to keep track of one on-going transaction at a time. I think you’re capable of doing the retransmission logic in a few lines of code.

Furthermore, the timers and interrupt processes between different embedded processor architectures, vary quite a bit. So it often makes sense to write the packet sending processes yourself.

Finally, you might be sending the packets over odd transport bearers such as a SMS or a simple radio bearer. In which case, it’s easiest to deal with buffers. If I built retransmission handlers, they’d all be UDP/IP specific and would bloat the code.
