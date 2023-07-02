# roda por todos os arquivos .dis e cria uma lista com todos os comando ASm UNICOS que tem no arquivo
# os comando sempre estão na direita de cada linha dos arquivos. exemplo:
# endereço: hexidacimal     <COMANDO ASM>
# o codigo abaixo pega todos os comandos ASM e coloca em uma lista ignorando os comandos repetidos
# para falar se um comando é repedito só vamos considerar o mnemonico do comando, idnorando seus argumentos (o que vem depois de um espaço)
# <COMANDO ASM> = <mnemonico> <argumento1> , <argumento2> 

import os

# lista com todos os comandos ASM unicos
lista_comandos_asm = []

# loop por todos os arquivos .dis
for file in os.listdir("."):
    if file.endswith(".dis"):
        # abre o arquivo .dis
        with open(file) as f:
            # loop por todas as linhas do arquivo
            for line in f:
                if(len(line) > 1):
                    # pega o comando ASM da linha
                    comando_asm = line.split()[2]
                    
                    # verifica se o comando já está na lista
                    if comando_asm not in lista_comandos_asm:
                        # adiciona o comando na lista
                        lista_comandos_asm.append(comando_asm)

# imprime a lista de comandos ASM e escreve em um arquivo
with open("lista_comandos_asm.txt", "w") as f:
    for comando_asm in lista_comandos_asm:
        print(comando_asm)
        f.write(comando_asm + "\n")
        