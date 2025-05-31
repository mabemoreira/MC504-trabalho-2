#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <stdlib.h>


#define __NR_get_mensagem_original 468
#define __NR_set_mensagem_cifrada 469

//garante a segurança porque a chave nao pode se repetir
int verifica_chave_repetida(char **chaves, int num_chaves, const char *chave) {
    for (int i = 0; i < num_chaves; i++) {
        if (chaves[i] && strcmp(chaves[i], chave) == 0) {
            return 1; 
        }
    }
    return 0; 
}


int chama_syscall(char* mensagem, char* copia, const char* chave, char* mensagem_cifrada, char * mensagem_decifrada){
    int retorno;
    size_t len_mensagem = strlen(mensagem);
    size_t len_chave = strlen(chave);
    printf("\n\nMensagem original: %s\n", mensagem);
    printf("Chave utilizada: %s\n", chave);

    //cifra a mensagem e suja o vetor original
    retorno = syscall(__NR_set_mensagem_cifrada, mensagem, chave, mensagem_cifrada, len_mensagem , len_chave);

    if (retorno < 0) {
        perror("Erro ao chamar syscall set_mensagem_cifrada");
        return -1;
    }
    //imprime resultados da primeira syscall
    printf("\nMensagem cifrada com sucesso.\n");
    printf("Mensagem cifrada (hex): ");
    for (size_t i = 0; i < len_mensagem; i++) {
        printf("%02x ", (unsigned char)mensagem_cifrada[i]);
    }
    printf("\n");
    printf("Mensagem suja: %s\n", mensagem);

    //recebendo a mensagem decifrada
    retorno = syscall(__NR_get_mensagem_original, mensagem_decifrada, mensagem_cifrada, chave, len_mensagem, len_chave);

    if (retorno < 0) {
        perror("Erro ao chamar syscall get_mensagem_original");
        return -1;
    }
    mensagem_decifrada[len_mensagem] = '\0';

    printf("\nMensagem decifrada com sucesso.\n");
    printf("Mensagem decifrada: %s\n", mensagem_decifrada);

    // Verifica se a mensagem decifrada é a mensagem original
    if (strcmp(mensagem_decifrada, copia) != 0) {
        fprintf(stderr, "A mensagem decifrada não corresponde à mensagem original.\n\n\n");
        return -1;
    }
    printf("A mensagem decifrada corresponde à mensagem original.\n\n\n");
    return 0;
}

int main() {

    int iteracoes;
    printf("Quantas iterações deseja realizar? ");
        if (scanf("%d", &iteracoes) != 1 || iteracoes <= 0) {
            fprintf(stderr, "Número de iterações inválido.\n");
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            return -1;
        }
    char *chaves[iteracoes];

    for (int k = 0; k < iteracoes; k++) chaves[k] = NULL; //para nao dar segmentation fault

    for(int i = 0; i < iteracoes; i++) {
        int tamanho_mensagem, tamanho_chave; 
        char *mensagem, *copia, *chave, *mensagem_cifrada, *mensagem_decifrada;
        printf("Qual o tamanho da mensagem? ");
        if (scanf("%d", &tamanho_mensagem) != 1 || tamanho_mensagem <= 0) {
            fprintf(stderr, "Tamanho da mensagem inválido.\n");
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            return -1;
        }

        printf("Qual o tamanho da chave? ");
        if (scanf("%d", &tamanho_chave) != 1 || tamanho_chave <= 0) {
            fprintf(stderr, "Tamanho da chave inválido.\n");
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            return -1;
        }
        if(tamanho_chave < tamanho_mensagem){
            fprintf(stderr, "Tamanho da chave deve ser igual ao tamanho da mensagem.\n");
            return -1;
        }
        
        mensagem = (char *)malloc(tamanho_mensagem + 1); // +1 por conta do /0
        copia = (char *)malloc(tamanho_mensagem + 1); // precisamos dela porque vamos sujar a mensagem original
        chave = (char *)malloc(tamanho_chave + 1);
        mensagem_cifrada = (char *)malloc(tamanho_mensagem + 1);
        mensagem_decifrada = (char *)malloc(tamanho_mensagem + 1);

        if (!mensagem || !copia || !chave || !mensagem_cifrada || !mensagem_decifrada) {
            fprintf(stderr, "Erro ao alocar memória.\n");
            return -1;
        }

        printf("Digite a mensagem original: ");
        scanf(" %[^\n]", mensagem);
        mensagem[tamanho_mensagem] = '\0';

        strncpy(copia, mensagem, tamanho_mensagem);
        copia[tamanho_mensagem] = '\0';

        printf("Digite a chave: ");
        scanf(" %[^\n]", chave);
        chave[tamanho_chave] = '\0';

        if(verifica_chave_repetida(chaves, i, chave)) { // se a segurança está comprometida
            fprintf(stderr, "Chave repetida detectada, reinicie o processo\n");
            free(mensagem);
            free(copia);
            free(chave);
            free(mensagem_cifrada);
            free(mensagem_decifrada);
            continue;
        }
    
        else {
            chaves[i] = (char *)malloc(tamanho_chave + 1);
            if (!chaves[i]) {
                fprintf(stderr, "Erro ao alocar memória para chave.\n");
                return -1;
            }
            strncpy(chaves[i], chave, tamanho_chave);
            chaves[i][tamanho_chave] = '\0';
        }

        int retorno = chama_syscall(mensagem, copia, chave, mensagem_cifrada, mensagem_decifrada);

        if(retorno < 0) {
            free(mensagem);
            free(copia);
            free(chave);
            free(mensagem_cifrada);
            free(mensagem_decifrada);
            printf("Erro ao executar syscall.\n");
            return -1;
        }

        free(mensagem);
        free(copia);
        free(mensagem_cifrada);
        free(mensagem_decifrada);
        free(chave);
        printf("Teste %d concluído com sucesso.\n", i + 1);
    }
    
    printf("Teste concluído com sucesso.\n");

    return 0;
}