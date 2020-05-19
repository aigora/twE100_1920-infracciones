//ALBERTO DIAZ BEJERANO (54560)
//ALVARO ESPINOSA RICO (54581)
//LUCAS DE SARRIA GUENANE (54554)
//MAR FERNANDEZ DE USERA (54596)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N 10

// estructuras
typedef struct {
    int dia;
    int mes;
    int anio;
}T_FECHA;

typedef struct {
    int id_radar;
    char matricula[N];
    int velocidad;
    float sancion;
    T_FECHA fecha;
}T_MULTA;

typedef struct {
    int id_radar;
    int velocidad_limite;
    int umbral20;
    int umbral40;
    int umbral_resto;
}T_RADAR;

typedef struct {
    char dni[N];
    float tasa;
    char nobel;
    float sancion;
}T_ALCOHOL;

// prototipos
void CargaRadaresManual(T_RADAR *radares, int num_radares);
void CargaMultasManual(T_MULTA *multas, int num_multas);
void RellenarUnRadar(T_RADAR *prad);
void RellenarUnaMulta(T_MULTA *pmul);
void RellenarFecha(T_FECHA *pfecha);
float CalculaMultas(T_MULTA *multas, int num_multas, T_RADAR *radares, int num_radares);
int BuscarIndiceRadar(int identificador_radar, T_RADAR *radares, int num_radares);
void CargaRadaresFichero(FILE *f_radares,T_RADAR *radares,int num_radares);
void CargaMultasFichero(FILE *f_multas,T_MULTA *multas,int num_radares);
void CalculaNumRadares(FILE *f_radares,int *num_radares);
void CalculaNumMultas(FILE *f_multas,int *num_multas);
void RellenarMultaAlcohol(T_ALCOHOL *palc);
void CargarMultaAlcohol(T_ALCOHOL *multas, int num_multas);
float CalcularMultaAlcohol(T_ALCOHOL *multas, int num_multas);
void pedirDni(T_ALCOHOL *palc);


