# Socket Challenge in C

Implementação de sockets in C no padrão Unix para comunicação cliente-servidor via TCP/IPv4.


## Requisitos do desafio:

* Criar dois pequenos programas que devem ser compilados com o gcc em ambiente Linux.
* Cada programa deverá ter pelo menos dois arquivos .c onde o primeiro possuirá a função main e o segundo possuirá a implementação específica que será chamada pela função main.


### Programa 1
> Implementar um cliente tcp utilizando sockets.
  >  O endereço ip destino e a porta devem ser informados na linha de comando

### Programa 2
> Implementar um servidor tcp utilizando sockets para aceitar somente uma conexão.

 
### Como deve funcionar: 

1. Iniciar o Programa 1 (client) para estabelecer conexão com o Programa 2;
2. Iniciar o Programa 2 (server) para ficar aguardando uma conexão;
3. Enviar mensagens de texto digitadas no terminal do programa 1 para o 2;
4. Enviar mensagens de texto digitadas no terminal do programa 2 para o 1;
5. Se digitar # em qualquer um dos programas ele encerra;
6. Se iniciar somente o programa 1 (client) ele deve ficar tentando conectar:
    1. infinitamente até que estabeleça conexão ou seja encerrado.
  

## Informações

Os programas foram separados nos diretórios `Client` e `Server`.

O desafio solicita que o servidor aceite apenas 1 conexão e estabelece uma ordem na troca de mensagens.

Portanto, threads não foram implementadas por:
1. Não ser necessário gerenciar múltiplas conexões no servidor
2. Não ser necessário gerenciar comunicação "assíncrona" nos canais de entrada e saída do cliente e nem do servidor. Em outras palavras, aceitar a comunicação entre os pares em qualquer ordem (comunicação apenas sequencial: 'a' envia, 'b' responde e repete)

### Compilar os programas

Considerando a raiz do projeto.

**Cliente** 
1. Acessar o diretório da aplicação `Client` (ex. `cd Client`).
2. Invocar o compilador para gerar o programa do Cliente: `gcc -o client main.c tcp_socket_client.c`.
3. O executável gerado no passo 2 é o arquivo `client`.

**Servidor**
1. Acessar o diretório de aplicação `Server` (ex. `cd Server`). OBS: Se você estiver no diretório `Client`, poderá navegar até o `Server` com `cd ../Server`.
2. Invocar o compilador para gerar o programa do Servidor: `gcc -o server main.c tcp_socket_server.c`.
3. O executável gerado no passo 2 é o arquivo `server`.

### Executar os programas.

A ordem de execução dos programas é indiferente (pode ser primeiro o cliente e depois o servidor, ou vice-versa).

Execute os programas em 2 terminais separadamente.

Considerando os programas sendo executados na mesma máquina, os passos a seguir podem ajudar.

#### Iniciando o cliente

No terminal 1.

No diretório do executável do cliente gerado na compilação, execute o cliente especificando a **porta** e o **endereço** do servidor como argumentos da linha de comando do programa (ex. aplicação servidora no localhost na porta 1234).
> ./client -port 1234 -server 127.0.0.1

#### Iniciandoo servidor

No terminal 2.

No diretório do executável do servidor gerado na compilação, execute o servidor especificando apenas a porta que o servidor deverá ficar "ouvindo" (aguardando conexão) - use a mesma porta indicada no cliente.
> ./server -port 1234


#### Uso

Troque mensagens entre cliente e servidor (nos 2 terminais) e veja o resultado.

Digite # em qualquer um dos 2 programas que a execução será interrompida em ambos.


## Informações gerais:

* Código desenvolvido por: Eduardo Alves da Silva.
* Em: 23 de novembro de 2024.
* O maior esforço foi na reorganização do código base (próximo comentário) que estava todo contido nas funções `main` e possuía 3 trocas de mensagens entre cliente e servidor (interação apenas no servidor).
* Código base - teste de conexão de socket em C entre cliente e servidor aproveitado da disciplina de Sistemas Distribuídos lecionada em 2021/2 na Univali [Repositório e branch do Código base](https://github.com/edualvss/sisdis/tree/2021-2/socket_c).
* O código base foi inspirado em: https://www.geeksforgeeks.org/socket-programming-cc/.
* A parte de leitura dos parâmetros da linha de comando foi implementada do zero sem uso de nenhuma fonte.
