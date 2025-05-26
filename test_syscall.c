#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/syscall.h>
#include <unistd.h>

#define __NR_set_mensagem_cifrada 468
#define __NR_get_mensagem_original 469

int main() {

    int retorno;
    char buffer_set[256], buffer_get[256];
    
    char mensagem[] = "TesteMensagem";
    char chave[] = "SenhaSecretaTesteMensagem";

    printf("Mensagem original: %s\n", mensagem);
    
    retorno = syscall(__NR_set_mensagem_cifrada, mensagem, chave, buffer_set, strlen(mensagem), strlen(chave));

    if (retorno < 0) {
        perror("Erro ao chamar syscall set_mensagem_cifrada");
        return 1;
    }

    printf("\nMensagem cifrada com sucesso.\n");
    printf("Mensagem original: %s\n", mensagem);
    printf("Chave utilizada: %s\n", chave);
    printf("Mensagem cifrada (hex): ");
    for (size_t i = 0; i < strlen(mensagem); i++) {
        printf("%02x ", (unsigned char)buffer_set[i]);
    }
    printf("\n");

    retorno = syscall(__NR_get_mensagem_original, buffer_get, chave, buffer_set, strlen(buffer_set), strlen(chave));

    if (retorno < 0) {
        perror("Erro ao chamar syscall get_mensagem_original");
        return 1;
    }

    printf("\nMensagem original recuperada com sucesso.\n");
    printf("Mensagem decifrada: %s\n", buffer_get);

    if (strcmp(buffer_get, mensagem) != 0) {
        fprintf(stderr, "A mensagem original recuperada não corresponde à mensagem original.\n");
        return 1;
    }
    printf("A mensagem original recuperada corresponde à mensagem original.\n");

    return 0;
}