#include <Adafruit_INA219.h>
#include <Wire.h>
#define F_S 16.8 //for 4s lipo battery满电
#define F_SS 14 //4s空
#define S_S 25.2 //for 6s lipo battery
#define T 1000 //delay time

Adafruit_INA219 ina219;

void setup(void)
{

  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(7, OUTPUT);

  Serial.begin(115200);
  while (!Serial) {
    delay(1);
  }

  uint32_t currentFrequency;

  Serial.println("Hello!");

  // 初始化会默认使用最大测量范围(32V, 2A)，你也可以使用setCalibration指令更改，以获取更高的精度
  ina219.begin();
  // 用 32V, 1A 范围来测量会获取更高的电流测量精度，如果想使用这个选项就删除下面那行的注释
  //ina219.setCalibration_32V_1A();
  // 用16V, 400mA 范围来测量会获取更高的电流和电压测量精度，如果想使用这个选项就删除下面那行的注释
  //ina219.setCalibration_16V_400mA();


  Serial.println("Measuring voltage and current with INA219 ..."); //打印‘正在使用ina219测量电压和电流’


  digitalWrite(13, HIGH);
  digitalWrite(12, LOW);
  digitalWrite(8, LOW);
  digitalWrite(7, LOW);
  delay(300);
  digitalWrite(13, LOW);
  digitalWrite(12, HIGH);
  digitalWrite(8, LOW);
  digitalWrite(7, LOW);
  delay(300);
  digitalWrite(13, LOW);
  digitalWrite(12, LOW);
  digitalWrite(8, HIGH);
  digitalWrite(7, LOW);
  delay(300);
  digitalWrite(13, LOW);
  digitalWrite(12, LOW);
  digitalWrite(8, LOW);
  digitalWrite(7, HIGH);
  delay(300);
  digitalWrite(13, HIGH);
  digitalWrite(12, HIGH);
  digitalWrite(8, HIGH);
  digitalWrite(7, HIGH);


}

void loop(void)
{
  float shuntvoltage = 0;
  float busvoltage = 0;
  float current_mA = 0;
  float loadvoltage = 0;
  float power_mW = 0;

  shuntvoltage = ina219.getShuntVoltage_mV();
  busvoltage = ina219.getBusVoltage_V();
  current_mA = ina219.getCurrent_mA();
  power_mW = ina219.getPower_mW();
  loadvoltage = busvoltage + (shuntvoltage / 1000);

  Serial.print("Bus Voltage:   "); Serial.print(busvoltage); Serial.println(" V");    //打印总线电压
  Serial.print("Shunt Voltage: "); Serial.print(shuntvoltage); Serial.println(" mV"); //打印分流电压
  Serial.print("Load Voltage:  "); Serial.print(loadvoltage); Serial.println(" V");   //打印负载电压
  Serial.print("Current:       "); Serial.print(current_mA); Serial.println(" mA");   //打印电流
  Serial.print("Power:         "); Serial.print(power_mW); Serial.println(" mW");     //打印功率
  Serial.println("");
  delay(1000);



  //calculating...




  //When the remaining battery capacity is higher than 80%,let the first led blink and keep other three leds ON
  // if (busvoltage >= 0.8 * (F_S - F_SS) + F_SS)
  if (busvoltage >= 16.24)
  {
    digitalWrite(13, HIGH);
    digitalWrite(12, HIGH);
    digitalWrite(8, HIGH);
    digitalWrite(7, HIGH);
    delay(700);
    digitalWrite(7, LOW);
    delay(300);

  }

  //When the remaining battery capacity is higher than 60%,let the first led goes OFF and the second led blink while keeping other two leds ON
  //else if (0.8 * (F_S - F_SS) + F_SS > busvoltage >= 0.6 * (F_S - F_SS) + F_SS)
  else if (busvoltage >= 15.68 && busvoltage < 16.24)
  {
    digitalWrite(13, HIGH);
    digitalWrite(12, HIGH);
    digitalWrite(8, HIGH);
    digitalWrite(7, LOW);
    delay(700);
    digitalWrite(8, LOW);
    delay(300);

  }

  //else if (0.6 * (F_S - F_SS) + F_SS > busvoltage >= 0.4 * (F_S - F_SS) + F_SS)
  else if (busvoltage >= 15.12 && busvoltage < 15.68)
  {
    digitalWrite(13, HIGH);
    digitalWrite(12, HIGH);
    digitalWrite(8, LOW);
    digitalWrite(7, LOW);
    delay(700);
    digitalWrite(12, LOW);
    delay(300);

  }

  //else if (0.4 * (F_S - F_SS) + F_SS > busvoltage >= 0.2 * (F_S - F_SS) + F_SS)
  else if (busvoltage >= 14.56 && busvoltage < 15.12)
  {
    digitalWrite(13, HIGH);
    digitalWrite(12, LOW);
    digitalWrite(8, LOW);
    digitalWrite(7, LOW);
    delay(700);
    digitalWrite(13, LOW);
    delay(300);

  }

  //else if (0.1 * (F_S - F_SS) + F_SS >= busvoltage)
  else if (busvoltage <= 14.56)
  {    
    digitalWrite(13, LOW);
    digitalWrite(12, LOW);
    digitalWrite(8, LOW);
    digitalWrite(7, LOW);
    delay(500);
    digitalWrite(13, HIGH);
    digitalWrite(12, HIGH);
    digitalWrite(8, HIGH);
    digitalWrite(7, HIGH);
    delay(500);
    digitalWrite(13, LOW);
    digitalWrite(12, LOW);
    digitalWrite(8, LOW);
    digitalWrite(7, LOW);
    delay(500);
    digitalWrite(13, HIGH);
    digitalWrite(12, HIGH);
    digitalWrite(8, HIGH);
    digitalWrite(7, HIGH);
    delay(500);

  }

}
