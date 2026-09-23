# Condiciones de carrera y sección crítica

## ¿Qué es una condición de carrera?

Es cuando dos hilos usan al mismo tiempo una variable compartida (aquí
`global_counter`) y ninguno espera al otro. El resultado final depende
de en qué orden el sistema operativo va alternando entre los hilos, no
solo de la lógica del programa.

## ¿Por qué el resultado puede cambiar en cada ejecución?

Porque `global_counter++` no se hace en un solo paso: primero se lee el
valor, luego se le suma 1, y después se guarda. Son 3 pasos. Si un hilo
es interrumpido justo entre esos pasos, el otro hilo puede leer un valor
"viejo" y pisar el cambio del primero. Como el sistema operativo decide
en qué momento interrumpe cada hilo, el resultado no siempre es el
mismo. Con 1000 iteraciones casi no se nota (los hilos terminan muy
rápido), pero si se sube a millones de iteraciones sí se ve claramente
el problema.

## ¿Qué es la sección crítica?

Es la parte del código que toca el recurso compartido y que debe
ejecutarse de a un hilo a la vez. Acá es esta línea, dentro de los dos
bucles `for`:

```c
global_counter++;   // o global_counter--;
```

Con mutex queda protegida así:

```c
pthread_mutex_lock(&mutex);
global_counter++;      // sección crítica
pthread_mutex_unlock(&mutex);
```

Mientras un hilo está entre el `lock` y el `unlock`, el otro tiene que
esperar. Así ya no se pueden pisar los cambios.

## movl, addl y subl (del archivo .s)

`global_counter++;` se convierte en:

```asm
movl    global_counter(%rip), %eax   ; lee global_counter
addl    $1, %eax                      ; le suma 1
movl    %eax, global_counter(%rip)    ; lo guarda de vuelta
```

`global_counter--;` se convierte en:

```asm
movl    global_counter(%rip), %eax   ; lee global_counter
subl    $1, %eax                      ; le resta 1
movl    %eax, global_counter(%rip)    ; lo guarda de vuelta
```

- **movl**: mueve/copia un valor de 32 bits (se usa para leer y para
  escribir).
- **addl**: suma 1 al valor (el `++`).
- **subl**: resta 1 al valor (el `--`).

Como son 3 instrucciones separadas y no una sola, un hilo puede quedar
"a mitad de camino" cuando lo interrumpen, y ahí es donde se produce la
condición de carrera.

## Conclusión

Sin mutex el resultado no es confiable: a veces da 20 y a veces no, según
cómo se crucen los hilos. Con mutex siempre dio 20, en todas las
pruebas. Por eso hay que usar mutex cuando varios hilos tocan la misma
variable.
