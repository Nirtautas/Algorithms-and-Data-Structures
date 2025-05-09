//Projektas naudoja eiles implementacija - Autorius Kajus Kutelis, 2 GRUPE, 1 POGRUPIS.
#include "eile.h"
//Projektas naudeja prioritetines eiles implementacija - Autorius Vilius Paskonis, 2 GRUPE, 2 POGRUPIS.
#include "functions.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <limits.h>

//Pastovus kintamieji
#define SEPARATOR printf("============================================================\n");
#define MAX_LINE_LENGTH 256
#define ERROR_ARR_SIZE 6
#define FD_FILE_NAME_MAX 20
#define FR_FILE_NAME_MAX 20
#define WORKERS_MAXNUM 6
#define WORKER_SPLIT_COEFF 3

//Klaidu zinutes.
const char* ERROR_CODE[ERROR_ARR_SIZE] = {
    "Nepavyko atidaryti failo.\n",
    "Neteisingas parametru failas.\n",
    "Indeksas negali buti neigiamas.\n",
    "Indeksas negali buti didesnis uz ERROR_CODE masyvo dydi.\n",
    "Nepavyko sukurti failo.\n",
    "Iprasta klaida.\n"
};

//Kliento struktura.
typedef struct Client {
    int state; //Jei procesas 1: 0 Ne eileje, 1 Laukia, 2 Renka prekes, 3 Raso ceki, 4 Ima pinigus.
    int timer; //Jei procesas 2: 0 Ne eileje, 1 Laukia prie pardavejos, 2 Renka prekes, 3 Laukia prie kasininkes, 4 Raso ceki, 5 Ima pinigus, 6 Prioritetiskai laukia.
    int previousPQueueIndex;
    int indexLine;
    int id;
    int shoppingTime;
} Client;

//Nepastovus kintamieji.
int SIMULATION_TIME = -1;
int ENTER_CHANCE = -1;
int REGISTER_COST = -1;

int FETCHING_TIME_MAX = -1;
int FETCHING_TIME_MIN = -1;
int CHECK_TIME_MAX = -1;
int CHECK_TIME_MIN = -1;
int MONEY_TIME_MAX = -1;
int MONEY_TIME_MIN = -1;

//Laiko kintamieji.
int maxw_time = INT_MIN, minw_time = INT_MAX, midw_time = 0;
int pmaxw_time = INT_MIN, pminw_time = INT_MAX, pmidw_time = 0;

//Kainos kintamieji.
int regCost;
int pregCost;

//Failu kintamieji.
FILE* fd;
FILE* fr;
FILE* tmp;
char line[MAX_LINE_LENGTH];
char fdName[FD_FILE_NAME_MAX];
char frName[FR_FILE_NAME_MAX];
char* tmpName = "tmp.txt";

//Klientu ID skaitliukas.
int idcounter = 0;

//Aktyviu klientu saraso kintamieji.
int* actClients = NULL;
int* pactClients = NULL;
int actClientsSize = 0;
int pactClientsSize = 0;

//Eiliu kintamieji.
int regQueues[WORKERS_MAXNUM];
int regQueuesSize = WORKERS_MAXNUM;
int pregQueues[WORKERS_MAXNUM - WORKER_SPLIT_COEFF];
int pregQueuesSize = WORKERS_MAXNUM - WORKER_SPLIT_COEFF;

//Prioritetiniu eiliu kintamieji.
int pregPQueues[WORKERS_MAXNUM - (WORKERS_MAXNUM - WORKER_SPLIT_COEFF)];
int pregPQueuesSize = WORKERS_MAXNUM - (WORKERS_MAXNUM - WORKER_SPLIT_COEFF);

