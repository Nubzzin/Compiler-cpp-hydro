## Um compillador feito em c++ para uma linguagem teste com syntax simples

Ele transforma "return <numero_inteiro>;" em assembly
sem uso de AST ou algoritmos mais complexos

depende de `nasm`,`ld` e `cmake`

Uso:

`cmake --build build`

`./build/hydro hydro.hy`

`./outasm` 

Para visualizar o exitcode:

`echo $?`