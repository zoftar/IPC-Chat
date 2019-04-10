#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<stdbool.h>
#include <ctype.h>

// typ 100 - serwer
// typ 1-9 - user
// typ 11-13 - grupa

struct message {
    long id;
    char username[20];
    char text[1000];
}write_msg, read_msg;

struct alert {
    long type;
    int success_id;
    long user_id;
    char text[500];
}signal, feedback;

void print_menu() {
    printf("****************************************\n");
    printf ("Dostepne operacje:\n");
    printf ("[1]Wyslij wiadomosc do pojedynczego uzytkownika\n");
    printf ("[2]Wyslij wiadomosc do grupy\n");
    printf ("[3]Odbierz wiadomosci\n");
    printf ("[4]Wyswietl liste zalogowanych uzytkownikow\n");
    printf ("[5]Wyswietl liste grup\n");
    printf ("[6]Zapisz sie do grupy\n");
    printf ("[7]Wypisz sie z grupy\n");
    printf ("[8]Wyswietl liste uzytkownikow nalezacych do grupy\n");
    printf ("[9]Wyloguj i zamknij\n");
    printf ("Podaj numer operacji:\n");
}

int main() {

    char login[20];
    long id = getpid();
    bool quit = false;
    int task;
    int g_id;
    int status;

    int send_queue = msgget(3333555, IPC_CREAT|0666);
    int read_queue = msgget(5555333, IPC_CREAT|0666);

    if (send_queue>=0 && read_queue>=0){
        do {
            printf("Zaloguj sie, podajac nazwe uzytkownika:\n");
            scanf("%s", login);
            for (int i=0; i<20; i++) {
                if (!isalpha(login[i])) {
                    login[i]='\0';
                    break;
                }
            }

            signal.type = 101;
            signal.user_id = id;
            strcpy(signal.text, login);

            msgsnd(send_queue, &signal, sizeof(signal)-sizeof(long), 0);
            printf("Wyslano zadanie logowania do serwera...\n");
            msgrcv(read_queue, &feedback, sizeof(feedback)-sizeof(long), id, 0);
            printf(feedback.text);
            id = feedback.user_id;
        }while(feedback.success_id == 2);

        signal.user_id = id;
        write_msg.id = id;

        char tmp_text[1000];

        do {
            print_menu();
            do {
                scanf("%d", &task);
                switch(task) {
                case 1: //wyslij do 1
                    printf("Podaj adresata:\n");
                    scanf("%s", &login);
                    for (int i=0; i<20; i++) {
                        if (!isalpha(login[i])) {
                            login[i]='\0';
                            break;
                        }
                    }
                    strcpy(write_msg.username, login);
                    printf("Wpisz tresc wiadomosci (max. 1000 znakow):\n");
                    scanf("%s", &tmp_text);
                    strcpy(write_msg.text, tmp_text);
                    msgsnd(send_queue, &write_msg, sizeof(write_msg)-sizeof(long), 0);
                    msgrcv(read_queue, &feedback, sizeof(feedback)-sizeof(long), id, 0);
                    printf("%s", feedback.text);
                    break;
                case 2: //wyslij do grupy
                    printf("Podaj nazwe grupy:\n");
                    scanf("%s", &login);
                    for (int i=0; i<20; i++) {
                        if (!isalpha(login[i])) {
                            login[i]='\0';
                            break;
                        }
                    }
                    strcpy(write_msg.username, login);
                    printf("Wpisz tresc wiadomosci (max. 1000 znakow):\n");
                    scanf("%s", &tmp_text);
                    strcpy(write_msg.text, tmp_text);

                    msgsnd(send_queue, &write_msg, sizeof(write_msg)-sizeof(long), 0);
                    msgrcv(read_queue, &feedback, sizeof(feedback)-sizeof(long), id, 0);
                    printf("%s", feedback.text);

                    break;
                case 3: //odbierz
                    status = msgrcv(read_queue, &read_msg, sizeof(read_msg)-sizeof(long),id, IPC_NOWAIT);
                    if (status>0)
                        printf("Wiadomosc od <%s>\n%s\n", read_msg.username, read_msg.text);

                    else printf("Brak nowych wiadomosci\n");
                    break;
                case 4: //listazalogowanych
                    signal.type = 103;
                    msgsnd(send_queue, &signal, sizeof(signal)-sizeof(long), 0);
                    msgrcv(read_queue, &feedback, sizeof(feedback)-sizeof(long), id, 0);
                     printf("%s", feedback.text);
                    break;
                case 5: //listagrup
                    signal.type = 107;
                    msgsnd(send_queue, &signal, sizeof(signal)-sizeof(long), 0);
                    msgrcv(read_queue, &feedback, sizeof(feedback)-sizeof(long), id, 0);
                     printf("%s", feedback.text);
                    break;
                case 6: //zapisz do grupy
                    signal.type = 105;
                    printf("Podaj nazwe grupy:\n");
                    scanf("%s", &login);
                    for (int i=0; i<20; i++) {
                        if (!isalpha(login[i])) {
                            login[i]='\0';
                            break;
                        }
                    }
                    strcpy(signal.text, login);
                    msgsnd(send_queue, &signal, sizeof(signal)-sizeof(long), 0);
                    msgrcv(read_queue, &feedback, sizeof(feedback)-sizeof(long), id, 0);
                    printf("%s", feedback.text);
                    break;
                case 7: //wypisz z grupy
                    signal.type=106;
                    scanf("%s", &login);
                    for (int i=0; i<20; i++) {
                        if (!isalpha(login[i])) {
                            login[i]='\0';
                            break;
                        }
                    }
                    strcpy(signal.text, login);
                    msgsnd(send_queue, &signal, sizeof(signal)-sizeof(long), 0);
                    msgrcv(read_queue, &feedback, sizeof(feedback)-sizeof(long), id, 0);
                    printf("%s", feedback.text);
                    break;
                case 8: //lista w grupie
                    signal.type=104;
                    printf("Podaj Nazwe grupy:\n");
                    scanf("%s", &login);
                    for (int i=0; i<20; i++) {
                        if (!isalpha(login[i])) {
                            login[i]='\0';
                            break;
                        }
                    }
                    //signal.success_id=g_id;
                    strcpy(signal.text, login);
                    msgsnd(send_queue, &signal, sizeof(signal)-sizeof(long), 0);
                    msgrcv(read_queue, &feedback, sizeof(feedback)-sizeof(long), id, 0);
                     printf("%s", feedback.text);
                    break;
                case 9: //wyjdz
                    signal.type = 102;
                    msgsnd(send_queue, &signal, sizeof(signal)-sizeof(long), 0);
                    msgrcv(read_queue, &feedback, sizeof(feedback)-sizeof(long), id, 0);
                     printf("%s", feedback.text);
                    quit = true;
                    break;
                default:
                    printf("Niepoprawne ID operacji. Podaj jeszcze raz: \n");
                    break;

                }
            }while (task<1 || task>9);

        } while (!quit);
    }
    else printf("Blad przy otwieraniu kolejki\n");


}