//Funkciju apibrezimai.
void initQ(int* qarr, int qsize);
void initPQ(int *pqarr, int pqsize);
int shrQ(int *qarr, int size);
int shrPQ(int* pqarr);
int finQ(int* qarr, int qsize, int el);
int finPQ(int* pqarr, int pqsize, int el);
void destQ(int* qarr, int* qsize);
void destPQ(int* pqarr, int* pqsize);
int* ladd(int* arr, int* size, int el);
int* lrem(int* arr, int* size, int el);
int lfin(int* arr, int size, int el);
void flprn(int *arr, int size);
void fprn(int *parr, int psize);
int* addClient_RAND(int* arr, int* size, int** parr, int* psize, int chance);
void lineClient_RAND(int* qarr, int size, int client, bool isRAND, bool isProc2);
void plineClient_RAND(int* pqarr, int client, bool isRAND);
void plineClient_INDEX(int* pqarr, int client);
void changeState_RAND(int client);
void calcTime(int* maxt, int* mint, int* midt, int time);
void printParameters();
void printInfo();
void fprintInfo();
void printError(int index, bool shouldExit);
void printConclusion();
void fprintCounclusion();

//=====================================================================
//PAGRINDINE FUNKCIJA
//=====================================================================

int main()
{
    int qindex, pqindex;
    Client* client;
    srand(time(NULL));
    initQ(regQueues, regQueuesSize);
    initQ(pregQueues, pregQueuesSize);
    initPQ(pregPQueues, pregPQueuesSize);
    tmp = fopen(tmpName, "w");

    printInfo();
    //Nuskaitomi parametrai
    while(1) {
        printf("Iveskite ivesties failo pavadinima: ");
        scanf("%s", fdName);
        if ((fd = fopen(fdName, "r")) == NULL) {
            while (getchar() != '\n');
            printError(0, false);
        } else {
            if (fscanf(fd, "%d%d%d%d%d%d%d%d%d", &SIMULATION_TIME, &ENTER_CHANCE, &REGISTER_COST, &FETCHING_TIME_MAX, &FETCHING_TIME_MIN, &CHECK_TIME_MAX, &CHECK_TIME_MIN, &MONEY_TIME_MAX, &MONEY_TIME_MIN) == 9) {
                printf("Programos parametrai paimti sekmingai!\n");
            } else
                printError(1, true);
            break;
        }
    }
    printParameters();

    while(1) {
        printf("Iveskite isvesties failo pavadinima: ");
        scanf("%s", frName);
        if ((fr = fopen(frName, "w")) == NULL) {
            while (getchar() != '\n');
            printError(4, false);
        } else {
            printf("Failas sukurtas sekmingai!\n");
            break;
        }
    }
    printf("Programa dirba...\n");
    fprintInfo();

    for (int time = 1 ; time <= SIMULATION_TIME ; ++time) {
        fprintf(tmp, "T=%d min.\n", time);
        fprintf(fr, "T=%d min.\n", time);
        actClients = addClient_RAND(actClients, &actClientsSize, &pactClients, &pactClientsSize, ENTER_CHANCE);

        //Procesas 1
        for (int j = 0 ; j < actClientsSize ; ++j) {
            client = (Client*) actClients[j];
            client->shoppingTime += 1;

            if (client->state == 0) {
                lineClient_RAND(regQueues, regQueuesSize, actClients[j], false, false);
            } else if (finQ(regQueues, regQueuesSize, actClients[j]) != -1) { //Ar stovi primas eileje?
                if (client->state == 4) {
                    if (client->timer == 0) {
                        qindex = finQ(regQueues, regQueuesSize, actClients[j]);
                        client->shoppingTime -= 2;
                        calcTime(&maxw_time, &minw_time, &midw_time, client->shoppingTime);
                        dequeue((Queue*) regQueues[qindex]);
                        actClients = lrem(actClients, &actClientsSize, actClients[j]);
                    } else
                        client->timer -= 1;
                } else {
                    if (client->timer == 0)
                        changeState_RAND(actClients[j]);
                    else
                        client->timer -= 1;
                }
            }
        }
        flprn(actClients, actClientsSize);

        //Procesas 2
        for (int k = 0 ; k < pactClientsSize ; ++k) {
            client = (Client*) pactClients[k];
            client->shoppingTime += 1;

            if (client->state == 0) {
                plineClient_RAND(pregPQueues, pactClients[k], false);
            } else if (finPQ(pregPQueues, pregPQueuesSize, pactClients[k]) != -1 && client->state == 6) {
                pqindex = finPQ(pregPQueues, pregPQueuesSize, pactClients[k]);
                client->shoppingTime -= 2;
                calcTime(&pmaxw_time, &pminw_time, &pmidw_time, client->shoppingTime);
                Remove((S_PriorityQueue*) pregPQueues[pqindex]);
                pactClients = lrem(pactClients, &pactClientsSize, pactClients[k]);
            } else if (finPQ(pregPQueues, pregPQueuesSize, pactClients[k]) != -1) { //Ar stovi pirmas p eileje?
                if (client->state == 2) {
                    if (client->timer == 0) {// CIA KAZKAS NEGERAI
                    //mest is p eiles ir imest i eile
                    pqindex = finPQ(pregPQueues, pregPQueuesSize, pactClients[k]);
                    Remove((S_PriorityQueue*) pregPQueues[pqindex]);
                    lineClient_RAND(pregQueues, pregQueuesSize, pactClients[k], false, true);
                    if (finQ(pregQueues, pregQueuesSize, pactClients[k]) != -1) {
                        client->state = 4;
                        client->timer = rand() % (CHECK_TIME_MAX - CHECK_TIME_MIN) + CHECK_TIME_MIN; // Galimai sita eilute
                    }
                    } else
                        client->timer -= 1;
                } else {
                    if (client->timer == 0) {
                        client->state += 1;
                        client->timer = rand() % (FETCHING_TIME_MAX - FETCHING_TIME_MIN) + FETCHING_TIME_MIN;
                    } else
                        client->timer -= 1;
                }
            } else if (finQ(pregQueues, pregQueuesSize, pactClients[k]) != -1) { //Ar stovi pirmas eileje?
                if (client->state == 5) {
                    if (client->timer == 0) {// Permest atgal i prioritetine eile
                        qindex = finQ(pregQueues, pregQueuesSize, pactClients[k]);
                        dequeue((Queue*) pregQueues[qindex]);
                        plineClient_INDEX(pregPQueues, pactClients[k]);
                    } else
                        client->timer -= 1;
                } else {
                    if (client->timer == 0) {
                        client->state += 1;
                        if (client->state == 4)
                            client->timer = rand() % (CHECK_TIME_MAX - CHECK_TIME_MIN) + CHECK_TIME_MIN;
                        if (client->state == 5)
                            client->timer = rand() % (MONEY_TIME_MAX - MONEY_TIME_MIN) + MONEY_TIME_MIN;
                    } else
                        client->timer -=1;
                }
            }
        }
        fprn(pactClients, pactClientsSize);
    }
    fclose(tmp);
    tmp = fopen(tmpName, "r");
    fprintf(fr , "DALIS 2.2. Procesas 2. Protokolas.\n\n");
    while (fgets(line, MAX_LINE_LENGTH, tmp))
        fputs(line, fr);
    fclose(tmp);

    midw_time = midw_time / (idcounter - 1);
    pmidw_time = pmidw_time / (idcounter - 1);
    SEPARATOR;
    printf("DALIS 3. Rezultatai.\n\n");
    fprintf(fr, "DALIS 3. Rezultatai.\n\n");

    printf("DALIS 3.1. Procesas 1. Rezultatas.\n\n");
    fprintf(fr, "DALIS 3.1. Procesas 1. Rezultatas.\n\n");

    printf("Kasos aparatu kaina           %3d eur.\nMaksimalus apsipirkimo laikas %3d min.\nMinimalus apsipirkimo laikas  %3d min.\nVidutinis apsipirkimo laikas  %3d min.\n\n", WORKERS_MAXNUM * REGISTER_COST, maxw_time, minw_time, midw_time);
    fprintf(fr, "Kasos aparatu kaina           %3d eur.\nMaksimalus apsipirkimo laikas %3d min.\nMinimalus apsipirkimo laikas  %3d min.\nVidutinis apsipirkimo laikas  %3d min.\n\n", WORKERS_MAXNUM * REGISTER_COST, maxw_time, minw_time, midw_time);

    printf("DALIS 3.2. Procesas 2. Rezultatas.\n\n");
    fprintf(fr, "DALIS 3.2. Procesas 2. Rezultatas.\n\n");

    printf("Kasos aparatu kaina           %3d eur.\nMaksimalus apsipirkimo laikas %3d min.\nMinimalus apsipirkimo laikas  %3d min.\nVidutinis apsipirkimo laikas  %3d min.\n\n", REGISTER_COST * (WORKERS_MAXNUM - WORKER_SPLIT_COEFF), pmaxw_time, pminw_time, pmidw_time);
    fprintf(fr, "Kasos aparatu kaina           %3d eur.\nMaksimalus apsipirkimo laikas %3d min.\nMinimalus apsipirkimo laikas  %3d min.\nVidutinis apsipirkimo laikas  %3d min.\n\n", REGISTER_COST * (WORKERS_MAXNUM - WORKER_SPLIT_COEFF), pmaxw_time, pminw_time, pmidw_time);
    printConclusion();
    fprintCounclusion();
    SEPARATOR;

    fclose(fd);
    fclose(fr);
    destQ(regQueues, &regQueuesSize);
    destQ(pregQueues, &pregQueuesSize);
    destPQ(pregPQueues, &pregPQueuesSize);
    return 0;
}

