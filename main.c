
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <pthread.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#include <sys/time.h>
#include <sys/stat.h>
#include <assert.h>
#include <pthread.h>


#define TEMPERATURA 25
#define HUMIDADE 75
#define LUMINOSIDADE 300

pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;

int  volt(int volthex){
    int x;
    return x = (volthex/4096)*1.5;
}

int  temp(int temphex){
    int x;
    return x = temphex*0.01 - 39.6;
}

int  hum(int humhex){
    int x;
    return x = humhex*0.0367 - 2.0468 - 1.5955*pow(10,(-5))*pow(humhex,2);
}

int  lum(int lumhex){
    int x;
    return x = 0.625*pow(10,6)*(lumhex/4096)*(1.5/100000)*1000;
}

void * sensor_prnt_file (char* strc, FILE* file_ptr){
//    printf("enter senr_prnt_file\n");
    int counter=0;
    
    while(counter < 20){
        fgets(strc, 70, stdin);
        printf("enter senr_prnt_file\n");
        pthread_mutex_lock(&m);
        file_ptr = fopen("text.txt", "w+");
        fprintf(file_ptr, "%s\n", strc);
        printf("%s\n", strc);
        fclose(file_ptr);
        pthread_mutex_unlock(&m);
    }
    pthread_exit(NULL);
}

void * comand_read_file(FILE * file_ptr, FILE* strdata){
    
    printf("enter command_read_file\n\n");
    int i = 0;
    
    char* strrd = (char*) malloc (sizeof(char*)*(70));
    
    char* strid = (char*) malloc (sizeof(char*)*(6));
    
    char* strdat = (char*) malloc (sizeof(char*)*(30));
    
    int strdatt;
    
    do{
        printf("boas\n");
        
        pthread_mutex_lock(&m);
        file_ptr=fopen("text.txt", "a+");
        i = fscanf(file_ptr, "%s", strrd);
        fclose(file_ptr);
        strncpy(strid, strrd + 15 , 5);
        
        strdatt =(int) strtol(strid, NULL, 16);
        
        switch(strdatt){
            case 1:
                strncpy(strdat, strrd + 30, 30);
                fprintf(strdata, " id 1: %s\n", strdat);
                pthread_mutex_unlock(&m);
                break;
            case 2:
                strncpy(strdat, strrd + 30, 30);
                fprintf(strdata, " id 2: %s\n", strdat);
                pthread_mutex_unlock(&m);
                break;
            case 3:
                strncpy(strdat, strrd + 30, 30);
                fprintf(strdata, " id 2: %s\n", strdat);
                pthread_mutex_unlock(&m);
                break;
            default:
                pthread_mutex_unlock(&m);
                break;
        
        }
    }while(i!=EOF);
    //pthread_mutex_unlock(&m);
    return NULL;
}

