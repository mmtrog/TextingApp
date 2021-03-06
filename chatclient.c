/*Client */
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <time.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/select.h>
#include "menu.h"
#include "common.h"
#include "login.h"

#define QUIT_STRING "/end"
#define CREATEROOM "/createroom"
#define GETROOM "/getroom"
#define LISTROOM "/listroom"
#define LISTUSERGROUP "/listusergroup"
#define KICKUSER "/kickuser"
#define HELP "/help"
#define TOUSER "/touser"
#define BUFF_SIZE 8192

//Colors code

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"


void showgroups(long lent, char *text)
{
	char *tptr,*a1,*length;
	length = strtok(text,"/");
	int length1 = atoi(length);
	tptr = text;
	a1 = text+strlen(text)+1;
	// printf("%d",lent);
	// printf("%s\n",a1);
	printf("%s", KBLU);
	printf("%18s %19s %19s\n", "Room's name", "Capacity", "Online");
	printf("%s", KNRM);
	for (int i=0;i<length1;i++)
	{
		char *name, *capa, *occu, *user;

		name = strtok(NULL,"/");
		// tptr = name + strlen(name) + 1;
		capa = strtok(NULL,"/");
		// tptr = capa + strlen(capa) + 1;
		occu = strtok(NULL,"/");
		// tptr = occu + strlen(occu) + 1;

		printf("%15s %19s %19s\n", name, capa, occu);
	}
}

void showgroups1(long lent, char *text)
{
	char *str;

	// tptr = text;
	// a1=text+strlen(text)+1;
	// printf("%d",lent);
	// printf("%s\n",a1);
	printf("%s", KBLU);
	printf("%18s\n","Room's name");
	printf("%s", KNRM);
	str = strtok(text,"/");
	while (str!=NULL)
	{
		
		printf("%18s\n", str);
		str=strtok(NULL,"/");
	}
}

/*
void showUser(long lent, char *text)
{
 	//char *tptr;

 	tptr = text;
 	printf("%18s %19s %19s\n", "Username", "Feeling", "Sock");
 	while (tptr < text + lent)
 	{
 		char *username, *status, *stt, *sock;
 		int st;

 		username = tptr;
 		tptr = username + strlen(username) + 1;
 		//printf("%s",username);
 		status = tptr;
 		tptr = status + strlen(status) + 1;

 		sock = tptr;
 		tptr = sock + strlen(sock) + 1;

		st = atoi(status);

		switch (st)
		{
		case 1:
			strcpy(stt, "Smile");
		 	break;
		case 2:
		 	strcpy(stt, "Sad");
		 	break;
		case 3:
		 	strcpy(stt, "Busy");
		 	break;
		case 4:
		 	strcpy(stt, "Need_Chat");
			break;
		}
 		printf("%18s %19s %19s\n", username, stt, sock);
 	} 
}
*/

void showUser1(char *text)
{
	char *tptr ;
	int st;
	tptr = text;
	printf("%s", KBLU);
	printf("%18s\n", "Username");
	printf("%s", KNRM);
	char *username,*status;

	username = strtok(text,"/");
	
	while (username!=NULL)
	{
		printf("%18s\n", username);
		username = strtok(NULL,"/");
	}
}

int update(int sock)
{
	Packet *pkt;
	char bufr[MAXPKTLEN];
	char *bufrptr;
	int bufrlen;
	char *status;
	printf("%s=== UPDATE ===\n", KYEL);
	printf("%s", KNRM);
	while (getchar() != '\n')
		;
	do
	{
		all_emotion();
		fgets(bufr, MAXPKTLEN, stdin);
	} while (atoi(bufr) > 4 || atoi(bufr) < 1);

	bufr[strlen(bufr) - 1] = '\0';
	
	if (strcmp(bufr, "") == 0 || strncmp(bufr, QUIT_STRING, strlen(QUIT_STRING)) == 0)
	{
		close(sock);
		exit(0);
	}
	status = strdup(bufr);

	/* G???i tin nh???n */
	bufrptr = bufr;
	strcpy(bufrptr, status);
	bufrptr += strlen(bufrptr) + 1;
	bufrlen = bufrptr - bufr;
	sendpkt(sock, UPDATE, bufrlen, bufr);
	/* Nh???n ph???n h???i t??? server */
	//printf("%s",&bufr);
	pkt = recvpkt(sock);
	if (!pkt)
	{
		printf("%sERROR: Server die!!!\n", KRED);
		printf("%s", KNRM);
		exit(1);
	}

	/*Error */
	if (pkt->type == JOIN_REJECTED)
	{
		printf("%s", KRED);
		printf("ADMIN: %s\n", pkt->text);
		printf("%s", KNRM);
		free(status);
		return (0);
	}
	else
	{
		//check[sock] = 0;
		printf("%s!\n", pkt->text);
		free(status);
		return 1;
	}
}

int sendListOn(int sock)
{
	Packet *pkt;
	/* Y??u c???u th??ng tin ph??ng chat */
	sendpkt(sock, LISTUSERON, 0, NULL);

	/* Nh???n ph???n h???i t??? ph??ng chat */
	pkt = recvpkt(sock);
	
	if (!pkt)
	{
		printf("%sERROR: Server die!!!\n", KRED);
		printf("%s", KNRM);
		exit(1);
	}

	if (pkt->type != LISTUSERON)
	{
		fprintf(stderr, "%sERROR: Unexpected reply from server2!!!\n", KRED);
		printf("%s", KNRM);
		exit(1);
	}

	/* Hi???n th??? ph??ng chat */
	showUser1(pkt->text);
	// showUser(pkt->lent, pkt->text);
	return 1;
}