//=====================================================================
//FUNKCIJU APRASAI
//=====================================================================

void initQ(int* qarr, int qsize) {
    for (int i = 0 ; i < qsize ; ++i)
        qarr[i] = (int) createQueue();
}

void initPQ(int *pqarr, int pqsize) {
    for (int i = 0 ; i < pqsize ; ++i)
        pqarr[i] = (int) Create();
}

int shrQ(int* qarr, int size) {
    int shrIndex = 0;

    for (int i = 0 ; i < size ; ++i) {
        if (getSize((Queue*) qarr[shrIndex]) > getSize((Queue*) qarr[i]))
            shrIndex = i;
    }
    return shrIndex;
}

int shrPQ(int* pqarr) {
    int pshrIndex = 0;

    for (int i = 0 ; i < pregPQueuesSize ; ++i) {
        if (getPQSize((S_PriorityQueue*) pqarr[pshrIndex]) > getPQSize((S_PriorityQueue*) pqarr[i]))
            pshrIndex = i;
    }
    return pshrIndex;
}

int finQ(int* qarr, int qsize, int el) {
    for (int i = 0 ; i < qsize ; ++i) {
        if (el == getFront((Queue*) qarr[i]))
            return i;
    }
    return -1;
}

int finPQ(int* pqarr, int pqsize, int el) {
    for (int i = 0 ; i < pqsize ; ++i) {
        if (el == getPQFrontValue((S_PriorityQueue*) pqarr[i]))
            return i;
    }
    return -1;
}

