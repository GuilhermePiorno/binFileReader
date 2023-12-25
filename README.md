# binFileReader
## Leitor de arquivos binários.

Program to visualize binary files, displays data in hex or can be decoded with custom settings from user.

Highly recommended be ran at fullscreen for better visualization. 

If still having issues with formating, try changing byte grouping and bytes per line.


TODOs:

-Organize code and simplify it before it becomes a bigger mess.

~~-Add option to "back out" form most options. As it is right now if you enter Header customization before settings things up and realize it, you can't back out for example.~~

-Condense some options or move to a "config" submenu if it's not a very used option. (maybe toggle between Dec and Hex offsets don't deserve to be on the main menu, or headers customization
should be already setup during decoding setup)

~~-Adicionar opção de cabeçalho para dados decodificados. / Implement option to add a header for the decoded info.~~

~~-Adicionar funcionalidade de agrupamento de bytes. / Implement byte grouping when displaying data.~~

~~- Consider a mode that corresponds with the decoding being used. e.g.: If decoding looks like this: int/float/char/int. ~~
~~Byte grouping could switch to 4bytes/4bytes/1byte/4bytes and 13 bytes per line. ~~
Done this for bytes per line, to do this for grouping would probably require a rewrite of the main "hexdump" function.

-Fix issue that causes spacing and table border displaying incorrectly.

-Consider a different character display for chars between values 0 and 37 currently being displayed as a dot (".").

## Example:
CLI:

Compiling and opening a binReader:
```
gcc -o binReader binReader
./binReader
```
Opening a file:
```
file: dados.dat
```
Selecting byte grouping size:
```
option: 4
type group size: 4
```
Selecting how many bytes per line will be displayed:
```
option: 2
type number of bytes per line: 16
```
Decoding data:
```
option: 3
type for the 1st block: int
type for the 2nd block: int
type for the 3rd block: float
type for the 4th block: int
```
Setup custom decode column header
```
option: 6
fill in with a name for the column: matricula
fill in with a name for the column: next
fill in with a name for the column: media
fill in with a name for the column: status
```



![Output should look like this.](/example.png)
