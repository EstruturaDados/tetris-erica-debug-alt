/*
  ==============================================
  Tetris Stack - Gerenciador de Peças (Nível Avançado)
  ----------------------------------------------
  - Fila circular de peças futuras (capacidade 5)
  - Pilha de peças reservadas (capacidade 3)
  - Ações:
      1. Jogar peça
      2. Reservar peça
      3. Usar peça reservada
      4. Trocar peça da frente da fila com o topo da pilha
      5. Troca múltipla (3 primeiras da fila <-> 3 da pilha)
  - Peças são geradas automaticamente por gerarPeca()
  - Cada peça tem tipo ('I','O','T','L') e ID único
  ==============================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM_FILA 5
#define TAM_PILHA 3

/* ------------------- Estruturas ------------------- */
typedef struct {
    char tipo;  // 'I', 'O', 'T', 'L'
    int id;     // identificador único
} Peca;

typedef struct {
    Peca itens[TAM_FILA];
    int inicio, fim, tamanho;
} Fila;

typedef struct {
    Peca itens[TAM_PILHA];
    int topo;
} Pilha;

/* ------------------- Funções da Fila ------------------- */
void inicializarFila(Fila *f) {
    f->inicio = 0;
    f->fim = 0;
    f->tamanho = 0;
}

int filaVazia(Fila *f) {
    return f->tamanho == 0;
}

int filaCheia(Fila *f) {
    return f->tamanho == TAM_FILA;
}

int enfileirar(Fila *f, Peca p) {
    if (filaCheia(f)) return 0;
    f->itens[f->fim] = p;
    f->fim = (f->fim + 1) % TAM_FILA;
    f->tamanho++;
    return 1;
}

int desenfileirar(Fila *f, Peca *p) {
    if (filaVazia(f)) return 0;
    *p = f->itens[f->inicio];
    f->inicio = (f->inicio + 1) % TAM_FILA;
    f->tamanho--;
    return 1;
}

void exibirFila(Fila *f) {
    printf("Fila de peças:\t");
    if (filaVazia(f)) {
        printf("(vazia)\n");
        return;
    }
    for (int i = 0; i < f->tamanho; i++) {
        int pos = (f->inicio + i) % TAM_FILA;
        printf("[%c %d] ", f->itens[pos].tipo, f->itens[pos].id);
    }
    printf("\n");
}

/* ------------------- Funções da Pilha ------------------- */
void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

int pilhaVazia(Pilha *p) {
    return p->topo == -1;
}

int pilhaCheia(Pilha *p) {
    return p->topo == TAM_PILHA - 1;
}

int empilhar(Pilha *p, Peca pe) {
    if (pilhaCheia(p)) return 0;
    p->topo++;
    p->itens[p->topo] = pe;
    return 1;
}

int desempilhar(Pilha *p, Peca *pe) {
    if (pilhaVazia(p)) return 0;
    *pe = p->itens[p->topo];
    p->topo--;
    return 1;
}

void exibirPilha(Pilha *p) {
    printf("Pilha de reserva\t(Topo -> Base): ");
    if (pilhaVazia(p)) {
        printf("(vazia)\n");
        return;
    }
    for (int i = p->topo; i >= 0; i--) {
        printf("[%c %d] ", p->itens[i].tipo, p->itens[i].id);
    }
    printf("\n");
}

/* ------------------- Funções auxiliares ------------------- */
Peca gerarPeca(int id) {
    Peca nova;
    char tipos[] = {'I', 'O', 'T', 'L'};
    nova.tipo = tipos[rand() % 4];
    nova.id = id;
    return nova;
}

void exibirEstado(Fila *f, Pilha *p) {
    printf("\n---------------------------------------------\n");
    exibirFila(f);
    exibirPilha(p);
    printf("---------------------------------------------\n");
}

