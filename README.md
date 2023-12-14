# binFileReader
## Leitor de arquivos binários.

Program to visualize binary files, displays data in hex or can be decoded with custom settings from user.

Programa para exibir dados de arquivos binários em hexadecimal puro ou com decodificação selecionada pelo usuário.



TODOs:

-Adicionar opção de cabeçalho para dados decodificados. / Implement option to add a header for the decoded info.

-Adicionar funcionalidade de agrupamento de bytes. / Implement byte grouping when displaying data.

-Consertar espaçamento e bordas de tabela no linux. / Fix issue that causes spacing and table border displaying incorrectly.

## Example:
CLI:
```
gcc -o binReader binReader
./binReader
file: dados.dat
option: 2
number of blocks: 4
type for the 1st block: int
type for the 2nd block: int
type for the 3rd block: float
type for the 4th block: int
```

![alt text](/example.png)
