#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "Map.h"


char * _strdup (const char *s)
{
    size_t len = strlen (s) + 1;
    void *new = malloc (len);

    if (new == NULL)
        return NULL;

    return (char *) memcpy (new, s, len);
}

const char *get_csv_field (char * tmp, int i)
{
    //se crea una copia del string tmp
    char * line = _strdup (tmp);
    const char * tok;
    for (tok = strtok (line, ","); tok && *tok; tok = strtok (NULL, ",\n"))
    {
        if (!--i)
        {
            return tok;
        }
    }
    return NULL;
}
long long stringHash(const void * key)
{
    long long hash = 5381;

    const char * ptr;

    for (ptr = key; *ptr != '\0'; ptr++)
    {
        hash = ((hash << 5) + hash) + tolower(*ptr); /* hash * 33 + c */
    }

    return hash;
}

int stringEqual(const void * key1, const void * key2)
{
    const char * A = key1;
    const char * B = key2;

    return strcmp(A, B) == 0;
}


typedef struct Contenedor
{

    char * id;

    char * estado;

    char * nave;

    char * destino;

    char * valor_carga;//hacerle casting al leer del csv!!!!!!!!!!

} Contenedor;

Contenedor * crearContenedor(char * linea)
{
    Contenedor* contenedorcito =(Contenedor*)malloc(sizeof(Contenedor));
    contenedorcito->id = (char*)get_csv_field(linea,1);
    contenedorcito->estado =   (char*)get_csv_field(linea, 2);
    contenedorcito->nave =   (char*)get_csv_field(linea, 3);
    contenedorcito ->destino = (char*)get_csv_field(linea, 4);
    contenedorcito->valor_carga = (char*)get_csv_field(linea, 5);//hacerle casting!!!!!
    return contenedorcito;
}

typedef struct Nave
{

    char * Nombre;

    char * ValordeCarga;

    char * Estado;

    char * Destino;


} Nave;

Nave * crearNave(char * linea)
{
    Nave* navecita =(Nave*)malloc(sizeof(Nave));
    navecita->Nombre = (char*)get_csv_field(linea,1);
    navecita->ValordeCarga =   (char*)get_csv_field(linea, 2);
    navecita->Estado =   (char*)get_csv_field(linea, 3);
    navecita->Destino = (char*)get_csv_field(linea, 4);
    return navecita;
}

typedef struct Dock
{
    char* estado;
    char*  nombre_nave;
} Dock;

typedef struct Plataforma
{

    int *  max_valor_carga;

    int * valor_carga_actual;

    Dock * puerto_uno;

    Dock * puerto_dos;

    Dock * puerto_tres;
    //agregarle un nro de contenedores para despacho
    //cantidad de contenedores segun carga

} Plataforma;
Plataforma * crearPlataforma(){
    Plataforma * plataforma = (Plataforma*)malloc(sizeof(Plataforma));
    Dock * Dock_uno = (Dock*)malloc(sizeof(Dock));
    Dock * Dock_dos = (Dock*)malloc(sizeof(Dock));
    Dock * Dock_tres = (Dock*)malloc(sizeof(Dock));
    Dock_uno->estado = false;
    Dock_dos->estado = false;
    Dock_tres->estado = false;
    plataforma->puerto_uno = Dock_uno;
    plataforma->puerto_dos = Dock_dos;
    plataforma->puerto_tres = Dock_tres;

    return plataforma;

}


