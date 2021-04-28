#include <Adafruit_INA219.h>
#include <Wire.h>
#include <Enerlib.h>
#include <Bounce2.h>
#define F_S 16.8 //for 4s lipo battery满电
#define F_SS 14 //4s空
#define S_S 25.2 //for 6s lipo battery
#define T 1000 //delay time
#define LED1 4
#define LED2 5
#define LED3 6
#define LED4 7

Adafruit_INA219 ina219;

//睡眠模式
Energy energy;             // 宣告"Energy"程式物件
const byte swPin = 2;      // 开关脚位
byte times = 0;            // 记录执行次数
volatile byte state = 0;   // 暂存执行状态

//按键防抖
Bounce debouncer = Bounce();
int val = 0; //变量val存储按钮的状态,因为是下拉电阻，默认是LOW(即0)
int old_val = 0; //存储val变量的上一个时间状态
int stateb = 0; //打印到串口的数据，按钮每被按下一次，stateb自增1



void wakeISR() {
  if (energy.WasSleeping()) {
    state = 1;
  } else {
    state = 2;
  }
} 


void setup(void)
{

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(swPin, INPUT);
  digitalWrite(swPin, HIGH);

  debouncer.attach(swPin);
  debouncer.interval(5);

  attachInterrupt(0, wakeISR, CHANGE);  // 附加中斷服務常式
  Serial.println("Running...");


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



}

void printStateb()

{

  stateb++;

  Serial.println(stateb);

}

void loop(void)
{
  float shuntvoltage = 0;
  float busvoltage = 0;
  float current_mA = 0;
  float loadvoltage = 0;
  float power_mW = 0;


  debouncer.update();//更新

  val = debouncer.read(); ////读取输入数值并且存储

  if ((val == HIGH) && (old_val == LOW)) //检查按钮的变化情况

  {

    printStateb();

  }

  old_val = val; //val现在是旧的了，暂存一下


  if (state == 1) {
    Serial.println("Was sleeping...");
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, LOW);
    digitalWrite(LED4, LOW);
    delay(300);
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, HIGH);
    digitalWrite(LED3, LOW);
    digitalWrite(LED4, LOW);
    delay(300);
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, HIGH);
    digitalWrite(LED4, LOW);
    delay(300);
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, HIGH);
    digitalWrite(LED4, LOW);
    delay(300);
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, LOW);
    digitalWrite(LED4, HIGH);
    delay(300);
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, LOW);
    digitalWrite(LED4, LOW);
    delay(300);
  }

  else if (state == 2) {

    Serial.println("Was awake...");
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, LOW);
    digitalWrite(LED4, LOW);
    delay(300);
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, HIGH);
    digitalWrite(LED3, LOW);
    digitalWrite(LED4, LOW);
    delay(300);
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, HIGH);
    digitalWrite(LED4, LOW);
    delay(300);
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, HIGH);
    digitalWrite(LED4, LOW);
    delay(300);
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, LOW);
    digitalWrite(LED4, HIGH);
    delay(300);
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, LOW);
    digitalWrite(LED4, LOW);
    delay(300);
  }
  state = 0;

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
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, HIGH);
    digitalWrite(LED3, HIGH);
    digitalWrite(LED4, HIGH);
    delay(700);
    digitalWrite(LED4, LOW);
    delay(300);
    times++;
  }

  //When the remaining battery capacity is higher than 60%,let the first led goes OFF and the second led blink while keeping other two leds ON
  //else if (0.8 * (F_S - F_SS) + F_SS > busvoltage >= 0.6 * (F_S - F_SS) + F_SS)
  else if (busvoltage >= 15.68 && busvoltage < 16.24)
  {
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, HIGH);
    digitalWrite(LED3, HIGH);
    digitalWrite(LED4, LOW);
    delay(700);
    digitalWrite(LED3, LOW);
    delay(300);
    times++;
  }

  //else if (0.6 * (F_S - F_SS) + F_SS > busvoltage >= 0.4 * (F_S - F_SS) + F_SS)
  else if (busvoltage >= 15.12 && busvoltage < 15.68)
  {
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, HIGH);
    digitalWrite(LED3, LOW);
    digitalWrite(LED4, LOW);
    delay(700);
    digitalWrite(LED2, LOW);
    delay(300);
    times++;
  }

  //else if (0.4 * (F_S - F_SS) + F_SS > busvoltage >= 0.2 * (F_S - F_SS) + F_SS)
  else if (busvoltage >= 14.56 && busvoltage < 15.12)
  {
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, LOW);
    digitalWrite(LED4, LOW);
    delay(700);
    digitalWrite(LED1, LOW);
    delay(300);
    times++;
  }

  //else if (0.1 * (F_S - F_SS) + F_SS >= busvoltage)
  else if (busvoltage <= 14.56)
  {
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, HIGH);
    digitalWrite(LED3, HIGH);
    digitalWrite(LED4, HIGH);
    delay(500);
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, LOW);
    digitalWrite(LED4, LOW);
    delay(500);
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, HIGH);
    digitalWrite(LED3, HIGH);
    digitalWrite(LED4, HIGH);
    delay(500);
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, LOW);
    digitalWrite(LED4, LOW);
    delay(500);

    times++;
  }


  if (times > 5) {
    times = 0;

    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, LOW);
    digitalWrite(LED4, LOW);

    Serial.println("Go to sleep...");

    energy.PowerDown();
  }

}
