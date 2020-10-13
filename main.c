#include <stdio.h>
#include <stdlib.h>
#include "hashmap.h"
#include <string.h>
#include "list.h"


typedef struct PokeID{//Variable que va dentro de las Pokedex (ID/Nombre)
    char name[50];//Nombre
    char type[50];//Tipos
    char prevEvo[50];//Evolucion previa
    char nextEvo[50];//
    int numeroPokedex;
    char region[50];
    int exist;//Existencias total del pokemon en el almacenamiento
}PokeID;

typedef struct Pokemon{ //Variable que va dentro de PokeStorage que es el mapa que almacena los pokemons capturados
    int id;
    char name[50];
    int PC;
    int PS;
    char gender[50];
}Pokemon;

const char *get_csv_field (char * tmp, int k) {
    int open_mark = 0;
    char* ret=(char*) malloc (100*sizeof(char));
    int ini_i=0, i=0;
    int j=0;
    while(tmp[i+1]!='\0'){

        if(tmp[i]== '\"'){
            open_mark = 1-open_mark;
            if(open_mark) ini_i = i+1;
            i++;
            continue;
        }

        if(open_mark || tmp[i]!= ','){
            if(k==j) ret[i-ini_i] = tmp[i];
            i++;
            continue;
        }

        if(tmp[i]== ','){
            if(k==j) {
               ret[i-ini_i] = 0;
               return ret;
            }
            j++; ini_i = i+1;
        }

        i++;
    }

    if(k==j) {
       ret[i-ini_i] = 0;
       return ret;
    }


    return NULL;
}
int encontrarComa(char * palabra); //Funcion que retorna 0 si la string contiene una coma y retorna 1 si no lo tiene
char * quitarEspacio(char * palabra);//Funcion que elimina el espacio dentro de un string
void leerArchivo(HashMap * PokedexID, HashMap * PokedexName, HashMap * PokeStorage, FILE * archivo);//Lee el archivo y almacena los pokemons en los mapas Pokedex(PokedexID, PokedexName)
void pokeAtrapado(HashMap * PokedexID, HashMap * PokedexName, HashMap *  PokeStorage);////Funcion que lee las variables para crear y añadir un nuevo Pokemon
void createPokeID(char * name, char * type, char * prevEvo, char * nextEvo, char * region, int idPokedex, PokeID * nuevaID);//La variable exist se añade después en la funcion pokeAtrapado
Pokemon * createPokemon(int id, int PC, int PS, char * name, char * gender);//Inicializa una variable Pokemon con los datos entregados y retorna la nueva variable
void buscarTipo (HashMap * PokedexName, HashMap * PokeStorage);
void buscarNombrePokemon (HashMap * PokeStorage);
void buscarNombrePokeID (HashMap * PokedexName);
void mostrarPokedex (HashMap * PokedexName);
void mostrarPokemonMayorPC (HashMap * PokeStorage);
void mostrarPokemonMayorPS (HashMap * PokeStorage);
void liberarPokemon (HashMap * PokeStorage, HashMap * PokedexID, HashMap * PokedexName);
void buscarRegion(HashMap * PokedexName, HashMap * PokeStorage);
void guardarPokedex(HashMap * PokedexName, HashMap * PokedexID, PokeID * nuevoID);


int main()
{
    HashMap * PokedexName = createMap(200);
    HashMap * PokedexID = createMap(200);//Almacena PokeID por numero de pokedex
    HashMap * PokeStorage = createMap(200);
    FILE * archivo = fopen("pokemon Archivo1.csv","r");
    leerArchivo(PokedexID, PokedexName, PokeStorage, archivo);
    int op = 0;
        while(op < 10)
    {
        printf("1.- Exportar Pokemon\n");
        printf("2.- Capturar Pokemon\n");
        printf("3.- Buscar por tipo\n");
        printf("4.- Buscar Pokemon por su nombre\n");
        printf("5.- Buscar Pokemon por nombreID\n");
        printf("6.- Mostrar todos los Pokemon vistos\n");
        printf("7.- Mostrar por mayor PC\n");
        printf("8.- Mostrar por mayor PS\n");
        printf("9.- Liberar\n");
        printf("10.- Buscar por region\n");
        printf("Indica la opcion: ");
        scanf("%d", &op);
        fflush(stdin);

        switch(op)
        {
            case 1:
                leerArchivo(PokedexID, PokedexName, PokeStorage, archivo);
                break;

            case 2:
                pokeAtrapado(PokedexID, PokedexName, PokeStorage);
                break;

            case 3:
                //buscarTipo(PokeStorage);
                break;

            case 4:
                buscarNombrePokemon(PokeStorage);
                break;

            case 5:
                buscarNombrePokeID(PokedexName);
                break;

            case 6:
                mostrarPokedex(PokedexName);
                break;

            case 7:
                mostrarPokemonMayorPC(PokeStorage);
                break;

            case 8:
                mostrarPokemonMayorPS(PokeStorage);
                break;

            case 9:
                liberarPokemon(PokeStorage, PokedexID, PokedexName);
                break;

            case 10:
                buscarRegion(PokedexName,PokeStorage);
                break;

            case 11:
                 break;
        }
    }
    return 0;
}

