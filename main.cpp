#include <stdlib.h>
#include <stdio.h>
#include <math.h>

void header(FILE* file){
  fprintf(file, "G21\n");
  fprintf(file, "G90\n");
  fprintf(file, "G28\n");
  fprintf(file, "M104 S190\n");
  fprintf(file, "M140 S50\n");
  fprintf(file, "M190 S50 \n");
  fprintf(file, "M109 S190\n");
  fprintf(file, "G92 E0\n");
  fprintf(file, "G1 E10 F1200\n");
  fprintf(file, "G92 E0\n");
  fprintf(file, "M106 S204\n");
 
}

float compute_e(float length, float tau = 0.2,float nw = 0.4, float d = 1.75){

return(tau * nw * length /(M_PI * (d*d)/4));
}

void footer(FILE* file, float z){
    fprintf(file, "G1 X0.0 Y200.0 Z%.2f F1200\n", (z+20.0)); 
}

int cubeN(){
  FILE* file = NULL;
  file = fopen("cubeN.gcode", "w+");
  if (file == NULL){
    // On affiche un message d'erreur si on veut
    printf("Impossible d'ouvrir le fichier test1.txt");
    return 1;
  }
  header(file);
  float X = 100.0;
  float Y = 100.0;
  float Z = 0.2;
  int test = 0;
  float E = 0;
  fprintf(file, "G1 X%.2f Y%.2f Z%.2f F1200\n", X,Y,Z);
  
  for(float j = 0; j<25;j++){
    Z += 0.2;
    X = 100.0;
    Y = 100.0;
    test = 0;
    for(float i =0; i<25;i++){
      X += 0.2;
      fprintf(file, "G1 X%.2f Y%.2f Z%.2f E%.2f\n", X,Y,Z,E);
      test+=1;
      if(((int)test)%2==0)
	{
	  Y-=5.0;
	}
      
      else{
	Y+=5.0;
      }
      E+=0.2;
      fprintf(file, "G1 X%.2f Y%.2f Z%.2f E%.2f \n", X,Y,Z,E);
    }
  }
   footer(file,Z);
  fclose(file);
  return 0;
}

int cubeY(){
  FILE* file = NULL;
  file = fopen("cubeY.gcode", "w+");
  if (file == NULL){
    // On affiche un message d'erreur si on veut
    printf("Impossible d'ouvrir le fichier test1.txt");
    return 1;
  }
  header(file);
  float X = 100.0;
  float Y = 100.0;
  float Z = 0.2;
  int test = 0;
  int testZ = 0;
  float E = 0;
  fprintf(file, "G1 X%.2f Y%.2f Z%.2f F1200\n", X,Y,Z);
  
  for(float j = 0; j<25;j++){
    Z += 0.2;
    X = 100.0;
    Y = 100.0;
    test = 0;
    testZ +=1;
    if(testZ%2==0){
      for(float i =0; i<25;i++){
	X += 0.2;
	fprintf(file, "G1 X%.2f Y%.2f Z%.2f E%.2f\n", X,Y,Z,E);
	test+=1;
	if(((int)test)%2==0)
	  {
	    Y-=5.0;
	  }
      
	else{
	  Y+=5.0;
	}
	E+=0.2;
	fprintf(file, "G1 X%.2f Y%.2f Z%.2f E%.2f \n", X,Y,Z,E);
      }
    }
    else{
      for(float i =0; i<25;i++){
	Y += 0.2;
	fprintf(file, "G1 X%.2f Y%.2f Z%.2f E%.2f\n", X,Y,Z,E);
	test+=1;
	if(((int)test)%2==0)
	  {
	    X-=5.0;
	  }
      
	else{
	  X+=5.0;
	}
	E+=0.2;
	fprintf(file, "G1 X%.2f Y%.2f Z%.2f E%.2f \n", X,Y,Z,E);
      }
    }
    
  }
   footer(file,Z);
  fclose(file);
  return 0;
}