void destQ(int* qarr, int* qsize) {
    for (int i = 0 ; i < *qsize ; ++i)
        destroyQueue((Queue*) qarr[i]);
    *qsize = 0;
}

void destPQ(int* pqarr, int* pqsize) {
    for (int i = 0 ; i < *pqsize ; ++i)
    DeletePQueue((S_PriorityQueue*) pqarr[i]);
    *pqsize = 0;
}

int* ladd(int* arr, int* size, int el) {
    ++(*size);
    arr = (int*) realloc(arr, *size * sizeof(int));
    arr[*size - 1] = el;
    return arr;
}

int* lrem(int* arr, int* size, int el) {
    int rem;

    if ((rem = lfin(arr, *size, el)) != -1) {
        for (int i = rem ; i < *size - 1 ; ++i) {
            arr[i] = arr[i + 1];
        }
        --(*size);
        arr = (int *) realloc(arr, *size * sizeof(int));
    }
    return arr;
}

int lfin(int* arr, int size, int el) {
    for (int i = 0 ; i < size ; ++i) {
        if (arr[i] == el)
            return i;
    }
    return -1;
}

void flprn(int *arr, int size) {
    if (arr == NULL || size < 0) {
        fprintf(fr, "Pirkeju nera.\n\n");
        return;
    }

    //fprintf(fr, "Dydis %d.\n\n", size);
    for (int i = 0 ; i < size ; ++i) {
        Client* client = (Client*) arr[i];
        fprintf(fr, "Pirkejas %2d parduotuveje praleides laiko %2d min. Busena. ", client->id + 1, client->shoppingTime - 1);
        switch(client->state) {
        case 0:
            fprintf(fr, "NE eileje.\n");
            break;
        case 1:
            if (client->shoppingTime == 1)
                fprintf(fr, "ATEJO i parduotuve ir atsistojo prie pardavejos %d.\n", client->indexLine + 1);
            else
                fprintf(fr, "LAUKIA eileje prie pardavejos %d.\n", client->indexLine + 1);
            break;
        case 2:
            fprintf(fr, "LAUKIA prekiu is pardavejos %d.\n", client->indexLine + 1);
            break;
        case 3:
            fprintf(fr, "LAUKIA cekio is pardavejos %d.\n", client->indexLine + 1);
            break;
        case 4:
            if (client->timer == 0)
                fprintf(fr, "Gavo graza is pardavejos %d ir ISEJO is parduotuves.\n", client->indexLine + 1);
            else
                fprintf(fr, "LAUKIA grazos is pardavejos %d.\n", client->indexLine + 1);
            break;
        default:
            fprintf(fr, "Neturetu atsitikti.\n");
        }
        //fprintf(fr, "Elementas %d - %d ID %d Busena %d, kasoje %d Apsipirkimo laikas %d.\n", i, arr[i], client->id, client->state, client->indexLine, client->shoppingTime);
    }
    fprintf(fr, "\n");
}