void leerArchivo(HashMap * PokedexID, HashMap * PokedexName, HashMap * PokeStorage, FILE * archivo){

    char * linea = (char*)malloc(1024*sizeof(char));
    char * skip = (char*)malloc(1024*sizeof(char));
    PokeID * nuevaID = (PokeID *)malloc(sizeof(PokeID));
    Pokemon * nuevoPoke = (Pokemon *)malloc(sizeof(Pokemon));
    skip = fgets(linea, 100, archivo);
    int found=0;
    printf("primera linea = %s", skip);
    char * auxkey = (char*)malloc(50*sizeof(char));
    char * name = (char*)malloc(50*sizeof(char));
    char * auxchar = (char*)malloc(50*sizeof(char));
    char * auxdup = (char*)malloc(50*sizeof(char));
    char * auxtype = (char*)malloc(50*sizeof(char));
    while(fgets (linea, 1023, archivo) != NULL){
       found++;
       //printf("encontrados %d\n", found);
       strcpy(name, strdup(get_csv_field(linea,1)));
       auxchar = strtok(name, " ");
       nuevoPoke->id = atoi(strdup(get_csv_field(linea, 0)));//ID
       strcpy(nuevoPoke->name, auxchar);
       nuevoPoke->PC = atoi(get_csv_field(linea,3));//PC
       nuevoPoke->PS = atoi(get_csv_field(linea,4));//PS
       strcpy(nuevoPoke->gender, strdup(get_csv_field(linea,5)));//Genero
       strdup(nuevoPoke->gender);
       strcpy(nuevaID->name, strdup(get_csv_field(linea,1)));//Nombre
       strdup(nuevaID->name);
       ////////////////////////////////////////////////////
       strcpy(nuevaID->type,get_csv_field(linea, 2));
       itoa(nuevoPoke->id, auxkey, 10);
       //printf("Name poke %s con id %d key auxiliar = id %s\n", nuevoPoke->name, nuevoPoke->id, auxkey);
       strcpy(nuevaID->prevEvo, strdup(get_csv_field(linea,6)));//Ev. previa
       strdup(nuevaID->prevEvo);
       strcpy(nuevaID->nextEvo, strdup(get_csv_field(linea,7)));//Ev. post
       strdup(nuevaID->nextEvo);
       nuevaID->numeroPokedex = atoi(get_csv_field(linea,8));//Num. pokedex
       strcpy(nuevaID->region, strdup(get_csv_field(linea,9)));//Region
       strdup(nuevaID->region);

       insertMap(PokeStorage,strdup(auxkey),nuevoPoke);
       guardarPokedex(PokedexName, PokedexID, nuevaID);
       nuevaID = (PokeID *)malloc(sizeof(PokeID));
       nuevoPoke = (Pokemon *)malloc(sizeof(Pokemon));
    }
    printf("Archivo leido...\n");
}

void guardarPokedex(HashMap * PokedexName, HashMap * PokedexID, PokeID * nuevoID){
    char * auxkey = (char *) malloc (50 * sizeof(char));
    char * auxname = (char *) malloc (50 * sizeof(char));
    int exist;
    auxname = strtok(nuevoID->name, " ");
    PokeID * auxPoke = searchMap(PokedexName, auxname);
    if(auxPoke == NULL){
        nuevoID->exist = 1;
        insertMap(PokedexName,auxname, nuevoID);
        itoa(nuevoID->numeroPokedex, auxkey, 10);
        insertMap(PokedexID, auxkey, nuevoID);
        return;
    }
    if(auxPoke != NULL){
        exist = auxPoke->exist + 1;
        auxPoke->exist = exist;
        itoa(auxPoke->numeroPokedex, auxkey, 10);
        auxPoke = searchMap(PokedexID, auxkey);
        auxPoke->exist = exist;
        return;
    }

}

