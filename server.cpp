// simple client example for cantcoap
#include <sys/types.h>
#include <sys/socket.h>
#define __USE_POSIX 1
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/time.h>
#include <math.h>
#include "nethelper.h"
#include "cantcoap.h"
#include "uthash.h"

//void callback(char *uri, method);

struct URIHashEntry {
    const char *uri; 
    int id;
    UT_hash_handle hh;
};


void failGracefully(int x) {
	exit(x);
}

int main(int argc, char **argv) {

	// parse options	
	if(argc!=3) {
		printf("USAGE\r\n   %s listenAddress listenPort\r\n",argv[0]);
		return 0;
	}

	char *listenAddressString = argv[1];
	char *listenPortString    = argv[2];

	// setup bind address
	struct addrinfo *bindAddr;
	INFO("Setting up bind address");
	int ret = setupAddress(listenAddressString,listenPortString,&bindAddr,SOCK_DGRAM,AF_INET);
	if(ret!=0) {
		INFO("Error setting up bind address, exiting.");
		return -1;
	}

	// iterate through returned structure to see what we got
	printAddressStructures(bindAddr);

	// setup socket
	int sockfd = socket(bindAddr->ai_family,bindAddr->ai_socktype,bindAddr->ai_protocol);

	// call bind
	DBG("Binding socket.");
	if(bind(sockfd,bindAddr->ai_addr,bindAddr->ai_addrlen)!=0) {
		DBG("Error binding socket");
		perror(NULL);
		failGracefully(5);
	}
	
	//
	printAddress(bindAddr);

	// setup URI callbacks using uthash hash table
	struct URIHashEntry *entry = NULL, *directory = NULL, *t = NULL;
   entry = (struct URIHashEntry*)malloc(sizeof(struct URIHashEntry));
	const char *uri = "test";
	entry->uri = uri;
   HASH_ADD_KEYPTR(hh, directory, entry->uri, strlen(entry->uri), entry);

   HASH_FIND_STR(directory,"betty",t);
	if(t) {
		DBG("betty's id is %d", t->id);
	} else {
		DBG("hash not found");
	}
   HASH_FIND_STR(directory,"test",t);
	if(t) {
		DBG("test's id is %d.", t->id);
	} else {
		DBG("hash not found.");
	}

	// temporary
	#define BUF_LEN 500
	char buffer[BUF_LEN];
	sockaddr recvAddr;
	socklen_t recvAddrLen;
	CoapPDU *recvPDU = NULL;

	// just block completely since this is only an example
	// you're not going to use this for a production system are you ;)
	while(1) {
		// receive packet
		ret = recvfrom(sockfd,&buffer,BUF_LEN,0,&recvAddr,&recvAddrLen);
		if(ret==-1) {
			INFO("Error receiving data");
			return -1;
		}

/*
		// try to get hostname and service
		if(getnameinfo((struct sockaddr*)&fromAddr,fromAddrLen,hostStr,hostStrLen,servStr,servStrLen,0)==0) {
			INFO("Received %ld bytes from %s:%s",ret,hostStr,servStr);
		} else {
			INFO("Received %ld bytes from %s:%d",ret,inet_ntoa(fromAddr.sin_addr),ntohs(fromAddr.sin_port));
		}
		*/


		// validate packet
		recvPDU = new CoapPDU((uint8_t*)buffer,ret);
		if(recvPDU->isValid()!=1) {
			INFO("Malformed CoAP packet");
			delete recvPDU;
			continue;
		}
		INFO("Valid CoAP PDU received");
		recvPDU->printHuman();
		delete recvPDU;
	}

    // free the hash table contents
	 /*
    HASH_ITER(hh, users, s, tmp) {
      HASH_DEL(users, s);
      free(s);
    }
	 */
	
	return 0;
}

