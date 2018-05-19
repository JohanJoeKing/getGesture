/************************************************************
* - Filename : getGesture.ino
* - Author : Johan
* - Date : 2018/5/18
* - Version : 1.0
* - Description : three axis acceleration sensor demo
* - Function list :
* 1.setup
* 2.loop
* 3.LED_flash
* - Others :
* // author email : 1412758626@qq.com
*************************************************************/


#include <LiquidCrystal.h>


LiquidCrystal LCD(12,11,10,9,8,7,6,5,4,3,2);


const int X_PIN = A2;   // X轴引脚
const int Y_PIN = A1;   // Y轴引脚
const int Z_PIN = A0;   // Z轴引脚
int X;                  // X轴值
int Y;                  // Y轴值
int Z;                  // Z轴值
float Vzerog = 1.5;     //ADXL335 三轴加速度计 0g 基准电压典型值
int Sensitivity = 300;  //ADXL335 三轴加速度计灵敏度参数典型值
int ledPin = 13;        // LED灯的引脚




/*
 * setup
 */
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  LCD.begin(16,2);
  LCD.print("Johan346"); 
}




/*
 * loop
 */
void loop() {
  // put your main code here, to run repeatedly:
  LCD.setCursor(0,1);
  LCD.print(millis()/1000);

  // 下载状态量
  X = analogRead(X_PIN);
  Y = analogRead(Y_PIN);
  Z = analogRead(Z_PIN);
  
  //将模拟量转换成电压值
  float vx = X * 5.0 / 1024.0;
  float vy = Y * 5.0 / 1024.0;
  float vz = Z * 5.0 / 1024.0;
  
 //将模拟量转换成每个轴向的g值,1.5是三轴加速度计的0g基准电压,单位为V,300是三轴加速度计的灵敏度,单位是mv/g
  float Rx = (vx - Vzerog) / (Sensitivity / 1000.0);
  float Ry = (vy - Vzerog) / (Sensitivity / 1000.0);
  float Rz = (vz - Vzerog) / (Sensitivity / 1000.0);
  
 //得出重力加速度g值大小
  float R = sqrt(pow(Rx,2) + pow(Ry,2) + pow(Rz,2));
  
 //计算与三轴的夹角对应弧度值
  float Axr = acos(Rx / R);
  float Ayr = acos(Ry / R);
  float Azr = acos(Rz / R);
  
 //将水平弧度值转换为对应角度,并通过串口打印
  float anglex = 180.0 * Axr / 3.14;
  float angley = 180.0 * Ayr / 3.14;
  float anglez = 180.0 * Azr / 3.14;

  /*
  向串口输出
  此处的协议格式就根据自己的需要设计了，如：
  格式协议为：X#Y#Z
  状态值全部化为000.000格式，故一条消息的总长度为17-20
  串口接收端接收到的数据不等于这个长度值的时候可选择丢弃处理
  */
  // 发送
  String sx, sy, sz;
  String k = "";
  sx = k + anglex;
  sy = k + angley;
  sz = k + anglez;
  Serial.println(sx + "#" + sy + "#" + sz);

  // LED闪烁3下，延迟1s
  LED_flash(3);
  delay(1000);
}





/*
 * LED_flash
 */
void LED_flash(int times){
  for(int i=0;i<times;i++ ){
        digitalWrite(ledPin,HIGH);
        delay(200);
        digitalWrite(ledPin,LOW);
        delay(200); 
    }
}

