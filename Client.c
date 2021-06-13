/* NES416 Assignmet 1: Client_Server_Net_Calculator 
(Client_Code File)  
 > Mar 23 2021
 > Team members:- 
         * Ayoub Abedalhameed 124052
         * Zeina Falah 128652  */ 



#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>
 #include <unistd.h>
 #include <errno.h>
 
int main (int argc, char** argv)
{




// Checking command-line arguments count. (Must be 2)
if (argc != 3) {printf("Invalid Argument Count, usag: Server <Listining Port>"); return -1 ; } 

/*
printf("Given Serevr IP: %s\n" , argv[1]) ;
printf("Given Server Port: %s\n" , argv[2]) ;
*/
//Initializing SOcket Address Structure: 

char rcvbuff[60] ; 
char sendbuff[60] ; 
struct sockaddr_in ClientAdd ; 

int op1, op2 ;
    char operation ;
int x = 0 ;



memset(&ClientAdd, 0 , sizeof(ClientAdd)) ;

ClientAdd.sin_family= AF_INET ;
ClientAdd.sin_port= htons(55052) ; 
ClientAdd.sin_addr.s_addr= INADDR_ANY ;


//Creating Client Socket:

int MySock ; 
MySock = socket( AF_INET,  SOCK_STREAM, 0); 
if(MySock<0)
{perror("Error While Creating Client Socket%d"); return -1 ;}
printf("Socket Created Successfully: SockNO: %d\n",MySock); 



// Binding Client Socket to the Local P_Address 

if(bind(MySock, (struct sockaddr*) &ClientAdd, sizeof(ClientAdd)) < 0 )  //Test: printf("(%s, %d)", inet_ntoa(ClientAdd.sin_addr) ,(int)ntohs(ClientAdd.sin_port) ) ; 

{  perror("Warning:"); printf("Kernel may choose new local port#\n") ; }
printf("Socket Binded Success.\n") ; 
//Test: printf("(%s, %d)", inet_ntoa(ClientAdd.sin_addr) ,(int)ntohs(ClientAdd.sin_port) ) ; 




// Intilaizing Server Socket Address Structure: 

struct sockaddr_in ServerAddrStructure ; 
memset(&ServerAddrStructure, 0 , sizeof(ServerAddrStructure)) ;
ServerAddrStructure.sin_family= AF_INET ;
ServerAddrStructure.sin_port= htons(   atoi(argv[2])   ) ;  
inet_aton(argv[1], &ServerAddrStructure.sin_addr);



printf ("Built Server Address: %s\n", inet_ntoa(ServerAddrStructure.sin_addr)) ; 
int NewSock ; 

// Connecting To the Server: 
if ( connect(MySock, (struct sockaddr*) &ServerAddrStructure, sizeof(ServerAddrStructure)) < 0)
{perror("Error While Connecting to the Server: "); return -1 ; }

printf("\n\n-> Welcome to NetCalculater!\n\n"); 

//recv(MySock, rcvbuff, sizeof(rcvbuff), 0 );
//read (MySock, rcvbuff, sizeof(rcvbuff) ); 
//printf("from server:%s\n", rcvbuff) ; 

 

do{

printf("\nPlease Enter Expression or Type (exit) to quit.\n") ;
fgets(sendbuff, sizeof(sendbuff), stdin); 
sendbuff[strlen(sendbuff)-1] = '\0';
//Test:printf("Local exp is :%s\n", sendbuff ) ; 

if(write (MySock, sendbuff, strlen(sendbuff)) == -1 )
{perror("Error While Sending Buffer:"); return -1 ;}

//recv(MySock, rcvbuff, sizeof(rcvbuff), 0 );
read (MySock, rcvbuff, sizeof(rcvbuff)); 
printf("(%s, %d)", inet_ntoa(ServerAddrStructure.sin_addr) ,(int)ntohs(ServerAddrStructure.sin_port) ) ; 
printf("  %s\n", rcvbuff) ; 

}while (strcmp("exit", sendbuff)!=0) ; 


printf("\nClient Terminates ... Good Bye\n\n") ; 


close(MySock) ; 


return 0 ;   
}