int kickuser(int sock){
	Packet *pkt;
	char bufr[MAXPKTLEN];
	sendpkt(sock, LIST_USERGR, 0, NULL);

	/* Nh???n ph???n h???i t??? ph??ng chat */
	pkt = recvpkt(sock);
	
	if (!pkt)
	{
		printf("%sERROR: Server die!!!\n", KRED);
		printf("%s", KNRM);
		exit(1);
	}

	if (pkt->type != LIST_USERGR)
	{
		fprintf(stderr, "%sERROR: Unexpected reply from server3!!!\n", KRED);
		printf("%s", KNRM);
		exit(1);
	}

	/* Hi???n th??? ph??ng chat */
	showUser1(pkt->text);
	printf("%sKick username?\n", KYEL);
	printf("%s", KNRM);
	fgets(bufr, MAXPKTLEN, stdin);
	bufr[strlen(bufr) - 1] = '\0';
	sendpkt(sock, KICK, strlen(bufr)+1, bufr);
	pkt = recvpkt(sock);
	
	//printf("%s\n",pkt->text);
}

int sendListUserGr(int sock)
{
	Packet *pkt;
	/* Y??u c???u th??ng tin ph??ng chat */
	sendpkt(sock, LIST_USERGR, 0, NULL);

	/* Nh???n ph???n h???i t??? ph??ng chat */
	pkt = recvpkt(sock);
	
	if (!pkt)
	{
		printf("%sERROR: Server die!!!\n", KRED);
		printf("%s", KNRM);
		exit(1);
	}

	if (pkt->type != LIST_USERGR)
	{
		fprintf(stderr, "%sERROR: Unexpected reply from server3!!!\n", KRED);
		printf("%s", KNRM);
		exit(1);
	}

	/* Hi???n th??? ph??ng chat */
	showUser1(pkt->text);
	return 1;
}

int sendListGr(int sock)
{
	Packet *pkt;
	/* Y??u c???u th??ng tin ph??ng chat */
	sendpkt(sock, LIST_GROUPS, 0, NULL);

	/* Nh???n ph???n h???i t??? ph??ng chat */
	pkt = recvpkt(sock);
	
	if (!pkt)
	{
		printf("%sERROR: Server die!!!\n", KRED);
		printf("%s", KNRM);
		exit(1);
	}

	if (pkt->type != LIST_GROUPS)
	{
		fprintf(stderr, "%sERROR: Unexpected reply from server3!!!\n", KRED);
		printf("%s", KNRM);
		exit(1);
	}

	/* Hi???n th??? ph??ng chat */
	showgroups(pkt->lent, pkt->text);
	return 1;
}

/* Tham gia nh??m chat */
int joinagroup(int sock)
{

	Packet *pkt;
	char bufr[MAXPKTLEN];
	char *bufrptr;
	int bufrlen;
	char *gname;
	char *mname;

	/* Y??u c???u th??ng tin ph??ng chat */
	sendpkt(sock, LIST_GROUPS, 0, NULL);

	/* Nh???n ph???n h???i t??? ph??ng chat */
	pkt = recvpkt(sock);
	if (!pkt)
	{
		printf("%sERROR: Server die!!!\n", KRED);
		printf("%s", KNRM);
		exit(1);
	}

	if (pkt->type != LIST_GROUPS)
	{
		fprintf(stderr, "%sERROR: Unexpected reply from server4!!!\n", KRED);
		printf("%s", KNRM);
		exit(1);
	}

	/* Hi???n th??? ph??ng chat */
	showgroups(pkt->lent, pkt->text);
	
	/* T??n ph??ng chat */
	printf("%sWhich group?\n ", KYEL);
	printf("%s", KNRM);
	fgets(bufr, MAXPKTLEN, stdin);
	bufr[strlen(bufr) - 1] = '\0';

	/* Tho??t */
	if (strcmp(bufr, "") == 0 || strncmp(bufr, QUIT_STRING, strlen(QUIT_STRING)) == 0)
	{
		close(sock);
		exit(0);
	}
	gname = strdup(bufr);

	/* G???i tin nh???n */
	bufrptr = bufr;
	strcpy(bufrptr, gname);
	bufrptr += strlen(bufrptr) + 1;
	bufrlen = bufrptr - bufr;
	sendpkt(sock, JOIN_GROUP, bufrlen, bufr);
	
	/* Nh???n ph???n h???i t??? server */
	pkt = recvpkt(sock);
	
	if (!pkt)
	{
		printf("%sERROR: Server die!!!\n", KRED);
		printf("%s", KNRM);
		exit(1);
	}
	if (pkt->type != JOIN_ACCEPTED && pkt->type != JOIN_REJECTED)
	{
		fprintf(stderr, "%sERROR: Unexpected reply from server5!!!\n", KRED);
		printf("%s", KNRM);
		exit(1);
	}

	/*T??? ch???i cho v??o ph??ng */
	if (pkt->type == JOIN_REJECTED)
	{
		printf("%sADMIN: %s\n", KRED, pkt->text);
		printf("%s", KNRM);
		free(gname);
		return (0);
	}
	else /* Tham gia th??nh c??ng */
	{
		printf("%sADMIN: You joined '%s'!\n", KRED, gname);
		printf("%s", KNRM);
		printf("(%sPress '/help' to help or '/end' to exit!%s)\n", KYEL, KNRM);
		free(gname);
		return (1);
	}
}

