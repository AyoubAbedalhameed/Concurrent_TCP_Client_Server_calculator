/* NES416 Assignmet 1: Client_Server_Net_Calculator 
(Server_Code File)  
 > Mar 23 2021
 > Team members:- 
         * Ayoub Abedalhameed 124052
         * Zeina Falah 128652  */ 




#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>
#include <sys/errno.h>

//Fuction calc: calculate expr, return the (float) result
double calc (const int* op1, const int* op2, const char* operation )
{
switch (*operation)
{
case '+': return (double)(*op1+*op2);
case '-': return (double)(*op1-*op2);
case '/': return ( (double) *op1 / (double) *op2 ) ;
case '*': return (double) ((*op1)*(*op2)) ;

default: return -1 ; 
}}


// Function CheckSymbol: Check passed char, if it is valid symb 
char CheckSymbol (char symb)

{if (symb == '+' || symb == '-' || symb == '/' || symb =='*') return symb ; 
else { printf("Error while determining operation symbol"); return -1 ;}}





//GetExp: it process the string expression if it is valide > it returns the operands & operation or it returns if it is Not.
// Deals with all Exp' format cases, whether it includes spaces or not & regardless where the spaces are (Beggining,Middle, Last). 
//Limitations: Buffer Size, Integers Only, 4 Valid operations  (+, -, /, *).


int GetExp (char* exp, int* op1, char* operation, int* op2)

{

int size = strlen(exp) ;          // Size of passed "Exp"
//printf("size = %d", size);
int start = 0 ; int end = 0 ; 
char Temp [50] ;                  //Local Buff


// TestF: printf("@GetExp Executing ..\n" ) ; 
// Detreminig the first operand: 
// TestF: printf("@Determinig The First Op .. \n") ; 
for(int i = 0 ; i<size ; i++)
{
if (exp[i]== ' ' && (start == end) )  //The case when expression starts with #ofspaces. 
{start=start+1; end=end+1; /*printf("case1\n"); */continue ; }


else if (exp[i]>=48 && exp[i]<= 57)     //Is it Num. ? 
{end +=1 ; /*printf("case2\n");  */continue;}
else break; 
}

//printf("start:end %d:%d", start,end); 
if (start != end)    
{
strncpy(Temp, exp+start, (end - start)); 
*op1 = atoi(Temp);

} else {printf("@Error in stage1\n"); return -1 ; }            //Expression INVALID





// Finding Operation Sym:

for (int i = end ; i < size ; i++ )
{

if (exp[i]== ' ' ) continue ; 
else  if (CheckSymbol(exp[i]) != -1) 
{*operation = exp[i] ; end=i+1; break; }
else {printf("@Error in stage2\n");return -1 ;}  //Expression INVALID : No_Operation_Founded
}




// Determining Second operand. 

start=end ; 

// printf("start:end:size   %d:%d:%d\n", start, end,size);

for (int i = end ; i < size ; i++ )
{

if (exp[i]== ' ' && (start == end) ) 
{start++; end++; continue ; }
 else if (exp[i]>=48 && exp[i]<= 57)
{end +=1 ;  continue;}
else break ; 
}
if (start != end)

{strncpy(Temp, exp+start, end-start+1); 
*op2 = atoi(Temp);} 

else { printf("@Error in stage3\n");return -1 ;} //Expression INVALID
return 0 ; 
}













int main (int argc, char** argv)
{

// Checking command-line arguments count. (Must be 1)
if (argc != 2 ) {printf("Invalid Argument Count, usag: Server <Listining Port>"); return -1 ; } 





// Allocating Buff's for sending/receiving data. 
char SendBuff [60] ; 
char ReceiveBuff [60] ; 

int op1, op2; 
    float result = -1 ; 
    char operation = ' '; 
 
int ServerSock ; 

// Creating Socket: 
ServerSock = socket(AF_INET, SOCK_STREAM, 0 );
if(ServerSock<0) 
{ perror("Error While Creating Server Socket\n");   return -1 ;}
 printf("Socket Created Successfully\n"); 

// Initializing SockAddress Structure: 
struct sockaddr_in  ServerLocal ; 

memset( &ServerLocal, 0, sizeof(ServerLocal));
ServerLocal.sin_family = AF_INET; 
ServerLocal.sin_port= htons(atoi(argv[1])) ; 
ServerLocal.sin_addr.s_addr= INADDR_ANY; 



//Binding Socket 
if (       bind(ServerSock, (struct sockaddr*) &ServerLocal, sizeof(ServerLocal))      <0)
    { perror("Warnning:"); printf("\nPlease choose different port# or Wait 4Min, (This Port in TIME_WAIT state./n)"); return -1; } 

// Switch Socket to the passive-listining State. 
if (listen(ServerSock, 5)<0)
     { perror("Error While Binding the Socket to the local address"); return -1 ; }
printf("Socket State switched To: PASSIVE_LISTENNING\n") ; 


// Declaring SockAdd Structure for the client. (used in accept())
 struct sockaddr_in  ClientAdd ; 
memset( &ClientAdd, 0, sizeof(ClientAdd));
socklen_t ClientAddLength ; 
int NewSock ; 




 
 // Acccepting Connections: 



while (1)
{ 

     printf("\n  .... Waiting for New Session ....\n") ;     

NewSock = accept(ServerSock, (struct sockaddr * ) &ClientAdd, &ClientAddLength);
if (NewSock<0)
     { perror("While Accepting Connection"); return -1 ; }
printf("Connection Accepted from: %s for Sock#:%d\n", inet_ntoa(ClientAdd.sin_addr), NewSock) ;

//send(NewSock, welcome, strlen(welcome), 0);
// write (NewSock, welcome, strlen(welcome));

printf("\n __________________ NEW SESSION_____________________\n\n") ;
ssize_t dataSize = 0 ; 
while (1)
{
// printf("While#%d", ++counter) ; 


memset(SendBuff,0, sizeof(SendBuff));
memset(ReceiveBuff,0, sizeof(ReceiveBuff));

printf ("Waiting for client request:\n") ; 
if (read (NewSock, ReceiveBuff, sizeof(ReceiveBuff)) == -1 ) {printf ("Error while reading socket.\n") ; return -1 ; }


printf("Client:(%s, %d):", inet_ntoa(ClientAdd.sin_addr) ,(int) ntohs(ClientAdd.sin_port) ) ; 
printf("%s\n", ReceiveBuff);

//Processing the requested epression: 


if(  strcmp("exit", ReceiveBuff)  ==0  )   /* checking if client send exit, the server close 
                                              the R/W Socket but keeps the listen socket */ 
 {printf("\n @Connection Closed \n"); break ;} 

if(   GetExp(ReceiveBuff, &op1, &operation, &op2)  == -1 ) {printf("/nError While Processing Exp\n"); break;}  

printf("Numeric Exp is: (%d%c%d)\n",op1,operation, op2) ; 
result = calc(&op1, &op2, &operation) ;


printf("@Sending Result to the client: (%f)\n", result ) ;  

sprintf(SendBuff, "%f",result) ; //Copying the result to the send buff. //Test: printf(" The Send buffur, Check ?%s\n", SendBuff) ; 

if (write (NewSock, SendBuff, sizeof(SendBuff)) == -1)          // Sending Buffer: 
{printf("Error while writing on the socket, you can try again. \n"); break;}

printf("\n___________________________________________________________\n")  ;

}

close(NewSock) ;} //Closing Client_Serving_Socket}

close(ServerSock); // Closing Listening socket. 


return 0 ; 
}