int main() {

    T_RADAR *v_radares;
    T_MULTA *v_multas;
    T_ALCOHOL *v_alcohol;
    int op, op2;
    int n_multas;
    int n_radares;
    float multas_totales;
    FILE *pf_multas;
    FILE *pf_radares;

    //Te da a elegir entre dos programas, en caso de no elegir ni la primera ni la segunda vuelve a preguntar
    do {
        printf("Elige programa: Multas por velocidad(1) o multas por alcohol(2) \n");
        scanf(" %i", &op);
        if (op != 1 && op != 2) {
            printf("Error, el programa selecionado no existe\n\n");
        }
    } while (op != 1 && op != 2);

    switch (op) {
        case 1:
            printf("\nFuncion primer programa: multas por velocidad");
            
            do{
                printf("\nElige entre: introducir las multas manualmete(1) o introducir multas automaticamente(2)\n");
                scanf("%i", &op2);
                if (op2 != 1 && op2 != 2){
                    printf("Error, el programa selecionado no existe\n\n");
                }
            }while (op2 != 1 && op2 != 2);
            
            switch (op2){
                case 1:
                    printf("\nFuncion multas por radar -> carga manual");//puedes seleccionar rellenar el radar manualmente
                    //pide el numero de radares y se repite hasta que sea un numero positivo
                    do {
                        printf("\nIntroduce el numero de radares: ");
                        scanf(" %d", &n_radares);
                        if (n_radares <= 0) {
                            printf("\nDebe ser positivo");
                        }
                    } while (n_radares <= 0);

                    //pide el numero de multas y se repite hasta que el numero sea posivo
                    do {
                        printf("\nIntroduce el numero de multas: ");
                        scanf(" %d", &n_multas);
                        if (n_multas <= 0) {
                            printf("\nDebe ser positivo");
                        }
                    } while (n_multas <= 0);

                    //memoria dimanica para vector de radares y multas
                    v_radares = (T_RADAR *) calloc(n_radares, sizeof(T_RADAR));
                    v_multas = (T_MULTA *) calloc(n_multas, sizeof(T_MULTA));
                    if (v_radares == NULL || v_multas == NULL) {
                        printf("\nError guardando menoria dinamica");
                    } else {
                        CargaRadaresManual(v_radares, n_radares);
                        CargaMultasManual(v_multas, n_multas);
                        multas_totales = CalculaMultas(v_multas, n_multas, v_radares, n_radares);
                        printf("\nEl valor total de las sanciones impuestas es: %.2f euros", multas_totales);
                        free(v_radares);
                        free(v_multas);
                    }

                    break;
                    
                case 2:
                    printf("\nFuncion multas por radar -> carga automatica");//se cargan los radares automaticamente a traves de los ficheros
                    
                    pf_multas=fopen("multas.txt","r");
                    pf_radares=fopen("radares.txt","r");
                    if(pf_multas==NULL||pf_radares==NULL){
                        printf("\nError abriendo los archivos");
                        if(pf_multas==NULL){
                            fclose(pf_multas);
                        }
                        if(pf_radares==NULL){
                            fclose(pf_radares);
                        }
                    }
                    else{
                        CalculaNumRadares(pf_radares,&n_radares);
                        CalculaNumMultas(pf_multas,&n_multas);
                        v_radares=(T_RADAR *)calloc(n_radares,sizeof(T_RADAR));
                        v_multas=(T_MULTA *)calloc(n_multas,sizeof(T_MULTA));
                        if(v_radares==NULL||v_multas==NULL){
                            printf("\nError asignando memoria");
                            if(v_radares!=NULL){
                                free(v_radares);
                            }
                            if(v_multas!=NULL){
                                free(v_multas);
                            }
                        }
                        else{
                            CargaRadaresFichero(pf_radares,v_radares,n_radares);
                            CargaMultasFichero(pf_multas,v_multas,n_multas);
                            multas_totales=CalculaMultas(v_multas,n_multas,v_radares,n_radares);
                            printf("\nEl valor total de la sancion impuesta es: %.2f",multas_totales);
                            free(v_radares);
                            free(v_multas);
                        }
                        fclose(pf_multas);
                        fclose(pf_multas);
                    }
                    break;

                default:
                    printf("\nError");
            }
            return 0;

        case 2:
            printf("\nFuncion segundo programa: multas por alcoholemia");
            //Pide el numero de multas y se repite hasta que sea un numero positivo
            do {
                printf("\nIntroduce el numero de multas: ");
                scanf(" %d", &n_multas);
                if (n_multas <= 0) {
                    printf("\nDebe ser positivo");
                }
            } while (n_multas <= 0);
            //memoria dinamica para vetor multas
            v_alcohol = (T_ALCOHOL *) calloc(n_multas, sizeof(T_ALCOHOL));
            if (v_alcohol == NULL) {
                printf("\nError guardando menoria dinamica");
            } else {
                CargarMultaAlcohol(v_alcohol, n_multas); 
                printf("\nEl valor total de las sanciones impuestas es: %.2f euros", CalcularMultaAlcohol(v_alcohol, n_multas));

                free(v_alcohol);
                break;

                default:
                    printf("\nError");
            }
            return 0;
    }

}
//funcion para rellenar tantos radares como se ha introducido anteriormnete 
void CargaRadaresManual(T_RADAR *radares, int num_radares){
    int i;
    for(i=0; i<num_radares; i++){
        RellenarUnRadar(&radares[i]);
    }
    return;
}
// funcion para rellenar tantas multas como se ha introducido anteriormnete
void CargaMultasManual(T_MULTA *multas,int num_multas){
    int i;
    for(i=0; i<num_multas; i++){
        printf("\n MULTA %d", i+1);
        RellenarUnaMulta(&multas[i]);
    }
    return;
}

//funcion para rellenar todos los datos del radar
void RellenarUnRadar(T_RADAR *prad){
    printf("\nIntroduce el identificador del radar: ");
    scanf(" %d",&(prad->id_radar));
    printf("\nIntroduce la velocidad limite: ");
    scanf(" %d",&(prad->velocidad_limite));
    printf("\nIntroduce la cantidad en el umbral 0-20: ");
    scanf(" %d",&(prad->umbral20));
    printf("\nIntroduce la cantidad en el umbral de 20-40: ");
    scanf(" %d",&(prad->umbral40));
    printf("\nIntroduce la cantidad en el umbral >40: ");
    scanf(" %d",&(prad->umbral_resto));
    return;
}