void fprn(int *parr, int psize) {
    if (parr == NULL || psize < 0) {
        fprintf(tmp, "Pirkeju nera.\n\n");
        return;
    }

    //fprintf(fr, "Dydis %d.\n\n", size);
    for (int i = 0 ; i < psize ; ++i) {
        Client* client = (Client*) parr[i];
        fprintf(tmp, "Pirkejas %2d parduotuveje praleides laiko %2d min. Busena. ", client->id + 1, client->shoppingTime - 1);
        //fprintf(tmp, "Laikmatis %4d. Pirkejas %4d. ", client->timer, client->id);

        switch(client->state) {
        case 0:
            fprintf(tmp, "NE eileje.\n");
            break;
        case 1:
            if (client->shoppingTime == 1)
                fprintf(tmp, "ATEJO i parduotuve ir atsistojo prie pardavejos %d.\n", client->indexLine + 1);
            else
                fprintf(tmp, "LAUKIA eileje prie pardavejos %d.\n", client->indexLine + 1);
            break;
        case 2:
            fprintf(tmp, "LAUKIA surenkamu prekiu is pardavejos %d.\n", client->indexLine + 1);
            break;
        case 3:
            fprintf(tmp, "LAUKIA eileje prie kasininkes %d.\n", client->indexLine + 1);
            break;
        case 4:
            fprintf(tmp, "LAUKIA cekio is kasininkes %d.\n", client->indexLine + 1);
            break;
        case 5:
            fprintf(tmp, "LAUKIA grazos is kasininkes %d.\n", client->indexLine + 1);
            break;
        case 6:
            if (client->timer == 0)
                fprintf(tmp, "Be eiles gavo prekes is pardavejos %d ir ISEJO is parduotuves.\n", client->indexLine + 1);
            else
                fprintf(tmp, "LAUKIA prekiu be eiles prie pardavejos %d.\n", client->indexLine + 1);
            break;
        default:
            fprintf(tmp, "Neturetu atsitikti.\n");
            printf("aaa");
        }
        //printf("Elementas %d - %d ID %d Busena %d, kasoje %d Apsipirkimo laikas %d.\n", i, arr[i], client->id, client->state, client->indexLine, client->shoppingTime);
    }
    fprintf(tmp, "\n");
}

int* addClient_RAND(int* arr, int* size, int** parr, int* psize, int chance) {
    Client* client;
    Client* pclient;

    if ((rand() % 100) + 1 <= chance) {
        client = (Client*) malloc(sizeof(Client));
        client->state = 0;
        client->timer = 0;
        client->indexLine = -1;
        client->id = idcounter;
        ++idcounter;
        client->previousPQueueIndex = -1;
        client->shoppingTime = 0;
        arr = ladd(arr, size, (int) client);

        pclient = (Client*) malloc(sizeof(Client));
        pclient->state = 0;
        pclient->timer = 0;
        pclient->indexLine = -1;
        pclient->id = idcounter - 1;
        pclient->previousPQueueIndex = -1;
        pclient->shoppingTime = 0;
        *parr = ladd(*parr, psize, (int) pclient);
    }
    return arr;
}