void * comand_data_conv_file(FILE* origin, FILE* dest1, FILE* dest2, FILE* dest3, FILE* dest1_prog, FILE* dest2_prog, FILE* dest3_prog){
    int i = 0;
    
    int voltagem, temperatura, humidade, luminosidade;
    
    char* strrd = (char*) malloc (sizeof(char*)*(69));
    
    char* strid = (char*) malloc (sizeof(char*)*(6));
    
    char* strdat = (char*) malloc (sizeof(char*)*(30));
    
    printf("enter comand_data_conv_file\n\n");

    while(i!=EOF){
        if(origin == NULL) break;
        pthread_mutex_lock(&m);
        i = fscanf(origin, "%s", strrd);
        strncpy(strid, strrd, 5);
        switch((int)strtol(strid, NULL, 16)){
            case 1:
                voltagem     = volt((int)strtol(strncpy(strdat, strrd, 5), NULL, 16));
                temperatura  = temp((int)strtol(strncpy(strdat, strrd + 17, 5), NULL, 16));
                humidade     = hum ((int)strtol(strncpy(strdat, strrd + 23, 5), NULL, 16));
                luminosidade = lum ((int)strtol(strncpy(strdat, strrd + 5, 5), NULL, 16));
                fprintf(dest1, "bateria: %d volts \t temperatura :%d ºC\t humidade:%d \t luminosidade:%d\n ", voltagem, temperatura, humidade, luminosidade);
                fprintf(dest1_prog, "%d %d %d %d\n ", voltagem, temperatura, humidade, luminosidade);
                pthread_mutex_unlock(&m);
                break;
            case 2:
                voltagem     = volt((int)strtol(strncpy(strdat, strrd, 5), NULL, 16));
                temperatura  = temp((int)strtol(strncpy(strdat, strrd + 17, 5), NULL, 16));
                humidade     = hum ((int)strtol(strncpy(strdat, strrd + 23, 5), NULL, 16));
                luminosidade = lum ((int)strtol(strncpy(strdat, strrd + 5, 5), NULL, 16));
                fprintf(dest2, "bateria: %d volts \t temperatura :%d ºC\t humidade:%d \t luminosidade:%d\n ", voltagem, temperatura, humidade, luminosidade);
                fprintf(dest2_prog, "%d %d %d %d\n ", voltagem, temperatura, humidade, luminosidade);
                pthread_mutex_unlock(&m);
                break;
            case 3:
                voltagem     = volt((int)strtol(strncpy(strdat, strrd, 5), NULL, 16));
                temperatura  = temp((int)strtol(strncpy(strdat, strrd + 17, 5), NULL, 16));
                humidade     = hum ((int)strtol(strncpy(strdat, strrd + 23, 5), NULL, 16));
                luminosidade = lum ((int)strtol(strncpy(strdat, strrd + 5, 5), NULL, 16));
                fprintf(dest3, "bateria: %d volts \t temperatura :%d ºC\t humidade:%d \t luminosidade:%d\n ", voltagem, temperatura, humidade, luminosidade);
                fprintf(dest3_prog, "%d %d %d %d\n ", voltagem, temperatura, humidade, luminosidade);
                pthread_mutex_unlock(&m);
                break;
            default:
                pthread_mutex_unlock(&m);
                break;
        }
    }
    return NULL;
}

void * user_prnt_file(FILE * file_user){
    
    return NULL;
}

void * comparar_sensor_user(FILE * user, FILE * compar1, FILE * compar2, FILE * compar3, FILE * atuador){
    
    return NULL;
}

void * prnt_diagram(FILE* diag){
    int i=0;
    
    printf("enter print_diag\n\n");
    while (i<=10){
        if (i==0){
            fprintf(diag, " |-------------------|              \n");
            printf(" |-------------------|              \n");
        }
        else if (((i>=1)&&(i<=5))||((i==9))) {
            fprintf(diag, " |            |      |           \n");
            printf(" |            |      |           \n");
        }
        else if (i==6){
            fprintf(diag, " |--------/---|      |             \n");
            printf(" |--------/---|      |             \n");
        }
        else if (i==7) {
            fprintf(diag, " |                   /             \n" );
            printf(" |                   /             \n" );
        }
        else if (i==8){
            fprintf(diag, " |--------/---|      |         \n");
            printf(" |--------/---|      |         \n");
        }
        else if (i==10){
            fprintf(diag, " |-------------------|             \n");
            printf(" |-------------------|             \n");
        }
        else break;
        i++;
    }
    //printf("boas!!, nao sei correr funçoes\n");
    pthread_exit(NULL);
}
 
void * regular_condicoes(bool ac, bool janela, bool humi, bool lamp, FILE * div){
    
    int temp=0, humdad=0, lux=0;
    char *c=(char*)malloc(sizeof(char*)*12);
    char *aux=(char*)malloc(sizeof(char*)*5);
    
    printf("enter regular_comdiçoes\n\n");
    
    while((fseek(div, -12, SEEK_END)) == 0){
        
        if(div == NULL) break;
        fgets(c, 12, div);
        temp    = (int)strtol(strncpy(aux, c+3, 2), NULL, 10);
        humdad  = (int)strtol(strncpy(aux, c+6, 2), NULL, 10);
        lux     = (int)strtol(strncpy(aux, c+9, 2), NULL, 10);
        
        if(temp < TEMPERATURA){
            ac= 1;
            janela= 0;
        }
        if(temp > TEMPERATURA){
            ac = 0;
            janela = 1;
        }
        if(lux < LUMINOSIDADE){
            if((janela == 0)&&(ac == 0)) lamp= 1;
            else if(janela == 1) lamp = 0;
        }
        if( humdad > HUMIDADE) humi = 1;
        else if ( humdad < HUMIDADE) humi = 0;
    }
    return NULL;
}