void buscarRegion(HashMap * PokedexName, HashMap * PokeStorage){
  char * region = (char *)malloc(50*sizeof(char));
  Pokemon * busqueda = firstMap(PokeStorage);
  PokeID * auxPoke = searchMap(PokedexName, busqueda->name);
  char * lectura = (char *)malloc(50*sizeof(char));
  char auxiliar[50] ;
  printf("Ingresa la region\n");
  gets(lectura);
  strdup(lectura);
  printf("Region ingresada: %s\n",lectura);
  int found = 0;

  while(busqueda != NULL){
        found++;
    strcpy(auxiliar, auxPoke->region);
    if(strcmp(lectura, auxiliar)==0){
      found++;
      printf("-----------------------------------\n");
      printf("Pokemon encontrado en la region %s \n",region);
      printf("Nombre: %s \n  ", auxPoke->name);
      //printf("Tipo(s): %s\n",auxPoke->type); arreglar tipo que ahora es un array de strings
      printf("Evolucion previa: %s\n",auxPoke->prevEvo);
      printf("Evolucion posterior: %s\n",auxPoke->nextEvo);
      printf("Numero pokedex: %d\n",auxPoke->numeroPokedex);
      printf("Existencias: %d\n",auxPoke->exist);
      printf("Region: %s\n",auxPoke->region);
      printf("-----------------------------------\n");
    }
    busqueda = nextMap(PokeStorage);
    auxPoke = searchMap(PokedexName, busqueda->name);
  }
  if(found == 0)printf("No se encontraron Pokemon en la region ingresada\n");
  return;
}

void buscarNombrePokemon (HashMap * PokeStorage){
  Pokemon * busqueda = firstMap(PokeStorage);
  char * lectura = (char *)malloc(50*sizeof(char));
  printf("Ingresa el nombre del pokemon\n");
  gets(lectura);
  strdup(lectura);
  printf("Nombre ingresado: %s\n",lectura);
  int found = 0;
  char * aux = (char *)malloc(50*sizeof(char));

  while(busqueda != NULL){
    aux = strtok(busqueda->name, " ");
    if(strcmp(aux,lectura)==0){
        found++;
        printf("-----------------------------------\n");
        printf("Pokemon encontrado: %s \n", busqueda->name);
        printf("PC: %d \n", busqueda->PC);
        printf("PS: %d \n", busqueda->PS);
        printf("Genero: %s \n", busqueda->gender);
        printf("-----------------------------------\n");
    }
    busqueda = nextMap(PokeStorage);
  }
  if(found == 0)printf("No se encontraron Pokemon de nombre %s...\n",lectura);
}

void buscarNombrePokeID(HashMap * PokedexName){
    PokeID * busqueda = firstMap(PokedexName);
  char * lectura = (char *)malloc(50*sizeof(char));
  printf("Ingresa el nombre del pokemon\n");
  gets(lectura);
  strdup(lectura);
  printf("Nombre ingresado: %s\n",lectura);
  int found = 0;
  char * aux = (char *)malloc(50*sizeof(char));

  while(busqueda != NULL){
    aux = strtok(busqueda->name, " ");
    if(strcmp(aux,lectura)==0){
        found++;
        printf("-----------------------------------\n");
        printf("Pokemon encontrado: %s \n", busqueda->name);
       // printf("Tipos: %s \n", busqueda->type);arreglar tipo que ahora es array de strings
        printf("Evolucion posterior: %s \n", busqueda->nextEvo);
        printf("Evolucion previa: %s \n", busqueda->prevEvo);
        printf("Region: %s \n", busqueda->region);
        printf("Numero de Pokedex: %d\n",busqueda->numeroPokedex);
        printf("Existencias: %d\n",busqueda->exist);
        printf("-----------------------------------\n");
    }
    busqueda = nextMap(PokedexName);
  }
  if(found == 0)printf("No se encontraron Pokemon de nombre %s...\n",lectura);
}

