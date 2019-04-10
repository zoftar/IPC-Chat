#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<stdio.h>
#include<stdbool.h>
#include <ctype.h>

// typ 100 - serwer
// typ 1-9 - user
// typ 11-13 - grupa

struct message {
    long id;
    char username[20];
    char text[1000];
}msg;

struct alert {
    long type;
    int other_id;
    long user_id;
    char text[500];
}request, signal;

struct user_info {
    long id;
    bool active;
    char name[20];
}users[10];

struct group {
    long id;
    bool members[10];
    char name[20];
} groups[4];

int main(int argcount, char *args[]) {
    bool quit= false;
    FILE *file;
    file = fopen(args[1], "r");
    if (file == NULL) printf ("Bledna nazwa pliku wejsciowego\n");
    else {
    char *input= NULL;
    size_t size=0;
    char c;
    int i, j;

    printf("Uzytkownicy\nID|Nazwa\n");

    for (i=1; i<10; i++) {
        getline(&input, &size, file);
        for (j=0; j<size/4; j++) {
            if (!isalpha(input[j])) {input[j]='\0'; break;}
        }
        strcpy(users[i].name, input);
        users[i].id = (long)i;
        users[i].active = false;
        printf("%d|%s\n", i, users[i].name);
    }

    printf("Grupy\n");

    for (i=1; i<=3; i++) {
        getline(&input, &size, file);
        for (j=0; j<size/4; j++) if (!isalpha(input[j])) input[j]='\0';
        strcpy(groups[i].name, input);
        groups[i].id = 10+i;
        printf("%s\n", groups[i].name);
        for (j=1; j<=9; j++) {
            getline(&input, &size, file);
            c = input[0];
            if (c=='1') groups[i].members[j]=true;
            else groups[i].members[j]=false;
        }
    }

    int receive_queue = msgget(3333555, IPC_CREAT|IPC_EXCL|0660);
    int forward_queue = msgget(5555333, IPC_CREAT|IPC_EXCL|0660);
    int tmp_id, group;
    int status;

    if (receive_queue>=0 && forward_queue>=0) {
        while (!quit) {
            status = msgrcv(receive_queue, &request, sizeof(request)-sizeof(long), 101, IPC_NOWAIT);
            if (status>=0) {

                strcpy(signal.text, "");
                signal.type = request.user_id;
                tmp_id = request.user_id;
                printf("Zadanie id:%ld od %ld\n", request.type, request.user_id);
                 //logowanie
                    bool proper_username = false;
                    for (i=1; i<=9; i++) {
                        if (strcmp(request.text, users[i].name)==0) {
                            proper_username = true;
                            tmp_id = i;
                            break;
                        }
                    }
                    if (proper_username) {
                        printf("Dodalem uzytkownika %s\n", request.text);
                        strcpy(signal.text, "Zalogowales sie poprawnie\n");
                        signal.other_id = 1;
                        signal.user_id = tmp_id;
                        users[tmp_id].active=true;
                    }
                    else {
                        strcpy(signal.text, "Niepoprawny login\n");
                        signal.other_id = 2;
                    }
                    msgsnd(forward_queue, &signal, sizeof(signal)-sizeof(long), 0);
                }

            status = msgrcv(receive_queue, &request, sizeof(request)-sizeof(long), 102, IPC_NOWAIT);
            if (status>=0) { //wylogowywanie
                    strcpy(signal.text, "");
                signal.type = request.user_id;
                tmp_id = request.user_id;
                printf("Zadanie id:%ld od %ld\n", request.type, request.user_id);
                    users[tmp_id].active=false;
                    signal.other_id = 1;
                    strcpy(signal.text, "Wylogowales sie\n");
                    printf("%s sie wylogowal(a)\n", users[tmp_id].name);
                    msgsnd(forward_queue, &signal, sizeof(signal)-sizeof(long), 0);
                    quit = true;
                    for (i=1; i<=9; i++) if (users[i].active) quit = false;
                }

                /*********************/
                status = msgrcv(receive_queue, &request, sizeof(request)-sizeof(long), 103, IPC_NOWAIT);
            if (status>=0) { //wylogowywanie
                    strcpy(signal.text, "");
                signal.type = request.user_id;
                tmp_id = request.user_id;
                printf("Zadanie id:%ld od %ld\n", request.type, request.user_id);
                    strcpy(signal.text, "Zalogowani uzytkownicy:\n");
                    for (int i=1; i<=9; i++) {
                        if (users[i].active) {
                            strcat(signal.text, users[i].name);
                            strcat(signal.text, "\n");
                        }
                    }
                    //printf("drukuje liste uzytkownikow...\n");
                    signal.other_id = 1;
                    msgsnd(forward_queue, &signal, sizeof(signal)-sizeof(long), 0);
                    //printf("wyslalem liste\n");
                }
                /***********************/
                status = msgrcv(receive_queue, &request, sizeof(request)-sizeof(long), 104, IPC_NOWAIT);
            if (status>=0) { //wylogowywanie
                    strcpy(signal.text, "");
                signal.type = request.user_id;
                tmp_id = request.user_id;
                printf("Zadanie id:%ld od %ld\n", request.type, request.user_id);
                    //printf("Sprawdzam nazwe...\n");
                    char group_name[20];
                    strcpy(group_name, request.text);
                    bool proper_name = false;
                    for (i=1; i<=3; i++) {
                        if (strcmp(group_name, groups[i].name)==0) {
                            proper_name = true;
                            group = i;
                            break;
                        }
                    }

                    if (proper_name) {
                        //printf("Nazwa ok...\n");
                        strcpy(signal.text, "Uzytkownicy nalezacy do tej grupy:\n");
                        for (int i=1; i<=9; i++) {
                            if (groups[group].members[i]) {
                                strcat(signal.text, users[i].name);
                                strcat(signal.text, "\n");
                            }
                        }
                        signal.other_id = 1;
                    }
                    else {
                        strcpy(signal.text, "Niepoprawna nazwa grupy\n");
                        signal.other_id = 2;
                    }

                    msgsnd(forward_queue, &signal, sizeof(signal)-sizeof(long), 0);
                    //printf("wyslalem komunikat do %ld\n", signal.type);
                }
                /***********************/
                status = msgrcv(receive_queue, &request, sizeof(request)-sizeof(long), 105, IPC_NOWAIT);
                if (status>=0) { //wylogowywanie
                    strcpy(signal.text, "");
                    signal.type = request.user_id;
                    tmp_id = request.user_id;
                    printf("Zadanie id:%ld od %ld\n", request.type, request.user_id);
                    char group_name[20];
                    strcpy(group_name, request.text);
                    bool proper_name = false;
                    for (i=1; i<=3; i++) {
                        if (strcmp(group_name, groups[i].name)==0) {
                            proper_name = true;
                            break;
                        }
                    }
                    if (proper_name) {
                        groups[i].members[tmp_id]=true;
                        signal.other_id = 1;
                        strcpy(signal.text, "Zapisales sie do grupy\n");
                        //printf("<%s> zostal zapisany do grupy\n", users[tmp_id].name);
                    }
                    else {
                        strcpy(signal.text, "Niepoprawna nazwa grupy\n");
                        signal.other_id = 2;
                    }
                    msgsnd(forward_queue, &signal, sizeof(signal)-sizeof(long), 0);
                }
                /***********************/

                status = msgrcv(receive_queue, &request, sizeof(request)-sizeof(long), 106, IPC_NOWAIT);
                if (status>=0) { //wylogowywanie
                    strcpy(signal.text, "");
                    signal.type = request.user_id;
                    tmp_id = request.user_id;
                    printf("Zadanie id:%ld od %ld\n", request.type, request.user_id); //wypisz z grupy
                    //group = request.other_id;
                    char group_name[20];
                    strcpy(group_name, request.text);
                    bool proper_name = false;
                    for (i=1; i<=3; i++) {
                        if (strcmp(group_name, groups[i].name)==0) {
                            proper_name = true;
                            break;
                        }
                    }
                    if (proper_name) {
                        groups[i].members[tmp_id]=false;
                        signal.other_id = 1;
                        strcpy(signal.text, "Wypisales sie z grupy\n");
                        //printf("<%s> zostal wypisany z grupy\n", users[tmp_id].name);
                    }
                    else {
                        strcpy(signal.text, "Niepoprawna nazwa grupy\n");
                        signal.other_id = 2;
                    }
                    msgsnd(forward_queue, &signal, sizeof(signal)-sizeof(long), 0);
                }
                /***********************/
                status = msgrcv(receive_queue, &request, sizeof(request)-sizeof(long), 107, IPC_NOWAIT);
            if (status>=0) { //wylogowywanie
                    strcpy(signal.text, "");
                signal.type = request.user_id;
                tmp_id = request.user_id;
                printf("Zadanie id:%ld od %ld\n", request.type, request.user_id); //wyswietl liste grup
                    //strcpy(signal.text, list_of_groups());
                    strcpy(signal.text, "Grupy:\n");
                    for (int i=1; i<=3; i++) {
                        char grp_inf[30];
                        sprintf(grp_inf, "[%d] %s\n", i+10, groups[i].name);
                        strcat(signal.text, grp_inf);
                    }
                    //printf("Drukuje grupy...\n");
                    signal.other_id = 1;
                    msgsnd(forward_queue, &signal, sizeof(signal)-sizeof(long), 0);
                }


            for (long h=1; h<=9; h++) {
                status = msgrcv(receive_queue, &msg, sizeof(msg)-sizeof(long), h, IPC_NOWAIT);
                if (status>0) h=10;
            }
            if (status>0) {
                bool proper_username = false;
                tmp_id = msg.id;
                for (i=1; i<=9; i++) {
                    if (strcmp(request.text, users[i].name)==0) {
                        proper_username = true;
                        break;
                    }
                }
                if (proper_username) {
                    msg.id = i;
                    strcpy(msg.username, users[tmp_id].name);
                    msgsnd(forward_queue, &msg, sizeof(msg)-sizeof(long), 0);
                    signal.type = tmp_id;
                    signal.other_id = 1;
                    strcpy(signal.text, "Udalo sie wyslac wiadomosc\n");
                    msgsnd(forward_queue, &signal, sizeof(signal)-sizeof(long), 0);
                }
                else {
                    for (i=1; i<=3; i++) {
                        if (strcmp(request.text, groups[i].name)==0) {
                            proper_username = true;
                            group = i;
                            i=4;
                        }
                    }
                    if (proper_username) {
                        strcpy(msg.username, users[tmp_id].name);
                        for (j=1; j<=9; j++) {
                            if (groups[group].members[j]) {
                                msg.id = j;
                                msgsnd(forward_queue, &msg, sizeof(msg)-sizeof(long), 0);
                                //printf("Udalo sie wyslac wiadomosc d %s\n", users[j].name);
                            }

                        }
                        signal.type = tmp_id;
                        signal.other_id = 1;
                        strcpy(signal.text, "Udalo sie wyslac wiadomosc\n");
                        msgsnd(forward_queue, &signal, sizeof(signal)-sizeof(long), 0);
                    }
                    else {
                        signal.type = tmp_id;
                        signal.other_id = 2;
                        strcpy(signal.text, "Nie udalo sie wyslac wiadomosci\n");
                        msgsnd(forward_queue, &signal, sizeof(signal)-sizeof(long), 0);
                    }

                }
            }
        }
    }
    else printf("Wystapil blad podczas tworzenia kolejki\n");

    msgctl(receive_queue, IPC_RMID, NULL);
    msgctl(forward_queue, IPC_RMID, NULL);

    printf("Koncze program...\n");

    fclose(file);
    }



}