//funcion para rellenar todos los datos que componen la multa
void RellenarUnaMulta(T_MULTA *pmul){
    T_FECHA fecha;
    RellenarFecha(&fecha);
    printf("\nIntroduce el numero identificador del radar: ");
    scanf(" %d", &(pmul->id_radar));
    printf("\nIntroduce la matricula: ");
    fflush(stdin);
    fgets(pmul->matricula,N,stdin);
    printf("\nIntroduce la velocidad: ");
    scanf(" %d", &(pmul->velocidad));
    return;
}
//funcion para buscar el identicador del radar
int BuscarIndiceRadar(int identificador_radar,T_RADAR*radares,int num_radares)
{
    int i;
    int indice;
    indice=-1;
    i=0;
    while(i<num_radares&&indice==-1){
        if(radares[i].id_radar==identificador_radar){
            indice=i;
        }
        i++;
    }
    return indice;
}

//funcion calcula multas
float CalculaMultas(T_MULTA *multas,int num_multas,T_RADAR *radares,int num_radares)
{
    int i;
    float res;
    int indice;
    int diferencia;
    float umbral;
    res=0;
    for(i=0;i<num_multas;i++){ //recorre todas la multas
        indice=BuscarIndiceRadar(multas[i].id_radar,radares,num_radares);
        if(indice==-1){
            printf("\nEl identificador %d no existe",multas[i].id_radar);
        }
        else{
            diferencia=multas[i].velocidad-radares[indice].velocidad_limite;
            if(diferencia>0){
                umbral=diferencia*100/(float)radares[indice].velocidad_limite;
                if(umbral>0&&umbral<=20){
                    multas[i].sancion=radares[indice].umbral20;
                }
                else{
                    if(umbral>20&&umbral<=40){
                        multas[i].sancion=radares[indice].umbral40;
                    }
                    else{
                        multas[i].sancion=radares[indice].umbral_resto;
                    }
                }
                res=res+multas[i].sancion;
                printf("\nLa sancion del vehiculo %s ha sido de %.f euros",multas[i].matricula,multas[i].sancion);
            }
        }
    }
    return res;
}


//funcion para rellenar la estructura de la fecha
void RellenarFecha(T_FECHA *pfecha){
    printf("\nIntroduce la fecha:");
    printf("\nDia: ");
    scanf(" %d", &(pfecha->dia));
    printf("\nMes: ");
    scanf(" %d", &(pfecha->mes));
    printf("\nAnio: ");
    scanf(" %d", &(pfecha->anio));
    return;
}

void CalculaNumRadares(FILE *f_radares,int *num_radares)
{
    int cont;
    int valor;
    T_RADAR r;
    cont=0;
    do{
        valor=fscanf(f_radares,"%d %d %d %d %d",&r.id_radar,&r.velocidad_limite,&r.umbral20,&r.umbral40,&r.umbral_resto);
        if(valor==5){
            cont++;
        }
    }while(valor==5);
    *num_radares=cont;
    return;
}
void CalculaNumMultas(FILE *f_multas,int *num_multas)
{
    int cont;
    int valor;
    T_MULTA m;
    cont=0;
    do{
        valor=fscanf(f_multas,"%d %d %d %d %s %d",&m.fecha.dia,&m.fecha.mes,&m.fecha.anio,&m.id_radar,m.matricula,&m.velocidad);
        if(valor==6){
            cont++;
        }
    }while(valor==6);
    *num_multas=cont;
    return;
} //funcion para cargar los radares con el fichero
void CargaRadaresFichero(FILE *f_radares,T_RADAR *radares,int num_radares)
{
    int i;
    rewind(f_radares);
    for(i=0;i<num_radares;i++){
        fscanf(f_radares,"%d %d %d %d %d",&(radares[i].id_radar),&(radares[i].velocidad_limite),&(radares[i].umbral20),&(radares[i].umbral40),&(radares[i].umbral_resto));
    }
    return;
} //funcion para cargar las multas con el fichero
void CargaMultasFichero(FILE *f_multas,T_MULTA *multas,int num_radares)
{
    int i;
    rewind(f_multas);
    for(i=0;i<num_radares;i++){
        fscanf(f_multas,"%d %d %d %d %s %d",&(multas[i].fecha.dia),&(multas[i].fecha.mes),&(multas[i].fecha.anio),&(multas[i].id_radar),(multas[i].matricula),&(multas[i].velocidad));
    }
    return;
}