void mostrarPokedex (HashMap * PokedexName){
    PokeID * busqueda = firstMap(PokedexName);
    while(busqueda!=NULL){
        printf("-----------------------------------\n");
        printf("Pokemon encontrado: %s \n", busqueda->name);
        //printf("Tipos: %s \n", busqueda->type);arreglar tipos
        printf("Evolucion posterior: %s \n", busqueda->nextEvo);
        printf("Evolucion previa: %s \n", busqueda->prevEvo);
        printf("Region: %s \n", busqueda->region);
        printf("Numero de Pokedex: %d\n",busqueda->numeroPokedex);
        printf("Existencias: %d\n",busqueda->exist);
        printf("-----------------------------------\n");
        busqueda = nextMap(PokedexName);
    }

}

void pokeAtrapado(HashMap * PokedexID, HashMap * PokedexName, HashMap *  PokeStorage){
    char * name = (char *)malloc(50* sizeof(char));
    char * type = (char *)malloc(50* sizeof(char));
    char * gender = (char *)malloc(50* sizeof(char));
    char * region = (char *)malloc(50* sizeof(char));
    char * prevEvo = (char *)malloc(50* sizeof(char));
    char * nextEvo = (char *)malloc(50* sizeof(char));
    char * auxkey = (char *)malloc(50* sizeof(char));
    int PC;
    int PS;
    int numPokedex;
    int opc;
    int count = 0;
    Pokemon * recorrido = firstMap(PokeStorage);
    while(recorrido!=NULL){
        count++;
        recorrido = nextMap(PokeStorage);
    }
    printf("Ingresa el nombre\n");
    gets(name);
    if(searchMap(PokedexName, name)==NULL && count<100){//Caso pokemon no guardado en pokedex
        printf("Ingresa el tipo(si es mas de 1 separalos con una coma)\n");//y pokestorage con espacio
        gets(type);
        printf("Ingresa el genero del Pokemon\n");
        gets(gender);
        printf("Ingresa la region del Pokemon\n");
        gets(region);
        printf("Ingresa la evolucion previa del Pokemon\n");
        gets(prevEvo);
        printf("Ingresa la evolucion posterior del Pokemon\n");
        gets(nextEvo);
        printf("Ingresa los PC(Puntos de Combate) del Pokemon\n");
        scanf("%d",&PC);
        printf("Ingresa los PS(Puntos de Salud) del Pokemon\n");
        scanf("%d", &PS);
        printf("Ingresa el numero de Pokedex del Pokemon\n");
        scanf("%d", &numPokedex);
        Pokemon * nuevo = (Pokemon *) malloc (sizeof(Pokemon));
        PokeID * nuevoID = (PokeID *) malloc (sizeof(PokeID));
        strcpy(nuevo->name, name);
        strcpy(nuevo->gender, gender);
        nuevo->id = count +1;
        nuevo->PC = PC;
        nuevo->PS = PS;
        nuevoID->numeroPokedex = numPokedex;
        strcpy(nuevoID->name, name);
        strcpy(nuevoID->nextEvo, nextEvo);
        strcpy(nuevoID->prevEvo, prevEvo);
        strcpy(nuevoID->region, region);
        strcpy(nuevoID->type, type);
        nuevoID->exist = 1;
        insertMap(PokeStorage, nuevo->name, nuevo);
        insertMap(PokedexName, nuevoID->name, nuevoID);
        itoa(nuevoID->numeroPokedex, auxkey, 10);
        insertMap(PokedexID, auxkey, nuevoID);
    }
    else if(searchMap(PokedexID, name) != NULL && count<100){//Caso Pokemon existe en Pokedex
                                                                        //y pokestorage con espacio


        PokeID * aux = searchMap(PokedexName, name);
        int exist = aux->exist;
        exist++;
        aux->exist = exist;
        itoa(aux->numeroPokedex, auxkey, 10);
        aux = searchMap(PokedexID, auxkey);
        aux->exist = exist;



    } else if(count >= 100){
        printf("Almacenamiento Pokemon lleno.\n Ingresa 0 si quieres liberar un pokemon y 1 para salir");
        scanf("%d",&opc);
        if(opc == 1){
            liberarPokemon(PokeStorage, PokedexID, PokedexName);
        }
    }




}

