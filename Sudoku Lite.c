#include <stdio.h>      
#include <stdlib.h>     
#include <string.h>     
#include <time.h>       
srand(time(NULL)) 
// Biblioteca para entrada/saída 
// Funções utilitárias (atoi, rand, malloc…) 
// Manipulação de strings (strcpy, strtok…) 
// Para gerar números aleatórios com 
#define MAX_DESAFIOS 200   // Quantidade máxima de desafios no 
arquivo 
#define BUF_LINHA 256       
arquivo 
#define BUF_NOME 64         
// Tamanho máximo de uma linha lida do 
// Tamanho máximo de um nome de jogador 
#define MAX_SCORES 200      // Máximo de registros de pontuação 
/* -------------------- ARMAZENAMENTO SEM struct -------------------- */ 
/* desafios[i] é um tabuleiro 4x4 (0 = vazio) */ 
int desafios[MAX_DESAFIOS][4][4];   
// Armazena todos os desafios 
carregados 
int totalDesafios = 0;              
// Conta quantos desafios foram lidos 
/* scores em arrays paralelos */ 
char scoreNomes[MAX_SCORES][BUF_NOME];   
jogadores 
int scoreJogadas[MAX_SCORES];            
// 
Nomes dos 
// Quantidade de jogadas de 
cada jogador 
int scoreNiveis[MAX_SCORES];             
correspondente 
int totalScores = 0;                     
// Nível de dificuldade 
// Quantidade total de scores lidos 
/* -------------------- UTILITÁRIOS DE ENTRADA -------------------- */ 
void lerEntrada(char *buf, size_t n) { 
if (!fgets(buf, (int)n, stdin)) {  // Lê texto; se falhar, coloca string vazia 
buf[0] = '\0'; 
return; 
} 
size_t L = strlen(buf);            
// Descobre o tamanho da string 
if (L > 0 && buf[L-1] == '\n') {   // Remove '\n' do final 
buf[L-1] = '\0'; 
} 
} 
int lerInteiroOuSair(const char *prompt, int *numero) { 
    char buf[32]; 
    printf("%s", prompt);              // Exibe mensagem 
    lerEntrada(buf, sizeof(buf));      // Lê a entrada 
    if (buf[0] == 'q' || buf[0] == 'Q') {return 0;} // Permite sair 
    *numero = atoi(buf);               // Converte texto para inteiro 
    return 1; 
} 
 
/* -------------------- CARREGAMENTO DE DESAFIOS -------------------- */ 
int linhaParaTabuleiro(const char *linha, int grade[4][4]) { 
    /* Converte uma linha do arquivo (16 números) para um tabuleiro 4x4 */ 
    char tmp[BUF_LINHA]; 
    strncpy(tmp, linha, BUF_LINHA-1); 
    tmp[BUF_LINHA-1] = '\0'; 
    char *tok = strtok(tmp, " \t\r\n");   // Separa texto por espaços 
    int i = 0; 
    while (tok && i < 16) {               // Preenche os 16 valores 
        grade[i/4][i%4] = atoi(tok); 
        tok = strtok(NULL, " \t\r\n"); 
        i++; 
    } 
    return (i == 16); // Retorna 1 se as 16 posições foram preenchidas 
} 
 
int contarNumerosPreenchidos(int grade[4][4]) { 
    /* Conta quantos números não são zero no tabuleiro */ 
    int c = 0; 
    for (int i=0;i<4;i++) { 
        for (int j=0;j<4;j++){ 
          if (grade[i][j] != 0) { 
              c++; 
            } 
        } 
    } 
    return c; 
} 
 
int determinarNivel(int preenchidos) { 
    /* Determina dificuldade com base na quantidade de números iniciais */ 
    if (preenchidos >= 9) {return 0;} /* Facil */ 
    if (preenchidos >= 7) {return 1;} /* Medio */ 
    if (preenchidos >= 4) {return 2;} /* Dificil */ 
    return -1; 
} 
 
int carregarDesafios(const char *fn) { 
    /* Lê todos os desafios do arquivo desafios.txt */ 
    FILE *f = fopen(fn, "r"); 
    if (!f) {  
        totalDesafios = 0; 
        return 0; 
    } 
     
    char linha[BUF_LINHA]; 
    int idx = 0; 
    while (idx < MAX_DESAFIOS && fgets(linha, sizeof(linha), f)) { 
        int g[4][4]; 
        if (linhaParaTabuleiro(linha, g)) {    // Converte linha para matriz 
            for (int r=0;r<4;r++) { 
                for (int c=0;c<4;c++) { 
                    desafios[idx][r][c] = g[r][c]; 
                } 
            } 
            idx++;  // Aumenta quantidade de desafios validos 
        } 
    } 
    fclose(f); 
    totalDesafios = idx;  // Salva total lido 
    return 1; 
} 
 
