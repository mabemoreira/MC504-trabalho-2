#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/syscalls.h>


void get_mensagem_original(char *cifrada, char *chave, char *mensagem_decifrada, unsigned long tamanho_cifrada, unsigned long tamanho_chave) {
    if (tamanho_chave < tamanho_cifrada) {
        fprintf(stderr, "Erro: chave menor que a mensagem.\n");
        exit(EXIT_FAILURE);
    }

    void *kcifrada;
    kcifrada = kmalloc(tamanho_cifrada, GFP_KERNEL);
    if (!kcifrada)
        return -ENOMEM;
    if (copy_from_user(kcifrada, cifrada, tamanho_cifrada))
        return -EFAULT;

    void *kchave;
    kchave = kmalloc(tamanho_chave GFP_KERNEL);
    if (!kchave)
        return -ENOMEM;
    if (copy_from_user(kchave, chave, tamanho_cifrada))
        return -EFAULT;
    
    void *kmensagem;
    kmensagem = kmalloc(tamanho_cifrada, GFP_KERNEL);
    if (!kmensagem)
        return -ENOMEM;

    
    for (size_t i = 0; i < tamanho_cifrada; i++) {
        kmensagem[i] = kcifrada[i] ^ kchave[i];
    }
    kmensagem[tamanho_cifrada] = '\0';

    if(copy_to_user(mensagem_decifrada, kmensagem, tamanho_cifrada))
        return -EFAULT;
    kfree(kcifrada);
    kfree(kchave);
    kfree(kmensagem);
}

SYSCALL_DEFINE5(get_mensagem_original,  void __user *, mensagem,  void __user *, chave,  void __user *, 
                cifrada, unsigned long tamanho_cifrada,  unsigned long tamanho_chave){

    if (tamanho_chave < tamanho_cifrada) {
        fprintf(stderr, "Erro: chave menor que a mensagem.\n");
        exit(EXIT_FAILURE);
    }

    void *kcifrada;
    kcifrada = kmalloc(tamanho_cifrada, GFP_KERNEL);
    if (!kcifrada)
        return -ENOMEM;
    if (copy_from_user(kcifrada, cifrada, tamanho_cifrada))
        return -EFAULT;

    void *kchave;
    kchave = kmalloc(tamanho_chave GFP_KERNEL);
    if (!kchave)
        return -ENOMEM;
    if (copy_from_user(kchave, chave, tamanho_cifrada))
        return -EFAULT;
    
    void *kmensagem;
    kmensagem = kmalloc(tamanho_cifrada, GFP_KERNEL);
    if (!kmensagem)
        return -ENOMEM;

    
    for (size_t i = 0; i < tamanho_cifrada; i++) {
        kmensagem[i] = kcifrada[i] ^ kchave[i];
    }
    kmensagem[tamanho_cifrada] = '\0';

    if(copy_to_user(mensagem_decifrada, kmensagem, tamanho_cifrada))
        return -EFAULT;
    kfree(kcifrada);
    kfree(kchave);
    kfree(kmensagem);


}

SYSCALL_DEFINE5(set_mensagem_cifrada, void __user *, mensagem, void __user *, chave, 
    void __user *, cifrada, unsigned long tamanho_cifrada, unsigned long tamanho_chave) {
    
    if (tamanho_chave < tamanho_cifrada) {
    fprintf(stderr, "Erro: chave menor que a mensagem.\n");
    exit(EXIT_FAILURE);
    }


    void *kmensagem;
    *kmensagem = kmalloc(tamanho_cifrada, GFP_KERNEL);
    if (!kmensagem)
        return -ENOMEM;
    if (copy_from_user(kmensagem, mensagem, tamanho_cifrada))
        return -EFAULT;

    void *kchave;
    kchave = kmalloc(tamanho_chave, GFP_KERNEL);
    if (!kchave)
        return -ENOMEM;
    if (copy_from_user(kchave, chave, tamanho_chave))
        return -EFAULT;

    void *kcifrada;
    kcifrada = kmalloc(tamanho_cifrada, GFP_KERNEL);
    if (!kcifrada)
        return -ENOMEM;
    if (copy_from_user(kcifrada, cifrada, tamanho_cifrada))
        return -EFAULT;


    for (unsigned long i = 0; i < tamanho_cifrada; i++) {
        kcifrada[i] = kmensagem[i] ^ kchave[i];
    }

    if(copy_to_user(cifrada, kcifrada, tamanho_cifrada))
        return -EFAULT;



    kfree(kmensagem);
    kfree(kchave);
    kfree(kcifrada);

}

void get_mensagem_original(char *cifrada, char *chave, char *mensagem_decifrada, unsigned long tamanho_cifrada, unsigned long tamanho_chave) {
    if (tamanho_chave < tamanho_cifrada) {
        fprintf(stderr, "Erro: chave menor que a mensagem.\n");
        exit(EXIT_FAILURE);
    }

    void *kcifrada;
    kcifrada = kmalloc(tamanho_cifrada, GFP_KERNEL);
    if (!kcifrada)
        return -ENOMEM;
    if (copy_from_user(kcifrada, cifrada, tamanho_cifrada))
        return -EFAULT;

    void *kchave;
    kchave = kmalloc(tamanho_chave GFP_KERNEL);
    if (!kchave)
        return -ENOMEM;
    if (copy_from_user(kchave, chave, tamanho_cifrada))
        return -EFAULT;
    
    void *kmensagem;
    kmensagem = kmalloc(tamanho_cifrada, GFP_KERNEL);
    if (!kmensagem)
        return -ENOMEM;

    
    for (size_t i = 0; i < tamanho_cifrada; i++) {
        kmensagem[i] = kcifrada[i] ^ kchave[i];
    }
    kmensagem[tamanho_cifrada] = '\0';

    if(copy_to_user(mensagem_decifrada, kmensagem, tamanho_cifrada))
        return -EFAULT;
    kfree(kcifrada);
    kfree(kchave);
    kfree(kmensagem);
}
int main() {
    char mensagem[256] = "ola mc504";
    char chave[256] =    "aaaaaaaaa";
    char cifrada[256], decifrada[256];

    
    set_mensagem_cifrada(mensagem, chave, cifrada, 9, 9);

    printf("Cifrada: ");
    for (size_t i = 0; i < 9; i++) {
        printf("%d", cifrada[i]); 
    }
    printf("\n");


    get_mensagem_original(cifrada, chave, decifrada, 9, 9);

    printf("Decifrada: %s\n", decifrada);

    return 0;
}
