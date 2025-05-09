#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>

//Konstantos.
#define SEPARATOR printf("=======================================\n")
#define NEWLINE printf("\n")
#define FNEWLINE fprintf(fr, "\n")
#define MAX_CITY_NAME_LENGTH 20
#define MAX_FILE_NAME 20

//Funkciju apibrezimai.
char** initializeCityList(char*, char**, int*);
int** initializeGraph(char*, char**, int*, char*, char*);
char** nameExists(char**, int*, char*);
int cityNameToIndex(char**, int, char*);
void djikstraAlgorithm(int*, int*, int**, int, int, int, char**, FILE*);
int minDistance(int*, bool*, int);
void printCityList(char**, int);
void fprintCityList(char**, int, FILE*);
void printGraph(int**, int, char**, int, int*);
void fprintGraph(FILE* fr, int**, int, char**, int, int*);
void printPath(int*, int, char**, int*);
void fprintPath(int*, int, char**, int*, FILE*);
void printInfo();
void fprintInfo(FILE*);
void fprintVisited(bool*, int, FILE*);
void fprintWeights(int*, int, FILE*);
void fprintStep(int*, bool*, int, int, FILE*);
void rewriteData(char*, FILE*);

//Globalus kintamieji.
int stepCounter = 1;
int textBuffer = 5;
char** cityList = NULL;

//Pagrindine programa.
int main()
{
    int** graph = NULL;
    int cityListSize = 0;

    int maxChar = 0;
    int* sizes;

    char origin[MAX_CITY_NAME_LENGTH];
    char destination[MAX_CITY_NAME_LENGTH];

    int* distances;
    int* path;

    FILE* fd;
    char fdName[MAX_FILE_NAME];
    FILE* fr;
    char frName[MAX_FILE_NAME];

    printInfo();

    while(1) {
        printf("Iveskite duomenu failo pavadinima: ");
        scanf("%s", fdName);
        if ((fd = fopen(fdName, "r")) != NULL) {
            printf("Failas atidarytas sekmingai!\n");
            SEPARATOR;
            cityList = initializeCityList(fdName, cityList, &cityListSize);
            sizes = (int*) malloc(cityListSize * sizeof(int));
            graph = initializeGraph(fdName, cityList, &cityListSize, origin, destination);
            printf("\nSuformuota kaiminystes matrica.\n\n");
            printGraph(graph, cityListSize, cityList, maxChar, sizes);
            SEPARATOR;
            break;
        } else {
            while(getchar() != '\n');
            printf("Nepavyko atidaryti failo!\n");
        }
    }

    while(1) {
        printf("Iveskite isvesties failo pavadinima: ");
        scanf("%s", frName);
        if ((fr = fopen(frName, "w")) != NULL) {
            printf("Isveties failas sukurtas!\n");
            fprintInfo(fr);
            rewriteData(fdName, fr);
            fprintf(fr, "DALIS 1.3. Apdoroti duomenys.\n");
            fprintf(fr, "\nSuformuota kaiminystes matrica.\n\n");
            fprintGraph(fr, graph, cityListSize, cityList, maxChar, sizes);
            SEPARATOR;
            break;
        } else {
            while(getchar() != '\n');
            printf("Nepavyko sukurti isvesties failo!\n");
        }
    }

    distances = (int*) malloc(cityListSize * sizeof(int));
    path = (int*) malloc(cityListSize * sizeof(int));

    fprintf(fr, "\nDALIS 2. Vykdymas.\n\n");

    int inxOrigin = cityNameToIndex(cityList, cityListSize, origin);
    int inxDestination = cityNameToIndex(cityList, cityListSize, destination);
    djikstraAlgorithm(distances, path, graph, cityListSize, inxOrigin, inxDestination, cityList, fr);

    fprintf(fr, "DALIS 3.1. Rezultatas.\n\n");
    printf("DALIS 3.1. Rezultatas.\n\n");

    if (path[inxDestination] != -1) {
        fprintf(fr, "Pigiausias skrydis %s -> %s kainuoja %d Eur.\n\n", origin, destination, distances[inxDestination]);
        fprintf(fr, "Skrydzio marsrutas.\n");
        fprintf(fr, "%s", cityList[inxOrigin]);
        fprintPath(path, inxDestination, cityList, distances, fr);
        fprintf(fr, ".");

        printf("Pigiausias skrydis %s -> %s kainuoja %d Eur.\n\n", origin, destination, distances[inxDestination]);
        printf("Skrydzio marsrutas.\n");
        printf("%s", cityList[inxOrigin]);
        printPath(path, inxDestination, cityList, distances);
        printf(".");
    } else {
        fprintf(fr, "Marsrutas %s -> %s neegzistuoja.", cityList[inxOrigin], cityList[inxDestination]);
        printf("Marsrutas %s -> %s neegzistuoja.", cityList[inxOrigin], cityList[inxDestination]);
    }
    NEWLINE;
    SEPARATOR;

    fclose(fr);
    free(distances);
    free(path);
    free(cityList);
    free(graph);
    return 0;
}