int selecionarDesafioPorNivel(int nivel) { 
    /* Seleciona um desafio aleatório do nível solicitado */ 
    int cand[MAX_DESAFIOS]; 
    int nc = 0; 
    for (int i=0;i<totalDesafios;i++) { 
        int cnt = contarNumerosPreenchidos(desafios[i]); 
        if (determinarNivel(cnt) == nivel) {cand[nc++] = i;} 
    } 
    if (nc == 0) {return -1;}         // Nenhum desafio do nível 
    return cand[rand() % nc];         // Escolhe aleatoriamente 
} 
 
/* -------------------- IMPRESSÃO -------------------- */ 
void exibirTabuleiro(int g[4][4]) { 
    /* Exibe tabuleiro formatado */ 
    for (int i=0;i<4;i++) { 
        for (int j=0;j<4;j++) { 
            if (j==2) {printf("|");}  // Separador visual 
            if (g[i][j] == 0) {printf(".");} 
            else {printf("%d", g[i][j]);} 
            if (j<3) {printf(" ");} 
        } 
        printf("\n"); 
        if (i==1) {printf("---+---\n");} // Linha divisória 
    } 
} 
 
/* -------------------- VALIDAÇÃO E SOLVER -------------------- */ 
int valorValido(int g[4][4], int r, int c, int v) { 
    /* Verifica linha */ 
    for (int i=0;i<4;i++) { 
        if (g[r][i]==v) { 
            return 0; 
        } 
    } 
    /* Verifica coluna */ 
    for (int i=0;i<4;i++) { 
        if (g[i][c]==v) { 
            return 0; 
        } 
    } 
    /* Verifica bloco 2x2 */ 
    int br = (r/2)*2, bc = (c/2)*2; 
    for (int i=br;i<br+2;i++) { 
        for (int j=bc;j<bc+2;j++) { 
            if (g[i][j]==v) { 
                return 0; 
            } 
        } 
    } 
    return 1; // Valor permitido 
} 
 
int resolverTabuleiro(int g[4][4]) { 
    /* Resolve Sudoku 4x4 usando backtracking */ 
    int r=-1,c=-1; 
 
    /* Procura a primeira posição vazia */ 
    for (int i=0;i<4 && r==-1;i++) { 
        for (int j=0;j<4;j++) { 
            if (g[i][j]==0){ 
                r=i; c=j; 
                break; 
            } 
        } 
    } 
 
    if (r==-1) {return 1;}  // Sem espaços vazios → concluído 
 
    /* Tenta valores de 1 até 4 */ 
    for (int v=1; v<=4; v++) { 
        if (valorValido(g,r,c,v)) { 
            g[r][c] = v; 
            if (resolverTabuleiro(g)) {return 1;} // Solução encontrada 
            g[r][c] = 0;                           // Falhou → desfaz 
        } 
    } 
    return 0; // Não conseguiu resolver 
} 
 
/* -------------------- SCORES (sem structs) -------------------- */ 
int carregarPontuacoes(const char *fn) { 
    /* Lê arquivo de scores e armazena nos arrays paralelos */ 
    FILE *f = fopen(fn, "r"); 
    totalScores = 0; 
    if (!f) {return 0;} 
 
    while (totalScores < MAX_SCORES) { 
        char nivelTxt[32]; 
        char nomeTmp[BUF_NOME]; 
        int jog; 
 
        /* Lê 3 itens por linha: nome, jogadas, dificuldade */ 
        if (fscanf(f, "%63s %d %31s", nomeTmp, &jog, nivelTxt) != 3) {break;} 
 
        strncpy(scoreNomes[totalScores], nomeTmp, BUF_NOME-1); 
        scoreNomes[totalScores][BUF_NOME-1] = '\0'; 
        scoreJogadas[totalScores] = jog; 
 
        /* Converte texto da dificuldade em número */ 
        if (strcmp(nivelTxt,"Dificil")==0 || strcmp(nivelTxt,"Difícil")==0) 
{scoreNiveis[totalScores]=2;} 
        else if (strcmp(nivelTxt,"Medio")==0 || strcmp(nivelTxt,"Médio")==0) 
{scoreNiveis[totalScores]=1;} 
        else {scoreNiveis[totalScores]=0;} 
 
        totalScores++; 
    } 
    fclose(f); 
    return 1; 
} 
 