int main(int argc, const char * argv[]) {
    
    char* strcom = (char*) malloc (sizeof(char*)*(70));
    int c;
    
    pthread_t * p1 = (pthread_t *) malloc (sizeof(pthread_t));
    pthread_t * p2 = (pthread_t *) malloc (sizeof(pthread_t));
    pthread_t * p3 = (pthread_t *) malloc (sizeof(pthread_t));
    pthread_t * p4 = (pthread_t *) malloc (sizeof(pthread_t));
    pthread_t * p5 = (pthread_t *) malloc (sizeof(pthread_t));
    pthread_t * p6 = (pthread_t *) malloc (sizeof(pthread_t));
    
    bool ac1 = false, janela1 = false, lampada1 = false, humi1 = false;
    bool ac2 = false, janela2 = false, lampada2 = false, humi2 = false;
    bool ac3 = false, janela3 = false, lampada3 = false, humi3 = false;
    
    pthread_t * quarto1  = (pthread_t *) malloc (sizeof(pthread_t));
    pthread_t * quarto2  = (pthread_t *) malloc (sizeof(pthread_t));
    pthread_t * corredor = (pthread_t *) malloc (sizeof(pthread_t));
    
    FILE* file_ptr;
    
    FILE* file_data    = fopen("data.txt", "w");
    
    FILE* file_compar1 = fopen("comparar1.txt", "w");
    
    FILE* file_compar2 = fopen("comparar2.txt", "w");
    
    FILE* file_compar3 = fopen("comparar3.txt", "w");
    
    FILE* file_user    = fopen("user.txt", "w");
    
    FILE* file_atuador = fopen("atuador.txt", "w");
    
    FILE* file_diag    = fopen("diagrama.txt", "w");
    
    FILE* file_compar1_prog = fopen("compar1_prog.txt", "w");
    
    FILE* file_compar2_prog = fopen("compar2_prog.txt", "w");
    
    FILE* file_compar3_prog = fopen("compar3_prog.txt", "w");
    
   
    c = pthread_create(p1, NULL, sensor_prnt_file(strcom, file_ptr), NULL);
    if(c != 0) exit(0);
   
    c = pthread_create(p2, NULL, comand_read_file(file_ptr, file_data), NULL);
    if(c != 0) exit(0);
    
    c = pthread_create(p3, NULL, comand_data_conv_file(file_data, file_compar1, file_compar2, file_compar3, file_compar1_prog, file_compar2_prog, file_compar3_prog), NULL);
    if(c != 0) exit(0);
    
    c = pthread_create(p4, NULL, user_prnt_file(file_user), NULL);
    if(c != 0) exit(0);
    
    c = pthread_create(p5, NULL, comparar_sensor_user(file_user, file_compar1, file_compar2, file_compar3, file_atuador) , NULL);
    if(c != 0) exit(0);
    
    printf("reg1\n");
    c = pthread_create(quarto1, NULL, regular_condicoes(ac1, janela1, humi1, lampada1, file_compar1_prog), NULL);
    printf("reg1\n");
    if(c != 0) exit(0);
    
    c = pthread_create(quarto2, NULL, regular_condicoes(ac2, janela2, humi2, lampada2, file_compar2_prog), NULL);
    printf("reg2\n");
    if(c != 0) exit(0);
    
    c = pthread_create(corredor, NULL, regular_condicoes(ac3, janela3, humi3, lampada3, file_compar3_prog), NULL);
    printf("reg3\n");
    if(c != 0) exit(0);
    
    c = pthread_create(p6, NULL, prnt_diagram(file_diag), NULL);
    if(c != 0) exit(0);
    
    
    /*
    fclose(file_ptr);
    fclose(file_data);
    fclose(file_user);
    fclose(file_atuador);
    fclose(file_compar1);
    fclose(file_compar2);
    fclose(file_compar3);
    fclose(file_compar1_prog);
    fclose(file_compar2_prog);
    fclose(file_compar3_prog);
    */
}
