# Relatório de Paralelização: Brute-Force Password Cracking

**Disciplina:** Introdução à Programação Paralela e Distribuída<br>
**Professor:** Alexandro Jose Baldassin<br>
**Integrantes do Grupo:**
1. André Luis Dias Nogueira
2. Felipe Melchior de Britto
3. Rafael Daiki Kaneko
4. Ryan Hideki Tadeo Guimarães


## 1. Descrição do problema

A aplicação escolhida para este projeto foi o problema "Brute-Force Password Cracking" (Problema B) da 14ª Maratona de Programação Paralela (SBAC-PAD & WSCAD - 2019). 

O objetivo do programa é quebrar uma senha usando um algoritmo de força bruta, que testa exaustivamente todas as combinações possíveis de caracteres até encontrar a correspondência correta. Em sistemas de segurança, as senhas costumam ser armazenadas como *hashes* em vez de texto puro. O programa recebe como entrada um *hash* MD5 de 128 bits com 32 caracteres hexadecimais e gera todas as combinações possíveis de letras maiúsculas, minúsculas e números (tamanho de 1 a 10 caracteres) para encontrar a senha original cujo *hash* corresponda à entrada.


## 2. Estratégia de paralelização

A aplicação original (`password_bf.c`) usava uma função recursiva (`iterate`) para testar combinações. Para a paralelização (`password_bf_parallel.c`), escolhemos utilizar a API **OpenMP** devido à sua facilidade em lidar com laços paralelos em ambientes de memória compartilhada.

A estratégia adotada consistiu em:

1. **Paralelização no nível raiz da busca -** Em vez de tentar paralelizar níveis profundos da recursão,o que geraria um overhead excessivo de criação de *threads*, focamos no primeiro caractere da senha `idx == 0`. O laço inicial que itera sobre o alfabeto `letters` foi paralelizado usando a diretiva `#pragma omp parallel for`.
2. **Escalonamento Dinâmico -** Utilizamos a cláusula `schedule(dynamic)`. Como a busca é interrompida assim que a senha é encontrada, algumas ramificações da árvore de combinações podem terminar mais rápido que outras. O escalonamento dinâmico garante um melhor balanceamento de carga, distribuindo novas iterações para as *threads* que ficarem ociosas.
3. **Privatização de Variáveis -** Para evitar *condições de corrida*, criamos cópias locais do prefixo da senha `str_local` e do buffer de hash `hash2_local` para cada iteração do laço paralelo.
4. **Seção Crítica -** Quando uma *thread* encontra a senha correta, ela precisa atualizar a variável de controle `ok` e imprimir o resultado. Isso foi protegido por uma diretiva `#pragma omp critical` para garantir que apenas uma *thread* por vez acesse esse bloco, evitando inconsistências na saída padrão.


## 3. Resultados e Desempenho

### Especificações do ambiente de testes

Para garantir a reprodutibilidade dos resultados, os testes de tempo de execução foram realizados em uma máquina com o seguinte hardware e software:

**Processador:** Intel(R) Core(TM) i7-8565U CPU @ 1.80GHz (1.99 GHz) - Arquitetura x64

**Memória RAM:** 8,00 GB

**Placas Gráficas:** NVIDIA GeForce MX110 (2 GB) e Intel(R) UHD Graphics 620 (128 MB)

**Sistema Operacional:** Windows 11 Home Single Language (64 bits, Compilação 26200.8875)

**Armazenamento:** HDD/SSD com 1,08 TB usados de 1,14 TB disponíveis

### Tempos de execução

Os testes de tempo de execução foram realizados buscando a senha cujo hash é `afa345bc5ced1b9bf90a3ff76d8ac111` (resultado: `HPCMPP`).

| Versão / Ambiente | Número de Threads | Tempo de Execução |
| :--- | :---: | :---: |
| Sequencial | 1 | 90.91s |
| Paralelo (OpenMP) | 2 | 46.62s |
| Paralelo (OpenMP) | 4 | 27.24s |
| Paralelo (OpenMP) | 8 | 14.83s |

### Análise de Speedup

O *Speedup* ($S$) mede o quão mais rápido o código paralelo é em relação ao sequencial, sendo calculado pela fórmula:
$$S = \frac{T_{seq}}{T_{par}}$$

Aplicando a fórmula aos nossos resultados:
* **Para 2 threads:** $S_2 = \frac{90.91}{46.62} \approx 1.95$ (97.5% da eficiência ideal)
* **Para 4 threads:** $S_4 = \frac{90.91}{27.24} \approx 3.34$ (83.5% da eficiência ideal)
* **Para 8 threads:** $S_8 = \frac{90.91}{14.83} \approx 6.13$ (76.6% da eficiência ideal)

### Conclusão
O ganho de desempenho foi extremamente satisfatório, escalando muito bem com o aumento do número de threads lógicas. Obtivemos um *speedup* quase linear com 2 threads (1.95x) e um salto considerável para 8 threads (6.13x), reduzindo o tempo de resolução de mais ou menos 91 segundos para menos de 15 segundos. A queda suave na eficiência para 8 threads é esperada e se deve ao *overhead* de gerenciamento das threads do OpenMP e ao limite de escalabilidade natural ditado pela Lei de Amdahl.


## 4. Instruções de Compilação e Execução

### Comandos MAKEFILE
O arquivo `Makefile` providenciado já contém todas as rotinas necessárias:

- `make`: Compilar os códigos (sequencial e paralelo).
- `make runtime`: Executar o código sequencial contabilizando o tempo.
- `make runtime_parallel`: Executar o código paralelizado contabilizando o tempo (utiliza a variável `OMP_NUM_THREADS` se definida no ambiente).
- `make clean`: Limpa os arquivos objeto e binários gerados.

### Exemplo de Uso (Linux)

Para executar variando o número de *threads*, utilize os comandos abaixo:

```bash
# Execução sequencial
make runtime

# Execução paralela com 2 threads
OMP_NUM_THREADS=2 make runtime_parallel

# Execução paralela com 4 threads
OMP_NUM_THREADS=4 make runtime_parallel

# Execução paralela com 8 threads
OMP_NUM_THREADS=8 make runtime_parallel
