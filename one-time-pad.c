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
    }
    printf("Cifrada: ");
    for (size_t i = 0; i < tamanho_mensagem; i++) {
        printf("%02x ", cifrada[i]);
    }
    printf("\n");
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
    printf("Decifrada: %s\n", mensagem);
}

int main() {
    char mensagem[256] = "ola mc504";
    char chave[256] =    "aaaaaaaaa";
    unsigned char cifrada[256], decifrada[256];
   
    set_mensagem_cifrada(mensagem, chave, cifrada, 9, 9);
    get_mensagem_original(cifrada, chave, decifrada, 9, 9);

    return 0;
}
