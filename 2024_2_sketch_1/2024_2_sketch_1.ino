#include <Arduino.h>
#include <SPIFFS.h>
#include<M5ModuleDisplay.h>
#include <M5Unified.h>
#include <ESP32Servo.h>
Servo myservo;
int servoPin = 32;
#include <SD.h>
int count = 322;
int fillscreenflag1=0;
int fillscreenflag2=0;
int votescreenflag=1;
int moniter_time=0;
int chococorn_flag=0;
int chococorn_count=0;
int chocoroom_flag=0;
int chocoroom_count=0;
int displayWidth;
int displayHeight;
// PIRセンサーのピン番号
const int PIR_PIN = 33;  // PIRセンサーのデータピンをGPIO21に接続
int brightness = 128;    // 初期の明るさ（0～255）
unsigned long lastMotionTime = 0;  // 最後に動きを検知した時間
const unsigned long idleTime = 10000;  // 明るさを下げるまでの待機時間（ミリ秒）


void setup() {
    auto cfg = M5.config();
    cfg.external_display.module_display = true;
    cfg.module_display.logical_width = 1280;
    cfg.module_display.logical_height = 720;
    M5.begin(cfg);
    //
    int display_count = M5.getDisplayCount();
    
    M5.Lcd.setTextFont(&fonts::efontJA_16);
    M5.setPrimaryDisplayType({
      m5::board_t::board_M5ModuleDisplay,
    });
    int index_module_display = M5.getDisplayIndex(m5::board_t::board_M5ModuleDisplay);
    
    // PIRセンサーのピンを入力モードに設定
    pinMode(PIR_PIN, INPUT);
    myservo.setPeriodHertz(50);
    myservo.attach(servoPin, 400, 2400);
    

    
   
  


}