void salvarPontuacao(const char *fn, const char *nome, int jogadas, int 
nivel) { 
    /* Adiciona nova pontuação ao arquivo */ 
    FILE *f = fopen(fn, "a"); 
    if (!f) {return;} 
    const char *nivStr[3] = {"Facil","Medio","Dificil"}; 
    fprintf(f, "%s %d %s\n", nome, jogadas, nivStr[nivel]); 
    fclose(f); 
} 
 
/* comparator para índices de scores (usa arrays globais) */ 
int compararIndicesPontuacao(const void *a, const void *b) { 
    /* Ordena por número de jogadas e depois por nome */ 
    int ia = *(const int*)a; 
    int ib = *(const int*)b; 
 
    if (scoreJogadas[ia] != scoreJogadas[ib]) { 
        return scoreJogadas[ia] - scoreJogadas[ib]; 
    } 
    return strcmp(scoreNomes[ia], scoreNomes[ib]); 
} 
 
void exibirPontuacoes() { 
    /* Exibe scores agrupados por dificuldade */ 
    carregarPontuacoes("scores.txt"); 
    printf("SCORES\n\n"); 
 
    for (int levelOrder = 2; levelOrder >= 0; levelOrder--) { 
        if (levelOrder==2) {printf("Dificil:\n");} 
        else if (levelOrder==1) {printf("\nMedio:\n");} 
        else {printf("\nFacil:\n");} 
 
        int idxs[MAX_SCORES]; 
        int cnt = 0; 
 
        /* Filtra scores do nível atual */ 
        for (int i=0;i<totalScores;i++) { 
            if (scoreNiveis[i]==levelOrder) { 
                idxs[cnt++] = i; 
            } 
        } 
 
        if (cnt==0) {continue;} 
 
        /* Ordena índices */ 
        qsort(idxs, cnt, sizeof(int), compararIndicesPontuacao); 
 
        /* Mostra até 3 grupos de pontuação */ 
        int shownGroups = 0; 
        int i = 0; 
        while (i < cnt && shownGroups < 3) { 
            int curMoves = scoreJogadas[idxs[i]]; 
            int j = i; 
 
            /* Mostra todos os que têm o mesmo número de jogadas */ 
            while (j < cnt && scoreJogadas[idxs[j]] == curMoves) { 
                printf("%-6s %3d  %s\n", 
                       scoreNomes[idxs[j]], 
                       scoreJogadas[idxs[j]], 
                       (levelOrder==2)?"Dificil":(levelOrder==1)?"Medio":"Facil"); 
                j++; 
            } 
            shownGroups++; 
            i = j; 
        } 
        printf("\n"); 
    } 
 
    printf("Pressione ENTER para voltar ao MENU PRINCIPAL!"); 
    char tmp[16]; 
    lerEntrada(tmp, sizeof(tmp)); 
} 
 
