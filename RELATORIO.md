# Caso windows e gcc não reconheçam OpenSSL/md5.h, mas tente pelo wsl, se ainda não funcionar:

1. Intale o OpenSSL full version (não o light): https://slproweb.com/products/Win32OpenSSL.html

2. Adicionar o caminho do bin (Program Files\OpenSSL-Win64\bin) de OpenSSL em Path

3. Criar a variavel para o caminho do include (Program Files\OpenSSL-Win64\include) de OpenSSL em C_INCLUDE_PATH

4. Criar a variavel para o caminho da biblioteca (Program Files\OpenSSL-Win64\lib\VC\x64\MD) de OpenSSL em LIBRARY_PATH

5. Executar o makefile

6. Caso ainda dê erro, reinicie a ide e/ou abra um novo terminal

# Comandos MAKEFILE

- make: compilar o codgio
- make runtime: executar codigo sequencial
- make runtime_parallel: executar codigo paralelizado
