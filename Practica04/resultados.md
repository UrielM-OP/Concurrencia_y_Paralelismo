# Resultados con el Algoritmo de Peterson

## Tabla de resultados

| Hilos | Puntos | π calculado | Tiempo |
| ----: | ----------: | ----------: | -----: |
| 4 | 100,000,000 | 3.116684 | 0.271000 s |
| 8 | 100,000,000 | 3.137324 | 0.479000 s |
| 16 | 100,000,000 | 3.147832 | 2.258000 s |

## Cómo funciona el algoritmo de Peterson

Es un algoritmo que usualmente solo se puede usar para 2 hilos no para
más, solo en algunos casos hay otro método. Los hilos suben por
numHilos - 1 "pisos"; en cada piso, cada hilo compite contra todos los
demás con la misma lógica de flag/turno del Peterson clásico de 2
hilos. Solo el que gana en todos los pisos entra a la sección crítica.
Mientras espera, el hilo hace espera activa (revisa la condición en un
ciclo, sin dormir), lo cual es puramente software: no depende de
ninguna llamada al sistema operativo.

## Conclusión

El tiempo crece mucho más rápido que con mutex, y de forma no lineal.
El salto de 8 a 16 hilos (0.479 s → 2.258 s) es casi 5x, mientras que
de 4 a 8 apenas duplica. Esto tiene dos causas:

1. Costo algorítmico O(N²): cada acceso a la sección crítica revisa
   numHilos - 1 pisos, y en cada piso compara contra los numHilos
   hilos restantes. El trabajo por acceso crece cuadráticamente con el
   número de hilos, mientras que un mutex es O(1) sin importar cuántos
   hilos haya.

2. Espera activa con núcleos limitados: los hilos que esperan no
   duermen, siguen consumiendo CPU en un ciclo. Si hay más hilos que
   núcleos, se pelean por el mismo CPU y el rendimiento se desploma de
   ahí el salto tan fuerte en 16 hilos.