/* ------------------- Função principal ------------------- */
int main() {
    Fila fila;
    Pilha pilha;
    int opcao, contadorID = 0;
    Peca temp, nova;

    srand(time(NULL));
    inicializarFila(&fila);
    inicializarPilha(&pilha);

    // Preenche a fila inicial
    for (int i = 0; i < TAM_FILA; i++) {
        enfileirar(&fila, gerarPeca(contadorID++));
    }

    do {
        exibirEstado(&fila, &pilha);
        printf("\nAções disponíveis:\n");
        printf("1 - Jogar peça da frente da fila\n");
        printf("2 - Enviar peça da fila para a pilha de reserva\n");
        printf("3 - Usar peça da pilha de reserva\n");
        printf("4 - Trocar peça da frente da fila com o topo da pilha\n");
        printf("5 - Trocar as 3 primeiras da fila com as 3 da pilha\n");
        printf("0 - Sair\n");
        printf("Opção escolhida: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: // Jogar peça
                if (desenfileirar(&fila, &temp)) {
                    printf("\n🎮 Peça [%c %d] jogada!\n", temp.tipo, temp.id);
                    nova = gerarPeca(contadorID++);
                    enfileirar(&fila, nova);
                    printf("🔄 Nova peça [%c %d] adicionada à fila.\n", nova.tipo, nova.id);
                } else {
                    printf("\n⚠️ Fila vazia!\n");
                }
                break;

            case 2: // Reservar peça
                if (pilhaCheia(&pilha)) {
                    printf("\n⚠️ Pilha cheia! Não é possível reservar mais peças.\n");
                } else if (desenfileirar(&fila, &temp)) {
                    empilhar(&pilha, temp);
                    printf("\n📦 Peça [%c %d] movida para a pilha de reserva.\n", temp.tipo, temp.id);
                    nova = gerarPeca(contadorID++);
                    enfileirar(&fila, nova);
                    printf("🔄 Nova peça [%c %d] adicionada à fila.\n", nova.tipo, nova.id);
                } else {
                    printf("\n⚠️ Fila vazia!\n");
                }
                break;

            case 3: // Usar peça reservada
                if (desempilhar(&pilha, &temp)) {
                    printf("\n🪄 Peça reservada [%c %d] usada!\n", temp.tipo, temp.id);
                } else {
                    printf("\n⚠️ Pilha vazia!\n");
                }
                break;

            case 4: // Trocar topo da pilha com frente da fila
                if (pilhaVazia(&pilha)) {
                    printf("\n⚠️ Pilha vazia — não há peça para trocar.\n");
                } else if (filaVazia(&fila)) {
                    printf("\n⚠️ Fila vazia — não há peça para trocar.\n");
                } else {
                    int pos = fila.inicio;
                    Peca topo = pilha.itens[pilha.topo];
                    pilha.itens[pilha.topo] = fila.itens[pos];
                    fila.itens[pos] = topo;
                    printf("\n🔁 Troca realizada entre topo da pilha e frente da fila!\n");
                }
                break;

            case 5: // Troca múltipla (3 da pilha ↔ 3 da fila)
                if (pilha.topo < 2 || fila.tamanho < 3) {
                    printf("\n⚠️ É necessário ter pelo menos 3 peças na pilha e na fila!\n");
                } else {
                    for (int i = 0; i < 3; i++) {
                        int posFila = (fila.inicio + i) % TAM_FILA;
                        Peca aux = fila.itens[posFila];
                        fila.itens[posFila] = pilha.itens[pilha.topo - i];
                        pilha.itens[pilha.topo - i] = aux;
                    }
                    printf("\n🔃 Troca múltipla realizada entre as 3 primeiras da fila e as 3 da pilha!\n");
                }
                break;

            case 0:
                printf("\n👋 Encerrando o jogo. Até a próxima!\n");
                break;

            default:
                printf("\n⚠️ Opção inválida!\n");
        }

    } while (opcao != 0);

    return 0;
}


// #include <stdio.h>

// // Desafio Tetris Stack
// // Tema 3 - Integração de Fila e Pilha
// // Este código inicial serve como base para o desenvolvimento do sistema de controle de peças.
// // Use as instruções de cada nível para desenvolver o desafio.

// int main() {

//     // 🧩 Nível Novato: Fila de Peças Futuras
//     //
//     // - Crie uma struct Peca com os campos: tipo (char) e id (int).
//     // - Implemente uma fila circular com capacidade para 5 peças.
//     // - Crie funções como inicializarFila(), enqueue(), dequeue(), filaCheia(), filaVazia().
//     // - Cada peça deve ser gerada automaticamente com um tipo aleatório e id sequencial.
//     // - Exiba a fila após cada ação com uma função mostrarFila().
//     // - Use um menu com opções como:
//     //      1 - Jogar peça (remover da frente)
//     //      0 - Sair
//     // - A cada remoção, insira uma nova peça ao final da fila.



//     // 🧠 Nível Aventureiro: Adição da Pilha de Reserva
//     //
//     // - Implemente uma pilha linear com capacidade para 3 peças.
//     // - Crie funções como inicializarPilha(), push(), pop(), pilhaCheia(), pilhaVazia().
//     // - Permita enviar uma peça da fila para a pilha (reserva).
//     // - Crie um menu com opção:
//     //      2 - Enviar peça da fila para a reserva (pilha)
//     //      3 - Usar peça da reserva (remover do topo da pilha)
//     // - Exiba a pilha junto com a fila após cada ação com mostrarPilha().
//     // - Mantenha a fila sempre com 5 peças (repondo com gerarPeca()).


//     // 🔄 Nível Mestre: Integração Estratégica entre Fila e Pilha
//     //
//     // - Implemente interações avançadas entre as estruturas:
//     //      4 - Trocar a peça da frente da fila com o topo da pilha
//     //      5 - Trocar os 3 primeiros da fila com as 3 peças da pilha
//     // - Para a opção 4:
//     //      Verifique se a fila não está vazia e a pilha tem ao menos 1 peça.
//     //      Troque os elementos diretamente nos arrays.
//     // - Para a opção 5:
//     //      Verifique se a pilha tem exatamente 3 peças e a fila ao menos 3.
//     //      Use a lógica de índice circular para acessar os primeiros da fila.
//     // - Sempre valide as condições antes da troca e informe mensagens claras ao usuário.
//     // - Use funções auxiliares, se quiser, para modularizar a lógica de troca.
//     // - O menu deve ficar assim:
//     //      4 - Trocar peça da frente com topo da pilha
//     //      5 - Trocar 3 primeiros da fila com os 3 da pilha


//     return 0;
// }