int main()
{
    Map * mapa_contenedores = createMap(stringHash, stringEqual);
    Map * mapa_naves = createMap(stringHash,stringEqual);
    Map * mapa_estado= createMap(stringHash,stringEqual);
    Map * mapa_aux = createMap(stringHash, stringEqual);
    Map * mapa_control_nave = createMap(stringHash, stringEqual);
    FILE * fp=NULL;
    char linea[100];
    Contenedor* contenedor;
    Nave* nave;
    Plataforma * plataforma = crearPlataforma();
    int mascontenedores;
    int opcion=0;

    do{
        opcion =0;
        //Sistema iniciado, AVISO: se han cargado los archivos correspondientes a naves y containers.
        printf("\n\n=============ContainerIndexerPRO====================\n" );
        printf("                                                     ||\n");
        printf("Elija una de las siguientes opciones                 ||\n");
        printf("                                                     ||\n");
        printf("    1 Importar contenedores de un archivo CSV        ||\n");

        printf("    2 Importar naves desde un archivo CSV            ||\n");

        printf("    3 Agregar contenedor                             ||\n");

        printf("    4 Agregar nave                                   ||\n");

        printf("    5 Buscar contenedor por criterios()              ||\n");

        printf("    6 Atracar/Desatracar Nave                        ||\n");

        printf("    7 Embarcar/Desembarcar Nave                      ||\n");

        printf("    8 Despachar contenedores                         ||\n");

        printf("    9 Estado Actual de la plataforma                 ||\n");

        printf("    0 Salir                                          ||\n");
        printf("                                                     ||\n");
        printf("======================================================\n" );

        scanf("%d",&opcion);

     if (opcion == 1 ){

        fp = fopen("Containers.csv", "r");
        if (fp== NULL){
            printf("NO SE PUDO CARGAR EL ARCHIVO");
        }
        while( fgets(linea, 99, fp)!=NULL ){

          contenedor = crearContenedor(linea);

        //Buscar en el mapa contenedores si no se encuentra se agrega
            if(searchMap(mapa_contenedores, contenedor->id)==NULL){
                insertMap(mapa_contenedores,contenedor->id,contenedor);
            }

             //Buscar en el mapa naves si no se encuentra se agrega
            Map * mapa_aux_nave = createMap(stringHash,stringEqual);
            if(searchMap(mapa_naves, contenedor->nave)==NULL){  // Se busca el contenedor en el mapa naves
                insertMap(mapa_aux_nave,contenedor->id,contenedor);
                insertMap(mapa_naves,contenedor->nave, mapa_aux_nave);
            }else{
                mapa_aux_nave = searchMap(mapa_naves,contenedor->nave);
                insertMap(mapa_aux_nave,contenedor->id,contenedor);
            }
            //Buscar en el  mapa album si no se encuentra se agrega
            Map * mapa_aux_estado = createMap(stringHash, stringEqual);
            if (searchMap(mapa_estado,contenedor->estado)== NULL){
                insertMap(mapa_aux_estado,contenedor->id, contenedor);
                insertMap(mapa_estado,contenedor->estado, mapa_aux_estado);
            }else{
                mapa_aux_estado =searchMap(mapa_estado,contenedor->estado);
                insertMap(mapa_aux_estado,contenedor->id,contenedor);
            }


        }
         printf("\n\n\nSE INGRESO CORRECTAMENTE EL LISTADO DE CONTENEDORES\n");
        fclose(fp);
    }
     if (opcion == 2 ){

        fp = fopen("Naves.csv", "r");
        if (fp== NULL){
            printf("NO SE PUDO CARGAR EL ARCHIVO");
        }
        while( fgets(linea, 99, fp)!=NULL ){

          nave = crearNave(linea);

        //Buscar en el mapa naves si no se encuentra se agrega
            if(searchMap(mapa_control_nave, nave->Nombre)==NULL){
                insertMap(mapa_control_nave,nave->Nombre,nave);
            }
        }
         printf("\n\n\nSE INGRESO CORRECTAMENTE EL LISTADO DE NAVES\n");
        fclose(fp);
    }

    if (opcion == 3){
        printf("INGRESE LA INFORMACION DEL CONTENEDOR\n");
        printf("ID,Estado,Nave,Destino,Valor de Carga\n");
        printf("Recuerde separar con una ','  ejemplo : 99,Desembarque,TITANIC,Chile,5\n");
        scanf("%s", linea);
        contenedor = crearContenedor(linea);
        if (searchMap(mapa_contenedores,contenedor->id) ==NULL){
             insertMap(mapa_contenedores,contenedor->id,contenedor);

             //Buscar en el mapa artistas si no se encuentra se agrega
             Map * mapa_aux_nave = createMap(stringHash, stringEqual);
            if(searchMap(mapa_naves, contenedor->id)==NULL){  // Se busca la cancion en el mapa artistas
                insertMap(mapa_aux_nave,contenedor->id,contenedor);
                insertMap(mapa_naves,contenedor->nave, mapa_aux_nave);
            }else{
                    mapa_aux_nave = searchMap(mapa_naves,contenedor->nave);
                    insertMap(mapa_aux_nave,contenedor->id,contenedor);
            }
            //Buscar en el  mapa album si no se encuentra se agrega
            Map * mapa_aux_estado = createMap(stringHash,stringEqual);
            if (searchMap(mapa_estado,contenedor->estado)== NULL){
                insertMap(mapa_aux_estado,contenedor->id, contenedor);
                insertMap(mapa_estado,contenedor->estado, mapa_aux_estado);
            }else{
                mapa_aux_estado =searchMap(mapa_estado,contenedor->estado);
                insertMap(mapa_aux_estado,contenedor->id,contenedor);
            }
        }
        }

    if (opcion == 4){
         char nombre_nave[100];
         contenedor = NULL;
         printf("\nIngrese el nombre el nombre de la nueva nave \n");
         getchar();
         scanf("%90[^\n]s", nombre_nave);
         getchar();

         if (searchMap(mapa_naves,nombre_nave)== NULL){
             printf("\nLa Nave no se encuentra se agregará al Sistema\n");

             printf("\n ¿Ingresar contenedores a la Nave? SI = 1 NO = 0\n");
             scanf("%d", &mascontenedores);
             getchar();
             while (mascontenedores==1){
                     printf("Ingrese la siguiente informacion:\n");
                     printf("ID de Contenedor\n");

                     scanf("%90[^\n]s",contenedor->id);
                     getchar();
                     printf("%s",contenedor->id);
                     getchar();
                     mapa_aux=searchMap(mapa_contenedores,contenedor->id);
                     if(mapa_aux==NULL){
                         printf("estado del contenedor\n");
                         getchar();
                         scanf("%90[^\n]s", contenedor->estado);

                         contenedor->nave =nombre_nave;

                         printf("Destino del contenedor\n");
                         getchar();
                         scanf("%90[^\n]s", contenedor->destino);
                         getchar();

                         printf("Valor de Carga\n");
                         getchar();
                         scanf("%90[^\n]s", contenedor->valor_carga);
                         getchar();

                         insertMap(mapa_contenedores,contenedor->id,contenedor);
                         Map * mapa_aux_nave = createMap(stringHash, stringEqual);
                         insertMap(mapa_aux_nave,contenedor->id,contenedor);
                         insertMap(mapa_naves,contenedor->nave, mapa_aux_nave);
                         Map * mapa_aux_estado = createMap(stringHash, stringEqual);
                         insertMap(mapa_aux_estado,contenedor->id, contenedor);
                         insertMap(mapa_estado,contenedor->estado, mapa_aux_estado);

                     }else{
                         printf("Contendor ya existe, No puede ser ingresado\n");
                     }

                     printf("\n ¿Desea agragar otro contenedor? SI = 1 NO = 0\n");
                     scanf("%d", &mascontenedores);
             }
         }else{
             printf("\nLa nave ya existe \n");
       }
    }
    if (opcion == 5){
        printf("INGRESE EL NUMERO DE LA OPCION A BUSCAR \n");
        printf("0) MOSTRAR TODOS LOS CONTENEDORES INGRESADOS\n");
        printf("1) BUSCAR UN CONTENEDOR POR ID\n");
        printf("2) BUSCAR CONTENEDORES DE UNA NAVE \n");
        printf("3) BUSCAR CONTENEDORES SEGUN ESTADO\n");
        int opcionb;
        scanf("%d", &opcionb);

        if (opcionb == 0){//mostrar todos los contenedores
            printf("\n++++++++++++++++LISTADO DE CONTENEDORES EN EL SITEMA+++++++++++++++++++\n\n");
                contenedor = firstMap(mapa_contenedores);
                while (contenedor != NULL){
                    printf("\nCONTENEDOR-> ");
                    printf("ID: %s ", contenedor->id);
                    printf("ESTADO: %s ", contenedor->estado);
                    printf("DESTINO: %s ", contenedor->destino);
                    printf("NAVE: %s ", contenedor->nave);
                    printf("VALORdeCARGA: %s\n ", contenedor->valor_carga);
                    contenedor = nextMap(mapa_contenedores);
                }
            }

        if (opcionb == 1){//buscar contenedor por ID
             char id[30];
             printf("INGRESE LA IDENTIFICACION DEL CONTENEDOR BUSCADO\n");
             scanf("%s", id);
             contenedor = (searchMap(mapa_contenedores,id));

                  if (contenedor == NULL){
                        printf("NO SE ENCUENTRA EL CONTENEDOR BUSCADO");
                  }else{
                        printf(" SE ENCONTRO EL CONTENEDOR-> ");
                        printf("ID: %s ", contenedor->id);
                        printf("ESTADO: %s ", contenedor->estado);
                        printf("DESTINO: %s ", contenedor->destino);
                        printf("NAVE: %s ", contenedor->nave);
                        printf("VALORdeCARGA: %s \n", contenedor->valor_carga);
                  }


        }
          if (opcionb == 2){//Buscar contenedores de una nave
            char nave[30];
            getchar();
            printf("INGRESE EL NOMBRE DE LA NAVE\n");
            scanf("%90[^\n]s", nave);
            if (searchMap(mapa_naves, nave)== NULL){
                printf("La Nave no se encuentra registrada en el sistema %s\n", nave);
            }else{
                mapa_aux = searchMap(mapa_naves,nave);
                contenedor = firstMap(mapa_aux);
                printf("contenedores de la nave %s : \n", contenedor->nave);
                while (contenedor != NULL){
                    printf("\nCONTENEDOR-> ");
                    printf("ID: %s ", contenedor->id);
                    printf("ESTADO: %s ", contenedor->estado);
                    printf("DESTINO: %s ", contenedor->destino);
                    printf("NAVE: %s ", contenedor->nave);
                    printf("VALORdeCARGA: %s\n ", contenedor->valor_carga);
                    contenedor = nextMap(mapa_aux);
                }
            }
          }


        if (opcionb == 3){
            int opcionc;
            printf("INGRESE EL NUMERO DE LA OPCION CORRESPONDIENTE AL ESTADO:\n1) Desembarque \n2) Embarque (Plataforma) \n3) Destino \n4) Despachado \n");
            getchar();
            scanf("%d",&opcionc);
            if (opcionc==1){
                if (searchMap(mapa_estado,"Desembarque") == NULL){
                    printf("\nNO SE ENCUENTRA NINGUN CONTENEDOR CON ESE ESTADO \n");
                }else{
                    mapa_aux = searchMap(mapa_estado,"Desembarque");
                    contenedor = firstMap(mapa_aux);
                    printf("Contenedores en estado de Desembarque : \n");
                    while (contenedor != NULL){
                        printf("\nCONTENEDOR-> ");
                        printf("ID: %s ", contenedor->id);
                        printf("ESTADO: %s ", contenedor->estado);
                        printf("DESTINO: %s ", contenedor->destino);
                        printf("NAVE: %s ", contenedor->nave);
                        printf("VALORdeCARGA: %s\n ", contenedor->valor_carga);
                        contenedor= nextMap(mapa_aux);
                    }
                }

            }if (opcionc==2){
                if (searchMap(mapa_estado,"Embarque") == NULL){
                    printf("\nNO SE ENCUENTRA NINGUN CONTENEDOR CON ESE ESTADO \n");
                }else{
                    mapa_aux = searchMap(mapa_estado,"Embarque");
                    contenedor = firstMap(mapa_aux);
                    printf("Contenedores en estado de Embarque : \n");
                    while (contenedor != NULL){
                        printf("\nCONTENEDOR-> ");
                        printf("ID: %s ", contenedor->id);
                        printf("ESTADO: %s ", contenedor->estado);
                        printf("DESTINO: %s ", contenedor->destino);
                        printf("NAVE: %s ", contenedor->nave);
                        printf("VALORdeCARGA: %s\n ", contenedor->valor_carga);
                        contenedor= nextMap(mapa_aux);
                    }
                }

            }if (opcionc==3){
                if (searchMap(mapa_estado,"Destino") == NULL){
                    printf("\nNO SE ENCUENTRA NINGUN CONTENEDOR CON ESE ESTADO \n");
                }else{
                    mapa_aux = searchMap(mapa_estado,"Destino");
                    contenedor = firstMap(mapa_aux);
                    printf("Contenedores en estado de Destino : \n");
                    while (contenedor != NULL){
                        printf("\nCONTENEDOR-> ");
                        printf("ID: %s ", contenedor->id);
                        printf("ESTADO: %s ", contenedor->estado);
                        printf("DESTINO: %s ", contenedor->destino);
                        printf("NAVE: %s ", contenedor->nave);
                        printf("VALORdeCARGA: %s\n ", contenedor->valor_carga);
                        contenedor= nextMap(mapa_aux);
                    }
                }

            }if (opcionc==4){
                if (searchMap(mapa_estado,"Despachado") == NULL){
                    printf("\nNO SE ENCUENTRA NINGUN CONTENEDOR CON ESE ESTADO \n");
                }else{
                    mapa_aux = searchMap(mapa_estado,"Despachado");
                    contenedor = firstMap(mapa_aux);
                    printf("Contenedores con estado: Despachado  \n");
                    while (contenedor != NULL){
                            printf("exito");
                        printf("\nCONTENEDOR-> ");
                        printf("ID: %s ", contenedor->id);
                        printf("ESTADO: %s ", contenedor->estado);
                        printf("DESTINO: %s ", contenedor->destino);
                        printf("NAVE: %s ", contenedor->nave);
                        printf("VALORdeCARGA: %s\n ", contenedor->valor_carga);
                        contenedor= nextMap(mapa_aux);
                    }
                }

            }
         }
        }
        if(opcion==6){
            int opcionc=0;
            printf("INGRESE EL NUMERO DE LA OPCION CORRESPONDIENTE:\n1) Mostrar Listado de Naves \n2) Atracar Nave \n3) Desatracar Nave\n");
            getchar();
            scanf("%d",&opcionc);
            if (opcionc==1){
                    nave = firstMap(mapa_control_nave);
                    printf("Listado de Naves que es posible atracar: \n");
                    while (nave != NULL){
                        if (strcmp(nave->Estado, "Espera")==0){
                                printf("\nNAVE-> ");
                                printf("NOMBRE: %s ", nave->Nombre);
                                printf("ESTADO: %s ", nave->Estado);
                                printf("VALORdeCARGA: %s\n ", nave->ValordeCarga);
                        }

                        nave= nextMap(mapa_control_nave);
                    }
            }
            if(opcionc==2){
                if(plataforma->puerto_uno->estado==false || plataforma->puerto_dos->estado==false ||plataforma->puerto_tres->estado==false){
                    char nombre_nave[100];
                    printf("Ingrese el nombre  de la Nave a Atracar\n");
                    getchar();
                    scanf("%90[^\n]s", nombre_nave);
                    getchar();
                    nave = searchMap(mapa_control_nave,nombre_nave);
                    if (nave==NULL || strcmp(nave->Estado,"Atracada")==0){
                        printf("No es posible atracar Nave, no existe o ya esta atracada\n");
                    }else{
                    printf("Ingrese el numero del Puerto en donde atracar la nave:\n1)Puerto_uno   2)Puerto_dos   3)Puerto_tres\n");
                    int op;
                    scanf("%d", &op);
                    if (op == 1 && plataforma->puerto_uno->estado == false){
                        plataforma->puerto_uno->estado = true;
                        strcpy(plataforma->puerto_uno->nombre_nave, nombre_nave);
                        strcpy(nave->Estado,"Atracada");
                        printf("\nNave Atracada con exito!\n");
                    }else
                    if (op == 2 && plataforma->puerto_dos->estado == false){
                        plataforma->puerto_dos->estado = true;
                        strcpy(plataforma->puerto_dos->nombre_nave, nombre_nave);
                        strcpy(nave->Estado,"Atracada");
                         printf("\nNave Atracada con exito!\n");
                    }else
                    if (op == 3 && plataforma->puerto_tres->estado == false){
                        plataforma->puerto_tres->estado = true;
                        strcpy(plataforma->puerto_tres->nombre_nave, nombre_nave);
                        strcpy(nave->Estado,"Atracada");
                         printf("\nNave Atracada con exito!\n");
                    }else{printf("\nNo ha sido posible atracar la nave, puerto ocupado!\n");}
                }
                }else{printf("\nNo hay DOCKS Disponibles\n");}
            }
            if (opcionc==3){
                if(plataforma->puerto_uno->estado==false && plataforma->puerto_dos->estado==false && plataforma->puerto_tres->estado==false){
                        printf("\nNo Hay Naves Atracadas!\n");
                }else{
                    printf("Seleccione numero de Nave a Desatracar\n\n");

                    if (plataforma->puerto_uno->estado==true){
                        printf("1) Puerto_uno: Nave-> %s\n",plataforma->puerto_uno->nombre_nave);
                    }
                    if (plataforma->puerto_dos->estado==true){
                        printf("2) Puerto_dos: Nave-> %s\n",plataforma->puerto_dos->nombre_nave);
                    }
                    if (plataforma->puerto_tres->estado==true){
                        printf("3) Puerto_tres: Nave-> %s\n",plataforma->puerto_tres->nombre_nave);
                    }
                    int opciond;
                    scanf("%d",&opciond);

                    if (opciond==1 && plataforma->puerto_uno->estado==true){
                        nave = searchMap(mapa_control_nave,plataforma->puerto_uno->nombre_nave);
                        if(strcmp(nave->Estado,"Despachada")==0){
                            plataforma->puerto_uno->estado = false;
                            printf("\nNave Desatracada con exito\n");
                        }else printf("\nNo se puede desatracar, la Nave no ha cumplido su itinerario\n");
                    }else
                    if(opciond==2 && plataforma->puerto_dos->estado==true){
                        nave = searchMap(mapa_control_nave,plataforma->puerto_uno->nombre_nave);
                        if(strcmp(nave->Estado,"Despachada")==0){
                            plataforma->puerto_dos->estado = false;
                            printf("\nNave Desatracada con exito\n");
                        }

                    }else
                    if(opciond==3 && plataforma->puerto_tres->estado==true){
                        nave = searchMap(mapa_control_nave,plataforma->puerto_uno->nombre_nave);
                        if(strcmp(nave->Estado,"Despachada")==0){
                            plataforma->puerto_tres->estado = false;
                            printf("\nNave Desatracada con exito\n");
                        }

                    }else{printf("\nOpcion NO VALIDA\n");}
                }
            }
        }
        if(opcion==7){
            if(plataforma->puerto_uno->estado==true || plataforma->puerto_dos->estado==true ||plataforma->puerto_tres->estado==true){
                if(plataforma->puerto_uno->estado==true){

                    nave = searchMap(mapa_control_nave,plataforma->puerto_uno->nombre_nave);
                    //Desembarque
                    if (strcmp(nave->Destino,"Chile")==0 && strcmp(nave->Estado,"Atracada")==0){
                        printf("Puerto uno: NAVE -> %s \n",plataforma->puerto_uno->nombre_nave);
                        mapa_aux = searchMap(mapa_estado,"Desembarque");
                        contenedor = firstMap(mapa_aux);

                        while (contenedor != NULL){
                            if (strcmp(contenedor->nave,nave->Nombre)==0){
                                strcpy(contenedor->estado,"Destino");

                                Map * mapa_aux_estado = createMap(stringHash, stringEqual);
                                if (searchMap(mapa_estado,contenedor->estado)== NULL){
                                    insertMap(mapa_aux_estado,contenedor->id, contenedor);
                                    insertMap(mapa_estado,contenedor->estado, mapa_aux_estado);
                                }else{
                                    mapa_aux_estado =searchMap(mapa_estado,contenedor->estado);
                                    insertMap(mapa_aux_estado,contenedor->id,contenedor);
                                }
                            }
                            eraseKeyMap(mapa_aux,contenedor->id);
                            contenedor= nextMap(mapa_aux);
                        }
                        strcpy(nave->Estado,"Despachada");
                    //Embarque
                    }else if(strcmp(nave->Destino,"Chile")!=0 && strcmp(nave->Estado,"Atracada")==0){
                        mapa_aux = searchMap(mapa_estado,"Embarque");
                        contenedor = firstMap(mapa_aux);
                        while (contenedor != NULL){
                            if (strcmp(contenedor->destino,nave->Nombre)==0){
                                strcpy(contenedor->nave,nave->Nombre);
                                strcpy(contenedor->destino,nave->Destino);
                            }
                            contenedor= nextMap(mapa_aux);
                        }
                    }
                }
                if(plataforma->puerto_dos->estado==true){
                    nave = searchMap(mapa_control_nave,plataforma->puerto_dos->nombre_nave);
                    //Desembarque
                    if (strcmp(nave->Destino,"Chile")==0 && strcmp(nave->Estado,"Atracada")==0){
                        printf("Puerto dos: NAVE -> %s \n",plataforma->puerto_uno->nombre_nave);
                        mapa_aux = searchMap(mapa_estado,"Desembarque");
                        contenedor = firstMap(mapa_aux);

                        while (contenedor != NULL){
                            if (strcmp(contenedor->nave,nave->Nombre)==0){
                                strcpy(contenedor->estado,"Destino");

                                Map * mapa_aux_estado = createMap(stringHash, stringEqual);
                                if (searchMap(mapa_estado,contenedor->estado)== NULL){
                                    insertMap(mapa_aux_estado,contenedor->id, contenedor);
                                    insertMap(mapa_estado,contenedor->estado, mapa_aux_estado);
                                }else{
                                    mapa_aux_estado =searchMap(mapa_estado,contenedor->estado);
                                    insertMap(mapa_aux_estado,contenedor->id,contenedor);
                                }
                            }
                            eraseKeyMap(mapa_aux,contenedor->id);
                            contenedor= nextMap(mapa_aux);
                        }
                        strcpy(nave->Estado,"Despachada");

                    }else if(strcmp(nave->Destino,"Chile")!=0 && strcmp(nave->Estado,"Atracada")==0){
                        mapa_aux = searchMap(mapa_estado,"Embarque");
                        contenedor = firstMap(mapa_aux);
                        while (contenedor != NULL){
                            if (strcmp(contenedor->destino,nave->Nombre)==0){
                                strcpy(contenedor->nave,nave->Nombre);
                                strcpy(contenedor->destino,nave->Destino);
                            }
                            contenedor= nextMap(mapa_aux);
                        }
                    }
                }if(plataforma->puerto_tres->estado==true){
                    nave = searchMap(mapa_control_nave,plataforma->puerto_tres->nombre_nave);
                    //Desembarque
                    if (strcmp(nave->Destino,"Chile")==0 && strcmp(nave->Estado,"Atracada")==0){
                        printf("Puerto tres: NAVE -> %s \n",plataforma->puerto_uno->nombre_nave);
                        mapa_aux = searchMap(mapa_estado,"Desembarque");
                        contenedor = firstMap(mapa_aux);

                        while (contenedor != NULL){
                            if (strcmp(contenedor->nave,nave->Nombre)==0){
                                strcpy(contenedor->estado,"Destino");

                                Map * mapa_aux_estado = createMap(stringHash, stringEqual);
                                if (searchMap(mapa_estado,contenedor->estado)== NULL){
                                    insertMap(mapa_aux_estado,contenedor->id, contenedor);
                                    insertMap(mapa_estado,contenedor->estado, mapa_aux_estado);
                                }else{
                                    mapa_aux_estado =searchMap(mapa_estado,contenedor->estado);
                                    insertMap(mapa_aux_estado,contenedor->id,contenedor);
                                }
                            }
                            eraseKeyMap(mapa_aux,contenedor->id);
                            contenedor= nextMap(mapa_aux);
                        }
                        strcpy(nave->Estado,"Despachada");

                    }else if(strcmp(nave->Destino,"Chile")!=0 && strcmp(nave->Estado,"Atracada")==0){
                        mapa_aux = searchMap(mapa_estado,"Embarque");
                        contenedor = firstMap(mapa_aux);
                        while (contenedor != NULL){
                            if (strcmp(contenedor->destino,nave->Nombre)==0){
                                strcpy(contenedor->nave,nave->Nombre);
                                strcpy(contenedor->destino,nave->Destino);
                            }
                            contenedor= nextMap(mapa_aux);
                        }
                    }
                }
                printf("Operaciones realizadas con exito\n");
            }else{printf("\nNo hay Naves Atracadas disponibles para hacer operaciones\n");}
        }if(opcion==8){
            mapa_aux = searchMap(mapa_estado,"Destino");
            contenedor = firstMap(mapa_aux);
                        while (contenedor != NULL){
                                strcpy(contenedor->estado,"Despachado");

                                Map * mapa_aux_estado = createMap(stringHash, stringEqual);
                                if (searchMap(mapa_estado,contenedor->estado)== NULL){
                                    insertMap(mapa_aux_estado,contenedor->id, contenedor);
                                    insertMap(mapa_estado,contenedor->estado, mapa_aux_estado);
                                }else{
                                    mapa_aux_estado =searchMap(mapa_estado,contenedor->estado);
                                    insertMap(mapa_aux_estado,contenedor->id,contenedor);
                                }
                            eraseKeyMap(mapa_aux,contenedor->id);
                            contenedor= nextMap(mapa_aux);
                            }
                printf("\nTODOS LOS CONTENDERES EN DESTINO HAN SIDO DESPACHADOS\n");

        }




    }while(opcion!=0);
    return 0;
}