//funcion para rellenar tantas multas como se ha introducido anteriormnete
void CargarMultaAlcohol(T_ALCOHOL *multas,int num_multas){
    int i;
    for(i=0; i<num_multas; i++){
        printf("\n MULTA %d", i+1);
        RellenarMultaAlcohol(&multas[i]);
    }
    return;
}

//funcion para rellenar los datos de la multa
void RellenarMultaAlcohol(T_ALCOHOL *palc){
    T_FECHA fecha;
    RellenarFecha(&fecha);
    pedirDni(palc);

    printf("\nIntroduce la tasa de alcohol del conductor: ");
    scanf(" %f", &(palc->tasa));
    //eliges entre nobel o no nobel y si no eliges correctamente te lo vuelve a preguntar
    do{
        printf("\n¿Es nobel el conductor?\n\ts (si la respuesta es SI)\n\tn (si la respuesta es NO)\n");
        scanf(" %c", &(palc->nobel));
        if (palc->nobel != 's' && palc->nobel != 'n') {
            printf("Error, seleccione correctamente la opcion\n\n");
        } 
    } while (palc->nobel != 's' && palc->nobel != 'n');
    
    return;
}
// Te calcula las multas segun si el conductor es nobel o no
float CalcularMultaAlcohol(T_ALCOHOL *multas,int num_multas){
    int i;
    float res = 0.0;

    for (i = 0; i < num_multas; ++i) {

        switch(multas[i].nobel){
            case 's':
                if(multas[i].tasa<0.15){
                    multas[i].sancion = 0;
                }else if(multas[i].tasa>0.15 && multas[i].tasa<=0.30){
                    multas[i].sancion = 500;
                }else if(multas[i].tasa>0.3 && multas[i].tasa<=0.6){
                    multas[i].sancion = 1000;
                }else if(multas[i].tasa>0.6){
                    multas[i].sancion = 6000;
                }
                break;

            case 'n':
                if(multas[i].tasa<0.25){
                    multas[i].sancion = 0;
                }else if(multas[i].tasa>0.25 && multas[i].tasa<=0.5){
                    multas[i].sancion = 500;
                }else if(multas[i].tasa>0.5 && multas[i].tasa<=0.6){
                    multas[i].sancion = 1000;
                }else if(multas[i].tasa>0.6){
                    multas[i].sancion = 6000;
                }
                break;

            default:
                printf("\nError");
        }
        res=res+multas[i].sancion;
        printf("\nLa sancion de la persona con DNI: %s ha sido de %.f euros",multas[i].dni, multas[i].sancion);

    }
    printf("\n");
    return res;
}
//funcion que comprueba que el dni sea correcto
void pedirDni(T_ALCOHOL *palc){ 
    int dniIncorrecto=1;
    do {
        printf("\nIntroduce el dni (Con letra mayuscula): ");
        fflush(stdin);
        fgets(palc->dni,N,stdin);
        
        if (strlen(palc->dni) != N-1) {
            printf("\nDni erroneo1\n");
        } else {
            char letras[]={'T','R','W','A','G','M','Y','F','P','D','X','B','N','J','Z','S','Q','V','H','L','C','K','E'};
            char letra;
            int numero;
            sscanf(palc->dni,"%d%c",&numero,&letra);
            if(letra==letras[numero%23])
            {
                printf("\nEl DNI es correcto\n");
                dniIncorrecto = 0;
            }
            else
            {
                printf("\nDni erroneo2\n");
            }
        }
        
    }while(dniIncorrecto);
} //Fin de programa