int join11(int sock)
{

	Packet *pkt;
	char bufr[MAXPKTLEN];
	char *bufrptr;
	int bufrlen;
	char *uname;
	char *mname;

	/* Y??u c???u th??ng tin ph??ng chat */
	sendpkt(sock, LISTUSERON, 0, NULL);

	/* Nh???n ph???n h???i t??? ph??ng chat */
	pkt = recvpkt(sock);
	if (!pkt)
	{
		printf("%sERROR: Server die!!!\n", KRED);
		printf("%s", KNRM);
		exit(1);
	}

	if (pkt->type != LISTUSERON)
	{
		fprintf(stderr, "%sERROR: Unexpected reply from server6!!!\n", KRED);
		printf("%s", KNRM);
		exit(1);
	}

	/* Hi???n th??? ph??ng chat */
	showUser1(pkt->text);

	/* T??n ph??ng chat */
	printf("%s", KYEL);
	printf("Which account?\n ");
	printf("%s", KNRM);

	fgets(bufr, MAXPKTLEN, stdin);
	bufr[strlen(bufr) - 1] = '\0';
	uname = strdup(bufr);
	// /* Tho??t */
	// if (strcmp(bufr, "") == 0 || strncmp(bufr, QUIT_STRING, strlen(QUIT_STRING)) == 0)
	// {
	// 	close(sock);
	// 	exit(0);
	// }
	// uname = malloc(strlen(bufr) + 1);
	// strcpy(uname, bufr);//strcpy(uname,bufr);

	/* G???i tin nh???n */
	// bufrptr = bufr;
	// strcpy(bufrptr, uname);
	// bufrptr += strlen(bufrptr) + 1;
	// bufrlen = bufrptr - bufr;
	sendpkt(sock, JOIN_2, strlen(uname)+1, uname);

	/* Nh???n ph???n h???i t??? server */
	pkt = recvpkt(sock);
	
	if (!pkt)
	{
		printf("%sERROR: Server die!!!\n", KRED);
		printf("%s", KNRM);
		exit(1);
	}
	if (pkt->type != JOIN_ACCEPTED && pkt->type != JOIN_REJECTED && pkt->type != DONE)
	{
		fprintf(stderr, "%sERROR: Unexpected reply from server7!!!\n", KRED);
		printf("%s", KNRM);
		exit(1);
	}

	/*T??? ch???i cho v??o ph??ng */
	if (pkt->type == JOIN_REJECTED)
	{
		printf("%sADMIN%s: %s\n", KRED, KNRM, pkt->text);
		// free(uname);
		return (0);
	}
	else 
	if (pkt->type == DONE){
		printf("%sADMIN: %s refuse",KRED, uname);
		printf("%s", KNRM);
		return(0);
	} 
	else/* Tham gia th??nh c??ng */
	{
		printf("%sADMIN%s: You are chatting with '%s', have fun.\n", KRED, KNRM, uname);
		free(uname);
		return (1);
	}
}

int login(int sock, int *check)
{
	Packet *pkt;
	char bufr[MAXNAMELEN];
	char *bufrptr,*bufrptr1;
	int bufrlen;
	char *username, *pass;
	if (check[sock] == 1)
		return 1;
	//username
	printf("%s=== LOG IN ===\n", KYEL);
	printf("%s", KNRM);
	while (getchar() != '\n')
		;
	printf("%sUsername: ", KMAG);
	printf("%s", KNRM);
	fgets(bufr, MAXPKTLEN, stdin);
	bufr[strlen(bufr) - 1] = '\0';

	if (strcmp(bufr, "") == 0 || strncmp(bufr, QUIT_STRING, strlen(QUIT_STRING)) == 0)
	{
		close(sock);
		exit(0);
	}
	bufrptr1 = strdup(bufr);
	username = strdup(bufr);
	
	//pass
	printf("%sPassword: ", KMAG);
	printf("%s", KNRM);
	fgets(bufr, MAXPKTLEN, stdin);
	bufr[strlen(bufr) - 1] = '\0';

	if (strcmp(bufr, "") == 0 || strncmp(bufr, QUIT_STRING, strlen(QUIT_STRING)) == 0)
	{
		close(sock);
		exit(0);
	}
	pass = strdup(bufr);
	int i, j, n1, n2;
	strcat(bufrptr1,"/");
	strcat(bufrptr1, bufr);
	// strcat(bufrptr1, pass);
	//printf("%s",bufrptr1);
	/* G???i tin nh???n */
	bufrptr = bufr;
	strcpy(bufrptr, username);
	// printf("%s",bufrptr);
	bufrptr += strlen(bufrptr) + 1;
	strcpy(bufrptr, pass);
	bufrptr += strlen(bufrptr) + 1;
	bufrlen = bufrptr - bufr;
	sendpkt(sock, LOG_IN, bufrlen, bufrptr1);

	/* Nh???n ph???n h???i t??? server */
	pkt = recvpkt(sock);
	
	if (!pkt)
	{
		printf("%sERROR: Server die!!!\n", KRED);
		printf("%s", KNRM);
		exit(1);
	}

	/*LOG IN sai */
	if (pkt->type == JOIN_REJECTED)
	{
		printf("%sADMIN: %s\n", KRED, pkt->text);
		printf("%s", KNRM);
		free(username);
		free(pass);
		return (0);
	}
	else
	{
		check[sock] = 1;
		printf("%s!\n", pkt->text);
		free(username);
		free(pass);
		return 1;
	}
}

int sendRegister(int sock)
{
	Packet *pkt;
	char bufr[MAXNAMELEN];
	char *bufrptr,*bufrptr1;
	int bufrlen;
	char *username, *pass;
	printf("%s=== REGISTER ===\n", KYEL);
	printf("%s", KNRM);
	while (getchar() != '\n')
		;
	printf("%sUsername: ", KMAG);
	printf("%s", KNRM);
	fgets(bufr, MAXPKTLEN, stdin);
	bufr[strlen(bufr) - 1] = '\0';

	if (strcmp(bufr, "") == 0 || strncmp(bufr, QUIT_STRING, strlen(QUIT_STRING)) == 0)
	{
		close(sock);
		exit(0);
	}
	bufrptr1 = strdup(bufr);
	username = strdup(bufr);

	//pass
	printf("%sPassword: ", KMAG);
	printf("%s", KNRM);
	fgets(bufr, MAXPKTLEN, stdin);
	bufr[strlen(bufr) - 1] = '\0';

	if (strcmp(bufr, "") == 0 || strncmp(bufr, QUIT_STRING, strlen(QUIT_STRING)) == 0)
	{
		close(sock);
		exit(0);
	}
	pass = strdup(bufr);
	strcat(bufrptr1,"/");
	strcat(bufrptr1, bufr);
	// strcat(bufrptr1, pass);
	//printf("%s",bufrptr1);
	/* G???i tin nh???n */
	bufrptr = bufr;
	strcpy(bufrptr, username);
	bufrptr += strlen(bufrptr) + 1;
	strcpy(bufrptr, pass);
	bufrptr += strlen(bufrptr) + 1;
	bufrlen = bufrptr - bufr;
	sendpkt(sock, REGISTER, bufrlen, bufrptr1);

	/* Nh???n ph???n h???i t??? server */
	pkt = recvpkt(sock);
	if (!pkt)
	{
		printf("%sERROR: Server die!!!\n", KRED);
		printf("%s", KNRM);
		exit(1);
	}
	
	/*Error */
	if (pkt->type == JOIN_REJECTED)
	{
		printf("%sADMIN: %s\n", KRED, pkt->text);
		printf("%s", KNRM);
		free(username);
		free(pass);
		return (0);
	}
	else
	{
		printf("%s!\n", pkt->text);
		free(username);
		free(pass);
		return 1;
	}
}