/* -------------------- JOGO -------------------- */ 
int tabuleiroCompleto(int g[4][4]) { 
    /* Verifica se não há zeros (tabuleiro completo) */ 
    for (int i=0;i<4;i++) { 
        for (int j=0;j<4;j++) { 
            if (g[i][j]==0) { 
                return 0; 
            } 
        } 
} 
return 1; 
} 
void jogarSudoku() { 
/* Permite ao usuário jogar um desafio */ 
printf("NIVEL\n(1) Facil\n(2) Medio\n(3) Dificil\nFaça sua escolha: "); 
char buf[32]; 
lerEntrada(buf, sizeof(buf)); 
int escolha = atoi(buf); 
if (escolha < 1 || escolha > 3) {return;} 
int nivel = escolha - 1; 
int sel = selecionarDesafioPorNivel(nivel); 
if (sel == -1) { 
printf("Nenhum desafio disponivel para esse nivel.\n"); 
return; 
} 
/* Prepara o jogo */ 
int grade[4][4], sol[4][4], fixo[4][4] = {0}; 
 
    for (int i=0;i<4;i++) { 
        for (int j=0;j<4;j++) { 
            grade[i][j] = desafios[sel][i][j]; 
            sol[i][j] = grade[i][j]; 
            if (grade[i][j]!=0) { 
                fixo[i][j]=1;    // Marca posição fixa 
            } 
        } 
    } 
 
    /* Gera solução para validação */ 
    if (!resolverTabuleiro(sol)) { 
        printf("Desafio sem solucao. Voltando ao menu.\n"); 
        return; 
    } 
 
    int jogadas = 0; 
 
    /* Loop principal do jogo */ 
    while (1) { 
        exibirTabuleiro(grade); 
 
        int lin; 
        if (!lerInteiroOuSair("\nLinha  (1 a 4 ou q) : ", &lin)) { 
            printf("\nSOLUCAO DO JOGO:\n"); 
            exibirTabuleiro(sol); 
            return; 
        } 
         
        if (lin < 1 || lin > 4) { 
            printf("\nEntrada invalida. Tente novamente!\n"); 
            continue; 
        } 
 
        int col; 
        if (!lerInteiroOuSair("Coluna (1 a 4 ou q) : ", &col)) { 
            printf("\nSOLUCAO DO JOGO:\n"); 
            exibirTabuleiro(sol); 
            return; 
        } 
         
        if (col < 1 || col > 4) { 
            printf("\nEntrada invalida. Tente novamente!\n"); 
            continue; 
        } 
 
        int val; 
        if (!lerInteiroOuSair("Valor  (1 a 4 ou q) : ", &val)) { 
            printf("\nSOLUCAO DO JOGO:\n"); 
            exibirTabuleiro(sol); 
            return; 
        } 
         
        if (val < 1 || val > 4) { 
            printf("\nEntrada invalida. Tente novamente!\n"); 
            continue; 
        } 
 
        int r = lin-1, c = col-1; 
 
        if (fixo[r][c]) { 
            printf("\nEsta posicao e fixa. Nao pode ser modificada.\n"); 
            continue; 
        } 
         
        if (!valorValido(grade,r,c,val)) { 
            printf("\nEsta jogada e invalida.Tente novamente!\n"); 
            continue; 
        } 
 
        grade[r][c] = val; 
        jogadas++; 
 
        /* Verifica vitória */ 
        if (tabuleiroCompleto(grade)) { 
            exibirTabuleiro(grade); 
            printf("\nParabens! Voce resolveu o desafio em %d jogadas!\n\n", 
jogadas); 
 
            /* carregar scores atuais e verificar se qualifica */ 
            carregarPontuacoes("scores.txt"); 
 
            int idxs[MAX_SCORES]; 
            int cnt = 0; 
 
            /* Filtra scores daquele nível */ 
            for (int i=0;i<totalScores;i++) { 
                if (scoreNiveis[i]==nivel) { 
                    idxs[cnt++] = i; 
                } 
            } 
 
            if (cnt > 0) { 
                qsort(idxs, cnt, sizeof(int), compararIndicesPontuacao); 
            } 
 
            int qualifies = 0; 
 
            /* Se menos de 3 scores → entra direto */ 
            if (cnt < 3) {qualifies = 1;} 
            else { 
                int thirdMoves = scoreJogadas[idxs[ (cnt>=3) ? 2 : (cnt-1) ]]; 
                if (jogadas <= thirdMoves) {qualifies = 1;} 
            } 
 
            if (qualifies) { 
                printf("Entre seu nome para a lista de scores : "); 
                char nome[BUF_NOME]; 
                lerEntrada(nome, sizeof(nome)); 
                if (nome[0] == '\0') {strcpy(nome, "Anon");} 
                salvarPontuacao("scores.txt", nome, jogadas, nivel); 
            } 
 
            printf("\nPressione ENTER para voltar ao MENU PRINCIPAL!"); 
            lerEntrada(buf, sizeof(buf)); 
            return; 
        } 
    } 
} 
 
// Menu 
int menuPrincipal() { 
    printf("MENU PRINCIPAL\n(1) Ver scores\n(2) Jogar\n(3) 
Sair\nEscolha uma opcao: "); 
    char buf[32]; 
    lerEntrada(buf, sizeof(buf)); 
    return atoi(buf);          // Converte escolha para número 
} 
 
// Main 
int main() { 
    srand((unsigned) time(NULL));  // Inicializa números aleatórios 
    carregarDesafios("desafios.txt");  // Carrega desafios do arquivo 
 
    while (1) { 
        int op = menuPrincipal(); 
        if (op == 1) {exibirPontuacoes();}   // Mostra ranking 
        else if (op == 2) {jogarSudoku();}   // Inicia jogo 
        else {break;}                        // Sai do programa 
    } 
    return 0; 
}