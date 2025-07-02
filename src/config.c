#define _CRT_SECURE_NO_WARNINGS

#include "config.h"

#include <stdio.h>

const int SELL_POS_X = (int)(750 * BASE_WIDTH_INT / BASE_WIDTH_FLOAT);
const int SELL_POS_Y = (int)(40 * BASE_HEIGHT_INT / BASE_HEIGHT_FLOAT);

const int FONT_SIZE = (int)(BASE_FONT_SIZE * ((float)BASE_HEIGHT_INT / BASE_HEIGHT_FLOAT));

int ReadHordesConfig(char *configPath, int hordes[MAX_HORDES], int maxHordes) {
    FILE *configFile = fopen(configPath, "r");
    if (configFile == NULL) {
        printf("Erro ao abrir arquivo %s", configPath);
        return -1;
    }
    
    // Retorna ponteiro pro inicio do arquivo.
    rewind(configFile);
    int read_val;
    int count = 0;

    while (count < maxHordes && fscanf(configFile, "%d", &read_val) == 1) {
        // Quando não há mais números a serem lidos.
        if (read_val == 0) break;
        hordes[count] = read_val;
        count++;
    }

    fclose(configFile);
    return count;
}
