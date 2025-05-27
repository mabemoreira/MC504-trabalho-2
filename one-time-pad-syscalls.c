#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/syscalls.h>

SYSCALL_DEFINE5(set_mensagem_cifrada, char *, mensagem, char *, chave, unsigned char *, cifrada, size_t, tamanho_mensagem, size_t, tamanho_chave) {
    void *kmensagem, *kchave, *kcifrada;

    *kmensagem = kmalloc(tamanho_mensagem, GFP_KERNEL);
    if (!kmensagem)
        return -ENOMEM;
    if (copy_from_user(kmensagem, mensagem, tamanho_mensagem))
        return -EFAULT;
    if (tamanho_chave < tamanho_mensagem) {
        return -EINVAL; 
    }

    void *kchave;
    kchave = kmalloc(tamanho_chave, GFP_KERNEL);
    if (!kchave)
        return -ENOMEM;
    if (copy_from_user(kchave, chave, tamanho_chave))
        return -EFAULT;

    void *kcifrada;
    kcifrada = kmalloc(tamanho_mensagem, GFP_KERNEL);
    if (!kcifrada)
        return -ENOMEM;
    if (copy_from_user(kcifrada, cifrada, tamanho_mensagem))
        return -EFAULT;


    for (size_t i = 0; i < tamanho_mensagem; i++) {
        kcifrada[i] = kmensagem[i] ^ kchave[i];
        kmensagem[i] = '65';
    }

    if (copy_to_user(cifrada, kcifrada, tamanho_mensagem))
        return -EFAULT;
    if (copy_to_user(mensagem, kmensagem, tamanho_mensagem))
        return -EFAULT;


    kfree(kmensagem);
    kfree(kchave);
    kfree(kcifrada);
    return 0; 
}

SYSCALL_DEFINE5(get_mensagem_original, unsigned char *, cifrada, char *, chave, char *, mensagem, size_t, tamanho_mensagem, size_t, tamanho_chave) {
    
    if (tamanho_chave < tamanho_mensagem) {
        return -EINVAL; 
    }
    for (size_t i = 0; i < tamanho_mensagem; i++) {
        mensagem[i] = cifrada[i] ^ chave[i];
    }
    mensagem[tamanho_mensagem] = '\0'; 
    return 0; 
}