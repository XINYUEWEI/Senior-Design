//#define AT 2 //AT引脚连接Pin2
#define LED 7  //LED引脚连接Pin13
void setup()
{
       pinMode(LED,OUTPUT);
       //pinMode(AT,OUTPUT);
       //digitalWrite(AT,HIGH);
       Serial1.begin(38400);//这里应该和你的模块初始通信波特率一致
       delay(100);
       Serial1.println("AT");
       delay(100);
       Serial1.println("AT+NAME=OPENJUMPER-Bluetooth-2");//命名模块名
       delay(100);
       Serial1.println("AT+ROLE=1");//设置主从模式：0从机，1主机
       delay(100);
       Serial1.println("AT+PSWD=1120");//设置配对密码，如1120
       delay(100);
       //Serial.println("AT+UART=115200,0,0");//设置波特率115200，停止位1，校验位无
       delay(100);
       Serial1.println("AT+RMAAD");//清空配对列表
}
void loop()
{
       digitalWrite(LED, HIGH);
       delay(500);
       digitalWrite(LED, LOW);
       delay(500);
}
