# MC504-trabalho-2
Repo com implementação das syscalls no kernel: [github.com/mabemoreira/kernel-mc504-trabalho-2](https://github.com/mabemoreira/kernel-mc504-trabalho-2/tree/nossas-syscalls)
**One-Time Pad (OTP)**  
O One-Time Pad (OTP) é um método de criptografia simétrica cuja ideia central é usar uma chave verdadeiramente aleatória e com o mesmo comprimento da mensagem que se deseja proteger.

---

## Objetivo das chamadas de sistema

As chamadas de sistema implementadas têm como objetivo básico permitir que um processo em espaço de usuário armazene (set) e recupere ou decifre (get) uma mensagem usando a técnica de One-Time Pad, mantendo os buffers em área de kernel. Em termos mínimos, elas funcionam como operações de “set” e “get” de uma variável no kernel, mas voltadas para criptografia XOR:

- **set_mensagem_cifrada**  
```c
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/syscalls.h>
#include <linux/gfp.h>

SYSCALL_DEFINE5(set_mensagem_cifrada, unsigned char __user *, mensagem, unsigned char __user *, chave, 
    unsigned char __user *, cifrada, unsigned long, tamanho_cifrada, unsigned long, tamanho_chave) {

    if (tamanho_chave < tamanho_cifrada) {
    fprintf(stderr, "Erro: chave menor que a mensagem.\n");
    exit(EXIT_FAILURE);
    }


    unsigned char *kmensagem;
    *kmensagem = kmalloc(tamanho_cifrada, GFP_KERNEL);
    if (!kmensagem)
        return -ENOMEM;
    if (copy_from_user(kmensagem, mensagem, tamanho_cifrada))
        return -EFAULT;

    unsigned char *kchave;
    kchave = kmalloc(tamanho_chave, GFP_KERNEL);
    if (!kchave)
        return -ENOMEM;
    if (copy_from_user(kchave, chave, tamanho_chave))
        return -EFAULT;

    unsigned char *kcifrada;
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
```
  - Recebe do usuário um buffer contendo a mensagem já cifrada (e, possivelmente, a própria chave).  
  - Aloca memória no kernel e copia esses dados para dentro do espaço de kernel.  
  - Dessa forma, o kernel passa a manter em uma variável interna o conteúdo cifrado (e/ou a chave), tornando-o disponível para chamadas futuras.

- **get_mensagem_original** 
```c
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/syscalls.h>
#include <linux/gfp.h>

SYSCALL_DEFINE5(get_mensagem_original, unsigned char __user *, mensagem, unsigned char __user *, chave, unsigned char __user *, cifrada, unsigned long, tamanho_cifrada, unsigned long, tamanho_chave)
{
    if (tamanho_chave < tamanho_cifrada) {
        printk(KERN_ERR "Erro: chave menor que a mensagem.\n");
        return -EINVAL;
    }

    unsigned char *kcifrada;
    kcifrada = kmalloc(tamanho_cifrada, GFP_KERNEL);
    if (!kcifrada)
        return -ENOMEM;
    if (copy_from_user(kcifrada, cifrada, tamanho_cifrada)) {
        kfree(kcifrada);
        return -EFAULT;
    }

    unsigned char *kchave;
    kchave = kmalloc(tamanho_chave, GFP_KERNEL);
    if (!kchave) {
        kfree(kcifrada);
        return -ENOMEM;
    }
    if (copy_from_user(kchave, chave, tamanho_cifrada)) {
        kfree(kcifrada);
        kfree(kchave);
        return -EFAULT;
    }

    unsigned char *kmensagem;
    kmensagem = kmalloc(tamanho_cifrada, GFP_KERNEL);
    if (!kmensagem) {
        kfree(kcifrada);
        kfree(kchave);
        return -ENOMEM;
    }

    for (size_t i = 0; i < tamanho_cifrada; i++) {
        kmensagem[i] = kcifrada[i] ^ kchave[i];
    }

    if (copy_to_user(mensagem, kmensagem, tamanho_cifrada)) {
        kfree(kcifrada);
        kfree(kchave);
        kfree(kmensagem);
        return -EFAULT;
    }

    kfree(kcifrada);
    kfree(kchave);
    kfree(kmensagem);

    return 0;
}
```
  - Recebe do usuário três buffers em espaço de usuário:  
    1. O buffer destino (`mensagem`)  
    2. O buffer da chave (`chave`)  
    3. O buffer da mensagem cifrada (`cifrada`)  
  - Recebe também os respectivos tamanhos de cada buffer.  
  - A implementação realiza as seguintes etapas:  
    1. Verifica se o tamanho da chave é pelo menos igual ao da mensagem.  
    2. Copia, de forma segura, os dados cifrados e a chave para a área de kernel.  
    3. Aplica a operação de XOR byte a byte (decifrando via One-Time Pad).  
    4. Copia o resultado (mensagem original) de volta para o buffer do usuário.

---

## Funcionalidade

Em outras palavras, a **funcionalidade** consiste em:

1. **Guardar um valor** (a mensagem cifrada ou a chave) em uma variável no kernel (no “set”).  
2. **Recuperar ou decifrar** esse valor (no “get”), copiando cuidadosamente entre espaço de usuário e kernel e executando a lógica de XOR para produzir a mensagem original.