void lineClient_RAND(int* qarr, int size, int client, bool isRAND, bool isProc2) {
    int qindex;

    if (isRAND == true) {
        qindex = rand() % size;
    } else
        qindex = shrQ(qarr, size);


    if (isProc2)
        ((Client*) client)->state = 3;
    else
        ((Client*) client)->state = 1;
    ((Client*) client)->indexLine = qindex;
    enqueue((Queue*) qarr[qindex], client);
}

void plineClient_RAND(int* pqarr, int client, bool isRAND) {
    int pqindex;

    if (isRAND == true) {
        pqindex = rand() % pregPQueuesSize;
    } else
        pqindex = shrPQ(pqarr);

    ((Client*) client)->state = 1;
    ((Client*) client)->indexLine = pqindex;
    ((Client*) client)->previousPQueueIndex = pqindex;
    Insert((S_PriorityQueue*) pqarr[pqindex], client, 1);
}

void plineClient_INDEX(int* pqarr, int client) {
    ((Client*) client)->state = 6;
    ((Client*) client)->indexLine = ((Client*) client)->previousPQueueIndex;
    Insert((S_PriorityQueue*) pqarr[((Client*) client)->previousPQueueIndex], client, 2);
}

void changeState_RAND(int client) {
    int time = 0;
    Client* pclient = (Client*) client;

    switch (pclient->state) {
    case 1:
        time = rand() % (FETCHING_TIME_MAX - FETCHING_TIME_MIN) + FETCHING_TIME_MIN;
        break;
    case 2:
        time = rand() % (CHECK_TIME_MAX - CHECK_TIME_MIN) + CHECK_TIME_MIN;
        break;
    case 3:
        time = rand() % (MONEY_TIME_MAX - MONEY_TIME_MIN) + MONEY_TIME_MIN;
        break;
    }
    pclient->state += 1;
    pclient->timer = time;
    return;
}

void calcTime(int* maxt, int* mint, int* midt, int time) {
    if (time > *maxt)
        *maxt = time;
    if (time < *mint)
        *mint = time;
    *midt = *midt + time;
}

void printParameters() {
    SEPARATOR;
    printf("DALIS 1.2. Pradiniai duomenys.\n\n");
    printf("Simuliacijos laikas            T =%3d min.\n", SIMULATION_TIME);
    printf("Atejimo i parduotuve tikimybe     %3d %%.\n", ENTER_CHANCE);
    printf("Kasos aparato kaina               %3d eur.\n", REGISTER_COST);
    printf("Prekiu atnesimo minimalus laikas  %3d min.\n", FETCHING_TIME_MIN);
    printf("Prekiu atnesimo maksimalus laikas %3d min.\n", FETCHING_TIME_MAX);
    printf("Cekio israsimo minimalus laikas   %3d min.\n", CHECK_TIME_MIN);
    printf("Cekio israsimo maksimalus laikas  %3d min.\n", CHECK_TIME_MAX);
    printf("Atsiskaitymo minimalus laikas     %3d min.\n", MONEY_TIME_MIN);
    printf("Atsiskaitymo maksimalus laikas    %3d min.\n", MONEY_TIME_MAX);
    printf("Darbuotoju kiekis                 %3d darb.\n", WORKERS_MAXNUM);
    SEPARATOR;
}

void printInfo() {
    SEPARATOR;
    printf("UZDUOTIS nr. 3, VARIANTAS 7.\n");
    printf("AUTORIUS Nirtautas Sadauskas, GRUPE 2, POGRUPIS 2.\n\n");

    SEPARATOR;
    printf("DALIS 1.1. SALYGA.\n\n");
    printf("Nesavitarnos parduotuve (ADT: eile, prioritetine eile). Procesas 1: pardaveja pilnai aptarnauja\n"
            "klienta (paduoda prekes, ismusa ceki, paima pinigus). Procesas 2: pardaveja surenka pirkejo\n"
            "pageidaujamas prekes ir jas suraso ant popieriaus lapelio, su siuo lapeliu klientas eina prie kasininkes,\n"
            "kuri ismusa ceki ir paima pinigus, su siuo cekiu pirkejas grizta prie ji aptarnavusios pardavejos ir, kai\n"
            "tik si baigia aptarnauti eilini pirkeja, be eiles gauna prekes. Patyrineti abu siuos procesus tiek klientu\n"
            "pasitenkinimo poziuriu (minimalus, vidutinis ir maksimalus pilno aptarnavimo laikas), tiek ekonominiu\n"
            "poziuriu (kiekvienas kasos aparatas kainuoja). Laikykime, kad bendras darbuotoju skaicius yra\n"
            "fiksuotas. Visi kiti rodikliai, nuo kuriu priklauso procesas, ivedami kaip programos parametrai.\n");
    SEPARATOR;
}

