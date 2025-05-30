#include "config.h"

#include <stdio.h>

// Definição de constantes de configuração
const int SELL_POS_X = (int)(750 * BASE_WIDTH_INT / BASE_WIDTH_FLOAT);
const int SELL_POS_Y = (int)(40 * BASE_HEIGHT_INT / BASE_HEIGHT_FLOAT);

// Define o tamanho da fonte base e a escala para a resolução atual.
const int FONT_SIZE = (int)(BASE_FONT_SIZE * ((float)BASE_HEIGHT_INT / BASE_HEIGHT_FLOAT));

int ReadHordesConfig(char *configPath, int hordes[], int maxHordes) {
    FILE *configFile = fopen(configPath, "r");  // r: readOnly mode.
    if (configFile == NULL) {
        printf("Erro ao abrir arquivo %s", configPath);
        return -1;
    }

    int read_val;
    int count = 0;

    // 1. Continua lendo enquanto 'fscanf' encontrar um inteiro.
    // 2. PARA se o contador atingir o tamanho máximo do array.
    while (count < maxHordes && fscanf(configFile, "%d", &read_val) == 1 ) {
        if (read_val == 0) break;
        hordes[count] = read_val;
        count++;
    }

    fclose(configFile);
    return count;  // Retorna o número de itens que foram lidos.
}
