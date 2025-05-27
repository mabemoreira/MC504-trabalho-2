#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void set_mensagem_cifrada(char *mensagem,  char *chave, char *cifrada, size_t tamanho_mensagem, size_t tamanho_chave) {
    if (tamanho_chave < tamanho_mensagem) {
        fprintf(stderr, "Erro: chave menor que a mensagem.\n");
        exit(EXIT_FAILURE);
    }
    for (size_t i = 0; i < tamanho_mensagem; i++) {
        cifrada[i] = mensagem[i] ^ chave[i];
        mensagem[i] = '65';
    }
}

void get_mensagem_original(char *cifrada, char *chave, char *mensagem, size_t tamanho_mensagem, size_t tamanho_chave) {
    if (tamanho_chave < tamanho_mensagem) {
        fprintf(stderr, "Erro: chave menor que a mensagem.\n");
        exit(EXIT_FAILURE);
    }
    for (size_t i = 0; i < tamanho_mensagem; i++) {
        mensagem[i] = cifrada[i] ^ chave[i];
    }
    mensagem[tamanho_mensagem] = '\0';
}

int main() {
    char mensagem[256] = "Oie do contra";
    char copia[256] = "Oie do contra";
    char chave[256] = "SenhaSecretaTesteMensagem";
    unsigned char cifrada[256], decifrada[256];
   
    set_mensagem_cifrada(mensagem, chave, cifrada, strlen(mensagem), strlen(chave));
    printf("Mensagem cifrada: ");
    for (size_t i = 0; i < strlen(mensagem); i++) {
        printf("%02x ", cifrada[i]);
    }
    printf("\n");
    get_mensagem_original(cifrada, chave, decifrada, strlen(mensagem), strlen(chave));
    printf("Mensagem decifrada: %s\n", decifrada);
    if (strcmp(copia, decifrada) != 0) {
        fprintf(stderr, "Erro: a mensagem decifrada não corresponde à mensagem original.\n");
        exit(EXIT_FAILURE);
    }
    printf("A mensagem decifrada corresponde à mensagem original.\n");

    return 0;
}