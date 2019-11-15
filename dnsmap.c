/*
 * ** dnsmap - DNS Network Mapper by Adrian 'pagvac' Pastor of GNUCITIZEN
 * ** Copyright (C) 2009 gnucitizen.org
 * **
 * ** This program is free software; you can redistribute it and/or modify
 * ** it under the terms of the GNU General Public License as published by
 * ** the Free Software Foundation; either version 2 of the License, or
 * ** (at your option) any later version.
 * **
 * ** This program is distributed in the hope that it will be useful,
 * ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 * ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * ** GNU General Public License for more details.
 * **
 * ** You should have received a copy of the GNU General Public License
 * ** along with this program; if not, write to the Free Software
 * ** Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 * */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <netdb.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "dnsmap.h" // built-in subdomains list

#define MAXSTRSIZE 	100
#define BANNER 		"dnsmap 0.22.2 - DNS Network Mapper by pagvac (gnucitizen.org)\n\n"
#define USAGE 		"usage: dnsmap <target-domain> [options]\noptions:\n"\
			"-w <wordlist-file>\n-r <results-path>\n"
#define EXAMPLES 	"i.e.:\ndnsmap target-domain.foo\n"\
			"dnsmap target-domain.foo -w yourwordlist.txt -r /tmp/\n"\
			"dnsmap target-fomain.foo -r ./\n\n"
#define WILDCARDERR	"the target domain appears to be using wildcards\n"\
			"(all brute-forced subdomains resolve to the same IP address(es))\n"\
			"suggestion: try using a different target domain\n"

#define FALSE 0
#define TRUE 1

// return true if domain wilcards are enabled
unsigned short int wildcarddetect(char *dom) {
        char strTmp[30]={'\0'},s[30]={'\0'};
        unsigned short int i=0,n=0,max=0;


        srand(time(NULL));
	max=rand()%20;
	// max should be between 10 and 20
	if(max<10)
		max=max+(10-max);
	
	// generate up to random 20 digits-long subdomain
	// i.e.: 06312580442146732554
 
	for(i=0;i<max;++i) {
                n=rand()%10;
                sprintf(strTmp, "%d", n);
                if(i==0)
                        strcpy(s,strTmp);
                else
                        strcat(s,strTmp);
        }
	strcat(s, ".");
        strcat(s, dom);
	
	// random subdomain resolves, thus wildcards are enabled
	if(gethostbyname(s))
		return TRUE;
	else
		return FALSE;
}