int sendToUser(int sock){
	char name[MAXPKTLEN];
	char content[MAXPKTLEN],txt[MAXPKTLEN];
	printf("%sName: ", KMAG);
	printf("%s", KNRM);
	fgets(name, MAXPKTLEN, stdin);
	printf("%sContent: ", KMAG);
	printf("%s", KNRM);
	fgets(content, MAXPKTLEN, stdin);
	// name[strlen(name) - 1] = '\0';
	strcpy(txt,name);
	strcat(txt,"/");
	strcat(txt,content);		
	sendpkt(sock, TO, strlen(txt)+1 ,txt);
	// sendpkt(sock, TO, , "abcs");
	// pkt = recvpkt(sock);
	return 1;
}

int sendCreatRoom(int sock)
{
	Packet *pkt;
	char bufr[MAXNAMELEN];
	char *bufrptr,*bufrptr1;
	int bufrlen;
	char *name;
	char *cap;
	printf("%s=== CREATE ===\n", KYEL);
	printf("%s", KNRM);
	printf("%sRoom's name: ", KMAG);
	printf("%s", KNRM);
	fgets(bufr, MAXPKTLEN, stdin);
	bufr[strlen(bufr) - 1] = '\0';

	if (strcmp(bufr, "") == 0 || strncmp(bufr, QUIT_STRING, strlen(QUIT_STRING)) == 0)
	{
		close(sock);
		exit(0);
	}
	
	bufrptr1 = strdup(bufr);
	name = strdup(bufr);

	//cap
	printf("Capacity: ");
	fgets(bufr, MAXPKTLEN, stdin);
	bufr[strlen(bufr) - 1] = '\0';

	if (strcmp(bufr, "") == 0 || strncmp(bufr, QUIT_STRING, strlen(QUIT_STRING)) == 0)
	{
		close(sock);
		exit(0);
	}
	cap = strdup(bufr);
	strcat(bufrptr1,"/");
	strcat(bufrptr1,cap);
	
	bufrptr = bufr;
	strcpy(bufrptr, name);
	bufrptr += strlen(bufrptr) + 1;
	strcpy(bufrptr, cap);
	bufrptr += strlen(bufrptr) + 1;
	bufrlen = bufrptr - bufr;

	sendpkt(sock, CREAT_ROOM, bufrlen, bufrptr1);

	/* Nh???n ph???n h???i t??? server */
	pkt = recvpkt(sock);
	
	if (!pkt)
	{
		printf("%sERROR: Server die!!!\n", KRED);
		printf("%s", KNRM);
		exit(1);
	}

	/*Error */
	if (pkt->type == UNDONE)
	{
		printf("%sADMIN: %s\n", KRED, pkt->text);
		printf("%s", KNRM);
		free(name);
		free(cap);
		return (0);
	}
	else
	{
		printf("%s!\n", pkt->text);
		if(pkt->type == JOIN_REJECTED) {
			free(name);
			free(cap);
			return 0;
		}
		bufrptr = bufr;
		strcpy(bufrptr,name);
		bufrptr += strlen(bufrptr) + 1;
		bufrlen = bufrptr - bufr;
		sendpkt(sock, JOIN_GROUP, bufrlen, bufr);

		/* Nh???n ph???n h???i t??? server */
		pkt = recvpkt(sock);
		if (!pkt)
		{
			printf("%sERROR: Server die!!!\n", KRED);
			printf("%s", KNRM);
			exit(1);
		}
		if (pkt->type != JOIN_ACCEPTED && pkt->type != JOIN_REJECTED)
		{
			fprintf(stderr, "%sERROR: Unexpected reply from server5!!!\n", KRED);
			printf("%s", KNRM);
			exit(1);
		}

		/*T??? ch???i cho v??o ph??ng */
		if (pkt->type == JOIN_REJECTED)
		{
			printf("%sADMIN: %s\n", KRED, pkt->text);
			printf("%s", KNRM);
			free(name);
			return (0);
		}
		else /* Tham gia th??nh c??ng */
		{
			printf("%sADMIN%s: You joined %s'%s'!\n", KRED, KNRM, KYEL, name);
			printf("%s", KNRM);
			printf("(%sPress '/help' to help or '/end' to exit!%s)\n", KYEL, KNRM);
			free(name);
			return (1);
		}
		free(name);
		free(cap);
		return 1;
	}
}

int logout(int sock, int *check)
{
	Packet *pkt;
	char bufr[MAXNAMELEN];
	char *bufrptr;
	int bufrlen;
	char *username;
	printf("%s=== LOG OUT ===\n", KYEL);
	printf("%s", KNRM);
	printf("%sUsername: ", KMAG);
	printf("%s", KNRM);
	fgets(bufr, MAXPKTLEN, stdin);
	bufr[strlen(bufr) - 1] = '\0';

	if (strcmp(bufr, "") == 0 || strncmp(bufr, QUIT_STRING, strlen(QUIT_STRING)) == 0)
	{
		close(sock);
		exit(0);
	}
	username = strdup(bufr);

	/* G???i tin nh???n */
	bufrptr = bufr;
	strcpy(bufrptr, username);
	bufrptr += strlen(bufrptr) + 1;
	bufrlen = bufrptr - bufr;
	sendpkt(sock, LOG_OUT, bufrlen, bufr);

	/* Nh???n ph???n h???i t??? server */
	pkt = recvpkt(sock);
	
	if (!pkt)
	{
		printf("%sERROR: Server die!!!\n", KRED);
		printf("%s", KNRM);
		exit(1);
	}

	/*Error */
	if (pkt->type == JOIN_REJECTED)
	{
		printf("%sADMIN: %s\n", KRED, pkt->text);
		printf("%s", KNRM);
		free(username);
		return (0);
	}
	else
	{
		check[sock] = 0;
		printf("%s%s!\n", KRED, pkt->text);
		printf("%s", KNRM);
		free(username);
		return 1;
	}
}


