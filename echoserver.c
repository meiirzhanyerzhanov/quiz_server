#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <sys/select.h>
#include "linkedlist02.h"
#include <assert.h>
#include <stdbool.h>
#include<pthread.h>

#define	QLEN			5
#define	BUFSIZE			4096
#define CHUNK           1024
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
int itoa(int value, char *sp, int radix)
{
    char tmp[16];// be careful with the length of the buffer
    char *tp = tmp;
    int i;
    unsigned v;

    int sign = (radix == 10 && value < 0);    
    if (sign)
        v = -value;
    else
        v = (unsigned)value;

    while (v || tp == tmp)
    {
        i = v % radix;
        v /= radix; // v/=radix uses less CPU clocks than v=v/radix does
        if (i < 10)
          *tp++ = i+'0';
        else
          *tp++ = i + 'a' - 10;
    }

    int len = tp - tmp;

    if (sign) 
    {
        *sp++ = '-';
        len++;
    }

    while (tp > tmp)
        *sp++ = *--tp;

    return len;
}
int countlines(char *filename)
{
  // count the number of lines in the file called filename
  FILE *fp = fopen(filename,"r");
  int ch=0;
  int lines=0;

  if (fp == NULL)
  return 0;

  lines++;
  while ((ch = fgetc(fp)) != EOF)
    {
      if (ch == '\n')
    lines++;
    }
  fclose(fp);
  return lines;
}