int main(int argc, char *argv[]) {

	unsigned short int i=0, j=0, found=0, ipCount=0, wordlist=FALSE, results=FALSE;
	char dom[MAXSTRSIZE]={'\0'}, logFile[MAXSTRSIZE]={'\0'}, 
		logFile2[MAXSTRSIZE]={'\0'}, wordlistFile[MAXSTRSIZE]={'\0'};
		
	struct hostent *h;
	FILE *fpWords,*fpLogs,*fpLogs2;

	if(argc==1) {
		printf("%s%s%s", BANNER, USAGE, EXAMPLES);	
		exit(1);	
	}
	
	/*
	else if ((h=gethostbyname(argv[1])) == NULL) {  // get the host info
	        herror("gethostbyname");
	        exit(1);
	}
	*/
	
	printf("%s", BANNER);

	// parse options
	for(i=0;i<argc;++i) { 
		// save results in file	
		if(!strcmp(argv[i],"-r")) {	
	       		// contruct path were csv and txt results files will be created 
			results=TRUE;	
			strncpy(logFile, argv[(i+1)], MAXSTRSIZE);
			strncpy(logFile2, argv[(i+1)], MAXSTRSIZE);	
			strncat(logFile, argv[1], MAXSTRSIZE);
			strncat(logFile2, argv[1], MAXSTRSIZE);
			strcat(logFile, ".csv");
			strcat(logFile2, ".txt");	
 		 	fpLogs=fopen(logFile, "a");
			fpLogs2=fopen(logFile2, "a");
			if(!fpLogs || !fpLogs2) {
				printf("%s\"%s\"!\n\n", "error creating results file on ", argv[(i+1)]);	
				exit(1);
			}
		}	
		// use provided wordlist as opposed to built-in one
		if(!strcmp(argv[i],"-w")) {
			wordlist=TRUE;
			strncpy(wordlistFile, argv[(i+1)],MAXSTRSIZE);
		}
	}

	// read subdomains from built-in list
	if(!wordlist) {
		//fpLogs=fopen(logFile, "a");

		// wildcard detection
		if(wildcarddetect(argv[1])) {
			printf("%s",WILDCARDERR);	
			exit(1);
		}
		printf("%s%s%s\n\n", "[+] searching (sub)domains for ", argv[1], " using built-in wordlist");
			
		for(i=0;i<(sizeof(sub)/MAXSUBSIZE);++i) {
			strncpy(dom, sub[i], MAXSTRSIZE);
			strcat(dom, ".");
			strncat(dom, argv[1], MAXSTRSIZE);
		
			if ((h=gethostbyname(dom))!=NULL) {
				++found;
				printf("%s\n", dom);	
			    	if(results) {
					fprintf(fpLogs, "%s", dom);
					fprintf(fpLogs2, "%s\n", dom);
				}	
				for(j=0;h->h_addr_list[j];++j,++ipCount) {
					printf("IP address #%d: %s\n", j+1, 
						inet_ntoa(*((struct in_addr *)h->h_addr_list[j])));	
					if(results) {
						fprintf(fpLogs, ",%s", 
							inet_ntoa(*((struct in_addr *)h->h_addr_list[j])));	
						fprintf(fpLogs2, "IP address #%d: %s\n", j+1,	
							inet_ntoa(*((struct in_addr *)h->h_addr_list[j])));	
					}
				}	
				printf("%s", "\n");
				if(results) {

					fprintf(fpLogs, "%s", "\n");
					fprintf(fpLogs2, "%s", "\n");
				}
			}    
		}
		if(results) {
			fclose(fpLogs);	
			fclose(fpLogs2);
		}
	}
	
	// read subdomains from wordlist file	
	else if(wordlist) {
                
		// wildcard detection
                if(wildcarddetect(argv[1])) {
			printf("%s",WILDCARDERR);                        
			exit(1);
                }


		//fpLogs=fopen(logFile, "a");
		fpWords=fopen(wordlistFile, "r");	
		if(fpWords) { 
                        printf("%s%s%s%s\n\n", "[+] searching (sub)domains for ", argv[1], " using ", wordlistFile);
                        while(!feof(fpWords)) {
				fscanf(fpWords, "%s", dom); 
                                strcat(dom, ".");
                                strncat(dom, argv[1], MAXSTRSIZE);

                                if ((h=gethostbyname(dom))!=NULL) {
                                        ++found;
                                        printf("%s\n", dom);
                                        if(results) {
                                                fprintf(fpLogs, "%s", dom);
						fprintf(fpLogs2, "%s\n", dom);
					}
                                        for(j=0;h->h_addr_list[j];++j,++ipCount) {
                                                printf("IP address #%d: %s\n", j+1, 
							inet_ntoa(*((struct in_addr *)h->h_addr_list[j])));
                                                if(results) {
                                                        fprintf(fpLogs, ",%s", 
								inet_ntoa(*((struct in_addr *)h->h_addr_list[j])));
							fprintf(fpLogs2, "IP address #%d: %s\n", j+1,
								inet_ntoa(*((struct in_addr *)h->h_addr_list[j])));	
						}
                                        }
                                        printf("%s", "\n");
                                        if(results) {
                                                fprintf(fpLogs, "%s", "\n");
						fprintf(fpLogs2, "%s", "\n");
					}
                                }

			}
			fclose(fpWords);
		}
		else {
			printf("%s\"%s\"!\n\n", "error opening wordlist file ", wordlistFile);
			exit(1);
		}
		if(results) {
			fclose(fpLogs);	
			fclose(fpLogs2);
		}
	}

	printf("[+] %d%s%d%s\n", found, " (sub)domains and ",ipCount, " IP address(es) found");
	if(results)
		printf("[+] results can be found on %s and %s\n\n", logFile, logFile2);	
	return 0;
}
