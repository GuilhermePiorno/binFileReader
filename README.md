# binFileReader
## Leitor de arquivos binários.

Program to visualize binary files, displays data in hex or can be decoded with custom settings from user.

Programa para exibir dados de arquivos binários em hexadecimal puro ou com decodificação selecionada pelo usuário.



TODOs:

-Adicionar opção de cabeçalho para dados decodificados. / Implement option to add a header for the decoded info.

~~-Adicionar funcionalidade de agrupamento de bytes. / Implement byte grouping when displaying data.~~

-Considerar um modo de agrupamento que condiz com o decoder. Ex: no caso de um decode int/float/char/int.
agrupar linha em 4 bytes/4 bytes/1 byte/4 bytes.

-Consertar espaçamento e bordas de tabela no linux. / Fix issue that causes spacing and table border displaying incorrectly.

-Considerar a exibição de chars entre 0 e 37 decodificados de forma diferente de apenas um ponto (".").

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
option: 3
type group size: 4
```

![alt text](/example.png)
