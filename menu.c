#include <stdio.h>

//Colors code

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

//--------------------------------------------------------------

void menu(){
	printf("-----------------------------------\n");
	printf("--        ");
	printf("%s", KRED);
	printf(" <3 ");
	printf("%s", KBLU);
	printf("WELCOME");
	printf("%s", KRED);
	printf(" <3 ");
	printf("%s", KNRM);
	printf("        --\n");

	printf("-----------------------------------\n");

	printf("1. Register\n");
	printf("2. Sign in\n");
	//printf("3. Sign out\n");
	printf("3. Exit\n");
	printf("Your choice (1-3):  ");
}

void all_emotion(){
	printf("\n-----------------------------------\n");
	printf("-             ");
	printf("%s", KBLU);
	printf("STATUS");
	printf("%s", KNRM);
	printf("             -\n");
	printf("-----------------------------------\n");
	printf("1. Smile\n");
	printf("2. Sad\n");
	printf("3. Busy\n");
	printf("4. Need_Chat\n");
	printf("Your choice (1-4):  ");
}

void chatFunction(){
	printf("\n-----------------------------------\n");
	printf("--      ");
	printf("%s", KRED);
	printf("##  ");
	printf("%s", KBLU);
	printf("TEXTING APP");
	printf("%s", KRED);
	printf("  ##");
	printf("%s", KNRM);
	printf("      --\n");
	printf("-----------------------------------\n");
	printf("0. List member Online\n");
	printf("1. Create room\n");
	printf("2. Get in room\n");
	printf("3. List of rooms\n");
	printf("4. Update status\n");
	printf("5. Chat 1vs1\n");
	printf("6. Sign Out\n");
	printf("Your choice (0-6):\n");
}

void menuhelp(){
	printf("%s/createroom", KRED);
	printf("%s", KNRM);
	printf(" : Create room\n");
	printf("%s/getroom", KRED);
	printf("%s", KNRM);
	printf(" : Get in room\n");
	printf("%s/listroom", KRED);
	printf("%s", KNRM);
	printf(" : List of rooms\n");
	printf("%s/listusergroup", KRED);
	printf("%s", KNRM);
	printf(" : List user of  rooms\n");
	printf("%s/touser", KRED);
	printf("%s", KNRM);	
	printf(" : Chat private to user\n");
	printf("%s/kickuser", KRED);
	printf("%s", KNRM);	
	printf(" : Kick user ( admin )\n");
}