//Funkciju aprasai.
//Inicijuoja iskirtiniu miestu sarasa.
char** initializeCityList(char* fileName, char** cityList, int* cityListSize) {
    int n, price;
    char city1[MAX_CITY_NAME_LENGTH], city2[MAX_CITY_NAME_LENGTH];
    FILE* fd;

    if ((fd = fopen(fileName, "r")) != NULL) {
        fscanf(fd, "%d", &n);
        for (int i = 0 ; i < n ; ++i){
            fscanf(fd, "%s %s%d", city1, city2, &price);
            cityList = nameExists(cityList, cityListSize, city1);
            cityList = nameExists(cityList, cityListSize, city2);
        }
        fscanf(fd, "%s %s", city1, city2);
        cityList = nameExists(cityList, cityListSize, city1);
        cityList = nameExists(cityList, cityListSize, city2);
    }
    fclose(fd);
    return cityList;
}

//Inicijuoja grafa.
int** initializeGraph(char* fileName, char** cityList, int* cityListSize, char* origin, char* destination) {
    int n, price, inx1, inx2;
    char city1[MAX_CITY_NAME_LENGTH], city2[MAX_CITY_NAME_LENGTH];
    int** graph;
    FILE* fd;

    graph = (int**) calloc((*cityListSize), sizeof(int*));
    for (int k = 0 ; k < *cityListSize ; ++k)
        graph[k] = (int*) calloc(*cityListSize, sizeof(int));

    if ((fd = fopen(fileName, "r")) != NULL) {
        fscanf(fd, "%d", &n);
        for (int j = 0 ; j < n ; ++j) {
            fscanf(fd, "%s %s%d", city1, city2, &price);
            inx1 = cityNameToIndex(cityList, *cityListSize, city1);
            inx2 = cityNameToIndex(cityList, *cityListSize, city2);
            if (inx2 != -1 && inx2 != -1) {
                graph[inx1][inx2] = price;
                graph[inx2][inx1] = price;
            }
        }
        fscanf(fd, "%s %s", origin, destination);
    }

    fclose(fd);
    return graph;
}

//Paziuri ar jau miestu sarase egzistuoja toks miesto vardas.
char** nameExists(char** cityList, int* cityListSize, char* city) {
    for (int i = 0 ; i < *cityListSize ; ++i) {
        if (!strcmp(cityList[i], city))
            return cityList;
    }

    ++(*cityListSize);
    cityList = (char**) realloc(cityList, *cityListSize * sizeof(char*));
    cityList[*cityListSize - 1] = (char*) malloc(MAX_CITY_NAME_LENGTH);
    strcpy(cityList[*cityListSize - 1], city);
    return cityList;
}

//Pagal miesto varda grazina saraso indeksa.
int cityNameToIndex(char** cityList, int cityListSize, char* city) {
    for (int i = 0; i < cityListSize ; ++i) {
        if (!strcmp(cityList[i], city))
            return i;
    }
    return -1;
}

