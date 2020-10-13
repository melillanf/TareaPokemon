---------- Readme de la tarea 2 ----------

Integrantes del equipo:
Pablo Julio
Fernando Melillan
Nicolás Oñate
Sebastian Yañez

Opciones que funcionan correctamente:

1.- Exportar Pokémon
2.- Capturar Pokémon
4.- Buscar Pokémon por su nombre
5.- Buscar Pokémon por nombre
6.- Mostrar todos los Pokémon vistos
7.- Mostrar por mayor PC
8.- Mostrar por mayor PS
9.- Liberar
10.- Buscar por región

Opciones que no funcionan correctamente: 

3.- Buscar por tipo

Datos adicionales de la tarea:

Al seleccionar la opción 1, solamente se retorna la frase "primera linea = (null)Archivo leido...". Esto se debe a que la función ya está implementada desde el inicio, por lo que no es necesario seleccionarla.

Al liberar un pokémon y luego buscarlo usando la función 4, seguirán apareciendo sus datos como si el pokémon aun se conservara, sin embargo, al buscarlo con las funciónes 5, 6 o 10, el número de existencias se reducirá en 1 (Si antes de ser de liberar un pokémon, su existencia era 2, ahora será 1).

La opción 3 no funciona correctamente puesto que los datos de los pokémon salían incompletos.

La opción 10 funciona correctamente, sin embargo, al terminar la búsqueda no se devuelve al menú y el programa espera a ser terminado.