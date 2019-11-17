/*
 * ** dnsmap - DNS Network Mapper by pagvac
 * ** Copyright (C) 2006 ikwt.com, elhacker.net
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

#define MAXDOMSIZE 	100
#define BANNER 		"\ndnsmap - DNS Network Mapper by pagvac\n(http://ikwt.com, http://foro.elhacker.net)\n"
#define USAGE 		"Usage: dnsmap <target-domain> [dictionary-file]\n"
#define EXAMPLES 	"Examples:\ndnsmap yourtarget.com\ndnsmap yourtarget.com yourwordlist.txt\n\n"


int main(int argc, char *argv[]) {

	int i=0, j=0, found=0;
	char dom[MAXDOMSIZE]={'\0'}, buf[MAXDOMSIZE]={'\0'};
	char randsub[MAXDOMSIZE]={"pseudorandsubabcdef1234dnsmap"};
	struct hostent *h;
	FILE *fp;

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
	
	// printf("%s\n", argv[1]);

	printf("%s", BANNER);

	// generate subdomains from built-in list
	if(argc==2)
	{	
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
		printf("%s%s\n\n", "Searching subhosts on domain ", argv[1]);
			
		for(i=0;i<NUMSUBS;++i)
		{
			strcpy(dom, sub[i]);
			strcat(dom, ".");
			strcat(dom, argv[1]);
		
			if ((h=gethostbyname(dom))!=NULL)
			{
				++found;
				printf("%s\n", dom);	
			    	for(j=0;h->h_addr_list[j];++j)
					printf("IP Address #%d:%s\n", j+1, inet_ntoa(*((struct in_addr *)h->h_addr_list[j])));
				printf("%s", "\n");
			}    
		}	
	}
	
	// generate subdomains from wordlist file	
	else if(argc==3)
	{
		fp=fopen(argv[2], "r");	
		if(fp)
		{
			printf("%s%s\n\n", "Searching subhosts on domain ", argv[1]);
			while(fgets(dom, MAXDOMSIZE, fp)!=NULL)
			{
				dom[strlen(dom)-1]='\0';
				strcat(dom, ".");
				strcat(dom, argv[1]);
				// printf("%s\n", dom);

				if ((h=gethostbyname(dom))!=NULL)
				{
					++found;
					printf("%s\n", dom);
					for(j=0;h->h_addr_list[j];++j)
						printf("IP Address #%d:%s\n", j+1, inet_ntoa(*((struct in_addr *)h->h_addr_list[j])));
					printf("%s", "\n");
				}
						       
			}
			fclose(fp);
		}
		else
		{
			printf("%s\"%s\"!\n\n", "Error opening wordlist file ", argv[2]);
			exit(1);
		}
	}

	printf("%d%s\n\n", found, " subhost(s) found");
	return 0;
}
