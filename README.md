# Simulador de Memória Virtual

## Funcionamento:
Esse projeto tem como objetivo simular o funcionamento de uma memória virtual. O programa recebe como entrada um arquivo de texto com uma sequência de endereços lógicos e simula o funcionamento de uma memória virtual com paginação. Além disso, também recebe como entrada o algoritmo de reposição, o tamanho da memória virtual e o tamanho das páginas. 

O programa começa lendo e processando os argumentos passados pela linha de comando. Então, aloca e inicializa uma memória virtual que consiste em um vetor de páginas, sendo que cada página é uma struct que guarda as seguintes informações:
  - Id da página
  - Bit que indica se a página foi modificada
  - Tempo do último acesso, para o algoritmo LRU
  - Bit de referência, para o algoritmo de segunda chance
  - Tempo de entrada na memória, para o algoritmo FIFO

Em seguida, o programa lê o arquivo de entrada e simula o funcionamento da memória virtual. Para cada endereço lógico lido, o programa calcula o id da página. Então, verifica se a página com o id calculado está na memória virtual.

- Se estiver, o programa atualiza o bit de referência, o bit que indica se houve uma escrita e o tempo do último acesso. 
- Se não estiver, o programa verifica se há uma página livre na memória virtual. 
  - Se houver, o programa aloca a página na memória virtual. 
  - Se não houver, o programa escolhe uma página para ser substituída de acordo com o algoritmo de substituição escolhido. Então, o programa aloca a página na memória virtual.

Por fim, o programa imprime um relatório com as seguintes informações:
  - Arquivo de entrada
  - Tamanho da memória
  - Tamanho das páginas 
  - Técnica de reposição
  - Número de acessos à memória
  - Páginas lidas
  - Páginas escritas

## Algoritmos de substituição de páginas:
O programa implementa os seguintes algoritmos de substituição de páginas:
- FIFO: substitui a página que está na memória há mais tempo.
- LRU: substitui a página que não foi acessada há mais tempo.
- Segunda chance: substitui a primeira página encontrada, passando de forma circular pela memória, que não foi referenciada desde a última vez que foi verificada.
- Random: substitui uma página aleatória.

Para implementar esses algoritmos, foi necessário adicionar o tempo de entrada na memória, o tempo do último acesso e o bit de referência como atributos da struct que representa uma página. Além disso, foi necessário adicionar um contador de tempo que é incrementado a cada acesso à memória e uma variável global que indica o último índice acessado pelo algoritmo de segunda chance.

Para cada algoritmo, foi criada uma função que recebe como parâmetro um vetor de páginas e o número de páginas totais e retorna o índice da página que deve ser substituída. Essas funções são chamadas no momento em que o programa precisa substituir uma página.

## Decisões de projeto:
- Assumimos que o arquivo de entrada sempre terá uma sequência de endereços lógicos válida, ou seja, não há endereços lógicos inválidos ou endereços lógicos que não possam ser convertidos para um número inteiro.
- Implementamos o algoritmo de segunda chance de forma que quando uma referência é feita a uma página, o índice global que salva a última página acessada pelo algoritmo é mantido o mesmo da última vez que o algoritmo foi chamado. Dessa forma, da próxima vez que o algoritmo for executado, ele continuará de onde parou ao invés de começar a partir da última página que foi referenciada.

## Análise de desempenho:
Para analisar o desempenho dos algoritmos de substituição de páginas, o programa foi executado com as seguintes configurações para todos os arquivos de entrada e algoritmos de substituição, sendo que a métrica utilizada foi o número de page faults que ocorreram durante a execução do programa:
1. Tamanho da página: 4 KB; Tamanho da memória variando entre os valores [128 KB, 256 KB, 512 KB, 1024 KB, 2048 KB, 4096 KB, 8192 KB, 16384 KB]

Imagens

2. Tamanho da memória: 1024 KB; Tamanho da página variando entre os valores [2 KB, 4 KB, 8 KB, 16 KB, 32 KB, 64 KB]

Imagens

Foi possível observar o mesmo comportamento em ambos os casos de testes: o algoritmo de segunda chance apresentou o melhor desempenho geral, perdendo para o LRU apenas no arquivo do compressor, o qual possui a maior quantidade de acessos a endereços repetidos. Além disso, o algoritmo Random apresentou o pior desempenho para todos os arquivos de entrada, seguido do FIFO.
Nos dois casos, houve uma diferença média de cerca de 20% entre o melhor e o pior algoritmo para todos os arquivos. Por outro lado, a diferença média entre o algoritmo de segunda chance e o LRU foi de menos de 0,1% para todos os arquivos. A maior diferença entre o melhor e o pior algoritmo aconteceu no arquivo da matriz e a menor diferença aconteceu no arquivo do compressor. Alternativamente, a maior diferença entre o algoritmo de segunda chance e o LRU aconteceu no arquivo do compressor e a menor diferença aconteceu no arquivo da matriz. Cabe ressaltar que as maiores diferenças ocorrem nas configurações com a menor quantidade de páginas.

O número de páginas da memória virtual foi o fator que mais influenciou no desempenho dos algoritmos de substituição de páginas. Isso acontece porque o número de páginas da memória virtual influencia diretamente na quantidade de páginas que podem ser mantidas na memória ao mesmo tempo. Portanto, o conjunto de testes 1 apresenta natureza inversa ao conjunto de testes 2, pois no primeiro o número de páginas aumenta ao longo do eixo x, enquanto no segundo o número de páginas diminui ao longo do eixo x.

Podemos concluir que os resultados obtidos são condizentes com a teoria, pois o algoritmo de segunda chance e o LRU levam em consideração acessos recentes à memória, aproveitando a localidade de referência temporal, diferente do FIFO e do Random. Além disso, o algoritmo de segunda chance apresentou o melhor desempenho geral, pois enquanto o LRU tem o melhor desempenho para acessos a endereços repetidos, o algoritmo de segunda chance consegue manter um conjunto de páginas mais homogêneo na memória, o que é mais vantajoso para arquivos com poucos acessos a endereços repetidos.   

A mesma análise pode ser feita para o número de escritas na memória, pois esse valor é influenciado pelo número de page faults.

Por fim, levando em consideração que o tempo total dos acessos é dominado pelo número de páginas lidas do disco, cabe ressaltar que o algoritmo que melhor balanceia o desempenho em termos de page fault com performance é o de segunda chance. Isso acontece porque o algoritmo de segunda chance apresenta um desempenho muito melhor do que o FIFO e o Random e, apesar de possuir um desempenho muito próximo ao LRU, apresenta uma complexidade de tempo menor, pois o LRU precisa percorrer todas as páginas da memória para encontrar a página que deve ser substituída, enquanto o algoritmo de segunda chance precisa apenas percorrer as páginas até encontrar uma página que não foi referenciada desde a última vez que foi verificada.