//Djikstros algoritmu randa maziausia kaina tarp miestu.
void djikstraAlgorithm(int* distances, int* path, int** graph, int size, int inxOrigin, int inxDestination, char** cityList, FILE* fr) {
    int subCounter = 1;
    bool sptSet[size];
    char city[MAX_CITY_NAME_LENGTH] = "FIRST";

    fprintf(fr, "VEIKSMAS_0. Pradine virsune %s paruosta darbui.\n\n", cityList[inxOrigin]);
    for (int i = 0 ; i < size ; ++i) {
        path[i] = -1;
        distances[i] = INT_MAX;
        sptSet[i] = false;
    }
    distances[inxOrigin] = 0;

    for (int j = 0 ; j < size - 1 ; ++j) {
        int minV = minDistance(distances, sptSet, size);
        sptSet[minV] = true;

        if (distances[minV] == 0)
            fprintf(fr, "VEIKSMAS_%d. Atidaroma %s.\n", stepCounter, cityList[minV]);
        else
            fprintf(fr, "VEIKSMAS_%d. Uzdaroma %s, atidaroma %s, nes svoris minV=%d i %s maziausias.\n", stepCounter, city, cityList[minV], distances[minV], cityList[minV]);

        for (int k = 0 ; k < size ; ++k) {
            if (!sptSet[k] && graph[minV][k] && distances[minV] != INT_MAX && distances[minV] + graph[minV][k] < distances[k]) {
                path[k] = minV;
                distances[k] = distances[minV] + graph[minV][k];
                fprintf(fr, "\n\tPOVEIKSMIS_%d.%d. Tikrinama %s -> %s.\n\tBUSENA.         ", stepCounter, subCounter, cityList[minV], cityList[k]);
                fprintf(fr, "Rasta pigiau %s -> %s, svoris minV=%d.\n", cityList[inxOrigin], cityList[k], distances[k]);
                ++subCounter;
            }
        }
        FNEWLINE;
        strcpy(city, cityList[minV]);
        subCounter = 1;
        ++stepCounter;
    }
    fprintf(fr, "VEIKSMAS_%d. Uzdaroma %s. Darbas baigtas.\n\n", stepCounter, city);
}

//Randa minimalu atstuma.
int minDistance(int* distances, bool* sptSet, int size) {
    int min = INT_MAX, minInx;

    for (int i = 0 ; i < size ; ++i) {
        if (sptSet[i] == false && distances[i] <= min) {
            min = distances[i];
            minInx = i;
        }
    }
    return minInx;
}

//Isspausdina isskirtiniu miestu sarasa.
void printCityList(char** list, int size) {
    for (int i = 0 ; i < size ; ++i)
        printf("M%d = %s.\n", i + 1, list[i]);
}

//Isspausdina isskirtiniu miestu sarasa i faila.
void fprintCityList(char** cityList, int cityListSize, FILE* fr) {
    for (int i = 0 ; i < cityListSize ; ++i)
        fprintf(fr, "M%d = %s.\n", i + 1, cityList[i]);
}

//Isspausdina grafa.
void printGraph(int** graph, int size, char** cityList, int maxChar, int* sizes) {
    printf("   |   ");
    for (int j = 0 ; j < size ; ++j) {
        printf("%-2s  ", cityList[j]);
    }
    NEWLINE;
    printf("-----------------------------------------\n");

    for (int i = 0 ; i < size ; ++i) {
        printf("%2s | ", cityList[i]);
        for (int j = 0 ; j < size ; ++j) {
            printf("%3d ", graph[i][j]);
        }
        printf("\n");
    }
}

//Isspausdina grafa i faila.
void fprintGraph(FILE* fr, int** graph, int size, char** cityList, int maxChar, int* sizes) {
    fprintf(fr, "   |   ");
    for (int j = 0 ; j < size ; ++j) {
        fprintf(fr, "%-2s  ", cityList[j]);
    }
    FNEWLINE;
    fprintf(fr, "-----------------------------------------\n");

    for (int i = 0 ; i < size ; ++i) {
        fprintf(fr, "%2s | ", cityList[i]);
        for (int j = 0 ; j < size ; ++j) {
            fprintf(fr, "%3d ", graph[i][j]);
        }
        FNEWLINE;
    }
}

