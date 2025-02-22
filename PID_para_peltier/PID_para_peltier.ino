#include <math.h> 

// constantes ajuste conforme necessario
const float TARGET_TEMP=273.15+34.5;//setada para 34.5 C mude conforme necessario
const float BETA  =3950.0; //beta do termistor
const float R_ZERO=10000.0; //Rzero do termistor
const float T_ZERO=298.15; // Tzero do thermistor
const float RESISTOR_REF=9890.0; // resistor de referencia

//ajuste essas constantes experimentalmente
const float KP=280.0; // constante de proporcionalidade
const float KD=0.00; // constante de derivada
const float KI=0.05; // constante de integral
//KP parece ok, KI 
//coisas de PID
long int previousTime=0;
float MAX_INTEGRAL=1000.0; //define o valor maximo de integral, mude caso necessario
float integral=0.0;
float previouserr=0.0;

//constantes de nivel de referencia
const int READ_LVL_REF=1023;
const int OUTPUT_LVL_REF=255;



void setup() {
 
  // definir a funcao de cada pino

 TCCR0B = (TCCR0B & 0b11111000) | 0x02; 
pinMode(6,OUTPUT); 
pinMode(7,OUTPUT);
pinMode(8,OUTPUT);

Serial.begin(9600);//iniciar o serial
}


//funcao para saber a resistencia do thermistor
float getResistance(int level, float refResistor){
  float res=(level*refResistor)/(READ_LVL_REF-level);
  return res;
  
  }

//funcao para pegar a temperatura de acordo com a resistencia do thermistor
 float getTemp(float thermistorRes, float Beta, float tzero, float rzero){
  float invT=(log(thermistorRes/rzero)/Beta)+1/tzero;
  float temperature=1/invT;
  return temperature;
  }

//funcao do PID
 int powerPID(float err, float kp,float ki,float kd,int els_time,float previous_error){
  
  float prop_response=kp*err;
  float der_response=kd*(err-previous_error)/(els_time/1000.0);
  integral=integral+(err+previous_error)*(els_time/2000.0);

 //nao deixa a integral ficar muito acima de um limite
  if (integral>MAX_INTEGRAL){integral=MAX_INTEGRAL;}
 if (integral<-MAX_INTEGRAL){integral=-MAX_INTEGRAL;}

  float integral_response=ki*integral;
  int power=(int)(prop_response+der_response+integral_response);
  if (abs(power)>OUTPUT_LVL_REF){
    power=power/abs(power)*OUTPUT_LVL_REF;}  
  return (int) power;
  }
  
void loop() {
  

  
  float actualThResistance=getResistance(analogRead(A0),RESISTOR_REF); //le do A0
  float actualTemp=getTemp(actualThResistance,BETA,T_ZERO,R_ZERO);
  float error=TARGET_TEMP-actualTemp;
  float difTime=millis()-previousTime;
  
  int app_power=powerPID(error,KP,KI,KD,difTime,previouserr);
  if (app_power>=0){
    //se for positivo liga o peltier numa polaridade para esquentar
      digitalWrite(7,HIGH);
   digitalWrite(8,LOW);
   analogWrite(6,app_power);
    }else { 
       //se for negativo liga o peltier em outra polaridade para esfriar
        digitalWrite(7,LOW);
  digitalWrite(8,HIGH);
   analogWrite(6,-app_power);

    }
  previousTime=millis();
   previouserr=error;

//imprime no monitor serial
Serial.print(millis());
Serial.print(" ");
  Serial.print(actualTemp-273.15);
  Serial.print(" ");
  Serial.print(integral+MAX_INTEGRAL);
  Serial.print(" ");
  Serial.print(app_power);
  Serial.print(" ");
  Serial.println(1);
  delay(1);
  
  

}