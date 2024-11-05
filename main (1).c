#include <stdio.h>
#include <stdlib.h>

#define TAMANHO_PAGINA 100
#define NUM_FRAMES 16
#define NUM_PAGINAS 32

// Estrutura para Frame na Memória Física
typedef struct {
    int identificador;
    int esta_livre; // 1 se o frame está livre, 0 se ocupado
} Frame;

// Estrutura para Página na Memória Virtual
typedef struct {
    int identificador;
    int esta_mapeado; // 1 se mapeado, 0 se não
} Pagina;

// Estrutura da Tabela de Páginas
typedef struct {
    int paginaID;
    int frameID; // -1 se não mapeado
} TabelaPagina;

// Inicializa a Memória Física
void inicializar_memoria_fisica(Frame *memoria_fisica) {
    for (int i = 0; i < NUM_FRAMES; i++) {
        memoria_fisica[i].identificador = i;
        memoria_fisica[i].esta_livre = 1;
    }
}

// Inicializa a Memória Virtual
void inicializar_memoria_virtual(Pagina *memoria_virtual) {
    for (int i = 0; i < NUM_PAGINAS; i++) {
        memoria_virtual[i].identificador = i;
        memoria_virtual[i].esta_mapeado = 0;
    }
}

// Inicializa a Tabela de Páginas
void inicializar_tabela_paginas(TabelaPagina *tabela_paginas) {
    for (int i = 0; i < NUM_PAGINAS; i++) {
        tabela_paginas[i].paginaID = i;
        tabela_paginas[i].frameID = -1; // Indica que não está mapeado
    }
}

// Função para mapear página para um frame
int mapear_pagina(Frame *memoria_fisica, TabelaPagina *tabela_paginas, int paginaID) {
    for (int i = 0; i < NUM_FRAMES; i++) {
        if (memoria_fisica[i].esta_livre) {
            memoria_fisica[i].esta_livre = 0;
            tabela_paginas[paginaID].frameID = memoria_fisica[i].identificador;
            return memoria_fisica[i].identificador;
        }
    }
    return -1; // Sem frames livres
}

// Função de Tradução de Endereço Virtual para Físico
int traduzir_endereco(TabelaPagina *tabela_paginas, int endereco_virtual) {
    int numero_pagina = endereco_virtual / TAMANHO_PAGINA;
    int offset = endereco_virtual % TAMANHO_PAGINA;

    int frameID = tabela_paginas[numero_pagina].frameID;
    if (frameID == -1) {
        printf("Page fault: Página %d não mapeada.\n", numero_pagina);
        return -1; // Indica page fault
    }

    return (frameID * TAMANHO_PAGINA) + offset;
}

// Programa de Teste
int main() {
    Frame memoria_fisica[NUM_FRAMES];
    Pagina memoria_virtual[NUM_PAGINAS];
    TabelaPagina tabela_paginas[NUM_PAGINAS];

    inicializar_memoria_fisica(memoria_fisica);
    inicializar_memoria_virtual(memoria_virtual);
    inicializar_tabela_paginas(tabela_paginas);

    // Exemplo de alocação de páginas
    int paginaID = 1;
    int frame_alocado = mapear_pagina(memoria_fisica, tabela_paginas, paginaID);
    if (frame_alocado != -1) {
        printf("Mapa de página: Página %d -> Frame %d\n", paginaID, frame_alocado);
    } else {
        printf("Erro: Nenhum frame disponível.\n");
    }

    // Exemplo de tradução de endereço
    int endereco_virtual = 300;
    int endereco_fisico = traduzir_endereco(tabela_paginas, endereco_virtual);
    if (endereco_fisico != -1) {
        printf("Tradução de endereço: Virtual %d -> Físico %d\n", endereco_virtual, endereco_fisico);
    }

    return 0;
}
