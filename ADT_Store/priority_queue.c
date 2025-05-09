#include <stdio.h>
#include <stdlib.h>
#include "functions.h"

void PrintMenu()
{
    printf("==================================================\n");
    printf("[1] - Sukurti nauja tuscia prioritetine eile.\n");
    printf("[2] - Irasyti nauja elementa i prioritetine eile.\n");
    printf("[3] - Pasalinti auksciausio prioriteto elementa.\n");
    printf("[4] - Sujungti dvi prioritetines eiles.\n");
    printf("[5] - Atspausdinti prioritetine eile.\n");
    printf("[6] - Patikrinti ar prioritetine eile tuscia.\n");
    printf("[7] - Sunaikinti visa prioritetine eile.\n");
    printf("[8] - Uzdaryti programa.\n");
    printf("==================================================\n");
}

void QueueList(int *queue_counter)
{
    printf("=============================\n");
    for (int i = 0; i < *queue_counter; i++)
    {
        printf("[%d] - Prioritetine eile nr. %d.\n", i + 1, i + 1);
    }
    printf("=============================\n");
    printf("Ivestis: ");
}

int main()
{
    S_PriorityQueue **p_queue;
    int queue_counter = 0;
    int value, priority;
    int queue_input, queue_input2;
    char user_input, enter;

    // User Interface start ====================================================
    printf("Si programa kuria prioritetines eiles. Prasome iveskite skaiciu, atitinkanti salia jo pamineta funkcija, ir spauskite klavisa [Return].\n");
    PrintMenu();
    printf("Rasykite \"?\", kad atspausdintu programos funkciju pasirinkimus.\n");

    while (1)
    {
        printf("Ivestis: ");
        if (scanf("%c%c", &user_input, &enter) == 2 && enter == '\n')
        {
            if (user_input == '?')
            {
                PrintMenu();
            }
            else if (user_input == '1') // Create()
            {
                p_queue[queue_counter] = Create();
                queue_counter++;
            }
            else if (user_input == '2') // Insert()
            {
                if (queue_counter >= 1)
                {
                    printf("Pasirinkite i kuri is sukurtu sarasu prideti nauja elementa, irasydami jam atitinkanti skaiciu, ir spauskite [Return]:\n");
                    QueueList(&queue_counter);
                    if ((scanf("%d%c", &queue_input, &enter) == 2) && (queue_input <= (queue_counter)) && (queue_input > 0))
                    {
                        printf("Prasome ivesti naujo elemento reiksme ir spauskite [Return]: ");
                        scanf("%d%c", &value, &enter);
                        printf("Prasome ivesti naujo elemento prioriteta ir spauskite [Return]: ");
                        scanf("%d%c", &priority, &enter);
                        Insert(p_queue[queue_input - 1], value, priority);
                        printf("Elementas sekmingai iterptas.\n");
                    }
                    else
                    {
                        printf("Error 2: pasirinkimas neegzistuoja.\n\n");
                    }
                }
                else
                {
                    printf("Error 1: Veiksmas negalimas. Sarasas nesukurtas.\n\n");
                }
            }
            else if (user_input == '3') // Remove()
            {
                if (queue_counter >= 1)
                {
                    printf("Pasirinkite is kurio is sukurtu sarasu pasalinti auksciausio prioriteto elementa, irasydami jam atitinkanti skaiciu, ir spauskite [Return]:\n");
                    QueueList(&queue_counter);
                    if ((scanf("%d%c", &queue_input, &enter) == 2) && (queue_input <= (queue_counter)) && (queue_input > 0))
                    {
                        Remove(p_queue[queue_input - 1]);
                    }
                    else
                    {
                        printf("Error 2: pasirinkimas neegzistuoja.\n\n");
                    }
                }
                else
                {
                    printf("Error 1: Veiksmas negalimas. Sarasas nesukurtas.\n\n");
                }
            }
            else if (user_input == '4') // Join()
            {
                if (queue_counter >= 1)
                {
                    printf("Pasirinkite is kuriuos du sarasus sujungti, irasydami ju atitinkamus skaicius, atskirtus tarpu, ir spauskite [Return]:\n");
                    QueueList(&queue_counter);
                    if ((scanf("%d %d%c", &queue_input, &queue_input2, &enter) == 3) && (queue_input <= (queue_counter)) && (queue_input > 0) && (queue_input2 <= (queue_counter)) && (queue_input2 > 0))
                    {
                        if (queue_input != queue_input2)
                        {
                            Join(p_queue[queue_input - 1], p_queue[queue_input2 - 1]);
                            printf("Prioritetines eiles \"%d\" ir \"%d\" sekmingai sujungtos.\n", queue_input, queue_input2);
                            queue_counter--;
                            for (int i = (queue_input2 - 1); i < queue_counter; i++)
                            {
                                Join(p_queue[i], p_queue[i + 1]);
                            }
                        }
                        else
                        {
                            printf("Error 4: Sujungimas nepavyko. Negalima sujungti eiles su pacia savimi.\n");
                        }
                    }
                    else
                    {
                        printf("Error 2: pasirinkimas neegzistuoja.\n\n");
                    }
                }
                else
                {
                    printf("Error 1: Veiksmas negalimas. Sarasas nesukurtas.\n\n");
                }
            }
            else if (user_input == '5') // Print()
            {
                if (queue_counter >= 1)
                {
                    printf("Pasirinkite kuria eile noretumete atspausdinti, irasydami jos atitinkama skaiciu, ir spauskite [Return]:\n");
                    QueueList(&queue_counter);
                    if ((scanf("%d%c", &queue_input, &enter) == 2) && (queue_input <= (queue_counter)) && (queue_input > 0))
                    {
                        printf("=============================\n");
                        Print(p_queue[queue_input - 1]);
                        printf("=============================\n");
                    }
                    else
                    {
                        printf("Error 2: pasirinkimas neegzistuoja.\n\n");
                    }
                }
                else
                {
                    printf("Error 1: Veiksmas negalimas. Sarasas nesukurtas.\n\n");
                }
            }
            else if (user_input == '6') // IsEmpty()
            {
                if (queue_counter >= 1)
                {
                    printf("Pasirinkite kuria eile noretumete patikrinti, irasydami jos atitinkama skaiciu, ir spauskite [Return]:\n");
                    QueueList(&queue_counter);
                    if ((scanf("%d%c", &queue_input, &enter) == 2) && (queue_input <= (queue_counter)) && (queue_input > 0))
                    {
                        if (IsEmpty(p_queue[queue_input - 1]) == 1)
                        {
                            printf("Eile nr. \"%d\" tuscia.\n", queue_input);
                        }
                        else
                        {
                            printf("Eile nr. \"%d\" ne tuscia.\n", queue_input);
                        }
                    }
                    else
                    {
                        printf("Error 2: pasirinkimas neegzistuoja.\n\n");
                    }
                }
                else
                {
                    printf("Error 1: Veiksmas negalimas. Sarasas nesukurtas.\n\n");
                }
            }
            else if (user_input == '7') // DeletePQueue()
            {
                if (queue_counter >= 1)
                {
                    printf("Pasirinkite kuria eile noretumete pasalinti, irasydami jos atitinkama skaiciu, ir spauskite [Return]:\n");
                    QueueList(&queue_counter);
                    if ((scanf("%d%c", &queue_input, &enter) == 2) && (queue_input <= (queue_counter)) && (queue_input > 0))
                    {
                        DeletePQueue(p_queue[queue_input - 1]);
                        printf("Prioritetine eile nr. \"%d\" pasalinta.\n", queue_input);
                        queue_counter--;
                        for (int i = (queue_input - 1); i < queue_counter; i++)
                        {
                            Join(p_queue[i], p_queue[i + 1]);
                        }
                    }
                    else
                    {
                        printf("Error 2: pasirinkimas neegzistuoja.\n\n");
                    }
                }
                else
                {
                    printf("Error 1: Veiksmas negalimas. Sarasas nesukurtas.\n\n");
                }
            }
            else if (user_input == '8')
            {
                printf("Programa uzdaroma.");
                break;
            }
            else
            {
                printf("Error 5: Ivestis neteisinga. Toks meniu punkstas neegzistuoja.\n\n");
            }
        }
        else
        {
            printf("Error 5: Ivestis neteisinga. Toks meniu punkstas neegzistuoja.\n\n");
        }
    }

    return 0;
}