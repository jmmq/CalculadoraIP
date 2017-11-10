#ifndef REDES_H
#define REDES_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct mapa_s{
    char array[32][3][24];
    int size;
}mapa_t;

mapa_t *mapa;

void print_ip(unsigned char ip[4]) {
    unsigned char i;
    for(i=0;i<4;i++) {
        if(i<3)
            printf("%i.",ip[i]);
        else
            printf("%i",ip[i]);
    }
}

void set_ip(unsigned char ip[4],unsigned char a,unsigned char b,unsigned char c,unsigned char d) {
    ip[0]=a;
    ip[1]=b;
    ip[2]=c;
    ip[3]=d;
}

unsigned char get_clase(unsigned char ip[4]) {
    if(ip[0]>=0&&ip[0]<=127) {
        return 'A';
    }
    else if(ip[0]>=128&&ip[0]<=191) {
        return 'B';
    }
    else if(ip[0]>=192&&ip[0]<=223) {
        return 'C';
    }
    else if(ip[0]>=224&&ip[0]<=239) {
        return 'D';
    }
    else if(ip[0]>=249&&ip[0]<=255) {
        return 'E';
    }
    else return '\0';
}

void get_dir_red(unsigned char dir_red[4], unsigned char ip[4],unsigned char msr[4]) {
    set_ip (
        dir_red,
        ip[0]&msr[0],
        ip[1]&msr[1],
        ip[2]&msr[2],
        ip[3]&msr[3]
    );
}

void get_dir_broad(unsigned char dir_broad[4], unsigned char ip[4],unsigned char msr[4]) {
    set_ip (
        dir_broad,
        ip[0]|(~msr[0]),
        ip[1]|(~msr[1]),
        ip[2]|(~msr[2]),
        ip[3]|(~msr[3])
    );
}

void get_rango_hosts(unsigned char rango_hosts[2][4], unsigned char dir_red[], unsigned char dir_broad[]) {
    set_ip(rango_hosts[0],dir_red[0],dir_red[1],dir_red[2],dir_red[3]+1);
    set_ip(rango_hosts[1],dir_broad[0],dir_broad[1],dir_broad[2],dir_broad[3]-1);
}

unsigned char get_tipo(unsigned char ip[4],unsigned char dir_red[4], unsigned char dir_broad[4]) {

    if (
        ip[0]==dir_red[0] &&
        ip[1]==dir_red[1] &&
        ip[2]==dir_red[2] &&
        ip[3]==dir_red[3]
    ) {
        return 'r';
    }
    else if (
        ip[0]==dir_broad[0] &&
        ip[1]==dir_broad[1] &&
        ip[2]==dir_broad[2] &&
        ip[3]==dir_broad[3]
    ) {
        return 'b';
    }
    else return 'h';
}

void print_tipo(unsigned char tipo) {
    if(tipo=='r')
        printf("\nTipo: direccion de red");
    else if(tipo=='b')
        printf("\nTipo: direccion de broadcast");
    else if(tipo=='h')
        printf("\nTipo: direccion de host");
}

void ip_error() {
    printf("Error: Ingreso una IP invalida");
}


void fill_string(char string[64]) {

}

void update_mapa(mapa_t *mapa,const char* nombre, const char* tipo, size_t tam) {
    strcpy(mapa->array[mapa->size][0],nombre);
    strcpy(mapa->array[mapa->size][1],tipo);
    char tam_s[24];
    sprintf(tam_s,"%i",tam);
    strcpy(mapa->array[mapa->size][2],tam_s);
    mapa->size++;
}

unsigned char input(char string[], size_t len) {
    fgets(string, len, stdin);
    if(strlen(string)==(len-1)&&string[len-2]!='\n') {
        int c;
        while ((c = fgetc(stdin)) != '\n' && c != EOF) {
            //printf("\n\nc: decimal = %d, character = %c",c,c);
        }
        return 1;
    }
    return 0;
}

void print_mapa(mapa_t *mapa) {
    printf("\n %-20s %-20s %-20s","Name","Type","Bytes");
    int i;
    for(i=0;i<mapa->size;i++)
    {
        printf("\n %-20s %-20s %-20s",mapa->array[i][0],mapa->array[i][1],mapa->array[i][2]);
    }
}

void msr_error() {
    printf("MSR invalida");
}

unsigned char check_espacios(unsigned char msr[4]) {
    unsigned char found_0=0;
    unsigned char i;
    unsigned char s;
    for(i=0;i<4;i++){
        for(s=128;s>0;s>>=1){
            if(found_0){
                if(msr[i]&s){
                    return 1;
                }
            }
            else {
                if( (msr[i]&s) == 0){
                    found_0=1;
                }
            }
        }
    }
    return 0;
}

