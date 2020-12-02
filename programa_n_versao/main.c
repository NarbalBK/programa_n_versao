//
//  main.c
//  programa_n_versao
//

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int canal[6] = {-1,-1,-1,-1,-1,-1};

void send_async(int *buf, int c);
void receive(int *buf, int c);
void thread_driver(void *threadno);
void thread_code_a(void *thread);
void thread_code_b(void *thread);
void thread_code_c(void *thread);
int comparacao(int vetor_comp[], int *versao_erro);

int main(void) {
    pthread_t TA;
    pthread_t TB;
    pthread_t TC;
    pthread_t TD;

    pthread_create(&TA, NULL, (void*)thread_code_a, NULL);
    pthread_create(&TB, NULL, (void*)thread_code_b, NULL);
    pthread_create(&TC, NULL, (void*)thread_code_c, NULL);
    pthread_create(&TD, NULL, (void*)thread_driver, NULL);

    pthread_join(TA, NULL);
    pthread_join(TB, NULL);
    pthread_join(TC, NULL);
    pthread_join(TD, NULL);

    system("pause");
    return 0;
}

void send_async(int *buf, int c){
  canal[c] = *buf;
  return;
}

void receive(int *buf, int c){
  while (canal[c] == -1);
  *buf = canal[c];
  canal[c] = -1;
  return;
}

void thread_driver(void *threadno) {
    int vetor_comp[3];
    int statusok = 0;
    int statuserro = 1;
    int voto_maj;
    int versao_erro;

    receive(&vetor_comp[0], 0);
    receive(&vetor_comp[1], 1);
    receive(&vetor_comp[2], 2);

    voto_maj = comparacao(vetor_comp, &versao_erro);
    printf("Valor correto: %d \n", voto_maj);

    if(versao_erro == 0){
      send_async(&statusok, 3);
      send_async(&statusok, 4);
      send_async(&statusok, 5);
    }else if(versao_erro == 1){
      send_async(&statuserro, 3);
      send_async(&statusok, 4);
      send_async(&statusok, 5);
    }else if(versao_erro == 2){
      send_async(&statusok, 3);
      send_async(&statuserro, 4);
      send_async(&statusok, 5);
    }else if(versao_erro == 3){
      send_async(&statusok, 3);
      send_async(&statusok, 4);
      send_async(&statuserro, 5);
    }
}

int comparacao(int vetor_comp[], int *versao_erro){
  int erro = 0;
  int maj;
  if(vetor_comp[0] != vetor_comp[1]){
    if(vetor_comp[0] != vetor_comp[2]){
      erro = 1;
      maj = vetor_comp[2];
    }else{
      erro = 2;
      maj = vetor_comp[0];
    }
  }else{
    if(vetor_comp[0] != vetor_comp[2]){
      erro = 3;
      maj = vetor_comp[0];
    }else{
      maj = vetor_comp[0];
    }
  }
  *versao_erro = erro;
  return maj;
}

void thread_code_a(void *thread){
  int voto = 3-3; //TODO fazer um programa que retorna um valor e colocar aqui
  int status;
  send_async(&voto, 0);
  receive(&status, 3);
  if (status == 0){
    puts("TA PROSSEGUE");
    while(1);
  }else{
    puts("TA GEROU ERRO");
  }
}

void thread_code_b(void *thread){
  int voto = 3*2; //TODO fazer um programa que retorna um valor e colocar aqui
  int status;
  send_async(&voto, 1);
  receive(&status, 4);
  if (status == 0){
    puts("TB PROSSEGUE");
    while(1);
  }else{
    puts("TB GEROU ERRO");
  }
}

void thread_code_c(void *thread){
    int voto = 3+3; //TODO fazer um programa que retorna um valor e colocar aqui
  int status;
  send_async(&voto, 2);
  receive(&status, 5);
  if (status == 0){
    puts("TC PROSSEGUE");
    while(1);
  }else{
    puts("TC GEROU ERRO");
  }
}
