# binFileReader
## Leitor de arquivos binários.

Program to visualize binary files, displays data in hex or can be decoded with custom settings from user.

Programa para exibir dados de arquivos binários em hexadecimal puro ou com decodificação selecionada pelo usuário.



TODOs:

-Organize code and simplify it before it becomes a bigger mess.

-Add option to "back out" form most options. As it is right now if you enter Header customization before settings things up and realize it, you can't back out for example.

-Condense some options or move to a "config" submenu if it's not a very used option. (maybe toggle between Dec and Hex offsets don't deserve to be on the main menu, or headers customization
should be already setup during decoding setup)

~~-Adicionar opção de cabeçalho para dados decodificados. / Implement option to add a header for the decoded info.~~

~~-Adicionar funcionalidade de agrupamento de bytes. / Implement byte grouping when displaying data.~~

- Consider a mode that corresponds with the decoding being used. e.g.: If decoding being looks like this: int/float/char/int. 
Byte grouping could switch to 4bytes/4bytes/1byte/4bytes and 13 bytes per line. 

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