int cylinderVide(){
  FILE* file = NULL;
  file = fopen("cylinderV.gcode", "w+");
  if (file == NULL){
    // On affiche un message d'erreur si on veut
    printf("Impossible d'ouvrir le fichier test1.txt");
    return 1;
  }
  header(file);
  float X = 100.0;
  float Y = 100.0;
  float Z = 0.2;
  float E = 0;
  fprintf(file, "G1 X%.2f Y%.2f Z%.2f F1200\n", X,Y,Z);

  int xcenter=100, ycenter=100, radius=4;

  X = xcenter + radius*cos(0);
  Y = ycenter + radius*sin(0);
  fprintf(file, "G1 X%.2f Y%.2f Z%.2f E%.2f F1200\n", X,Y,Z,E);

  for(int i = 0; i<25; i++){
    Z+=0.2;
    X = xcenter + radius*cos(0);
    Y = ycenter + radius*sin(0);
    fprintf(file, "G1 X%.2f Y%.2f Z%.2f F1200\n", X,Y,Z);
    for(double theta=0.0; theta<6.27; theta+=1.0/radius) {
      X = xcenter + radius*cos(theta);
      Y = ycenter + radius*sin(theta);
      E+=0.05;
      fprintf(file, "G1 X%.2f Y%.2f Z%.2f E%.2f F1200\n", X,Y,Z,E);
    }
  }
   footer(file,Z);
  fclose(file);
  return 0;
}

int cylinderPlein(){
  FILE* file = NULL;
  file = fopen("cylinderP.gcode", "w+");
  if (file == NULL){
    // On affiche un message d'erreur si on veut
    printf("Impossible d'ouvrir le fichier test1.txt");
    return 1;
  }
  header(file);
  float X = 100.0;
  float Y = 100.0;
  float Z = 0.2;
  float E = 0;
  fprintf(file, "G1 X%.2f Y%.2f Z%.2f F1200\n", X,Y,Z);
  int test = -1;
  int xcenter=100, ycenter=100, radius=4;

  X = xcenter + radius*cos(0);
  Y = ycenter + radius*sin(0);
  //fprintf(file, "G1 X%.2f Y%.2f Z%.2f E%.2f F1200\n", X,Y,Z,E);

  for(int i = 0; i<25; i++){
    Z+=0.2;
   
    test +=1;
    //inside to outside
    if(test%2==0){
      X=xcenter;
      Y=ycenter;
      fprintf(file, "G1 X%.2f Y%.2f Z%.2f F1200\n", X,Y,Z);
      for(float radius = 0.2; radius<4; radius+=0.2){
	X = xcenter + radius*cos(0);
	Y = ycenter + radius*sin(0);
	fprintf(file, "G1 X%.2f Y%.2f Z%.2f F1200\n", X,Y,Z);
		
	float X_a = X;
	float Y_a = Y;
	float norm = 0;
	for(double theta=0.0; theta<6.27; theta+=1.0/radius) {
	  X = xcenter + radius*cos(theta);
	  Y = ycenter + radius*sin(theta);
	  //E+=0.05;
	  norm = sqrt((X - X_a)*(X - X_a) + (Y - Y_a)*(Y - Y_a));
	  E += (compute_e(norm));
	  X_a = X;
		Y_a = Y;
	  fprintf(file, "G1 X%.2f Y%.2f Z%.2f E%.2f F1200\n", X,Y,Z,E);
	}
      }
    }
    //outsid to inside
    else{
      X = xcenter + radius*cos(0);
      Y = ycenter + radius*sin(0);
      fprintf(file, "G1 X%.2f Y%.2f Z%.2f F1200\n", X,Y,Z);
      for(float radius = 4; radius>0; radius-=0.2){
	X = xcenter + radius*cos(0);
	Y = ycenter + radius*sin(0);
	fprintf(file, "G1 X%.2f Y%.2f Z%.2f F1200\n", X,Y,Z);
  float X_a = X;
	float Y_a = Y;
	float norm = 0;
	for(double theta=0.0; theta<6.27; theta+=1.0/radius) {
	  X = xcenter + radius*cos(theta);
	  Y = ycenter + radius*sin(theta);
	  //E+=0.05;
		norm = sqrt((X - X_a)*(X - X_a) + (Y - Y_a)*(Y - Y_a));
	  E += (compute_e(norm));
	  X_a = X;
		Y_a = Y;
	  fprintf(file, "G1 X%.2f Y%.2f Z%.2f E%.2f F1200\n", X,Y,Z,E);
	}
      }
    }
  }
  footer(file,Z);
  fclose(file);
  return 0;
}




int main( int argc, char** argv )
{
  cubeN();
  cubeY();
  cylinderVide();
  return cylinderPlein();
}
