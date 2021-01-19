#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

long menu() {
    
    long seleccion = 0;
    
    //Menu
    printf("\nMENU\n");
    printf("1. Crear llaves\n");
    printf("2. Encriptar\n");
    printf("3. Desencriptar\n");
    printf("4. Salir\n");
    
    //Ingresar opcion
    printf("\nOpcion: ");
    scanf("%ld", &seleccion);
    
    return seleccion;
}

//Checar si el numero es primo o no
bool checar_primo(long numero){
    
    if(numero == 0 || numero == 1){
        return false;
    }
    
    for (long i=2; i< numero; i++) {
        if(numero % i == 0 && i != numero){
            return false;
        }
    }
    
    return true;

}

//Calcular el valor de n = p x q
long sacar_N(long p, long q){
    return p * q;
}

//Calcular el valor de PHI = (p-1) x (q-1)
long sacar_PHI(long p, long q){
    return (p-1)*(q-1);
}

long maximo_comun_divisor(long numero1, long numero2){
   
    long temporal;
    
    while (numero2 > 0) {
        temporal = numero1 % numero2;
        numero1 = numero2;
        numero2 = temporal;
    }
    return numero1;
}

//Checar si el primo es un primo relativo
bool checar_primo_relativo(long e, long phi){
    
    //e y phi deben de ser e(1<e<phi(n))
    if(maximo_comun_divisor(e, phi) == 1 && e > 1 && e < phi){
        return true;
    } else{
        return false;
    }
}

long modulo_inverso(long u, long v){
    
    long inv, u1, u3, v1, v3, t1, t3, q;
    long iter;

    u1 = 1;
    u3 = u;
    v1 = 0;
    v3 = v;
    
    iter = 1;
    
    
    while (v3 != 0)
    {
        
        q = u3 / v3;
        t3 = u3 % v3;
        t1 = u1 + q * v1;
       
        u1 = v1; v1 = t1; u3 = v3; v3 = t3;
        iter = -iter;
    }

    if (u3 != 1)
        return 0;
    
    if (iter < 0)
        inv = v - u1;
    else
        inv = u1;
    return inv;
}

//Algoritmo de exponenciacion modular
long AEM(long p, long e, long n){
 
  long r2 = 1;
  long r1 = 0;
  long Q = 0;
  long R = 0;
 
  while( e != 0 ){
     R = (e % 2);
     Q = ((e - R) / 2);
 
     r1 = ((p * p) % n);
 
       if(R == 1){
          r2 = ((r2 * p) % n);
       }
     p = r1;
     e = Q;
  }
return r2;
}

void encriptar(long e, long n){
    
    FILE *texto, *cifrado;
    
    cifrado = fopen("cifrado.txt", "w+");
    fclose(cifrado);
    
    
    texto = fopen("texto.txt", "r+");
    if (texto == NULL) {
        printf("Error al abrir el archivo de texto");
        menu();
    }
    
    cifrado = fopen("cifrado.txt", "w+");
    if (cifrado == NULL) {
        printf("Error al abrir el archivo destino");
        menu();
    }
    char ch;
    long valor;
    
    while (1) {
        ch = getc(texto);
        if (ch == -1){
            break;
        }
        valor = AEM(ch, e, n);
        fprintf(cifrado, "%ld ", valor);
        
    }
    fclose(texto);
    fclose(cifrado);
}

void desencriptar(long d, long n){
    
    FILE *cifrado, *descifrado;
    
    descifrado = fopen("descifrado.txt", "w+");
    fclose(descifrado);
    
    
    cifrado = fopen("cifrado.txt", "r");
    if(cifrado == NULL){
        printf("Error al abrir el archivo cifrado");
        menu();
    }
    
    descifrado = fopen("descifrado.txt", "w+");
    if (descifrado == NULL) {
        printf("Error al abrir el archivo destino");
        menu();
    }

    long valor;
  
    while (1) {
        if (fscanf(cifrado, "%ld", &valor) == -1){
            break;
        }else{
            valor = AEM(valor, d, n);
            fprintf(descifrado, "%c", valor);
        }
    }
    
    fclose(descifrado);
    fclose(cifrado);
    
}


