# Conclusion

En estas pruebas, 2 hilos dio el mejor tiempo (0.0193s en promedio).
Con 8 y 16 hilos el tiempo fue subiendo en vez de bajar.

## ¿Aumentar la cantidad de hilos siempre mejora el rendimiento?

No. Aqui mas hilos hizo el programa mas lento. La maquina donde se
corrieron las pruebas solo tiene 1 nucleo disponible, asi que los
hilos no corren en paralelo de verdad, solo se van turnando (context
switching), y crear mas hilos de los que la maquina puede atender solo
agrega overhead: mas cambios de contexto y mas hilos peleando por el
semaforo (sem_wait/sem_post) cada vez que un punto cae dentro del
circulo.

Por eso el numero ideal de hilos depende de cuantos nucleos tiene el
CPU. Pasado ese numero, agregar mas hilos ya no ayuda y hasta puede
empeorar el tiempo, como paso aqui.