char** str_split(char* a_str, const char a_delim)
	{
    char** result    = 0;
    size_t count     = 0;
    char* tmp        = a_str;
    char* last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;

    /* Count how many elements will be extracted. */
    while (*tmp)
    {
        if (a_delim == *tmp)
        {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }

    /* Add space for trailing token. */
    count += last_comma < (a_str + strlen(a_str) - 1);

    /* Add space for terminating null string so caller
       knows where the list of returned strings ends. */
    count++;

    result = malloc(sizeof(char*) * count);

    if (result)
    {
        size_t idx  = 0;
        char* token = strtok(a_str, delim);

        while (token)
        {
            assert(idx < count);
            *(result + idx++) = strdup(token);
            token = strtok(0, delim);
        }
        assert(idx == count - 1);
        *(result + idx) = 0;
    }

    return result;
}

typedef struct CLIENT{
	List *isingroup;
	List *score;
	List *username;
	List *ssock;
	
}CLIENT;

typedef struct QUIZTEXT{
	char quiztext[256][256];
	int size;
	int numlines;
	
}QUIZTEXT;

typedef struct GROUP{
	List *topic;
	List *groupname;
	List *size;
	CLIENT clients[128];
	List *filename;
	List *cursize;
	List *isstarted;	
	
    //List *quiztext;
	QUIZTEXT quiztext[128];
}GROUP;
GROUP group;
CLIENT client;
int groupn=0;
fd_set			rfds;
fd_set			afds;
int			msock;
int			nfds;
struct sockaddr_in	fsin;
//Global variables

	void func(){
		group.topic = List_create();
		group.groupname = List_create();
		group.size = List_create();
		group.filename = List_create();
		group.cursize = List_create();
		group.isstarted = List_create();
		
        
		
			   }
void func2(int i){
	
	
		group.clients[i].isingroup=List_create();
		group.clients[i].score=List_create();
		group.clients[i].username=List_create();
		group.clients[i].ssock=List_create();
			   
	}


int c=0;
int *p=&c;
int lnum=5;

void *startquiz(void *gname){
	printf("You are in startquiz\n");
	int insize=atoi(List_get(group.size, List_find(group.groupname, gname)));
	int incursize=atoi(List_get(group.cursize, List_find(group.groupname, gname)));
	while(insize+1>incursize){
		 insize=atoi(List_get(group.size, List_find(group.groupname, gname)));
		 incursize=atoi(List_get(group.cursize, List_find(group.groupname, gname)));
		if(insize==incursize){
			int i=0, j;
			for(i=0;i<insize+1;i++){
				
			for(j=0;j<5;j++)	
			write( atoi(List_get(group.clients[List_find(group.groupname, gname)].ssock, i)), group.quiztext[List_find(group.groupname, gname)].quiztext[j], group.quiztext[List_find(group.groupname, gname)].size);
			sleep(3);
			for(j=5;j<10;j++)	
			write( atoi(List_get(group.clients[List_find(group.groupname, gname)].ssock, i)), group.quiztext[List_find(group.groupname, gname)].quiztext[j], group.quiztext[List_find(group.groupname, gname)].size);
			sleep(3);
			for(j=10;j<17;j++)	
			write( atoi(List_get(group.clients[List_find(group.groupname, gname)].ssock, i)), group.quiztext[List_find(group.groupname, gname)].quiztext[j], group.quiztext[List_find(group.groupname, gname)].size);
			sleep(3);
			
				
			}
			incursize++;
		}
	}
	
}


void *con(void *sock){
	
	
	int ssock = *(int*)sock;
    
		
    int fd;
    int alen;
	printf( "A client has arrived for echoes.\n" );
	 //OPENGROUPS|topic|name|size|curr
	int cc;
	char			buf[BUFSIZE];
	char** tokens;
	int i=0, u=0;
	char str[BUFSIZE]="OPENGROUPS|";
	char str2[BUFSIZE]="";
	for(u=0;u<groupn;u++){
		sprintf(str2, "%s|%s|%s|%s|\n", List_get(group.topic, u), List_get(group.groupname, u), List_get(group.size, u), List_get(group.cursize, u));
		strcat(str, str2);
		printf("group:   %i\n", groupn);
	}
	//sprintf(str, "OPENGROUPS|%s|%s|%i|%i", List_getgroup.topic);
	if ( write( ssock, str, BUFSIZE ) < 0 )
				{
					
					close( ssock );
					
				}
	nfds = msock+1;
	FD_ZERO(&afds);
	FD_SET( msock, &afds );
	
for (;;)
		{
            printf( "fd=msock\n" );
            memcpy((char *)&rfds, (char *)&afds, sizeof(rfds));
             printf( "check1\n" );
            
           /*if (select(nfds, &rfds, (fd_set *)0, (fd_set *)0,
				(struct timeval *)0) < 0)
		{
			fprintf( stderr, "server select: %s\n", strerror(errno) );
			exit(-1);
            
		}*/
            printf( "check2\n" );
            if (FD_ISSET( msock, &rfds)) 
		{
			

			// we can call accept with no fear of blocking
			alen = sizeof(fsin);
			//ssock = accept( msock, (struct sockaddr *)&fsin, &alen );
			if (ssock < 0)
			{
				fprintf( stderr, "accept: %s\n", strerror(errno) );
				exit(-1);
			}

			// start listening to this guy 
			FD_SET( ssock, &afds );

			// increase the maximum
			if ( ssock+1 > nfds )
				nfds = ssock+1;
		}
			
			//pthread_mutex_lock(&lock);
			/*if ( (cc = read( ssock, buf, BUFSIZE )) <= 0 )
			{
				printf( "The client has gone.\n" );
				
				c--;
				printf( "%i\n", c );
				close(ssock);
				break;
			}*/
            printf( "ssock:= %i nfds:= %i   fd:= %i \n", ssock, nfds, fd );
            for ( fd = 0; fd < nfds; fd++ )
		{
            
			// check every socket to see if it's in the ready set
			
				if (fd != msock && FD_ISSET(fd, &rfds))
			{
				printf( "ssock:= %i nfds:= %i   fd:= %i \n", ssock, nfds, fd );
				// read without blocking because data is there
				if ( (cc = read( fd, buf, BUFSIZE )) <= 0 )
				{
					printf( "The client has gone.\n" );
					c--;
					(void) close(fd);
					FD_CLR( fd, &afds );
					FD_CLR( fd, &rfds );
					// lower the max socket number if needed
					if ( nfds == fd+1 )
						nfds--;
					return 0;
					

				}
				else
			{	
				tokens = str_split(buf, '|');
				printf( "check2\n" );
	if(strcmp("GROUP", *(tokens))==0){
		pthread_t group_thread;
		if( pthread_create( &group_thread , NULL ,  startquiz , (void*) *(tokens+2)) < 0)
        {
            perror("could not create thread");
            
        }
		if(List_find(group.groupname, *(tokens+2))!=-1){
            write( fd, "BAD\n", 5 );
        }
        else{
            char *findindex=*(tokens+2);
		List_append(group.topic, *(tokens+1));
		List_append(group.groupname, *(tokens+2));
			printf( "checkstrcpy\n" );
        
			printf( "checkstrpassed\n" );
		List_append(group.size, *(tokens+4));
		List_append(group.cursize, "0");
		List_append(group.isstarted, "false");
		List_print(group.topic);
		List_print(group.groupname);
		List_print(group.size);
		List_print(group.cursize);
		List_print(group.isstarted);
        func2(groupn);
			List_append(group.clients[groupn].username, *(tokens+3));
		List_append(group.clients[groupn].isingroup, "true");
        char itoaa[20];
				itoa(ssock,itoaa,20);
				List_append(group.clients[groupn].ssock, itoaa);
        List_print(group.clients[groupn].username);
               List_print(group.clients[groupn].isingroup);
               List_print(group.clients[groupn].ssock);
        //printf("\n\n\nusername:= %s isingroup:= %i  ssock:= %s \n\n\n", group.clients[groupn].username, group.clients[groupn].isingroup, group.clients[groupn].ssock);
        //sprintf(str2, "%s|%s|%s|%s|\n", List_get(group.topic, groupn), List_get(group.groupname, groupn), List_get(group.size, groupn), List_get(group.cursize, groupn));
		//strcat(str, str2);
        groupn++;
		 write( fd, "SENDQUIZ\n", 10 );
            int i=0;
            while(i==0){
            if ( (cc = read( fd, buf, BUFSIZE )) <= 0 )
				{
					printf( "The client has gone.\n" );
					(void) close(fd);
					FD_CLR( fd, &afds );
					// lower the max socket number if needed
					if ( nfds == fd+1 )
						nfds--;

				}
            else{
                
                 tokens = str_split(buf, '|');
                
                if(strcmp("QUIZ", *(tokens))==0 && *(tokens+2)!=""){
                      //List_append(group.quizsize, *(tokens+1));
                    List_append(group.filename, *(tokens+2));
                    //List_print(group.quizsize);
                    List_print(group.filename);
					
							FILE *file = fopen(*(tokens+2), "r");
								int readfile=0, nread=0;
								if (file == NULL ) {
		printf("Problem opening files.");
		//return 1;
	}
								char line[256];
								 for(nread=countlines(*(tokens+2));nread>0;nread--) {
									 fgets(line, sizeof(line), file);
										strcpy(group.quiztext[groupn-1].quiztext[readfile], line);
									 
											readfile++;
 							   }
					group.quiztext[groupn-1].size=atoi(*(tokens+1));
					group.quiztext[groupn-1].numlines=countlines(*(tokens+2))-1;
					
					
					/*nread=countlines(*(tokens+2));
					printf("number of line \n%i", nread);
					readfile=0;
					while(nread>0&&readfile<5){
    					printf("%s", group.quiztext[groupn-1].quiztext[readfile]);
						 write(fd, group.quiztext[groupn-1].quiztext[readfile], 256);
    					nread--;
    						readfile++;
    					}
		
					
					
					
					
					
                    //printf("\nOK\n");
                    /*FILE *fp = fopen ( List_get(group.filename, List_find(group.filename, findindex)), "r" );
                    char readfromfile[CHUNK];
                    size_t nread;
                    */
                   
                    write( fd, "OK\n", 3 );
               i=1;
                }
                else{
                 write( fd, "BAD\n", 5 );   
                }
                
                
            }
        }
            
        }
	}
    else if(strcmp("OPENGROUPS", buf)==0){
		List_print(group.clients[0].username);
		List_print(group.clients[0].ssock);
		int y=0;
		strcpy(str,"");
		strcat(str, "OPENGROUPS|");
		for(y=0;y<groupn;y++){
		sprintf(str2, "%s|%s|%s|%s|\n", List_get(group.topic, y), List_get(group.groupname, y), List_get(group.size, y), List_get(group.cursize, y));
		strcat(str, str2);
		}
		
		//sprintf(str2, "%s|%s|%s|%s|", List_get(group.topic, u), List_get(group.groupname, u), List_get(group.size, u), List_get(group.cursize, u));
		//strcat(str, str2);
		printf("group:   %i\n", groupn);
		printf("clients:   %i\n", c);
	
	//sprintf(str, "OPENGROUPS|%s|%s|%i|%i", List_getgroup.topic);
	if ( write( ssock, str, BUFSIZE ) < 0 )
				{
					
					close( ssock );
					
				}
	}
				else if(strcmp("JOIN", buf)==0){
					int indexofg=List_find(group.groupname, *(tokens+1));
					printf("index of group:   %i\n", indexofg);
					if(indexofg!=-1){
					List_append(group.clients[indexofg].username, *(tokens+2));
					List_append(group.clients[indexofg].isingroup, "true");
					List_append(group.clients[indexofg].score, "0");
					char itoaa[20], itoaaa[20];
					itoa(ssock,itoaa,20);
					List_append(group.clients[indexofg].ssock,itoaa);
					printf("HERERHERHEHR      ssock:= %s\n", itoaa);
					List_print(group.clients[indexofg].username);
               List_print(group.clients[indexofg].isingroup);
               List_print(group.clients[indexofg].ssock);
					List_print(group.clients[indexofg].score);
						char curs[5];
						strcpy(curs, List_get(group.cursize, indexofg));
						
						
						int cursint=atoi(curs);
						cursint++;
						itoa(cursint,itoaaa,20);
						List_insert(group.cursize, indexofg, itoaaa);
						List_remove(group.cursize, indexofg+1);
						List_print(group.cursize);
						printf("END OF JOIN:   %i\n", indexofg);
						
						
						 List_print(group.clients[indexofg].ssock);
					}
					else{
						write( fd, "There is no such group\n", 25 );
					}
					
				}
				/*
				else if(strcmp("CANCEL", buf)==0){
					
					int indexofg=List_find(group.groupname, *(tokens+1));
					List_remove(group.groupname, indexofg);
					List_remove(group.topic, indexofg);
					List_remove(group.size, indexofg);
					List_remove(group.filename, indexofg);
					List_remove(group.cursize, indexofg);
					List_remove(group.isstarted, indexofg);
					
					if(groupn-2>=0){
					List_insert(group.groupname, indexofg, List_pop(group.groupname, groupn-2));
					
					List_insert(group.groupname, indexofg, List_pop(group.topic, groupn-2));
					
					List_insert(group.groupname, indexofg, List_pop(group.size, groupn-2));
					
					List_insert(group.groupname, indexofg, List_pop(group.filename, groupn-2));
					
					List_insert(group.groupname, indexofg, List_pop(group.cursize, groupn-2));
					
					List_insert(group.groupname, indexofg, List_pop(group.isstarted, groupn-2));
					}
					int h=0;
					for(h=0;h<List_length(group.clients[indexofg].score);h++){
					List_remove(group.clients[indexofg].score, h);
						if(groupn-2>=0)
					List_insert(group.clients[indexofg].score, h, List_pop(group.clients[groupn-2].score, h));
						
					List_remove(group.clients[indexofg].isingroup, h);
						if(groupn-2>=0)
					List_insert(group.clients[indexofg].isingroup, h, List_pop(group.clients[groupn-2].isingroup, h));
						
					List_remove(group.clients[indexofg].username, h);
						if(groupn-2>=0)
					List_insert(group.clients[indexofg].username, h, List_pop(group.clients[groupn-2].username, h));
						
					List_remove(group.clients[indexofg].ssock, h);
						if(groupn-2>=0)
					List_insert(group.clients[indexofg].ssock, h, List_pop(group.clients[groupn-2].ssock, h));
					
					}
					List_print(group.topic);
					List_print(group.groupname);
					List_print(group.size);
					List_print(group.filename);
					List_print(group.cursize);
					List_print(group.isstarted);
					for(h=0;h<groupn-1;h++){
					List_print(group.clients[h].score);
						List_print(group.clients[h].username);
						List_print(group.clients[h].isingroup);
						List_print(group.clients[h].ssock);
					
				}
				
	
				
                        	
			}*/
				
			}

		
	}
			
			 //pthread_mutex_unlock(&lock);
		}	
	
}

}
int passivesock( char *service, char *protocol, int qlen, int *rport );

/*
**	The server ... 
*/
int main( int argc, char *argv[] )
{
	char			buf[BUFSIZE];
	char			*service;
	
   
	
    int			fd;
	int			nfds;
	int			alen;
	
	int			ssock;
	int			rport = 0;
	int			cc, numclients=0;
	func();
	//func2();
	/* groupcl = List_create();
	List_append(groupcl, "bob");
	List_append(groupcl, "   bob2|\n");
	List_append(groupcl, "   bob3|\n");
	hhh[0].groupname="KAHOOTS";
	hhh[0].size=6;
	hhh[0].cursize=0;
	
    /*List_append(l, "jim");
    List_append(l, "bob");
    List_append(l, "joe");
    List_append(l, "bean");
    List_append(l, "junior");
	
	 List_insert(l, 3, "HAROLD");
    List_insert(l, List_length(l), "CARL");
    List_insert(l, 0, "MIKE");

    List_remove(l, 2);

    List_print(l);
    printf("Length: %i\n", List_length(l));
    printf("Pop 3: %s\n", List_pop(l, 3));
    printf("Get 3: %s\n", List_get(l, 3));
    printf("Find \"junior\": %i\n", List_find(l, "junior"));

    List_destroy(l);*/
	/*group.clients[0]="Meiirzhan";
	printf("%s\n", group.clients[0]);
	*/
	switch (argc) 
	{
		case	1:
			// No args? let the OS choose a port and tell the user
			rport = 1;
			break;
		case	2:
			// User provides a port? then use it
			service = argv[1];
			break;
		default:
			fprintf( stderr, "usage: server [port]\n" );
			exit(-1);
	}

	msock = passivesock( service, "tcp", QLEN, &rport );
	if (rport)
	{
		//	Tell the user the selected port
		printf( "server: port %d\n", rport );	
		fflush( stdout );
	}

	
	for (;;)
	{
		int	ssock, *news;

		alen = sizeof(fsin);
		
		if (ssock < 0)
		{
			fprintf( stderr, "accept: %s\n", strerror(errno) );
			exit(-1);
		}

		
		fflush( stdout );
        
        
        
	
      while(ssock = accept( msock, (struct sockaddr *)&fsin, &alen )){  
      /* if( select(nfds, &rfds, (fd_set *)0, (fd_set *)0,
				(struct timeval *)0) < 0 ){
           fprintf( stderr, "server select: %s\n", strerror(errno) );
			exit(-1);
           
       }
        */
       printf("ssock in while : = %i\n", ssock);
		 pthread_t sniffer_thread;
		static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
		static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
		news = malloc(1);
        *news = ssock;
		puts("Handler assigned");
		c++;
		printf("%i\n", c);
		if( pthread_create( &sniffer_thread , NULL ,  con , (void*) news) < 0)
        {
            perror("could not create thread");
            return 1;
        }
        
    
      }
		 if (ssock < 0)
    {
        perror("accept failed");
        return 1;
    }
    return 0;
		
		/* start working for this guy */
		/* ECHO what the client says */
		
	}
}

Node *Node_create() {
    Node *node = malloc(sizeof(Node));
    assert(node != NULL);

    node->data = "";
    node->next = NULL;

    return node;
}


void Node_destroy(Node *node) {
    assert(node != NULL);
    free(node);
}


List *List_create() {
    List *list = malloc(sizeof(List));
    assert(list != NULL);

    Node *node = Node_create();
    list->first = node;

    return list;
}


void List_destroy(List *list) {
    assert(list != NULL);

    Node *node = list->first;
    Node *next;
    while (node != NULL) {
        next = node->next;
        free(node);
        node = next;
    }

    free(list);
}


void List_append(List *list, char *str) {
    assert(list != NULL);
    assert(str != NULL);

    Node *node = list->first;
    while (node->next != NULL) {
        node = node->next;
    }

    node->data = str;
    node->next = Node_create();
}


void List_insert(List *list, int index, char *str) {
    assert(list != NULL);
    assert(str !=NULL);
    assert(0 <= index);
    assert(index <= List_length(list));

    if (index == 0) {
        Node *after = list->first;
        list->first = Node_create();
        list->first->data = str;
        list->first->next = after;
    } else if (index == List_length(list)) {
        List_append(list, str);
    } else {
        Node *before = list->first;
        Node *after = list->first->next;
        while (index > 1) {
            index--;
            before = before->next;
            after = after->next;
        }
        before->next = Node_create();
        before->next->data = str;
        before->next->next = after;
    }
}


char *List_get(List *list, int index) {
    assert(list != NULL);
    assert(0 <= index);
    assert(index < List_length(list));

    Node *node = list->first;
    while (index > 0) {
        node = node->next;
        index--;
    }
    return node->data;
}


int List_find(List *list, char *str) {
    assert(list != NULL);
    assert(str != NULL);

    int index = 0;
    Node *node = list->first;
    while (node->next != NULL) {
        if (strlen(str) == strlen(node->data)) {
            int cmp = strcmp(str, node->data);
            if (cmp == 0) {
                return index;
            }
        }
        node = node->next;
        index++;
    }
    return -1;
}


void List_remove(List *list, int index) {
    assert(list != NULL);
    assert(0 <= index);
    assert(index < List_length(list));

    if (index == 0) {
        Node *node = list->first;
        list->first = list->first->next;
        Node_destroy(node);
    } else {
        Node *before = list->first;
        while (index > 1) {
            before = before->next;
            index--;
        }
        Node *node = before->next;
        before->next = before->next->next;
        Node_destroy(node);
    }
}


char *List_pop(List *list, int index) {
    assert(list != NULL);
    assert(0 <= index);
    assert(index < List_length(list));

    if (index == 0) {
        Node *node = list->first;
        list->first = list->first->next;
        char *data = node->data;
        Node_destroy(node);
        return data;
    } else {
        Node *before = list->first;
        while (index > 1) {
            before = before->next;
            index--;
        }
        Node *node = before->next;
        before->next = before->next->next;
        char *data = node->data;
        Node_destroy(node);
		
        return data;
    }
}


int List_length(List *list) {
    assert(list != NULL);

    Node *node = list->first;
    int length = 0;
    while (node->next != NULL) {
        length++;
        node = node->next;
    }

    return length;
}


void List_print(List *list) {
    assert(list != NULL);

    printf("[");
    Node *node = list->first;
    while (node->next != NULL) {
        printf("%s", node->data);
        node = node->next;
        if (node->next != NULL) {
            printf(", ");
        }
    }
    printf("]");
}