unsigned char get_unos(unsigned char msr[4]) {
    unsigned char unos=0;
    unsigned char i;
    unsigned char s;
    for(i=0;i<4;i++)
    {
        for(s=128;s>0;s>>=1) {
            if(msr[i]&s)
                unos++;
        }
    }
    return unos;
}

unsigned char validar_msr(unsigned char msr[4], unsigned char clase) {
    if(check_espacios(msr))
        return 1;
    if(get_unos(msr)>30)
        return 1;
    else if(clase=='A') {
        if(msr[0]==255) {
            return 0;
        }
    }
    else if(clase=='B') {
        if(msr[0]==255&&msr[1]==255)
            return 0;
    }
    else if(clase=='C') {
        if(msr[0]==255&&msr[1]==255&&msr[2]==255)
            return 0;
    }
    return 1;
}

void get_msr(unsigned char msr[4], unsigned char clase) {
    while(1) {
        printf("\nIngresa la MR o la MSR: ");
        char cadena[17];
        if(input(cadena,sizeof(cadena))){
            msr_error();
            continue;
        }
        int unchecked[4];
        int ret = sscanf(cadena,"%d.%d.%d.%d",&unchecked[0],&unchecked[1],&unchecked[2],&unchecked[3]);
        if(ret!=4) {
            msr_error();
            continue;
        }
        unsigned char i;
        for(i=0;i<4;i++) {
            if(unchecked[i]>255||unchecked[i]<0) {
                msr_error();
                continue;
            }
        }
        for(i=0;i<4;i++) {
            msr[i] = (unsigned char)unchecked[i];
        }
        if(validar_msr(msr,clase)) {
            msr_error();
            continue;

        }
        printf("MSR validada: ");
        print_ip(msr);
        return;

    }

}

void print_info(unsigned char tipo, unsigned char dir_red[4], unsigned char dir_broad[4], unsigned char rango_hosts[2][4]) {
    if(tipo=='r') {
        printf("\nDireccion de broadcast: ");
        print_ip(dir_broad);
        printf("\nRango de hosts: ");
        print_ip(rango_hosts[0]);
        printf(" - ");
        print_ip(rango_hosts[1]);
    }
    if(tipo=='h') {
        printf("\nDireccion de SR: ");
        print_ip(dir_red);
        printf("\nDireccion de broadcast: ");
        print_ip(dir_broad);
    }
    if(tipo=='b') {
        printf("\nDireccion de SR: ");
        print_ip(dir_red);
        printf("\nRango de hosts: ");
        print_ip(rango_hosts[0]);
        printf(" - ");
        print_ip(rango_hosts[1]);
    }
}


void print_bits(unsigned char clase, unsigned char msr[4]) {
    unsigned char bits_uno;
    unsigned char bits_red;
    unsigned char bits_prestados;
    unsigned char bits_host;
    int num_subredes;
    int num_host_sr;
    bits_uno=get_unos(msr);
    //printf("\nUnos = %i",bits_uno);
    if(clase=='A') {
        bits_red=8;
    }
    if(clase=='B') {
        bits_red=16;
    }
    if(clase=='C') {
        bits_red=24;
    }
    bits_prestados=bits_uno-bits_red;
    bits_host=32-bits_red-bits_prestados;
    num_subredes=pow(2,bits_prestados);
    num_host_sr=pow(2,bits_host);
    printf("\nBits de red: %d",bits_red);
    printf("\nBits prestados: %d",bits_prestados);
    printf("\nNumero de Subredes: %d",num_subredes);
    printf("\nBits de host: %d",bits_host);
    printf("\nNumero de Host/SR: %d - 2",num_host_sr);
}

void get_ip(unsigned char ip[4]) {
    while(1)
    {
        inicio:
        printf("\nIngresa la dir IP: ");
        char cadena[17];
        if(input(cadena,sizeof(cadena))){
            ip_error();
            continue;
        }
        int unchecked[4];
        int ret = sscanf(cadena,"%d.%d.%d.%d",&unchecked[0],&unchecked[1],&unchecked[2],&unchecked[3]);
        if(ret!=4) {
            ip_error();
            continue;
        }
        unsigned char i;
        for(i=0;i<4;i++) {
            if(unchecked[i]>255||unchecked[i]<0) {
                ip_error();
                goto inicio;
            }
        }
        for(i=0;i<4;i++) {
            ip[i] = (unsigned char)unchecked[i];
        }
        printf("IP validada: ");
        print_ip(ip);
        return;
    }
}


#endif // REDES_H