//Atspausdina kelia tarp miestu.
void printPath(int* path, int dest, char** cityList, int* distances) {
    if (path[dest] == -1)
        return;

    printPath(path, path[dest], cityList, distances);
    printf(" -> %s", cityList[dest]);
}

//Atspausdina kelia tarp mistu i faila.
void fprintPath(int* path, int dest, char** cityList, int* distances, FILE* fr) {
    if (path[dest] == -1)
        return;

    fprintPath(path, path[dest], cityList, distances, fr);
    fprintf(fr, " -> %s", cityList[dest]);
}

//Isspausdina informacija apie programa.
void printInfo() {
    SEPARATOR;
    printf("UZDUOTIS nr. 4, VARIANTAS 1.\n");
    printf("AUTORIUS Nirtautas Sadauskas, GRUPE 2, POGRUPIS 2.\n");

    SEPARATOR;
    printf("DALIS 1.1. SALYGA.\n\n");
    printf("Duotas lektuvu skrydziu sarasas: miestas, miestas, kaina.\n"
           "Rasti pigiausia skrydzio is vieno duoto miesto i kita duota miesta marsruta ir jo kaina.\n"
           "Numatyti atveji, kad toks marsrutas neegzistuoja.\n"
           "(grafo realizacija paremta kaimynystes matrica)\n");
    SEPARATOR;
}

//Isspausdina informacija apie programa i faila.
void fprintInfo(FILE* fr) {
    fprintf(fr, "UZDUOTIS nr. 4, VARIANTAS 1.\n");
    fprintf(fr, "AUTORIUS Nirtautas Sadauskas, GRUPE 2, POGRUPIS 2.\n\n");

    fprintf(fr, "DALIS 1.1. SALYGA.\n\n");
    fprintf(fr, "Duotas lektuvu skrydziu sarasas: miestas, miestas, kaina.\n"
           "Rasti pigiausia skrydzio is vieno duoto miesto i kita duota miesta marsruta ir jo kaina.\n"
           "Numatyti atveji, kad toks marsrutas neegzistuoja.\n"
           "(grafo realizacija paremta kaimynystes matrica)\n\n");
    fprintf(fr, "DALIS 1.2. DUOMENYS.\n\n");
}

//Isspausdina aplankytu virsuniu masyvo turini.
void fprintVisited(bool* array, int size, FILE* fr) {
    fprintf(fr, "%*cAr virsune aplankyta?       {", textBuffer + 2, ' ');
    for (int i = 0 ; i < size ; ++i) {
        if (i + 1 == size)
            fprintf(fr, "M%d %s", i, (array[i]) ? "Taip" : "Ne");
        else
            fprintf(fr, "M%d %s, ", i, (array[i]) ? "Taip" : "Ne");
    }
    fprintf(fr, "}\n");
}

//Isspausdina svoriu masyvo turini.
void fprintWeights(int* array, int size, FILE* fr) {
    fprintf(fr, "%*cSvoriai tarp miestu eurais. {", textBuffer + 2, ' ');
    for (int i = 1 ; i < size ; ++i) {
        if (i + 1 == size)
            fprintf(fr, "M0->M%d %d", i, (array[i] == INT_MAX) ? -1 : array[i]);
        else
            fprintf(fr, "M0->M%d %d, ", i, (array[i] == INT_MAX) ? -1 : array[i]);
    }
    fprintf(fr, "}\n");
}

//Perraso duomenis ir vieno failo i kita.
void rewriteData(char* fdName, FILE* fr) {
    FILE* fd = fopen(fdName, "r");
    int n, price;
    char city1[MAX_CITY_NAME_LENGTH], city2[MAX_CITY_NAME_LENGTH];

    fscanf(fd, "%d", &n);
    fprintf(fr, "Marsrutu sarasas.\n\n");
    for (int i = 0 ; i < n ; ++i) {
        fscanf(fd, "%s %s %d", city1, city2, &price);
        fprintf(fr, "%s <-> %s. %3d Eur.\n", city1, city2, price);
    }
    fprintf(fr, "\nPageidaujamas marsrutas. ");
    fscanf(fd, "%s %s", city1, city2);
    fprintf(fr, "%s -> %s.\n\n", city1, city2);
}