void loop() {
    // M5の状態を更新
    M5.update();

    // タッチポイントのチェック
    if (M5.Touch.getCount() > 0) {  // タッチポイントが存在する場合
        auto touch = M5.Touch.getDetail(0);
        if(touch.y<140 && touch.x<300){ // 最初のタッチポイントを取得
          M5.Displays(0).setCursor(10, 220);
          if(fillscreenflag1==0){
            M5.Displays(0).fillRect(10, 10, 300, 130,TFT_BLUE);
            fillscreenflag1=1;
            M5.Displays(0).setCursor(20,40);
            M5.Displays(0).setTextColor(TFT_WHITE, TFT_BLUE);
            M5.Displays(0).setTextSize(2);
            M5.Displays(0).printf("ご協力ありがとう");
            M5.Displays(0).setCursor(20,75);
            M5.Displays(0).printf("ございます");
            if(chocoroom_flag==1){
              chocoroom_count+=1;
            }else{
              chococorn_count+=1;
            }
            count+=1;
            myservo.write(135); delay(500);
            myservo.write(0); delay(500);
            // SDカードに記録
            
          }
          fillscreenflag2=0;
        } 
        if(touch.y>140 && touch.x<150){
          chococorn_flag=1;
          chocoroom_flag=0;
          votescreenflag=1;
          
        }
        if(touch.y>140 && touch.x>150){
          chococorn_flag=0;
          chocoroom_flag=1;
          votescreenflag=1;
            
        }
      }else{
        // タッチがない場合
        M5.Displays(0).setCursor(10, 220);
        if(votescreenflag==1){
          if(chococorn_flag==0){
              M5.Displays(0).fillRect(10, 140, 150, 220,  0xEEC3);
              M5.Displays(0).setCursor(40, 175);
              M5.Displays(0).setTextSize(1);
              M5.Displays(0).setTextColor(TFT_BLACK,  0xEEC3);
              M5.Displays(0).printf("犬派");
            }else{
              M5.Displays(0).fillRect(10, 140, 150, 220, 0xEEC3);
              M5.Displays(0).setCursor(70, 175);
              M5.Displays(0).setTextSize(1);
              M5.Displays(0).setTextColor(TFT_BLACK,  0xEEC3);
              M5.Displays(0).printf("犬派");
              M5.Displays(0).setCursor(70, 195);
              M5.Displays(0).printf("投票選択中");

            }
          if(chocoroom_flag==0){
            M5.Displays(0).fillRect(160, 140, 150, 220, 0xAFE0);
            M5.Displays(0).setCursor( 210, 175);
            M5.Displays(0).setTextSize(1);
            M5.Displays(0).setTextColor(TFT_BLACK, 0xAFE0);
            M5.Displays(0).printf("猫派");
          }else{
            M5.Displays(0).fillRect(160, 140, 150, 220, 0xAFE0);
            M5.Displays(0).setCursor( 210, 175);
            M5.Displays(0).setTextSize(1);
            M5.Displays(0).setTextColor(TFT_BLACK, 0xAFE0);
            M5.Displays(0).printf("猫派");
            M5.Displays(0).setCursor(170, 195);
              M5.Displays(0).printf("投票選択中");
          }
          votescreenflag=0;
        }
        
        if(fillscreenflag2==0){
          
          
          
          M5.Displays(0).fillRect(10, 10, 300, 130,TFT_RED);
          fillscreenflag2=1;
          M5.Displays(0).setCursor(20,40);
          M5.Displays(0).setTextColor(TFT_WHITE, TFT_RED);
          M5.Displays(0).setTextSize(2);
          M5.Displays(0).printf("キャップを入れて");
          M5.Displays(0).setCursor(20,75);
          M5.Displays(0).printf("押してね");
          M5.Displays(1).setTextFont(&fonts::efontJA_16);
          displayWidth = M5.Displays(1).width();
          displayHeight = M5.Displays(1).height();
          M5.Displays(1).setCursor(0, 0);
          M5.Displays(1).fillRect(0, 0, displayWidth/2, displayHeight*3/4,0xEEC3);
          M5.Displays(1).fillRect(displayWidth/2, 0, displayWidth/2, displayHeight*3/4,0xAFE0);
          M5.Displays(1).setTextSize(5);
          M5.Displays(1).setCursor(displayWidth/8-50, 180);
          M5.Displays(1).setTextColor(TFT_BLACK,  0xEEC3);
          M5.Displays(1).print("犬派");
          M5.Displays(1).setTextSize(5);
          M5.Displays(1).setCursor(displayWidth/8+100, 330);
          M5.Displays(1).setTextSize(10);
          M5.Displays(1).print(String(chococorn_count));
          M5.Displays(1).setCursor(displayWidth*5/8-50, 180);
          M5.Displays(1).setTextColor(TFT_BLACK, 0xAFE0);
          M5.Displays(1).setTextSize(5);
          M5.Displays(1).print("猫派");
          M5.Displays(1).setCursor(displayWidth*5/8+100, 330);
          M5.Displays(1).setTextSize(10);
          M5.Displays(1).print(String(chocoroom_count));
          M5.Displays(1).setTextColor(TFT_WHITE, TFT_BLACK);
          M5.Displays(1).setBrightness(brightness);
          M5.Displays(1).setTextSize(10);
          M5.Displays(1).setCursor(displayWidth/4, displayHeight*4/5);
          M5.Displays(1).print("総数："+String(count));
        }
        fillscreenflag1=0;

          
        
        
        


        

          
    }
  

    int motionDetected = digitalRead(PIR_PIN);

    if (motionDetected == HIGH) {
        // 動きを検知した場合
        brightness = 255;  // 明るさを最大に設定
        lastMotionTime = millis();  // 最後の動き検知時間を更新
        M5.Displays(0).setBrightness(brightness);
        
        
        
        
    }else{
        // 動きが一定時間検知されなかった場合
        brightness = 50;
          // 明るさを最低に設定
        M5.Displays(0).setBrightness(brightness);
        
        
        
          
        

    }
  
    
}
