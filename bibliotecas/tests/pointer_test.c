#include <stdio.h>

void pointer_fct(float *num, char data[], float *num_vec){
  data[0] = 'd';
  data[1] =  'e';
  data[2] = 'f';
  num_vec[0] = 4;
  num_vec[1] = 5;
  num_vec[2] = 6;
  *num = 2;
}

int main(){
  int i=0;
  float num = 1;
  char data[] = {'a','b','c'};
  float num_vec[] = {1, 2, 3};

  for(i=0;i<3;i++){
      printf("%c\n", data[i]);
  }

  for(i=0;i<3;i++){
      printf("%f\n", num_vec[i]);
  }

  printf("%f\n", num);

   pointer_fct(&num, data, num_vec);

   for(i=0;i<3;i++){
       printf("%c\n", data[i]);
   }

   for(i=0;i<3;i++){
       printf("%f\n", num_vec[i]);
   }

   printf("%f", num);

  return 0;
}
