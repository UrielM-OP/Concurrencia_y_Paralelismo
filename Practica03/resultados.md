# Resultados: número de hilos vs. tiempo de ejecución

`totalPuntos = 1,000,000` en todas las pruebas

Comandos usados:

    ./pi_montecarlo 2 1000000
    ./pi_montecarlo 8 1000000
    ./pi_montecarlo 16 1000000

| Numero de hilos | puntosDentro | π aproximado | Tiempo (s) |
|---|---|---|---|
| 2  | 786154 | 3.144616 | 0.066761 |
| 8  | 785349 | 3.141396 | 0.116217 |
| 16 | 785306 | 3.141224 | 0.139044 |