int main(){
    
    //Encabezado
    printf("\n");
    printf("* CIFRADO RSA *\n");
    
    long seleccion = 0;
    long p = 0;  //Numero primo1
    long q = 0;  //Numero primo2
    long n = 0;  //n = p x q
    long e = 0;  //Primo relativo
    long d = 0;  // d = (1/e) mod phi
    long phi = 0; //phi = (p-1)*(q-1)
        
    bool entrada_valida;
    
    while (seleccion != 4) {
        
        seleccion = menu();
        
        if(seleccion == 1){
            
            do{
                printf("\n Teclea 'p': ");
                scanf("%ld", &p);
                
                if(checar_primo(p) == false){
                    entrada_valida = false;
                    printf("El numero ingresado no es un numero primo. longenta de nuevo\n");
                } else{
                    entrada_valida = true;
                }
            } while (entrada_valida == false);
            
            do{
                printf("\n Teclea 'q': ");
                scanf("%ld", &q);
                
                if (checar_primo(q) == false) {
                    entrada_valida = false;
                    printf("El numero ingresado no es un numero primo. longenta de nuevo\n");
                } else{
                    entrada_valida = true;
                }
            } while(entrada_valida == false);
            
            n = sacar_N(p, q);
            phi = sacar_PHI(p, q);
            
            
            do{
                printf("\nTeclea 'e' Primo relativo: ");
                scanf("%ld", &e);
    
                if (checar_primo_relativo(e, phi) == false) {
                    entrada_valida = false;
                    printf("El valor de 'e' no es valido. longenta de nuevo\n");
                } else{
                    entrada_valida = true;
                }
            } while(entrada_valida == false);
            
            d = modulo_inverso(e, phi);
            
            
            printf("\n*************************************\n");
            printf(" Llave publica para encriptar\n");
            printf("KU=(%ld, %ld)\n", e, n);
    
            printf("\n");

            printf(" Llave privada para desencriptar\n");
            printf("KR=(%ld, %ld)\n", d, n);
            printf("\n*************************************\n");
            
        } else if (seleccion == 2){
            
            printf("\nEscribe tu texto a cifrar dentro del archivo texto.txt\n");
            printf("Teclea 'e': ");
            scanf("%ld", &e);
            printf("Teclea 'n': ");
            scanf("%ld", &n);
            
            double time_spent, end, begin = clock();
            
            encriptar(e,n);
            
            end = clock();
            
            time_spent = (double)((end-begin)/CLOCKS_PER_SEC);
    
            printf("\nENCRIPTADO EXITOSO\n");
            
            FILE *peso;
            peso=fopen("texto.txt","r");
            fseek(peso, 0L, SEEK_END);
           
            printf("\n*************************************\n");
            printf("RSA tardó %f segundos en encriptar un archivo de %lo bytes\n", time_spent, ftell(peso));
            printf("*************************************\n");
            fclose(peso);
            
            
            
            
        } else if(seleccion == 3){
            printf("El texto a descifrar debe estar dentro de cifrado.txt\n");
            printf("Teclea 'd': ");
            scanf("%ld", &d);
            printf("Teclea 'n': ");
            scanf("%ld", &n);
            
            double time_spent, end, begin = clock();
            
            desencriptar(d,n);
                    
            end = clock();
                    
            time_spent = (double)((end-begin)/CLOCKS_PER_SEC);
            
            printf("\nDESENCRIPTADO EXITOSO\n");
             
            FILE *peso;
            peso=fopen("cifrado.txt","r");
            fseek(peso, 0L, SEEK_END);
            
            printf("\n*************************************\n");
            printf("RSA tardó %f segundos en desencriptar un archivo de %lo bytes\n", time_spent, ftell(peso));
            printf("*************************************\n");
            fclose(peso);
        }else if (seleccion == 4){
            break;
        }
    }
}
