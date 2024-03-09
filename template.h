#pragma once

#define _CRT_SECURE_NO_WARNINGS 
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <string.h>  
#define CLS system("cls")
#define PAUSE system("pause")
#define FLUSH while(getchar() != '\n')

void getChoice(int* choice) {
    int run = 1; 
    do {
        printf("\nWould you like to start a new game (1) or load a previous game (0): "); 
        int pick = -1; 
        if (scanf("%d", &pick) != 1 || pick > 1 || pick < 0) { 
            FLUSH; 
            printf("\nPlease submit a proper value\n"); 
            continue;
        }
        *choice = pick; 
        run = 0; 
    } while (run);
    FLUSH; 
}

void getInt(int* choice, char* i) {
    int run = 1; 
    do {
        printf("%s", &*i);
        int pick = -1; 
        if (scanf("%d", &pick) != 1) {
            FLUSH;
            printf("\nPlease submit a proper value\n"); 
            continue;
        }
        *choice = pick; 
        run = 0; 
    } while (run);
    FLUSH; 
}

void getFloat(float* cost, char* i) {
    int run = 1; 
    do {
        printf("%s", &*i); 
        float price = -1;
        if (scanf("%f", &price) != 1 || price < 0) { 
            FLUSH; 
            printf("\nPlease submit a proper value\n"); 
            continue;
        }
        *cost = price; 
        run = 0; 
    } while (run);
    FLUSH; 
}

void getString(char* model, char* i) {
    int x;
    printf("%s", &*i);
    x = scanf("%[^\n]", &*model);
    FLUSH;
}

int randomNum(int LB, int UB) {
    srand((unsigned)time(NULL));
    int num = (LB + rand() % (UB - LB + 1));
}