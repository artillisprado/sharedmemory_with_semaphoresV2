
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "shared_memory.h"






static int get_shared_block(char *filename, int size) {
    key_t key;

    // recebe uma key associada ao nome do arquivo, para que outros programas possam acessá-la.
    key = ftok(filename, 0);
    if (key == IPC_RESULT_ERROR) {
        return IPC_RESULT_ERROR;
    }
    

    // obtém o bloco compartilhado ou cria-o se ele não existir
    return shmget(key, size, 0644 | IPC_CREAT);
}

char * attach_memory_block(char *filename, int size) {
    int shared_block_id = get_shared_block(filename, size);
    char *result;

    if (shared_block_id == IPC_RESULT_ERROR) {
        return NULL;
    }

    //(Anexar a um processo) Através do ID bloco mapeia o bloco no espaço de endereço do processo e nos fornece um ponteiro para esse bloco
    result = shmat(shared_block_id, NULL, 0);
    if (result == (char *)IPC_RESULT_ERROR) {
        return NULL;
    }

    return result;
}

bool detach_memory_block(char *block) {
    //(Desanexar)  Terminar de usar a memória compartilhada em um processo
    return (shmdt(block) != IPC_RESULT_ERROR);
}

bool destroy_memory_block(char *filename) {
    int shared_block_id = get_shared_block(filename, 0);
    
    if (shared_block_id == IPC_RESULT_ERROR) {
        return NULL;
    }
    //remover (destruir) um segmento de memória, especificando o comando IPC_RMID
    return (shmctl(shared_block_id, IPC_RMID, NULL) != IPC_RESULT_ERROR);
}