void buscarTipo (HashMap * PokedexName, HashMap * PokeStorage){
    char * lecturatipo = (char *) malloc (50 * sizeof(char));
    int found = 0, i;
    char * aux = (char *)malloc(50*sizeof(char));
    char * tipo = (char *)malloc(50*sizeof(char));
    PokeID * busqueda = firstMap(PokedexName);
    Pokemon * auxbusqueda;

    printf("Ingresa el nombre del pokemon\n");
    gets(lecturatipo);
    strdup(lecturatipo);

    while(busqueda != NULL){
        i = 0;
        busqueda = nextMap(PokedexName);
    }




}

char * quitarEspacio(char * palabra){
    int i, j = 0, leng;
    leng = strlen(palabra);
    char * aux = (char *)malloc(50*sizeof(char));
    for(i = 0; i<leng ; i++){
        if(palabra[i]!= ' '){
            aux[j] = palabra[i];
            j++;
        }
    }
    return aux;
}

void mostrarPokemonMayorPC (HashMap * PokeStorage){
    Pokemon * recorrido = firstMap(PokeStorage);
    int count = 0;
    int i = 0;
    while(recorrido != NULL){
        count++;
        recorrido = nextMap(PokeStorage);
        printf("%d pokemons copiados\n",count);
    }
    Pokemon * array[count];
    recorrido = firstMap(PokeStorage);
    count = 0;
    while(recorrido != NULL){
        count++;
        array[i] = recorrido;
        i++;
        recorrido = nextMap(PokeStorage);
    }
    for(i = 0; i<count-1;i++){
        for(int j = 0; j < count-1; j++){
            if(array[j]->PC < array[j+1]->PC){
                Pokemon * aux = array[j+1];
                array[j+1] = array[j];
                array[j] = aux;

            }
        }
    }
     for(i = 0; i<count-1; i++){
        printf("-----------------------------------\n");
        printf("%s | PC: %d | PS: %d | Genero: %s\n",array[i]->name, array[i]->PC,array[i]->PS, array[i]->gender);
        printf("-----------------------------------\n");
    }
}

void mostrarPokemonMayorPS (HashMap * PokeStorage){
    Pokemon * recorrido = firstMap(PokeStorage);
    int count = 0;
    int i = 0;
    while(recorrido != NULL){
        count++;
        recorrido = nextMap(PokeStorage);
        printf("%d pokemons copiados\n",count);
    }
    Pokemon * array[count];
    recorrido = firstMap(PokeStorage);
    count = 0;
    while(recorrido != NULL){
        count++;
        array[i] = recorrido;
        i++;
        recorrido = nextMap(PokeStorage);
    }
    for(i = 0; i<count-1;i++){
        for(int j = 0; j < count-1; j++){
            if(array[j]->PS < array[j+1]->PS){
                Pokemon * aux = array[j+1];
                array[j+1] = array[j];
                array[j] = aux;

            }
        }
    }
    for(i = 0; i<count-1; i++){
        printf("-----------------------------------\n");
        printf("%s | PS: %d | PC: %d | Genero: %s\n",array[i]->name, array[i]->PS,array[i]->PC, array[i]->gender);
        printf("-----------------------------------\n");
    }
}

void liberarPokemon (HashMap * PokeStorage, HashMap * PokedexID, HashMap * PokedexName){
    int id;
    Pokemon * recorrido = firstMap(PokeStorage);
    PokeID * auxID;
    Pokemon * auxPoke;
    int exist;
    char * auxname = (char *) malloc(sizeof(char));
    char * auxkey = (char *) malloc(sizeof(char));
    printf("Ingresa la ID del Pokemon que quieres borrar: \n");
    scanf("%d", &id);
    while(recorrido != NULL){
        if(recorrido->id == id){
            strcpy(auxname, recorrido->name);
            strdup(auxname);
            printf("Borrando ---%s---\n");
            eraseMap(PokeStorage, auxname);
            auxID = searchMap(PokedexName, auxname);
            exist = auxID->exist;
            exist--;
            auxID->exist = exist;
            itoa(auxID->numeroPokedex, auxkey, 10);
            auxID = searchMap(PokedexID, auxkey);
            auxID->exist = exist;
            printf("%s liberado\n",auxname);
            return;

        }
        recorrido = nextMap(PokeStorage);
    }

}
