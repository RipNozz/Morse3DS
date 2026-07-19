#include <3ds.h>
#include <stdio.h>
#include <string.h>

bool modoTextoAMorse = true;

char entrada[256] = "";
char salida[1024] = "";

PrintConsole topScreen;

typedef struct
{
    char letra;
    const char *morse;
} Morse;

Morse tabla[] =
{
    {'A', ".-"},
    {'B', "-..."},
    {'C', "-.-."},
    {'D', "-.."},
    {'E', "."},
    {'F', "..-."},
    {'G', "--."},
    {'H', "...."},
    {'I', ".."},
    {'J', ".---"},
    {'K', "-.-"},
    {'L', ".-.."},
    {'M', "--"},
    {'N', "-."},
    {'O', "---"},
    {'P', ".--."},
    {'Q', "--.-"},
    {'R', ".-."},
    {'S', "..."},
    {'T', "-"},
    {'U', "..-"},
    {'V', "...-"},
    {'W', ".--"},
    {'X', "-..-"},
    {'Y', "-.--"},
    {'Z', "--.."},

    {'0',"-----"},
    {'1',".----"},
    {'2',"..---"},
    {'3',"...--"},
    {'4',"....-"},
    {'5',"....."},
    {'6',"-...."},
    {'7',"--..."},
    {'8',"---.."},
    {'9',"----."}
};

#define TOTAL (sizeof(tabla)/sizeof(tabla[0]))

void dibujarPantalla()
{
    // Pantalla superior
    consoleSelect(&topScreen);
    consoleClear();

    printf("========================================\n");
    printf("            MORSE3DS v1.1\n");
    printf("========================================\n\n");

    if(modoTextoAMorse)
        printf("Modo: Texto -> Morse\n\n");
    else
        printf("Modo: Morse -> Texto\n\n");

    printf("Entrada:\n");
    printf("%s\n\n", entrada);

    printf("Resultado:\n");
    printf("%s\n", salida);
}

void textoAMorse()
{
    salida[0] = '\0';

    for (int i = 0; entrada[i] != '\0'; i++)
    {
        char c = entrada[i];

        // Convertir minúsculas a mayúsculas
        if (c >= 'a' && c <= 'z')
            c -= 32;

        // Espacio entre palabras
        if (c == ' ')
        {
            strcat(salida, "/ ");
            continue;
        }

        // Buscar la letra en la tabla
        for (int j = 0; j < TOTAL; j++)
        {
            if (tabla[j].letra == c)
            {
                strcat(salida, tabla[j].morse);
                strcat(salida, " ");
                break;
            }
        }
    }
}

char buscarLetra(const char *codigo)
{
    for(int i = 0; i < TOTAL; i++)
    {
        if(strcmp(tabla[i].morse, codigo) == 0)
            return tabla[i].letra;
    }

    return '?';
}

void morseATexto()
{
    salida[0] = '\0';

    char copia[1024];
    strncpy(copia, entrada, sizeof(copia)-1);
copia[sizeof(copia)-1] = '\0';

    char *token = strtok(copia, " ");

    while(token != NULL)
    {
        if(strcmp(token, "/") == 0)
        {
            strcat(salida, " ");
        }
        else
        {
            char letra[2];
            letra[0] = buscarLetra(token);
            letra[1] = '\0';

            strcat(salida, letra);
        }

        token = strtok(NULL, " ");
    }
}

int main()
{
    gfxInitDefault();

consoleInit(GFX_TOP, &topScreen);

    dibujarPantalla();

    while (aptMainLoop())
    {
        hidScanInput();

        u32 kDown = hidKeysDown();

        if(kDown & KEY_START)
            break;

        if(kDown & KEY_B)
{
    modoTextoAMorse = !modoTextoAMorse;

    entrada[0] = '\0';
    salida[0] = '\0';

    dibujarPantalla();
}

        if(kDown & KEY_A)
        {
            SwkbdState swkbd;
            static char texto[256];

            swkbdInit(&swkbd, SWKBD_TYPE_NORMAL, 1, 255);

            swkbdSetHintText(&swkbd, "Escribe aqui");

            if(swkbdInputText(&swkbd, texto, sizeof(texto)) == SWKBD_BUTTON_CONFIRM)
            {
                strcpy(entrada,texto);
                dibujarPantalla();
            }
        }

        if(kDown & KEY_Y)
{
    if(modoTextoAMorse)
    {
        textoAMorse();
    }
    else
    {
        morseATexto();
    }

    dibujarPantalla();
}
        gspWaitForVBlank();
    }

    gfxExit();

    return 0;
}