int main(int argc, char *argv[])
{
	int choiceFunc = 0;

	char bufr1[MAXPKTLEN];
	int sock;
	/*????? ki???m tra tr???ng th??i ????ng nh???p */

	int check[MAXNAMELEN];
	memset(check, 0, MAXNAMELEN);

	/* Ki???m tra t??nh h???p l??? c???a c?? ph??p  */
	if (argc != 3)
	{
		printf("%sWrong syntax!!!\n--> Correct Syntax: ./client AddressIP PortNumber\n", KRED);
		printf("%s", KNRM);
		return 1;
	}

	/* K???t n???i vs m??y ch??? */
	sock = hooktoserver(argv[2], argv[1]);

	if (sock == -1)
		exit(1);

	fflush(stdout); /* X??a b??? ?????m */

	/* Kh???i t???o t???p th??m d?? */
	fd_set clientfds1, tempfds1;
	fd_set clientfds, tempfds;
	FD_ZERO(&clientfds);
	FD_ZERO(&clientfds1);
	FD_ZERO(&tempfds1);
	FD_ZERO(&tempfds);
	FD_SET(sock, &clientfds); /* Th??m sock v??o t???p clientfds */
	FD_SET(sock, &clientfds1);
	FD_SET(0, &clientfds); /* Th??m 0 v??o t???p clientfds */
	FD_SET(0, &clientfds1);
	
	
	char choice[100] = " ";
	while (1)
	{
		/*Menu login/logout */
		menu();
		// char choice=' ';
		strcpy(choice, " ");
		__fpurge(stdin);
		scanf("%s", &choice);
		//printf("%s",Gettype(choice));
		if (strcmp(choice, "3") == 0)
		{
			printf("%s-->Exit!\n", KRED);
			printf("%s", KNRM);
			exit(0);
		}
		if (strcmp(choice, "1") == 0)
		{
			sendRegister(sock);
			continue;
		}
		else if (strcmp(choice, "2") == 0)
		{
			if (!login(sock, check))
			{
				continue;
			}
			chatFunction();
			do
			{
				__fpurge(stdin);

				tempfds = clientfds;

				if (select(FD_SETSIZE, &tempfds, NULL, NULL, NULL) == -1)
				{
					perror("ERROR: select(...)");
					exit(4);
				}

				if (FD_ISSET(0, &tempfds))
				{
					fgets(bufr1, MAXPKTLEN, stdin);
					sendpkt(sock, MENU, strlen(bufr1), bufr1);
				}
				/* X??? l?? th??ng tin t??? m??y ch??? */
				if (FD_ISSET(sock, &tempfds))
				{
				    fflush(stdin);
					Packet *pkt1;
					//char *tm;
					
					pkt1 = recvpkt(sock);
					
					if (!pkt1)
					{
						/* M??y ch??? ng???ng ho???t ?????ng */
						printf("%sERROR: Server die!!!\n", KRED);
						printf("%s", KNRM);
						exit(1);
					}

					/* Hi???n th??? tin nh???n v??n b???n */
					if (pkt1->type != MENU && pkt1->type != REQUEST)
					{
						fprintf(stderr, "%sERROR: Unexpected reply from server!!!\n", KRED);
						printf("%s", KNRM);
						exit(1);
					}

					else if (pkt1->type == REQUEST)
					{	
							Packet *pkt2;
							
							int pkt1_len = pkt1->lent;
						    
							char *uname;
							// uname=strtok(pkt1->text,":");
							
							char bufr[MAXPKTLEN],tl[MAXPKTLEN];
							// snprintf(uname, pkt1_len + 1, "%s", pkt1->text);
							
							printf("%sADMIN%s: Do you want to chat with %s'%s' \n%s'y'%s: accept or %s'n'%s: refuse\n", KRED, KNRM, KBLU, pkt1->text, KBLU, KNRM, KRED, KNRM);
							
							/* Ti???p t???c tr?? chuy???n */
							
							fgets(bufr, MAXPKTLEN, stdin);

							bufr[strlen(bufr) - 1] = '\0';
							strcpy(tl,bufr);
							strcat(tl,"/");
							strcat(tl,pkt1->text);
							
							// printf("%s",tl);
							sendpkt(sock,REQUEST1,strlen(tl)+1,tl);
							
							pkt2 = recvpkt(sock);
							
							if (pkt2->type == SUCCESS)
							{
								printf("%sADMIN%s: You are chatting with '%s', have fun.\n", KRED, KNRM, pkt1->text);
								while (1)
								{
									/* G???i select ????? theo d??i th??ng tin b??n ph??m v?? m??y ch??? */
									
									tempfds = clientfds;

									if (select(FD_SETSIZE, &tempfds, NULL, NULL, NULL) == -1)
									{
										perror("ERROR: select(...)");
										exit(4);
									}

									fflush(stdout);

									/* C??c b??? trong tempfds ki???m tra xem c?? ph???i l?? b??? socket k? N???u c?? ngh??a l?? m??y ch??? g???i tin nh???n
								, c??n n???u kh??ng th?? nh???p tin nh???n ????? g???i ?????n may ch??? */

									/* X??? l?? th??ng tin t??? m??y ch??? */
									if (FD_ISSET(sock, &tempfds))
									{
										Packet *pkt;
										pkt = recvpkt(sock);
										if (!pkt)
										{
											/* M??y ch??? ng???ng ho???t ?????ng */
											printf("%sERROR: Server die!!!\n", KRED);
											printf("%s", KNRM);
											exit(1);
										}

										/* Hi???n th??? tin nh???n v??n b???n */
										if (pkt->type != USER_TEXT1 && pkt->type != QUIT)
										{
											fprintf(stderr, "%sERROR: Unexpected reply from server!!!\n", KRED);
											printf("%s", KNRM);
											exit(1);
										}

										if (pkt->type == QUIT) break;
										printf("%sReply%s: %s", KBLU, KNRM, pkt->text);
										freepkt(pkt);
									}
									/* X??? l?? ?????u v??o */
									if (FD_ISSET(0, &tempfds))
									{
										char bufr[MAXPKTLEN];

										fgets(bufr, MAXPKTLEN, stdin);

										if (strncmp(bufr, QUIT_STRING, strlen(QUIT_STRING)) == 0)
										{
											/* Tho??t kh???i phong chat */
											sendpkt(sock, QUIT, 0, NULL);
											break;
										}

										/*G???i tin nh???n ?????n m??y ch??? */
										sendpkt(sock, USER_TEXT1, strlen(bufr) + 1, bufr);
									}
								}	
							} else 
							{
								// chatFunction();
							}
							chatFunction();
							// break;
					}
					else if (pkt1->type == MENU)
					{
						choiceFunc = atoi(pkt1->text);
						switch (choiceFunc)
						{
						case 0:
							{
								sendListOn(sock);
								break;
							}
						case 1:
							/*Tao phong */
							{
								if(!sendCreatRoom(sock)){
									break;
								};
								while (1)
								{
									/* G???i select ????? theo d??i th??ng tin b??n ph??m v?? m??y ch??? */
									tempfds = clientfds;

									if (select(FD_SETSIZE, &tempfds, NULL, NULL, NULL) == -1)
									{
										perror("select");
										exit(4);
									}


									/* X??? l?? th??ng tin t??? m??y ch??? */
									if (FD_ISSET(sock, &tempfds))
									{

										Packet *pkt;
										pkt = recvpkt(sock);
										
										if (!pkt)
										{
											/* M??y ch??? ng???ng ho???t ?????ng */
											printf("%sERROR: Server die!!!\n", KRED);
											printf("%s", KNRM);
											exit(1);
										}
										if (pkt->type == REQUEST)
											{	
												Packet *pkt2;
												int pkt1_len = pkt->lent;
												char *uname;
												// uname=strtok(pkt1->text,":");
												char bufr[MAXPKTLEN],tl[MAXPKTLEN];
												// snprintf(uname, pkt1_len + 1, "%s", pkt1->text);
												printf("%sADMIN%s: Do you want to chat with %s'%s' \n%s'y'%s: accept or %s'n'%s: refuse\n", KRED, KNRM, KBLU, pkt1->text, KBLU, KNRM, KRED, KNRM);
												printf("%s", KNRM);
												/* Ti???p t???c tr?? chuy???n */
												fgets(bufr, MAXPKTLEN, stdin);
												bufr[strlen(bufr) - 1] = '\0';
												strcpy(tl,bufr);
												strcat(tl,"/");
												strcat(tl,pkt->text);
												// printf("%s",tl);
												sendpkt(sock,REQUEST1,strlen(tl)+1,tl);
												pkt2 = recvpkt(sock);
												if (pkt2->type == SUCCESS){
													printf("%sADMIN%s: Now, you are chatting with '%s' \n", KRED, KNRM, pkt->text);
													while (1)
													{
														/* G???i select ????? theo d??i th??ng tin b??n ph??m v?? m??y ch??? */
														tempfds = clientfds;
														if (select(FD_SETSIZE, &tempfds, NULL, NULL, NULL) == -1)
														{
															perror("select");
															exit(4);
														}
														fflush(stdout);

														/* C??c b??? trong tempfds ki???m tra xem c?? ph???i l?? b??? socket k? N???u c?? ngh??a l?? m??y ch??? g???i tin nh???n
													, c??n n???u kh??ng th?? nh???p tin nh???n ????? g???i ?????n may ch??? */

														/* X??? l?? th??ng tin t??? m??y ch??? */
														if (FD_ISSET(sock, &tempfds))
														{
															Packet *pkt3;
															pkt3 = recvpkt(sock);
															if (!pkt3)
															{
																/* M??y ch??? ng???ng ho???t ?????ng */
																printf("%sERROR: Server die!!!\n", KRED);
																printf("%s", KNRM);
																exit(1);
															}

															/* Hi???n th??? tin nh???n v??n b???n */
															if (pkt3->type != USER_TEXT1 && pkt3->type != QUIT && pkt3->type != USER_TEXT)
															{
																fprintf(stderr, "%sERROR: Unexpected reply from server!!!\n", KRED);
																printf("%s", KNRM);
																exit(1);
															}
															if (pkt3->type == QUIT) {
																printf("%s quit\n",pkt->text);
																// sendpkt(sock, QUIT, 0, NULL);
																break;
															} else 
															if (pkt3->type == USER_TEXT1 ){
																printf("recv: %s", pkt3->text);
																freepkt(pkt3);
															}
														
														}
														/* X??? l?? ?????u v??o */
														if (FD_ISSET(0, &tempfds))
														{
															char bufr[MAXPKTLEN];
															fgets(bufr, MAXPKTLEN, stdin);
															if (strncmp(bufr, QUIT_STRING, strlen(QUIT_STRING)) == 0)
															{
																/* Tho??t kh???i phong chat */
																sendpkt(sock, QUIT, 0, NULL);
																break;
															}

															/*G???i tin nh???n ?????n m??y ch??? */
															sendpkt(sock, USER_TEXT1, strlen(bufr) + 1, bufr);
														}
													}	
												} else {
													// chatFunction();
												}
												// chatFunction();
												// break;
											}

										/* Hi???n th??? tin nh???n v??n b???n */
										if (pkt->type != USER_TEXT && pkt->type != REQUEST && pkt->type != KICKU)
										{
											fprintf(stderr, "ERROR: Unexpected reply from serve1!!!\n", KRED);
											printf("%s", KNRM);
											exit(1);
										}
										if (pkt->type == KICKU){
											sendpkt(sock, LEAVE_GROUP, 0, NULL);
											break;
										}
										if ( pkt->type == USER_TEXT ){
											char *us,*txt;
											us=strtok(pkt->text,"/");
											txt=strtok(NULL,"/");
											printf("%s: %s", us, txt);
											freepkt(pkt);
										}
										
									}
									/* X??? l?? ?????u v??o */
									if (FD_ISSET(0, &tempfds))
									{
										char bufr[MAXPKTLEN];
										fgets(bufr, MAXPKTLEN, stdin);
										if (strncmp(bufr, QUIT_STRING, strlen(QUIT_STRING)) == 0)
										{
											/* Tho??t kh???i phong chat */
											sendpkt(sock, LEAVE_GROUP, 0, NULL);
											break;
										}
										if (strncmp(bufr, KICKUSER, strlen(KICKUSER)) == 0)
										{
											kickuser(sock);
											// sendListUserGr(sock);
											// break;
											// sendpkt(sock,KICK,0,NULL);
										}else
										if (strncmp(bufr, LISTUSERGROUP, strlen(LISTUSERGROUP)) == 0)
										{
											sendListUserGr(sock);
											// break;
										}else
										if (strncmp(bufr, LISTROOM, strlen(LISTROOM)) == 0)
										{
											sendListGr(sock);
											// break;
										}else
										if (strncmp(bufr, CREATEROOM, strlen(CREATEROOM)) == 0)
										{
											if(!sendCreatRoom(sock)){
												break;
											};
											// break;
										}else
										if (strncmp(bufr, GETROOM, strlen(GETROOM)) == 0)
										{
											if (!joinagroup(sock))
												continue;
											// break;
										}else 
										if (strncmp(bufr, TOUSER, strlen(TOUSER)) == 0)
										{
											if(!sendToUser(sock)){
												break;
											};
										}else 
										if (strncmp(bufr, HELP, strlen(HELP)) == 0)
										{
											menuhelp();
											// break;
										} else {
											sendpkt(sock, USER_TEXT, strlen(bufr) + 1, bufr);
										}
										

										/*G???i tin nh???n ?????n m??y ch??? */
									}
								}
								break;
							}
						case 2: /*V??o ph??ng */
							/* Tham gia tr?? chuy???n */
							{
								if (!joinagroup(sock)){
									chatFunction();
									continue;
								}

								/* Ti???p t???c tr?? chuy???n */
								while (1)
								{
									/* G???i select ????? theo d??i th??ng tin b??n ph??m v?? m??y ch??? */
									tempfds = clientfds;

									if (select(FD_SETSIZE, &tempfds, NULL, NULL, NULL) == -1)
									{
										perror("select");
										exit(4);
									}

									/* C??c b??? trong tempfds ki???m tra xem c?? ph???i l?? b??? socket k? N???u c?? ngh??a l?? m??y ch??? g???i tin nh???n
							, c??n n???u kh??ng th?? nh???p tin nh???n ????? g???i ?????n may ch??? */

									/* X??? l?? th??ng tin t??? m??y ch??? */
									if (FD_ISSET(sock, &tempfds))
									{

										Packet *pkt;
										pkt = recvpkt(sock);
										
										if (!pkt)
										{
											/* M??y ch??? ng???ng ho???t ?????ng */
											printf("%sERROR: Server die!!!\n", KRED);
											printf("%s", KNRM);
											exit(1);
										}
										if (pkt->type == REQUEST)
											{	
												Packet *pkt2;
												int pkt1_len = pkt->lent;
												char *uname;
												// uname=strtok(pkt1->text,":");
												char bufr[MAXPKTLEN],tl[MAXPKTLEN];
												// snprintf(uname, pkt1_len + 1, "%s", pkt1->text);
												printf("%sADMIN%s: Do you want to chat with %s'%s' \n%s'y'%s: accept or %s'n'%s: refuse\n", KRED, KNRM, KBLU, pkt1->text, KBLU, KNRM, KRED, KNRM);
												printf("%s", KNRM);
												/* Ti???p t???c tr?? chuy???n */
												fgets(bufr, MAXPKTLEN, stdin);
												bufr[strlen(bufr) - 1] = '\0';
												strcpy(tl,bufr);
												strcat(tl,"/");
												strcat(tl,pkt->text);
												// printf("%s",tl);
												sendpkt(sock,REQUEST1,strlen(tl)+1,tl);
												pkt2 = recvpkt(sock);
												if (pkt2->type == SUCCESS){
													printf("%sADMIN%s: Now, you are chatting with '%s' \n", KRED, KNRM, pkt->text);
													while (1)
													{
														/* G???i select ????? theo d??i th??ng tin b??n ph??m v?? m??y ch??? */
														tempfds = clientfds;
														if (select(FD_SETSIZE, &tempfds, NULL, NULL, NULL) == -1)
														{
															perror("select");
															exit(4);
														}
														fflush(stdout);

														/* C??c b??? trong tempfds ki???m tra xem c?? ph???i l?? b??? socket k? N???u c?? ngh??a l?? m??y ch??? g???i tin nh???n
													, c??n n???u kh??ng th?? nh???p tin nh???n ????? g???i ?????n may ch??? */

														/* X??? l?? th??ng tin t??? m??y ch??? */
														if (FD_ISSET(sock, &tempfds))
														{
															Packet *pkt3;
															pkt3 = recvpkt(sock);
															if (!pkt3)
															{
																/* M??y ch??? ng???ng ho???t ?????ng */
																printf("%sERROR: Server die!!!\n", KRED);
																printf("%s", KNRM);
																exit(1);
															}

															/* Hi???n th??? tin nh???n v??n b???n */
															if (pkt3->type != USER_TEXT1 && pkt3->type != QUIT && pkt3->type != USER_TEXT)
															{
																fprintf(stderr, "%sERROR: Unexpected reply from server!!!\n", KRED);
																printf("%s", KNRM);
																exit(1);
															}
															if (pkt3->type == QUIT) {
																printf("%s Quit\n",pkt->text);
																// sendpkt(sock, QUIT, 0, NULL);
																break;
															} else 
															if (pkt3->type == USER_TEXT1 ){
																printf("%sReply%s: %s", KBLU, KNRM, pkt3->text);
																freepkt(pkt3);
															}
														
														}
														/* X??? l?? ?????u v??o */
														if (FD_ISSET(0, &tempfds))
														{
															char bufr[MAXPKTLEN];
															fgets(bufr, MAXPKTLEN, stdin);
															if (strncmp(bufr, QUIT_STRING, strlen(QUIT_STRING)) == 0)
															{
																/* Tho??t kh???i phong chat */
																printf("%sYou out chat with %s\n", KYEL, pkt->text);
																printf("%s", KNRM);
																sendpkt(sock, QUIT, 0, NULL);
																break;
															}

															/*G???i tin nh???n ?????n m??y ch??? */
															sendpkt(sock, USER_TEXT1, strlen(bufr) + 1, bufr);
														}
													}	
												} else {
													// chatFunction();
												}
												// chatFunction();
												// break;
											}

										/* Hi???n th??? tin nh???n v??n b???n */
										if (pkt->type != USER_TEXT && pkt->type != REQUEST && pkt->type != KICKU)
										{
											fprintf(stderr, "%sERROR: unexpected reply from serve1r!!!\n", KRED);
											printf("%s", KNRM);
											exit(1);
										}
										if (pkt->type == KICKU){
											sendpkt(sock, LEAVE_GROUP, 0, NULL);
											break;
										} 
										if ( pkt->type == USER_TEXT ){
											char *us,*txt;
											us=strtok(pkt->text,"/");
											txt=strtok(NULL,"/");
											printf("%s%s%s: %s", KBLU, us, KNRM, txt);
											freepkt(pkt);
										}
										
									}
									/* X??? l?? ?????u v??o */
									if (FD_ISSET(0, &tempfds))
									{
										char bufr[MAXPKTLEN];
										fgets(bufr, MAXPKTLEN, stdin);
										if (strncmp(bufr, QUIT_STRING, strlen(QUIT_STRING)) == 0)
										{
											/* Tho??t kh???i phong chat */
											sendpkt(sock, LEAVE_GROUP, 0, NULL);
											break;
										}
										if (strncmp(bufr, KICKUSER, strlen(KICKUSER)) == 0)
										{
											kickuser(sock);
											// sendListUserGr(sock);
											// break;
											// sendpkt(sock,KICK,0,NULL);
										}else
										if (strncmp(bufr, LISTUSERGROUP, strlen(LISTUSERGROUP)) == 0)
										{
											sendListUserGr(sock);
											// break;
										}else
										if (strncmp(bufr, LISTROOM, strlen(LISTROOM)) == 0)
										{
											sendListGr(sock);
											// break;
										}else
										if (strncmp(bufr, CREATEROOM, strlen(CREATEROOM)) == 0)
										{
											if(!sendCreatRoom(sock)){
												break;
											};
											// break;
										}else
										if (strncmp(bufr, GETROOM, strlen(GETROOM)) == 0)
										{
											if (!joinagroup(sock))
												continue;
											// break;
										}else 
										if (strncmp(bufr, TOUSER, strlen(TOUSER)) == 0)
										{
											if(!sendToUser(sock)){
												break;
											};
										}else 
										if (strncmp(bufr, HELP, strlen(HELP)) == 0)
										{
											menuhelp();
											// break;
										} else {
											sendpkt(sock, USER_TEXT, strlen(bufr) + 1, bufr);
										}
										

										/*G???i tin nh???n ?????n m??y ch??? */
									}
								}
								break;
							}
						case 3: /*Xem danh s??ch ph??ng */
						{
							sendListGr(sock);
							break;
						}
						case 4:
						{
							update(sock);
							break;
						}
						case 5:
						{
							if (join11(sock)==1)
							{

							/* Ti???p t???c tr?? chuy???n */
								while (1)
								{
									/* G???i select ????? theo d??i th??ng tin b??n ph??m v?? m??y ch??? */
									tempfds = clientfds;
									if (select(FD_SETSIZE, &tempfds, NULL, NULL, NULL) == -1)
									{
										perror("select");
										exit(4);
									}

									/* C??c b??? trong tempfds ki???m tra xem c?? ph???i l?? b??? socket k? N???u c?? ngh??a l?? m??y ch??? g???i tin nh???n
								, c??n n???u kh??ng th?? nh???p tin nh???n ????? g???i ?????n may ch??? */

									/* X??? l?? th??ng tin t??? m??y ch??? */
									if (FD_ISSET(sock, &tempfds))
									{

										Packet *pkt;
										pkt = recvpkt(sock);
										if (!pkt)
										{
											/* M??y ch??? ng???ng ho???t ?????ng */
											printf("%sERROR: Server die!!!\n", KRED);
											printf("%s", KNRM);
											exit(1);
										}

										/* Hi???n th??? tin nh???n v??n b???n */
										if (pkt->type != USER_TEXT1 && pkt->type != QUIT)
										{
											fprintf(stderr, "%sERROR: unexpected reply from serve1r!!!\n", KRED);
											printf("%s", KNRM);
											exit(1);
										}
										if (pkt->type == QUIT) break;
										printf("%sReply%s: %s", KBLU, KNRM, pkt->text);
										freepkt(pkt);
									}
									/* X??? l?? ?????u v??o */
									if (FD_ISSET(0, &tempfds))
									{
										char bufr[MAXPKTLEN];
										fgets(bufr, MAXPKTLEN, stdin);
										if (strncmp(bufr, QUIT_STRING, strlen(QUIT_STRING)) == 0)
										{
											/* Tho??t kh???i phong chat */
											sendpkt(sock, QUIT, 0, NULL);
											break;
										}

										/*G???i tin nh???n ?????n m??y ch??? */
										sendpkt(sock, USER_TEXT1, strlen(bufr) + 1, bufr);
									}
								}
							} else {
								break;
							}
							break;
						}
						case 6:
						{
							logout(sock, check);
							break;
						}
						}
						if(choiceFunc!=6) {chatFunction();}
					}
					freepkt(pkt1);
				}
				/* X??? l?? ?????u v??o */
			} while (choiceFunc != 6);
			choiceFunc = -1;
		}
		else
		{
			printf("%sPlease type again!%s\n", KRED, KNRM);
			continue;
		}
	}
}