void fprintInfo() {
    fprintf(fr, "UZDUOTIS nr. 3, VARIANTAS 7.\n");
    fprintf(fr, "AUTORIUS Nirtautas Sadauskas, GRUPE 2, POGRUPIS 2.\n\n");

    fprintf(fr, "DALIS 1.1. Salyga.\n\n");
    fprintf(fr, "Nesavitarnos parduotuve (ADT: eile, prioritetine eile). Procesas 1: pardaveja pilnai aptarnauja\n"
            "klienta (paduoda prekes, ismusa ceki, paima pinigus). Procesas 2: pardaveja surenka pirkejo\n"
            "pageidaujamas prekes ir jas suraso ant popieriaus lapelio, su siuo lapeliu klientas eina prie kasininkes,\n"
            "kuri ismusa ceki ir paima pinigus, su siuo cekiu pirkejas grizta prie ji aptarnavusios pardavejos ir, kai\n"
            "tik si baigia aptarnauti eilini pirkeja, be eiles gauna prekes. Patyrineti abu siuos procesus tiek klientu\n"
            "pasitenkinimo poziuriu (minimalus, vidutinis ir maksimalus pilno aptarnavimo laikas), tiek ekonominiu\n"
            "poziuriu (kiekvienas kasos aparatas kainuoja). Laikykime, kad bendras darbuotoju skaicius yra\n"
            "fiksuotas. Visi kiti rodikliai, nuo kuriu priklauso procesas, ivedami kaip programos parametrai.\n\n");

    fprintf(fr, "DALIS 1.2. Pradiniai duomenys.\n\n");
    fprintf(fr, "Simuliacijos laikas            T =%3d min.\n", SIMULATION_TIME);
    fprintf(fr, "Atejimo i parduotuve tikimybe     %3d %%.\n", ENTER_CHANCE);
    fprintf(fr, "Kasos aparato kaina               %3d eur.\n", REGISTER_COST);
    fprintf(fr, "Prekiu atnesimo minimalus laikas  %3d min.\n", FETCHING_TIME_MIN);
    fprintf(fr, "Prekiu atnesimo maksimalus laikas %3d min.\n", FETCHING_TIME_MAX);
    fprintf(fr, "Cekio israsimo minimalus laikas   %3d min.\n", CHECK_TIME_MIN);
    fprintf(fr, "Cekio israsimo maksimalus laikas  %3d min.\n", CHECK_TIME_MAX);
    fprintf(fr, "Atsiskaitymo minimalus laikas     %3d min.\n", MONEY_TIME_MIN);
    fprintf(fr, "Atsiskaitymo maksimalus laikas    %3d min.\n", MONEY_TIME_MAX);
    fprintf(fr, "Darbuotoju kiekis                 %3d darb.\n\n", WORKERS_MAXNUM);

    fprintf(fr, "DALIS 2. Protokolas.\n\n");
    fprintf(fr, "DALIS 2.1. Procesas 1. Protokolas.\n\n");
    //fprintf(fr, " %d.\n", );
}

void printError(int index, bool shouldExit){
    if (index < 0){
        printError(2, true);
    } else if (index >= (int) ERROR_ARR_SIZE){
        printError(3, true);
    } else
        printf("Klaida %d: %s", index, ERROR_CODE[index]);

    if (shouldExit)
        exit(index);
}

