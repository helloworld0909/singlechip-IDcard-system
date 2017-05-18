#include <boarddefs.h>
#include <IRremote.h>
#include <IRremoteInt.h>
#include <ir_Lego_PF_BitStreamEncoder.h>
#include <LiquidCrystal.h>
#include <SPI.h>
#include <RFID.h>

//D10 - 读卡器CS引脚、D5 - 读卡器RST引脚
RFID rfid(10,5);   
unsigned char status;
unsigned char id[MAX_LEN];  //MAX_LEN为16，数组最大长度
//LiquidCrystal lcd(12,11,5,4,3,2);      //构造一个LiquidCrystal的类成员。使用数字IO ，12,11,5,4,3,2
LiquidCrystal lcd(7,6,5,4,3,2);

int RECV_PIN = 8; // 红外一体化接收头连接到Arduino 11号引脚
 
IRrecv irrecv(RECV_PIN);
 
decode_results results; // 用于存储编码结果的对象

int count = 0;
boolean hasDot = 0;
boolean needClear = 0;
char str[16];
char tmp[MAX_LEN];

char inByte = 0;
String buff;

void setup()
{
  Serial.begin(9600); // 初始化串口通信
  irrecv.enableIRIn(); // 初始化红外解码

  lcd.begin(16,2);    //初始化LCD1602
  lcd.print("Welcome to use!");   //液晶显示Welcome to use！
  delay(1000);        //延时1000ms
  lcd.clear();        //液晶清屏

  SPI.begin();
  rfid.init(); //初始化
}

void execNumber(char input, int &count, LiquidCrystal lcd, char str[]){
  lcd.setCursor(count,1);
  lcd.print(input);
  str[count] = input;
  count += 1;
  delay(20);              //延时1000ms
}

void loop() {
   //Search card, return card types
  if (rfid.findCard(PICC_REQIDL, id) == MI_OK) {
    //防冲突检测,读取卡序列号
    if (rfid.anticoll(id) == MI_OK) {
      Serial.print("CARD:");
      //显示卡序列号
      for(int i = 0; i < 4; i++){
        Serial.print(0x0F & (id[i] >> 4),HEX);
        Serial.print(0x0F & id[i],HEX);
      }
      Serial.println("");
    }
    //选卡（锁定卡片，防止多数读取，去掉本行将连续读卡）
    rfid.selectTag(id);
  }
  rfid.halt();  //命令卡片进入休眠状态
  
  if (irrecv.decode(&results)) {
    //Serial.println(results.value, HEX);
    if(needClear){
      lcd.clear();
      needClear = 0;
    }

    lcd.setCursor(0,0);        //设置液晶开始显示的指针位置
    lcd.print("Input");  //液晶显示“LM35 temp =”
    lcd.setCursor(0,1);       //设置液晶开始显示的指针位置
    
    switch(results.value){
      case 0xFF30CF:
        execNumber('1', count, lcd, str);
        break;
      case 0xFF18E7:
        execNumber('2', count, lcd, str);
        break;
      case 0xFF7A85:
        execNumber('3', count, lcd, str);
        break;
      case 0xFF10EF:
        execNumber('4', count, lcd, str);
        break;
      case 0xFF38C7:
        execNumber('5', count, lcd, str);
        break;
      case 0xFF5AA5:
        execNumber('6', count, lcd, str);
        break;
      case 0xFF42BD:
        execNumber('7', count, lcd, str);
        break;
      case 0xFF4AB5:
        execNumber('8', count, lcd, str);
        break;
      case 0xFF52AD:
        execNumber('9', count, lcd, str);
        break;
      case 0xFF6897:
        execNumber('0', count, lcd, str);
        break;
      case 0xFF9867:
        if(not hasDot){
          hasDot = 1;
          execNumber('.', count, lcd, str);
        }
        break;
      case 0xFFB04F:
        lcd.clear();
        count = 0;
        hasDot = 0;
        delay(20);              //延时20ms
        break;
      case 0xFF02FD:            //确认
        lcd.clear();
        count = 0;
        hasDot = 0;
        Serial.print("TRAN:");
        //显示卡序列号
        Serial.println(str);
        memset(str, 0, sizeof(str));
        memset(id, 0, sizeof(id));
        delay(500);              //延时200ms
        break;
      default:
        break;
  }
  // delay(1000);              //延时1000ms
  irrecv.resume(); // 接收下一个编码
  }

  while (Serial.available() > 0){
    buff += char(Serial.read());
    delay(2);
  }
  if(buff.length() > 0){
    String tmp;
    lcd.clear();
    for(int i=0; i<buff.length(); i++){
      if(buff[i]=='\t'){
        lcd.setCursor(0,0);
        lcd.print(tmp);
        tmp = "";
      }
      if(buff[i]=='\n'){
        lcd.setCursor(0,1);
        lcd.print(tmp);
        tmp = "";
      }
      tmp += buff[i];
    }
    buff = "";
    needClear = 1;
  }
}
