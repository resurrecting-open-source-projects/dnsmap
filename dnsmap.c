/*
 * ** dnsmap - DNS Network Mapper by Adrian 'pagvac' Pastor of GNUCITIZEN
 * ** Copyright (C) 2008 gnucitizen.org, ikwt.org
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
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "dnsmap.h" // built-in subdomains list

#define MAXSTRINGSIZE 	100
#define BANNER 		"\ndnsmap 0.2 - DNS Network Mapper\nby Adrian 'pagvac' Pastor (gnucitizen.org)\n\n"
#define USAGE 		"Usage: dnsmap <target-domain> [dictionary-file]\n"
#define EXAMPLES 	"i.e.:\ndnsmap target-domain.foo\ndnsmap target-domain.foo yourwordlist.txt\n\n"

int main(int argc, char *argv[]) {

	int i=0, j=0, found=0, ipCount=0;
	char dom[MAXSTRINGSIZE]={'\0'}, buf[MAXSTRINGSIZE]={'\0'}, logFile[MAXSTRINGSIZE]={'\0'};
	char randsub[MAXSTRINGSIZE]={"pseudorandsubabcdef1234dnsmap"};
	struct hostent *h;
	FILE *fp,*fpLogs;

	if(argc==1)
	{
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

	strcat(logFile, argv[1]); strcat(logFile,".dnsmap.csv");	
	fpLogs=fopen(logFile, "a");

	// generate subdomains from built-in list
	if(argc==2)
	{	
		//fpLogs=fopen(logFile, "a");

		// wildcard detection dirty hack
		strcpy(dom, randsub);
		strcat(dom, ".");
		strcat(dom, argv[1]);
		if(gethostbyname(dom))
		{
			printf("%s\n", "The target domain appears to be using wildcards");
	                printf("%s\n", "(all brute-forced subdomains will resolve to the same IP address(es))");
			printf("%s\n\n", "Suggestion: try using a different target domain");
			exit(1);
		}
		printf("%s%s\n\n", "Searching (sub)domains on domain ", argv[1]);
			
		for(i=0;i<NUMSUBS;++i)
		{
			strcpy(dom, sub[i]);
			strcat(dom, ".");
			strcat(dom, argv[1]);
		
			if ((h=gethostbyname(dom))!=NULL)
			{
				++found;
				printf("%s\n", dom);	
			    	fprintf(fpLogs, "%s", dom);
				for(j=0;h->h_addr_list[j];++j,++ipCount) {
					printf("IP Address #%d:%s\n", j+1, inet_ntoa(*((struct in_addr *)h->h_addr_list[j])));	
					fprintf(fpLogs, ",%s", inet_ntoa(*((struct in_addr *)h->h_addr_list[j])));	
				}	
				printf("%s", "\n");
				fprintf(fpLogs, "%s", "\n");
			}    
		}
		fclose(fpLogs);	
	}
	
	// generate subdomains from wordlist file	
	else if(argc==3)
	{
		//fpLogs=fopen(logFile, "a");
		fp=fopen(argv[2], "r");	
		if(fp)
		{
			printf("%s%s\n\n", "Searching (sub)domains on domain ", argv[1]);
			while(fgets(dom, MAXSTRINGSIZE, fp)!=NULL)
			{
				dom[strlen(dom)-1]='\0';
				strcat(dom, ".");
				strcat(dom, argv[1]);
				// printf("%s,", dom);

				if ((h=gethostbyname(dom))!=NULL)
				{
					++found;
					printf("%s\n", dom);
					fprintf(fpLogs, "%s", dom);	
					for(j=0;h->h_addr_list[j];++j,++ipCount) {
	                                        printf("IP Address #%d:%s\n", j+1, inet_ntoa(*((struct in_addr *)h->h_addr_list[j])));
       	                                 	fprintf(fpLogs, ",%s", inet_ntoa(*((struct in_addr *)h->h_addr_list[j])));	
					}
					printf("%s", "\n");
					fprintf(fpLogs, "%s", "\n");
				}
						       
			}
			fclose(fp);
		}
		else
		{
			printf("%s\"%s\"!\n\n", "Error opening wordlist file ", argv[2]);
			exit(1);
		}
		fclose(fpLogs);	
	}

	printf("%d%s%d%s\n", found, " (sub)domains and ",ipCount, " IP address(es) found");
	printf("results can be found on %s\n\n", logFile);	
	return 0;
}