void printConclusion() {
    regCost = REGISTER_COST * WORKERS_MAXNUM;
    pregCost = REGISTER_COST * (WORKERS_MAXNUM - WORKER_SPLIT_COEFF);

    printf("DALIS 3.3. Isvados.\n\n");
    if (regCost > pregCost)
        printf("Procesas 2 yra geresnis uz procesa 1 kasos aparatu kainos atzvilgiu.\n");
    else if (regCost < pregCost)
        printf("Procesas 1 yra geresnis uz procesa 2 kasos aparatu kainos atzvilgiu.\n");
    else
        printf("Procesas 1 ir procesas 2 yra vienodai geri kasos aparatu kainos atzvilgiu\n");

    if (maxw_time > pmaxw_time)
        printf("Procesas 2 yra geresnis uz procesa 1 maksimalaus aptarnavimo laiko atzvilgiu.\n");
    else if (maxw_time < pmaxw_time)
        printf("Procesas 1 yra geresnis uz procesa 2 maksimalaus aptarnavimo laiko atzvilgiu.\n");
    else
        printf("Procesas 1 ir procesas 2 yra vienodai geri maksimalaus aptarnavimo laiko atzvilgiu.\n");

    if (midw_time > pmidw_time)
        printf("Procesas 2 yra geresnis uz procesa 1 vidutinio aptarnavimo laiko atzvilgiu.\n");
    else if (midw_time < pmidw_time)
        printf("Procesas 1 yra geresnis uz procesa 2 vidutinio aptarnavimo laiko atzvilgiu.\n");
    else
        printf("Procesas 1 ir procesas 2 yra vienodai geri vidutinio aptarnavimo laiko atzvilgiu.\n");

    if (minw_time > pminw_time)
        printf("Procesas 2 yra geresnis uz procesa 1 minimalaus aptarnavimo laiko atzvilgiu.\n");
    else if (minw_time < pminw_time)
        printf("Procesas 1 yra geresnis uz procesa 2 minimalaus aptarnavimo laiko atzvilgiu.\n");
    else
        printf("Procesas 1 ir procesas 2 yra vienodai geri minimalaus aptarnavimo laiko atzvilgiu.\n");
}

void fprintCounclusion() {
    fprintf(fr, "DALIS 3.3. Isvados.\n\n");
    regCost = REGISTER_COST * WORKERS_MAXNUM;
    pregCost = REGISTER_COST * (WORKERS_MAXNUM - WORKER_SPLIT_COEFF);

    if (regCost > pregCost)
        fprintf(fr, "Procesas 2 yra geresnis uz procesa 1 kasos aparatu kainos atzvilgiu.\n");
    else if (regCost < pregCost)
        fprintf(fr, "Procesas 1 yra geresnis uz procesa 2 kasos aparatu kainos atzvilgiu.\n");
    else
        fprintf(fr, "Procesas 1 ir procesas 2 yra vienodai geri kasos aparatu kainos atzvilgiu\n");

    if (maxw_time > pmaxw_time)
        fprintf(fr, "Procesas 2 yra geresnis uz procesa 1 maksimalaus aptarnavimo laiko atzvilgiu.\n");
    else if (maxw_time < pmaxw_time)
        fprintf(fr, "Procesas 1 yra geresnis uz procesa 2 maksimalaus aptarnavimo laiko atzvilgiu.\n");
    else
        fprintf(fr, "Procesas 1 ir procesas 2 yra vienodai geri maksimalaus aptarnavimo laiko atzvilgiu.\n");

    if (midw_time > pmidw_time)
        fprintf(fr, "Procesas 2 yra geresnis uz procesa 1 vidutinio aptarnavimo laiko atzvilgiu.\n");
    else if (midw_time < pmidw_time)
        fprintf(fr, "Procesas 1 yra geresnis uz procesa 2 vidutinio aptarnavimo laiko atzvilgiu.\n");
    else
        fprintf(fr, "Procesas 1 ir procesas 2 yra vienodai geri vidutinio aptarnavimo laiko atzvilgiu.\n");

    if (minw_time > pminw_time)
        fprintf(fr, "Procesas 2 yra geresnis uz procesa 1 minimalaus aptarnavimo laiko atzvilgiu.\n");
    else if (minw_time < pminw_time)
        fprintf(fr, "Procesas 1 yra geresnis uz procesa 2 minimalaus aptarnavimo laiko atzvilgiu.\n");
    else
        fprintf(fr, "Procesas 1 ir procesas 2 yra vienodai geri minimalaus aptarnavimo laiko atzvilgiu.");
}






