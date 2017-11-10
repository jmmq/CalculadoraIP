//identificar clase ip, y tipo de direccion(), indicar direccion de red, rango de hosts, dir de broadcast
#include"redes.h"

int main() {
    mapa = (mapa_t *)malloc(sizeof(mapa_t));
    memset(mapa,0, sizeof(mapa_t));
    unsigned char ip[4];
    update_mapa(mapa,"ip","unsigned char[4]",sizeof(ip));
    unsigned char msr[4];
    update_mapa(mapa,"msr","unsigned char[4]",sizeof(msr));
    unsigned char dir_red[4];
    update_mapa(mapa,"dir_red","unsigned char[4]",sizeof(dir_red));
    unsigned char dir_broad[4];
    update_mapa(mapa,"dir_broad","unsigned char[4]",sizeof(dir_broad));
    unsigned char rango_hosts[2][4];
    update_mapa(mapa,"rango_hosts","unsigned char[2][4]",sizeof(rango_hosts));
    unsigned char clase;
    update_mapa(mapa,"clase","unsigned char",sizeof(clase));
    unsigned char tipo;
    update_mapa(mapa,"tipo","unsigned char",sizeof(tipo));
    while(1) {
        get_ip(ip);
        clase = get_clase(ip);
        printf("\nClase: %c",clase);
        if(clase=='A'||clase=='B'||clase=='C') {
            get_msr(msr,clase);
            /*printf("\nMascara de red: ");
            print_ip(msr);*/
            print_bits(clase,msr);
            get_dir_red(dir_red,ip,msr);
            get_dir_broad(dir_broad,ip,msr);
            get_rango_hosts(rango_hosts,dir_red,dir_broad);
            tipo = get_tipo(ip,dir_red,dir_broad);
            print_tipo(tipo);
            print_info(tipo,dir_red,dir_broad,rango_hosts);
            printf("\n\nMapa de memoria:");
            print_mapa(mapa);
        }

        printf("\n________________________________\n");
    }

    return EXIT_SUCCESS;
}


