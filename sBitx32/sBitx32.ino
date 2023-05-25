

//Firmware Version
//    This firmware hass been gradually changed based on the original firmware 
//    created by Farhan, Jack, Jerry and others and KD8CEC.

// Standar version - No tasks
#define FIRMWARE_VERSION_INFO F("v 0.100")
#define EEPROM_SIZE 2048

/********************************************
// you must define this pins at file "User_setup.h" from TFT_eSPI library
// these lines have no effect and are only informative
// TFT pins for uBITX
#define BITX        // for use  at user_setup.h from TFT_eSPI library
#define TFT_RST  -1 // Reset pin (could connect to RST pin)
#define TFT_DC   21 // Data Command control pin   
#define TFT_MISO 12
#define TFT_MOSI 13
#define TFT_SCLK 14

#define TFT_CS   32 // Chip select control pin    
#define TOUCH_CS  5 // Chip select pin (T_CS) of touch screen
*********************************************/

/**
 Cat Support uBITX CEC Version 
 This firmware has been gradually changed based on the original firmware created by Farhan, Jack, Jerry and others.
 Most features(TX, Frequency Range, Ham Band, TX Control, CW delay, start Delay... more) have been added by KD8CEC.
 My wish is to keep the original author's Comment as long as the meaning does not change much, even if the code looks a bit long.
 Ian KD8CEC

 Original source comment            -------------------------------------------------------------
 * This source file is under General Public License version 3.
 * 
 * This version uses a built-in Si5351 library
 * Most source code are meant to be understood by the compilers and the computers. 
 * Code that has to be hackable needs to be well understood and properly documented. 
 * Donald Knuth coined the term Literate Programming to indicate code that is written be 
 * easily read and understood.
 * 
 * The Raduino is a small board that includes the Arduin Nano, a 16x2 LCD display and
 * an Si5351a frequency synthesizer. This board is manufactured by Paradigm Ecomm Pvt Ltd
 * 
 * Below are the libraries to be included for building the Raduino 
 * The EEPROM library is used to store settings like the frequency memory, caliberation data, 
 * callsign etc .
 *
 *  The main chip which generates upto three oscillators of various frequencies in the
 *  Raduino is the Si5351a. To learn more about Si5351a you can download the datasheet 
 *  from www.silabs.com although, strictly speaking it is not a requirment to understand this code. 
 *  Instead, you can look up the Si5351 library written by xxx, yyy. You can download and 
 *  install it from www.url.com to complile this file.
 *  The Wire.h library is used to talk to the Si5351 and we also declare an instance of 
 *  Si5351 object to control the clocks.
 */

#include "defines.h"              
#include "variables.h"                // include 
#include "commontexts.h"              
#include "ajaxcode.h"                  // include
#include <WebServer.h>
#include <Update.h>
#include <ESP8266FtpServer.h>
#include <WiFiUdp.h>
#include <NTPClient.h>                // Local
#include "Time.h"                     // Local 
#include "TimeLib.h"                  // Local
#include "FS.h"
#include "SPIFFS.h"
#include <HTTPClient.h>
#include <EEPROM.h>
#include <TFT_eSPI.h>                 // Graphics and font library for ILI9341 driver chip
#include "OneWire.h"                  // Local
#include "DallasTemperature.h"        // Local
#include "sbitx.h"
#include "sbitx_eemap.h"
#include "Adafruit_ADS1X15.h"
#include <ESP32_Servo.h>
#include "RemoteDebug.h"        //https://github.com/JoaoLopesF/RemoteDebug
#include <PubSubClient.h>
#include "Adafruit_TPA2016.h"
#include <ArduinoWebsockets.h>
#include "AsyncUDP.h"
#include "esp_wifi.h"
#include <Adafruit_PWMServoDriver.h>
#include "lyraTcom.h"
#include "vhf_module.h"
#include <analogWrite.h>

WiFiServer tcpserver;
WiFiUDP udpsmeter, udpfreq, ntpUDP;
using namespace websockets;
WebsocketsServer wsserver;
WebsocketsClient wsclient;    

TFT_eSPI tft=TFT_eSPI();  // Invoke library, pins defined in User_Setup.h
TFT_eSprite wfall = TFT_eSprite(&tft); // Sprite object graph1

FtpServer ftpSrv;   //set #define FTP_DEBUG in ESP8266FtpServer.h to see ftp verbose on serial
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org");
OneWire owire(W0);
DallasTemperature sensors0(&owire);
Adafruit_ADS1115 adsA;  /* Use this for the 16-bit version */
Adafruit_ADS1115 adsB;  /* Use this for the 16-bit version */
//Adafruit_ADS1115 adsA(0x48);  /* Use this for the 16-bit version */
//Adafruit_ADS1115 adsB(0x49);  /* Use this for the 16-bit version */
WebServer server(webportdefault);
RemoteDebug Debug;
WiFiClient espClient;
WiFiClient tcpclient;
PubSubClient PSclient(espClient);
Adafruit_TPA2016 audioamp = Adafruit_TPA2016();

Adafruit_PWMServoDriver servos = Adafruit_PWMServoDriver();
#define SERVOMIN  150 // This is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  600 // This is the 'maximum' pulse length count (out of 4096)
#define SERVO_FREQ 60 // Analog servos run at ~50 Hz updates

extern int enc_read(void);

#define WIRE_CLOCK 100000     // default = 100000
#define bfo_freq 40035000
#define adjust_freq1 8000
#define adjust_freq2 10000
//When the frequency is moved by the dial, the maximum value by KD8CEC
#define LOWEST_FREQ_DIAL  (3000l)
#define HIGHEST_FREQ_DIAL (60000000l)

//sideTonePitch + sideToneSub = sideTone
byte sideTonePitch=0;
byte sideToneSub = 0;

//DialLock
byte isTxType = 0;    //000000[0 - isSplit] [0 - isTXStop]

#define IAMBICB 0x10 // 0 for Iambic A, 1 for Iambic B
unsigned char keyerControl = IAMBICB;

int shiftDisplayAdjustVal = 0;  //

//Variables for auto cw mode
byte isCWAutoMode = 0;          //0 : none, 1 : CW_AutoMode_Menu_Selection, 2 : CW_AutoMode Sending
byte cwAutoTextCount = 0;       //cwAutoText Count
byte beforeCWTextIndex = 255;   //when auto cw start, always beforeCWTextIndex = 255, (for first time check)
byte cwAutoDialType = 0;        //0 : CW Text Change, 1 : Frequency Tune

#define AUTO_CW_RESERVE_MAX 3
byte autoCWSendReserv[AUTO_CW_RESERVE_MAX]; //Reserve CW Auto Send
byte autoCWSendReservCount = 0;             //Reserve CW Text Cound
byte sendingCWTextIndex = 0;                //cw auto seding Text Index

#include "basicfunctions.h"            // include
#include "htmlfunctions.h"             // include
#include "cwdecode.h"

boolean txCAT = false;        //turned on if the transmitting due to a CAT command
boolean txTFT = false;        //turned on if the transmitting due to a TFT command
byte inTx = 0;                //it is set to 1 if in transmit mode (whatever the reason : cw, ptt or cat)
char keyDown = 0;             //in cw mode, denotes the carrier is being transmitted
byte isIFShift=0;             //1 = ifShift, 2 extend

unsigned long maxRitdev=5000;  // max. deviation on RIT
unsigned countloop=0;
                              
/* Below are the basic functions that control the uBitx. Understanding the functions before 
 * you start hacking around */

//-1 : not found, 0 ~ 9 : Hamband index
byte getIndexHambanBbyFreq(unsigned long f)
{
  for (byte i = 0; i<conf.useHamBandCount; i++)
    if ((conf.hamBandRange[i][0]*1000 <= f) && (f <= conf.hamBandRange[i][1]*1000))
      return i;
  return 99;
}

// when Band change step = just hamband,  moveDirection : 1 = next, -1 : prior
void setNextHamBandFreq(unsigned long f, int moveDirection)
{
  unsigned long resultFreq = 0;
  if (conf.actualBand == 99) 
    { conf.actualBand=3; }   //out of hamband
  else if (moveDirection==1)
    {
    if (conf.actualBand < conf.useHamBandCount-1)
      conf.actualBand++;
    else
      conf.actualBand=0;
    }
  else if (moveDirection==-1)
    {
    if (conf.actualBand > 0)
      conf.actualBand--;
    else
      conf.actualBand=conf.useHamBandCount-1;
    }
  else
    conf.actualBand = 99;
  resultFreq = resultFreq & 0x1FFFFFFF;
  if ((resultFreq/1000) < conf.hamBandRange[conf.actualBand][0] || (resultFreq / 1000) > conf.hamBandRange[conf.actualBand][1])
    resultFreq = (unsigned long)(conf.hamBandRange[conf.actualBand][0]) * 1000;
  if (conf.actualBand != 99)
    {
    setFrequency(conf.freqbyband[conf.actualBand][conf.vfoActive==VFO_A?0:1]);
    }
  else
    setFrequency(resultFreq);
  if (conf.autoMode==1) { setUSB(resultFreq>=10000000?1:0); }
  displayFreq(0,1,1,1);
  saveconf();
}

float readSWR(int limit)
{
  float auxSWR=1;
  int16_t adc0, adc1;
  long ldc0=0; long ldc1=0; 
  for (byte i=0;i<conf.ATUIter;i++)
    {
    ldc0=ldc0+adsA.readADC_SingleEnded(VFORp); // VFORp=0
    ldc1=ldc1+adsA.readADC_SingleEnded(VREFp); // VREFp=1
    }
  adc0 = ldc0/conf.ATUIter; if (adc0<0) adc0=0;
  adc1 = ldc1/conf.ATUIter; if (adc1<0) adc1=0;
  
  vFORc=((float(adc0)*0.1875/1000)+0.25)*11*0.707;
  vREFc=((float(adc1)*0.1875/1000)+0.25)*11*0.707;
  wFORc=vFORc*vFORc*0.707/50; wREFc=vREFc*vREFc*0.707/50;
  if ((vFORc-vREFc)>0) SWRreal=(vFORc+vREFc)/(vFORc-vREFc); else SWRreal=1.0;
  auxSWR=(SWRreal*conf.ATUFactor)+conf.ATUOffset;
  if (auxSWR<limit) auxSWR=1;
  return(auxSWR);
}

/*KD8CEC
  When using the basic delay of the Arduino, the program freezes.
  When the delay is used, the program will generate an error because it is not communicating, 
  so Create a new delay function that can do background processing. */
unsigned long delayBeforeTime = 0;
byte delay_background(unsigned delayTime, byte fromType, byte swr)
{ //fromType : 4 autoCWKey -> Check Paddle
  delayBeforeTime = millis();
  
  while (millis() - delayBeforeTime <= delayTime) 
    {
    if (fromType == 4)
      {
      //CHECK PADDLE
      if (getPaddle() != 0) //Interrupt : Stop cw Auto mode by Paddle -> Change Auto to Manual
        return 1;
      //Check PTT while auto Sending
      autoSendPTTCheck();
      Check_Cat(3);
      }
    else if (fromType == 3)   // KEYER
      {
      if (millis()-lastswr>swrper)
        {
        displaySWR(0);
        }
      }
    else
      {
      delay(10);
      //Background Work      
      Check_Cat(fromType);
      }
    if (swr==1)   // SWR on
      {
      if ((((millis()-delayBeforeTime) % 100)>=95) && (((millis()-delayBeforeTime) % 100)<=105))
        {
        displaySWR(0);
        }
      }
    }
  return 0;
}
 
/**
 * Select the properly tx harmonic filters
 * The four harmonic filters use only three relays
 * the four LPFs cover 30-21 Mhz, 18 - 14 Mhz, 7-10 MHz and 3.5 to 5 Mhz
 * Briefly, it works like this, 
 * - When KT1 is OFF, the 'off' position routes the PA output through the 30 MHz LPF
 * - When KT1 is ON, it routes the PA output to KT2. Which is why you will see that
 *   the KT1 is on for the three other cases.
 * - When the KT1 is ON and KT2 is off, the off position of KT2 routes the PA output
 *   to 18 MHz LPF (That also works for 14 Mhz) 
 * - When KT1 is On, KT2 is On, it routes the PA output to KT3
 * - KT3, when switched on selects the 7-10 Mhz filter
 * - KT3 when switched off selects the 3.5-5 Mhz filter
 * See the circuit to understand this
 */

int prev_lpf = -1;

void setTXFilters(unsigned long freq){
  int lpf = 0;
  if (freq < 5500000)
    lpf = TX_LPF_D;
  else if (freq < 10500000)    
    lpf = TX_LPF_C;
  else if (freq < 18500000)    
    lpf = TX_LPF_B;
  else if (freq < 30000000)
    lpf = TX_LPF_A; 
  if (lpf == prev_lpf){
    return;
  }
  digitalWrite(TX_LPF_A, 0);
  digitalWrite(TX_LPF_B, 0);
  digitalWrite(TX_LPF_C, 0);
  digitalWrite(TX_LPF_D, 0);

  digitalWrite(lpf, 1); 
  prev_lpf = lpf;
}

void setOSCCurves(unsigned long f)
{
  
}

#define TUNING_SHIFT 0

void setFrequency(unsigned long f){
  conf.frequency=f;
  setTXFilters(f);
  // aquí hay que poner las variantes según el modo USB, LSB y CW
  if (conf.cwMode == 1)
    OSC2 = f + bfo_freq - 24000 - sideTonePitch;
  else  // SSB
    OSC2 = f + bfo_freq - 24000;
  si5351bx_setfreq(2, OSC2 + adjust_freq2);   // 
}

/* original con Raspberry
 *  // ejemplo USB, freq recibida = 10.000.000 Hz
 *  // OSC2 = 10 000 000 + 40 035 000 - 24000 = 50 011 000
 *  // OSC1 = 40 035 000
void radio_tune_to(u_int32_t f){
  if (rx_list->mode == MODE_CW)
    si5351bx_setfreq(2, f + bfo_freq - 24000 + TUNING_SHIFT - rx_pitch);
  else if (rx_list->mode == MODE_CWR)
    si5351bx_setfreq(2, f + bfo_freq - 24000 + TUNING_SHIFT + rx_pitch);
  else
    si5351bx_setfreq(2, f + bfo_freq - 24000 + TUNING_SHIFT);
  // TUNING_SHIFT = 0;
  // bfo_freq = 40035000
  //printf("Setting radio to %d\n", f);
}
*/


void curvemode()
{
  si5351bx_setfreq(0, 0);   // CLK0=0
  unsigned long Fmain=10000000;
  unsigned long F2=45000000+10000000;
  unsigned long F1=10000000;
  unsigned long F0=11055500;
  int step=10;
  for (int i=0;i<500;i++)
    {
    unsigned long f2=(F2-step*160)+(i*step);
    unsigned long f1=f2-11059000-Fmain;
    unsigned long f0=F0+(i*step);
    f1=0;
    f2=0;
    si5351bx_setfreq(2, f2);
    si5351bx_setfreq(1, f1);
    si5351bx_setfreq(0, f0);
    Serial2.print(f0); 
    int N=10;
    unsigned long vtot=0; 
    for (int i=0;i<N;i++)
      {
      int16_t vread=adsA.readADC_SingleEnded(3);    // es el valor leído del ADC sin convertir
      vtot=vtot+vread;
      }
    Serial2.print(","); 
    Serial2.println(vtot/N);
    }
  setFrequency(conf.frequency);
}

void tftErrormsg(char *texto1, char *texto2, char *texto3)
{
  clearTFT();
  tft.setTextSize(4); tft.setTextColor(TFT_RED);
  tft.drawString(texto1,30,90);
  tft.setTextSize(2); tft.setTextColor(TFT_YELLOW);
  tft.drawString(texto2,0,170);
  tft.setTextSize(2); tft.setTextColor(TFT_YELLOW);
  tft.drawString(texto3,0,200);
  delay(1000);
}

/* startTx is called by the PTT, cw keyer and CAT protocol to
 * put the uBitx in tx mode. It takes care of rit settings, sideband settings
 * Note: In cw mode, doesnt key the radio, only puts it in tx mode */
 
void startTx(byte txMode, byte isDisplayUpdate){
  //Check Hamband only TX //Not found Hamband index by now frequency
  tftpage=0;
  unsigned long auxfreq;
/**  if ((isTxType & 0x01) != 0x01)
    {
    digitalWrite(TX_RX, 1);
    }
  inTx = 1;**/
  
  if (conf.ritOn)
    {
    //save the current as the rx frequency
    conf.ritRxFrequency = conf.frequency;
    auxfreq=conf.ritTxFrequency;
//    setFrequency(conf.ritTxFrequency);
    }
  else 
    {
    if (conf.splitOn == 1) { setVFO(conf.vfoActive==VFO_A?VFO_B:VFO_A); }
    auxfreq=conf.frequency;
    } //end of else
  byte auxf=getIndexHambanBbyFreq(auxfreq);
  if (conf.TXall==0)    // TX only ham bands
    if ((auxfreq<conf.hamBandRange[auxf][0]*1000) || (auxfreq>conf.hamBandRange[auxf][1]*1000)) 
      {
      tftErrormsg("OUT OF BAND","Modify parameter","TX range");
      //tftpage=2;
      return;
      } 
  if ((isTxType & 0x01) != 0x01) { digitalWrite(TX_RX, 1); }
  inTx = 1;
  setFrequency(auxfreq);
  //reduce latency time when begin of CW mode
  if (isDisplayUpdate == 1)
    updateDisplay(1);
}

void stopTx(void){
  inTx = 0;
  digitalWrite(TX_RX, 0);           //turn off the tx
  if (conf.ritOn)
    {
    setFrequency(conf.ritRxFrequency);
    }
  else
    {
    if (conf.splitOn == 1) { setVFO(conf.vfoActive==VFO_A?VFO_B:VFO_A); }
    setFrequency(conf.frequency);
    } //end of else
  updateDisplay(1);
}


/** Basic User Interface Routines. These check the front panel for any activity */

/**
 * The PTT is checked only if we are not already in a cw transmit session
 * If the PTT is pressed, we shift to the ritbase if the rit was on
 * flip the T/R line to T and update the display to denote transmission */

void checkPTT(){  
  //we don't check for ptt when transmitting cw
  //if (cwTimeout > 0) return;
  if (digitalRead(PTT) == 0 && inTx == 0 && conf.cwMode==0)
    {
    startTx(TX_SSB, 1);  
    delay(50); //debounce the PTT
    }
  if ((digitalRead(PTT) == 1) && (inTx == 1)  && (conf.cwMode==0))
    {
    stopTx(); 
    }
}

void checkButton(){
 //only if the button is pressed
 int keyStatus = getBtnStatus();
 if (keyStatus == -1) return;
// delay(50);
 keyStatus = getBtnStatus();   //will be remove 3 lines
 if (keyStatus == -1) return;
 Check_Cat(0);
}

/************************************
Replace function by KD8CEC
prevent error controls applied Threshold for reduct errors,  dial Lock, dynamic Step
 *************************************/
byte threshold = 2;  //noe action for count
unsigned long lastEncInputtime = 0;
int encodedSumValue = 0;
byte lastMovedirection = 0;     //0 : stop, 1 : cw, 2 : ccw
#define encodeTimeOut 1000

void doScanF()
{
  if (scanF==1)   // down
    {
    conf.frequency -= (conf.arTuneStep[conf.tuneStepIndex]);
    if (conf.scanallf==0)
      if (conf.frequency<conf.hamBandRange[conf.actualBand][0]*1000)
        conf.frequency=conf.hamBandRange[conf.actualBand][1]*1000;
    }
  else if (scanF==2)     // up
    {
    conf.frequency += (conf.arTuneStep[conf.tuneStepIndex]);
    if (conf.scanallf==0)
      if (conf.frequency>conf.hamBandRange[conf.actualBand][1]*1000)
        conf.frequency = conf.hamBandRange[conf.actualBand][0]*1000;
    }
  setFrequency(conf.frequency);
  displayFreq(1,1,0,0);
}

void saveVFOtoMem()   
{
  int i=0;
  while ((i<maxMem) && (memo.act[i]==1)) i++;
  if (i<maxMem)
    {
    strcpy(memo.descr[i], "");
    int auxI=getCharTFT(memo.descr[i],10); 
    if (auxI !=-1) 
      {
      strcpy(memo.descr[i],auxtft);  
      memo.act[i]=1;
      memo.vfoActive[i]=conf.vfoActive;
      memo.isUSB[i]=conf.isUSB;
      memo.cwMode[i]=conf.cwMode;
      memo.ritOn[i]=conf.ritOn;
      memo.splitOn[i]=conf.splitOn;
      memo.frequency[i]=conf.frequency;
      memo.ritTxFrequency[i]=conf.ritOn==1?conf.ritTxFrequency:conf.frequency;
      //memo.cwModespl[i]=conf.cwModeB;
      memo.ftxspl[i]=conf.frequencyB;
      savememo();
      }
    }
}
void setMEMtoVFO(int pos)
{
  if (memo.act[pos]==1)
    {
    setVFO(memo.vfoActive[pos]);
    setUSB(memo.isUSB[pos]);
    setCW(memo.cwMode[pos]);
    setRIT(memo.ritOn[pos]);
    setSPLIT(memo.splitOn[pos]);
    conf.frequency=memo.frequency[pos];
    conf.ritTxFrequency=memo.ritTxFrequency[pos];
    //conf.cwModeB=memo.cwModespl[pos]=0;
    conf.frequencyB=memo.ftxspl[pos];
    setFrequency(conf.frequency);
    conf.lastmempos=pos;
    saveconf();
    }
}

void setFreq(int s)
{
  conf.frequency += (conf.arTuneStep[conf.tuneStepIndex] * s);  //applied weight (s is speed) //if want need more increase size, change step size
  if (conf.vfoActive==VFO_A) conf.frequencyA=conf.frequency; else conf.frequencyB=conf.frequency;
  setFrequency(conf.frequency);
  displayFreq(1,1,1,1);
}

int modeencoder=0;    // 0:tunning, 1:volume, 2:gain, 3:spectrum scale, 4,5,6: filter
int modefunction=0;   // modifica el comportamiento del botón principal

void setAudioFilter(int mode, int s)
{
  if ((s<-2) || (s>2)) return;
  if (mode==4)    // audiolower
    {
    audiolowerold=conf.audiolower;
    conf.audiolower=conf.audiolower+(20*s);
    if (conf.audiolower<20) conf.audiolower=20; 
    if (conf.audiolower>conf.audioupper) conf.audiolower=conf.audioupper;
    sendFilterLyraT(conf.audiolower,conf.audioupper);
    displaySpectrum(10,135,256,70);
    displaybtFilter();
    saveconf();
    }
  else if (mode==5)    // audioupper
    {
    audioupperold=conf.audioupper;
    conf.audioupper=conf.audioupper+(20*s);  
    if (conf.audioupper>=12000) conf.audioupper=12000; 
    if (conf.audioupper<conf.audiolower) conf.audioupper=conf.audiolower;
    sendFilterLyraT(conf.audiolower,conf.audioupper);
    displaySpectrum(10,135,256,70);
    displaybtFilter();
    saveconf();
    }
  else if (mode==6)    // audiopitch
    {
    audiopitchold=conf.audiopitch;
    conf.audiopitch=conf.audiopitch+(20*s);  
    if (conf.audiopitch<100) conf.audiopitch=100; 
    if (conf.audiopitch>5000) conf.audiopitch=5000; 
    displaySpectrum(10,135,256,70);
    displaybtFilter();
    saveconf();
    }
}

void doTuningWithThresHold(){
  int s=0;
  s = enc_read();
  if (s == 0) {
    if (encodedSumValue != 0 && (millis() - encodeTimeOut) > lastEncInputtime)
      encodedSumValue = 0;
    lastMovedirection = 0;
    return;
    }
  lastEncInputtime = millis();
  lasttimeknob=millis();        // reset temp to read spectrum
  encodedSumValue += (s > 0 ? 1 : -1);
  if (((encodedSumValue *  encodedSumValue) <= (threshold * threshold)))
    return;
  encodedSumValue = 0;
  if (modeencoder==0)   // tunning
    {
    setFreq(s);         // 5 ms
    }
  else if ((modeencoder>=4) && (modeencoder<=6))  // audiolower, audioupper, audiopitch
    {
    setAudioFilter(modeencoder,s);  
    }
}

boolean firstmem=false;

void doMem()    //
{
  if (getBtnStatus()==1) 
    {
    if (memo.act[mempos]==1)
      {
      setMemMode(0);
      return;
      }
    }
  boolean cambio=false;
  int knob = enc_read();
  if (knob < -1) 
    {
    if (memlin>0) memlin--;
    if (mempos>0) { mempos--; cambio=true; }
    }
  else if (knob > 1) 
    {
    if (memlin<5) memlin++;
    if (mempos<maxMem) { mempos++; cambio=true; }
    }
  if ((cambio) || (firstmem))
    { 
    firstmem=false;
    displayFreq(0,1,1,1);
    displayYN(1,1,0);
    displayMemList(); 
    setMEMtoVFO(mempos); 
    }
}

/* RIT only steps back and forth by 100 hz at a time */
void doRIT(){
  int knob = enc_read();
  unsigned long old_freq = conf.frequency;
  if (conf.tuneStepIndex<6) setSTEP(6);
  if (knob < 0) 
    conf.frequency -= (conf.arTuneStep[conf.tuneStepIndex]);  
  else if (knob > 0) 
    conf.frequency += (conf.arTuneStep[conf.tuneStepIndex]);  
  if (conf.frequency>conf.ritTxFrequency+maxRitdev)conf.frequency=old_freq;
  if (conf.frequency<conf.ritTxFrequency-maxRitdev)conf.frequency=old_freq;
  if (old_freq != conf.frequency){
    setFrequency(conf.frequency);
    displayFreq(0,1,1,1);
    }
}

void showSettings()
{

  s2("-----------------------------------------"); s2(crlf);
  s2("Size of CONF:"); s2(sizeof(conf)); s2(crlf);
  s2("LIBRE0:"); s2(conf.LIBRE0); s2(crlf);
  s2("calibration:"); s2(conf.calibration); s2(crlf);
  s2("usbCarrier:"); s2(conf.usbCarrier); s2(crlf);
  s2("cwmCarrier:"); s2(conf.cwmCarrier); s2(crlf);
  s2("splitOn:"); s2(conf.splitOn); s2(crlf);
  s2("isUSB:"); s2(conf.isUSB); s2(crlf);
  s2("cwMode:"); s2(conf.cwMode); s2(crlf);
  s2("cwSpeed:"); s2(conf.cwSpeed); s2(crlf);
  s2("ritOn:"); s2(conf.ritOn); s2(crlf);
  s2("cwModeA:"); s2(conf.cwModeA); s2(crlf);
  s2("cwModeB:"); s2(conf.cwModeB); s2(crlf);
  s2("attLevel:"); s2(conf.attLevel); s2(crlf);
  s2("sdrModeOn:"); s2(conf.sdrModeOn); s2(crlf);
  s2("TempAlarm:"); s2(conf.TempAlarm); s2(crlf);
  s2("TempTxOff:"); s2(conf.TempTxOff); s2(crlf);
  s2("framemode:"); s2(conf.framemode); s2(crlf);
  s2("frame1mode:"); s2(conf.frame1mode); s2(crlf);
  s2("frame2mode:"); s2(conf.frame2mode); s2(crlf);
  s2("frame3mode:"); s2(conf.frame3mode); s2(crlf);
  s2("vfoActive:"); s2(conf.vfoActive); s2(crlf);
  s2("isUSBA:"); s2(conf.isUSBA); s2(crlf);
  s2("ATUFactor:"); s2(conf.ATUFactor); s2(crlf);
  s2("ATUOffset:"); s2(conf.ATUOffset); s2(crlf);
  s2("sideTone:"); s2(conf.sideTone); s2(crlf);
  s2("cwDelayTime:"); s2(conf.cwDelayTime); s2(crlf);
  s2("delayBeforeCWStartTime:"); s2(conf.delayBeforeCWStartTime); s2(crlf);
  s2("SI5351BX_ADDR:"); s2(conf.SI5351BX_ADDR); s2(crlf);
  s2("cwKeyType:"); s2(conf.cwKeyType); s2(crlf);
  s2("tuneStepIndex:"); s2(conf.tuneStepIndex); s2(crlf);
  s2("commonOption0:"); s2(conf.commonOption0); s2(crlf);
  s2("addrADS1115A:"); s2(conf.addrADS1115A); s2(crlf);
  s2("addrADS1115B:"); s2(conf.addrADS1115B); s2(crlf);
  s2("useHamBandCount:"); s2(conf.useHamBandCount); s2(crlf);
  s2("tuneTXType:"); s2(conf.tuneTXType); s2(crlf);
  s2("isShiftDisplayCWFreq:"); s2(conf.isShiftDisplayCWFreq); s2(crlf);
  s2("hamBandsRange"); s2(crlf);
  for (byte i=0;i<10;i++)
    {
    s2(i);
    s2(":");s2(conf.hamBandName[i]);
    s2(":");s2(conf.hamBandRange[i][0]);
    s2("-"); s2(conf.hamBandRange[i][1]);
  } s2(crlf);
  s2("ifShiftValue:"); s2(conf.ifShiftValue);s2(crlf);
  s2("sMeterLevels (0-15):");
  for (byte i=0;i<16;i++) { s2(":"); s2(conf.sMeterLevels[i]); } s2(crlf);
  s2("userCallsignLength:"); s2(conf.userCallsignLength); s2(crlf);
  s2("CallSign:"); s2(conf.CallSign); s2(crlf);
  s2("WsprMSGCount:"); s2(conf.WsprMSGCount); s2(crlf);
  s2("frequency:"); s2(conf.frequency); s2(crlf);
  s2("frequencyA:"); s2(conf.frequencyA); s2(crlf);
  s2("frequencyB:"); s2(conf.frequencyB); s2(crlf);
  s2("ritRxFrequency:"); s2(conf.ritRxFrequency); s2(crlf);
  s2("ritTxFrequency:"); s2(conf.ritTxFrequency); s2(crlf);
  s2("scaledSMeter:"); s2(conf.scaledSMeter); s2(crlf);
  s2("wifimode:"); s2(conf.wifimode); s2(crlf);
  s2("canalAP:"); s2(conf.canalAP); s2(crlf);
  s2("ssidSTA:"); s2(conf.ssidSTA); s2(crlf);
  s2("passSTA:"); s2(conf.passSTA); s2(crlf);
  s2("ssidAP:"); s2(conf.ssidAP); s2(crlf);
  s2("passAP:"); s2(conf.passAP); s2(crlf);
  s2("EEip:"); for (byte i=0;i<4;i++) { s2(conf.EEip[i]);s2(" "); }  s2(crlf);
  s2("EEgw:"); for (byte i=0;i<4;i++) { s2(conf.EEgw[i]);s2(" "); }  s2(crlf);
  s2("EEmask:"); for (byte i=0;i<4;i++) { s2(conf.EEmask[i]);s2(" "); }  s2(crlf);
  s2("EEdns:"); for (byte i=0;i<4;i++) { s2(conf.EEdns[i]);s2(" "); }  s2(crlf);
  s2("EEdns2:"); for (byte i=0;i<4;i++) { s2(conf.EEdns2[i]);s2(" "); }  s2(crlf);
  s2("EEmac:"); for (byte i=0;i<6;i++) { s2(conf.EEmac[i]);s2(" "); }  s2(crlf);
  s2("myippub:"); s2(conf.myippub); s2(crlf);
  s2("staticIP:"); s2(conf.staticIP); s2(crlf);
  s2("ftpenable:"); s2(conf.ftpenable); s2(crlf);
  s2("webPort:"); s2(conf.webPort); s2(crlf);
  s2("firstIF:"); s2(conf.firstIF); s2(crlf);
  //s2("scanallf:"); s2(conf.scanallf); s2(crlf);
  s2("arTuneStep:"); 
  for (byte i=0;i<9;i++)
    { s2(i);s2(":");s2(conf.arTuneStep[i]); s2(" "); }
  s2(crlf);
  s2("latitude:"); s2(conf.latitud); s2(crlf);
  s2("longitude:"); s2(conf.longitud); s2(crlf);
  s2("lang:"); s2(conf.lang); s2(crlf);

  s2("rstper:"); s2(conf.rstper); s2(crlf);
  s2("usepassDev:"); s2(conf.usepassDev); s2(crlf);
  s2("probecode:"); s2(crlf);
  for (byte i=0;i<8;i++)
    { 
    s2(i);s2(":");
    for (byte j=0;j<8;j++) { s2(conf.probecode[i][j]); s2(" ");  }
    s2(crlf);
    }
  s2("userDev:"); s2(conf.userDev); s2(crlf);
  s2("passDev:"); s2(conf.passDev); s2(crlf);
  s2("iftttenabled:"); s2(conf.iftttenabled); s2(crlf);
  s2("iftttkey:"); s2(conf.iftttkey); s2(crlf);
  s2("dweetenabled:"); s2(conf.dweetenable); s2(crlf);
  s2("iottweetuser:"); s2(conf.dweetuser); s2(crlf);
  s2("iottweetkey:"); s2(conf.dweetkey); s2(crlf);
  s2("mqttenabled:"); s2(conf.mqttenabled); s2(crlf);
  s2("mqttserver:"); s2(conf.mqttserver); s2(crlf);
  s2("mqttpath:");  s2(crlf);
  for (byte i=0;i<6;i++) { s2(conf.mqttpath[i]); s2("/"); } s2(crlf);;
  s2("hostmyip:"); s2(conf.hostmyip); s2(crlf);
  s2("actualBand:"); s2(conf.actualBand); s2(crlf);
  s2("autoWiFi:"); s2(conf.autoWiFi); s2(crlf);
  s2("scanallf:"); s2(conf.scanallf); s2(crlf);
  s2("TXall:"); s2(conf.TXall); s2(crlf);
  s2("autoMode:"); s2(conf.autoMode); s2(crlf);
  s2("calData:");  s2(crlf);
  for (byte i=0;i<5;i++) { s2(conf.calData[i]); s2(" "); } s2(crlf);
  s2("memMode:"); s2(conf.TXall); s2(crlf);
  s2("lastmempos:"); s2(conf.lastmempos); s2(crlf);

  s2("ATUdelay:"); s2(conf.ATUdelay); s2(crlf);
  s2("TXall:"); s2(conf.TXall); s2(crlf);
  s2("nprobe:"); s2(conf.usepassDev); s2(crlf);
  for (byte i=0;i<8;i++)
    { 
    s2(i);s2(":");
    s2(conf.nprobe[i]); s2(" ");  
    }
  s2(crlf);
  s2("============================================="); s2(crlf);
}

/* The settings are read from FILE  */
void initSettings(){
  memset(spval,0,sizeof(spval));
  if (btnDown())
    {
    ////////////////////////////////////////////////////
    tft.drawString("Reset Factory...",0,40);
    s2("Setting default values");s2(crlf);
    saveconf(); // para iniciar fichero 
    showSettings();
    }
  else
    {
    if (!checkfiles()) s2("Some file is missing");s2(crlf);
    int auxread=readconfEEPROM();
    s2("Read readconf():"); s2(auxread); s2(crlf);
    if(readmemo()!=sizeof(memo)) { savememo(); }
    conf.memMode=0;
    conf.EEip[3]=149;
    }
  if (conf.cwKeyType==1) keyerControl &= ~IAMBICB; else if (conf.cwKeyType==2) keyerControl |= IAMBICB;   
    
  //CW Key ADC Range ======= adjust set value for reduce cw keying error
  //by KD8CEC
  
  //Enable / Diable Check for CW Display Cofiguration Group 
  if ((conf.commonOption0 & 0x80) != 0x00)
    {
    //Adjust CW Mode Freq
    shiftDisplayAdjustVal = (conf.isShiftDisplayCWFreq & 0x3F) * 10;
    //check Minus
    if ((conf.isShiftDisplayCWFreq & 0x40) == 0x40)
      shiftDisplayAdjustVal = shiftDisplayAdjustVal * -1;
   //Shift Display Check (Default : 0)
    conf.isShiftDisplayCWFreq = (conf.isShiftDisplayCWFreq & 0x80) == 0?1:0;
    }
   //Stored IF Shift Option
  if ((conf.commonOption0 & 0x40) != 0x00)
    {
    isIFShift = conf.ifShiftValue != 0;
    }

  //default Value (for original hardware)
  if (conf.cwAdcSTFrom >= conf.cwAdcSTTo)
    {
    conf.cwAdcSTFrom = 0;
    conf.cwAdcSTTo = 50;
    }

  if (conf.cwAdcBothFrom >= conf.cwAdcBothTo) { conf.cwAdcBothFrom = 701; conf.cwAdcBothTo = 900;  }
  if (conf.cwAdcDotFrom >= conf.cwAdcDotTo) { conf.cwAdcDotFrom = 901; conf.cwAdcDotTo = 1300; }
  if (conf.cwAdcDashFrom >= conf.cwAdcDashTo) { conf.cwAdcDashFrom = 1800; conf.cwAdcDashTo = 2300; }
  //end of CW Keying Variables
  
  if (conf.cwDelayTime < 1 || conf.cwDelayTime > 250)  conf.cwDelayTime = 60;
//  if (conf.vfoA_mode < 2) conf.vfoA_mode = 2;   // LSB default
//  if (conf.isUSBB < 2) conf.isUSBB = 3;   // USB default
  
  //original code with modified by kd8cec
//  if (conf.usbCarrier > 11060000l || conf.usbCarrier < 11048000l)
  if (conf.usbCarrier > 12010000l || conf.usbCarrier < 11048000l)
    conf.usbCarrier = 11056000l;   // EA4GZI
//  if (conf.cwmCarrier > 11060000l || conf.cwmCarrier < 11048000l)
  if (conf.cwmCarrier > 12010000l || conf.cwmCarrier < 11048000l)
    conf.cwmCarrier = 11056000l;    // EA4GZI
  if (conf.frequencyA > 35000000l || 3500000l > conf.frequencyA) {
     conf.frequencyA = 7150000l;
     conf.isUSBA = 0; //LSB
    }
  if (conf.frequencyB > 35000000l || 3500000l > conf.frequencyB) {
    conf.frequencyB = 14150000l;  
    conf.isUSBB = 1; //USB
    }
  //end of original code section
  
  if (conf.sideTone < 100 || 2000 < conf.sideTone) conf.sideTone = 800;
  if (conf.cwSpeed < 10 || 1000 < conf.cwSpeed) conf.cwSpeed = 100;
  if (conf.sideTone < 300 || conf.sideTone > 1000) {
    sideTonePitch = 0;
    sideToneSub = 0;;
    }
  else{
    sideTonePitch = (conf.sideTone - 300) / 50;
    sideToneSub = conf.sideTone % 50;
    }
  initButtons();
}

void initPorts(){
  pinMode(ENC_A, INPUT_PULLUP);   // en ESP32 necesita pullup externo
  pinMode(ENC_B, INPUT_PULLUP);   // en ESP32 necesita pullup externo
  pinMode(FBUTTON, INPUT_PULLUP); // en ESP32 necesita pullup externo
  pinMode(PTT, INPUT_PULLUP);     // en ESP32 necesita pullup externo
  pinMode(ANALOG_KEYER, INPUT);   // Keyer

  //pinMode(CW_TONE, OUTPUT);    digitalWrite(CW_TONE, 0);
  pinMode(TX_RX,OUTPUT);       digitalWrite(TX_RX, 0);
  pinMode(TX_LPF_A, OUTPUT);   digitalWrite(TX_LPF_A, 0);
  pinMode(TX_LPF_B, OUTPUT);   digitalWrite(TX_LPF_B, 0);
  pinMode(TX_LPF_C, OUTPUT);   digitalWrite(TX_LPF_C, 0);
  pinMode(TX_LPF_D, OUTPUT);   digitalWrite(TX_LPF_D, 0);
  pinMode(CW_KEY, OUTPUT);     digitalWrite(CW_KEY, 0);
}

void connectSTA()
{
  s2("  Connecting: ");  s2(conf.ssidSTA);
  s2("/");  s2(conf.passSTA);
  displayMsg("Connecting a",conf.ssidSTA,"********",40,140,210,75);
  byte cont = 0;
  while((!WiFi.isConnected()) && (cont++ < 20))  { delay(500); s2("."); }
  clearMsg(40,140,210,75);
  displayWiFiSt();
  s2(WiFi.isConnected()?" OK":" ERROR"); s2(crlf);
  s2("  STA IP: ");  s2(WiFi.localIP()); s2(crlf);
  s2("  STA MAC: ");  s2(WiFi.macAddress()); s2(crlf);
  s2("  Subnet Mask: ");  s2(WiFi.subnetMask()); s2(crlf);
  s2("  Gateway IP: "); s2(WiFi.gatewayIP()); s2(crlf);
  s2("  DNS: ");  s2(WiFi.dnsIP()); s2(crlf);
}

void readVIpower()
{
  int16_t vtotadc=adsB.readADC_SingleEnded(VTOTp);    // es el valor leído del ADC sin convertir
  int16_t itotadc=adsB.readADC_SingleEnded(ITOTp);    // es el valor leído del ADC sin convertir
  float factorv=290.0;    // factor teórico
  float factori=1000.0;   // factor teórico
  float factorvr=1.0;     // factor corrección
  float factorir=0.75;   // factor corrección
  vtotvalue=factorvr*vtotadc*0.1875/factorv;
  itotvalue=-(factorir)*(itotadc*0.1875-2500)*10/factori;
}

void sendTemp()
{
  sendData(tcptemp1); 
  sendData(tcptemp2); 
  sendData(tcptemp3); 
}

void task1() {
  setvhffreqrx(435000);

  tini=millis();
  //readVIpower();
  countfaulttime++;   // si se hace mayor que TempDesactPrg,desactiva ejecucion programas dependientes de fecha
  if (inTx==1) {
    leevaloresOW();
    sendTemp();
    }
  //if ((tftpage!=21) && (tftpage!=22) &&(tftpage!=23))
  if (tftpage==0)
    displayStatus();
  if(conf.rstper>0) { if(millis() > 3600000*conf.rstper) { Serial2.println("RESTART"); ESP.restart();  } }
  mact1=millis();
  //sendFreq();   // NO DEBERÍA SER NECESARIO
  sendData(tcpvtotvalue);
  sendData(tcpitotvalue);
}

void mqttcallback(char* topic, uint8_t* payload, unsigned int length) 
{
  String msg="";
  for (byte j=0; j<length;j++) msg+=(char)payload[j];
  //if (strstr(topic,"/RST")!=NULL) {  ESP.restart(); }  // recibido RST, resetear
}

#define TOPICROOT "ubitx/"
char mqtttopicip[50] = TOPICROOT;
char mqtttopictcpport[50] = TOPICROOT;
char mqtttopicwebport[50] = TOPICROOT;
char mqtttopicudpSport[50] = TOPICROOT;
char mqtttopicudpFport[50] = TOPICROOT;

void initPubSub()
{
  PSclient.setServer(conf.mqttserver, 1883); 
  PSclient.setCallback(mqttcallback); 
  strcat(mqtttopicip,conf.CallSign);  strcat(mqtttopicip,"/ip");
  strcat(mqtttopictcpport,conf.CallSign);  strcat(mqtttopictcpport,"/tcpport");
  strcat(mqtttopicwebport,conf.CallSign);  strcat(mqtttopicwebport,"/webport");
  strcat(mqtttopicudpSport,conf.CallSign);  strcat(mqtttopicudpSport,"/udps");
  strcat(mqtttopicudpFport,conf.CallSign);  strcat(mqtttopicudpFport,"/udpf");
}

void initNetServices()
{
  task1();
  if ((conf.wifimode>0))  // 
    {
    Serial2.println("IP services");
    if (conf.ftpenable)
      {
      initFTP(); s2("  FTP server started, port "); s2(conf.ftpPort); Serial2.println(" admin/admin");  
      }
    else
      Serial2.println("  FTP server disabled");
    if (conf.webenable)
      {  
      initWebserver(); s2("  Web server started, port "); s2(conf.webPort);
      }
    else
      s2("  WEB server disabled");
    s2(crlf);
    if (conf.tcpenable)
      {
      initTCPS(); s2("  TCP server started, port "); s2(conf.tcpPort); 
      }
    else
      s2("  TCP server disabled");
    s2(crlf);
    if (conf.udpenable)
      { initUDPS(); 
        s2("  UDP-S service started, port "); s2(conf.udpPortSmeter);s2(crlf); 
        s2("  UDP-F service started, port "); s2(conf.udpPortFreq); }
    else
      s2("  UDP server disabled");
    s2(crlf);
    if (conf.debugenable)
      { initremoteDebug();  s2("  Debug service started, port ");s2(conf.debugPort); }
    else
      s2("  Debug server disabled");
    s2(crlf);
    }
  if ((conf.wifimode==1) || (conf.wifimode==3))  // STA o AP+STA
    {
    int auxI=checkInternet();  s2("  Check Internet: ");s2(auxI==HTTP_CODE_OK?"OK":"ERROR");s2(crlf);
    internetON=(auxI==HTTP_CODE_OK); 
    if (internetON)
      {
      s2("Internet services");s2(crlf);
      initTime();       s2("  initTime");s2(crlf);
      int auxR=checkMyIP();   
         s2("  MyIP server: "); s2(conf.hostmyip);
         s2(auxR==HTTP_CODE_OK?" OK ":" ERROR "); s2(conf.myippub); s2(crlf);
      }
    senddweet();
    }
  if (conf.mqttenabled==1)
    {
    initPubSub();  
    }
  else
    {
      
    }
}


void initWiFi() {
  if(conf.wifimode==0) { WiFi.mode(WIFI_OFF); }
  else if (conf.wifimode==1) { WiFi.mode(WIFI_STA); }
  else if (conf.wifimode==2) { WiFi.mode(WIFI_AP); }
  else if (conf.wifimode==3) { WiFi.mode(WIFI_AP_STA); }
  else { WiFi.mode(WIFI_AP); }
  
  WiFi.setAutoConnect(true);
  WiFi.setAutoReconnect(true);
  s2("WiFiMode: "); s2(wifimodetext[conf.wifimode]);s2(crlf);
  WiFi.enableAP((conf.wifimode==2) || (conf.wifimode==3));
  if ((conf.wifimode==2) || (conf.wifimode==3)) {       // AP o AP+STA
    WiFi.channel(conf.canalAP);
    WiFi.softAP(conf.ssidAP, conf.passAP, conf.canalAP, false, 2);
    s2("AP mode:");s2(crlf);
    s2("  Channel:");s2(WiFi.channel());s2(crlf);
    s2("  AP MAC:"); s2(WiFi.softAPmacAddress());s2(crlf);
    s2("  IP:");s2(WiFi.softAPIP());s2(crlf);
    }
  if ((conf.wifimode==1) || (conf.wifimode==3))        // STA o AP+STA
    {
    s2("STA mode:");s2(crlf);
    if(conf.staticIP==1) { 
      WiFi.config(conf.EEip, conf.EEgw, conf.EEmask, conf.EEdns, conf.EEdns2); 
        }
//      WiFi.begin(conf.ssidSTA, conf.passSTA);
      WiFi.begin(conf.ssidSTA, conf.passSTA, true);
      s2("  Static IP:");s2(conf.staticIP?"YES ":"NO ");
      for(byte i=0; i<4; i++) { s2(conf.EEip[i]); s2("."); }  s2(crlf);
      s2("  Gateway:"); for(byte i=0; i<4; i++) { s2(conf.EEgw[i]); s2("."); } s2(crlf);
      s2("  EEmask:"); for(byte i=0; i<4; i++) { s2(conf.EEmask[i]); s2("."); } s2(crlf);
      s2("  EEdns:"); for(byte i=0; i<4; i++) { s2(conf.EEdns[i]); s2("."); } s2(crlf);
      s2("  EEdns2:"); for(byte i=0; i<4; i++) { s2(conf.EEdns2[i]); s2("."); } s2(crlf);
    }
  if (conf.autoWiFi==1)
    {
    if ((conf.wifimode==1) || (conf.wifimode==3))
      {
      connectSTA();
      updateDisplay(1);
      }
    }
}

void initTime() {
  ///// Time
  timeClient.begin();
  timeClient.setTimeOffset(timezoneval[conf.timezone]);
  if(timeClient.update()==1)
    { countfaulttime=0; setTime(timeClient.getEpochTime()); }
  else 
    { s2("timeclient.update:"); s2(timeClient.update()); }
}

void initWebserver() { initHTML();  s2("  HTML server started"); s2(crlf); server.begin(); } 
void initFTP() {ftpSrv.begin("admin","admin");  }  //username, password for ftp.  set ports in ESP8266FtpServer.h  (default 21, 50009 for PASV)
void handleWebclient() { server.handleClient(); }
void handleFTP() { ftpSrv.handleFTP(); }
void initSPIFSS(boolean testfiles, boolean format) {
  s2("SPIFFS");s2(b);
  if (SPIFFS.begin(format)) { s2(OK); } else  { s2(ERROR); }
  s2(crlf);
  if (testfiles) {
    File dir=SPIFFS.open("/");
    File file=dir.openNextFile();
    while(file) 
      {
      s2(b); s2(file.name()); s2(b); s2(file.size());s2(crlf); 
      file.close();
      file=dir.openNextFile(); 
      }
  }
}

void printhelp()
{
  //s2("alias,aliasdevice      (set alias)");s2(crlf);
  //s2("debug                  (debug ON/OFF)");s2(crlf);
  s2("a               (auto WiFi: yes/no)");s2(crlf);
  s2("f               (check files)");s2(crlf);
  s2("F               (reset factory)");s2(crlf);
  s2("h               (help)");    s2(crlf);
  s2("p,SSIDpass      (set passSTA)");s2(crlf);
  s2("P,SSID-APpass   (set pass AP)");s2(crlf);
  s2("r               (reset)"); s2(crlf);   
  s2("s,SSIDname      (set SSID STA)");s2(crlf);
  s2("S,SSID-AP       (set SSID AP)");s2(crlf);
  s2("t               (format SPIFFS)");s2(crlf);
  s2("u               (show status)");s2(crlf);
  s2("w,n             (wifimode:0,1,2,3=OFF,STA,AP,STA+AP)");s2(crlf);
  s2("---------------------------------------");s2(crlf);
}

void printstatus()
{
  s2("WiFi Mode: "); s2(conf.wifimode==0?"OFF":conf.wifimode==1?"STA":conf.wifimode==2?"AP":"AP+STA");s2(crlf);
  s2("IP: ");        s2(WiFi.localIP());s2(crlf);
  s2("Port: ");      s2(conf.webPort);s2(crlf);
  s2("mask: ");      s2(WiFi.subnetMask());s2(crlf);
  s2("GW: ");        s2(WiFi.gatewayIP());s2(crlf);
  s2("ssid: ");      s2(conf.ssidSTA);s2(crlf);
  s2("pass: ");      s2(conf.passSTA);s2(crlf);
  s2("ssidAP: ");    s2(conf.ssidAP);s2(crlf);
  s2("passAP: ");    s2(conf.passAP);s2(crlf);
  s2("autoWiFi: ");   s2(conf.autoWiFi==0?"NO":"YES");s2(crlf);
  s2("Conn: ");      s2(WiFi.isConnected()?ok:"NO");s2(crlf);
  s2("----------------");s2(crlf);
}

void execcomserial() {
  char command=cinput;
  String param;
  param=sinput.substring(sinput.indexOf(",")+1);
  boolean done=false;
  if (command=='a') { conf.autoWiFi=conf.autoWiFi==0?1:0; done=true; }
  if (command=='A') { setVCAP(CAP1, param.toInt()); done=true; }
  if (command=='B') { setVCAP1(CAP1, param.toInt()); done=true; }
  else if( command=='f') {
    File dir=SPIFFS.open(barra);
    File file=dir.openNextFile();
    while(file) { s2(file.name()); s2(b); s2(file.size()); s2(crlf); file=dir.openNextFile(); }
    s2("files ");
    s2(checkfiles()?"OK":"ERROR"); s2(crlf);} 
  else if (command=='F') { initFab(); }
  else if (command=='h') { printhelp(); } 
  else if (command=='p') { param.toCharArray(conf.passSTA,20); done=true; }
  else if (command=='P') { param.toCharArray(conf.passAP,20); done=true; }
  else if (command=='r') { ESP.restart(); }
  else if (command=='s') { param.toCharArray(conf.ssidSTA,20); done=true; }
  else if (command=='S') { param.toCharArray(conf.ssidAP,20); done=true; }
  else if (command=='t') { SPIFFS.format(); }
  else if (command=='u') { printstatus(); } 
  else if (command=='w') { if ((param.toInt()>=0) &&(param.toInt()<=3)) { conf.wifimode=param.toInt(); done=true; }}
  else if (command=='c') { curvemode(); }
  else { s2("Command not found"); s2(crlf); printhelp(); }
  if (done) { saveconf(); s2("Done"); s2(crlf); printstatus(); }
}

void handleSerial()
{
  if (Serial2.available()) 
    {
    boolean ejec=false;
    char tChar = Serial2.read(); 
    if (tChar=='\r')          // si es #13, CR
      {
      char tCharTrash=Serial2.read();       // leemos #10 y ejecutamos
      ejec=true;
      }
    else if (tChar=='\n') 
      { ejec=true; }    // si es #10, hemos terminado y ejecutamos
    else                    // si no es #10, lo añadimos
      {
      if (cinput=='\0') { cinput=tChar; }   // nuevo comando
      else { sinput=sinput+tChar; }         // añadir al parámetro
      } 
    if (ejec)
      {
      if (conf.serial2Mode==0)    // modo debug
        execcomserial();
      else
        {
        if ((conf.connMode==1) || (conf.connMode==2) || (conf.connMode==3))  // modo IP or Serial2-IP, mod Manager
          handleRecDataIP(cinput, sinput);  
        }
      sinput=""; cinput='\0';  
      }
    }
}

void initTone()
{
  ledcSetup(0,conf.sideTone,12);
  //ledcAttachPin(CW_TONE,0);
}

void initDS18B20() {
  sensors0.begin();
  sensors0.setResolution(9);
  nTemp=sensors0.getDeviceCount();
  if(nTemp>maxTemp) { nTemp=maxTemp; }
  s2("DS18B20 probes"); s2(crlf);
  s2(b); s2(b); s2(t(sondastemp));  s2(dp);
  s2(nTemp); s2(crlf); s2(b); s2(b); s2(t(tModo));  s2(dp);
  s2(b); s2(b); s2((sensors0.isParasitePowerMode())?c(tparasite):c(tpower));  s2(crlf);
  for(byte i=0; i<maxTemp; i++)       {   // busca sondas conectadas
    if (sensors0.getAddress(addr1Wire[i], i))    {
      s2(b);s2(b);
      for(uint8_t j=0; j<8; j++) { if(addr1Wire[i][j]<16) { s2(cero); } s2(addr1Wire[i][j]); }
      s2(crlf);
      }
    }
  s2("  Started");s2(crlf);
  leevaloresOW();     s2("  Values read");s2(crlf);}

void initADS()
{
  adsA.begin(0x48);
  //adsA.setGain(GAIN_TWOTHIRDS);  // +/- 6.144V  1 bit = 0.1875mV (default)
  adsB.begin(0x49);
  //adsB.setGain(GAIN_TWOTHIRDS);  // +/- 6.144V  1 bit = 0.1875mV (default)
}

void initI2C()
{
  Wire.begin(SDA,SCL);
  Wire.setClock(WIRE_CLOCK);
}

void setCAP(uint8_t nservo, int16_t angle)
{
  uint16_t tof = SERVOMIN + angle * (SERVOMAX-SERVOMIN) / 180; 
  servos.setPWM(nservo, 0, tof);
}

void setVCAP(uint8_t nservo, int16_t value)
{
  servos.setPWM(nservo, 0, value);

}
void setVCAP1(uint8_t nservo, int16_t value)
{
  servos.setPWMFreq(value);  // This is the maximum PWM frequency
}

void initATU()
{
 // SERVOS
  servos.begin();
  servos.setOscillatorFrequency(27000000);
  servos.setPWMFreq(SERVO_FREQ);  // Analog servos run at ~50 Hz updates
  delay(10);
  Serial2.print("posATUC1:"); Serial2.println(conf.posATUC1);
  Serial2.print("posATUC1:"); Serial2.println(conf.posATUC1);
  setCAP(CAP1, conf.posATUC1);
  setCAP(CAP2, conf.posATUC2);
}

void initremoteDebug()        //  REMOTE DEBUG
{
    String hostNameWifi = HOST_NAME;
    hostNameWifi.concat(".local");
#ifdef USE_MDNS  // Use the MDNS ?
    if (MDNS.begin(HOST_NAME)) {
        s2("* MDNS responder started. Hostname -> ");
        s2(HOST_NAME);
    }
    MDNS.addService("telnet", "tcp", 23);
#endif
  // Initialize RemoteDebug
  Debug.begin(HOST_NAME);         // Initialize the WiFi server
  Debug.setResetCmdEnabled(true); // Enable the reset command
  Debug.showProfiler(true);       // Profiler (Good to measure times, to optimize codes)
  Debug.showColors(true);         // Colors
}

void sintATU()
{
  clearTFT();
  s2("Tunning"); s2(crlf);
  setCAP(CAP1,180);
  setCAP(CAP2,180);
  delay(500);
  swrmin=999; posmin1=180; 
  for (int i=180; i>0; i--) {   // goes from 0 degrees to 180 degrees
    Serial2.print("i: "); Serial2.print(i);
    Serial2.print(" SWR: "); Serial2.print(SWR);
    displaySWR(1);
    setCAP(CAP1,i);
    if (SWRreal<swrmin) 
      { 
      posmin1=i-1; swrmin=SWRreal; 
      }
    Serial2.print(" posmin1: "); Serial2.println(posmin1);
    }
  delay(100);
  conf.posATUC1=posmin1;
  //cap1.write(conf.posATUC1);            // tell servo to go to position in variable 'pos'
  saveconf();
  analogWrite(3,128);

  // cap 2
  swrmin=999; posmin2=180;
  for (int i=180; i>0; i--) {   // goes from 0 degrees to 180 degrees
    Serial2.print("i: "); Serial2.print(i);
    Serial2.print(" SWR: "); Serial2.print(SWR);
    displaySWR(1);
    setCAP(CAP2,i);
    if (SWRreal<swrmin) 
      { 
      posmin2=i-1; 
      swrmin=SWRreal; 
      }
    Serial2.print(" posmin1: "); Serial2.println(posmin1);
    }
  delay(100);
  conf.posATUC2=posmin2;
  saveconf();
  //cap2.write(conf.posATUC2);            // tell servo to go to position in variable 'pos'
  delay(500);
}

void deleteMemo()
{
  memset(buffmemo,0,sizeof(memo));
  savememo(); 
}

void initTPA2016()
{
  audioamp.begin();
  audioamp.setAGCCompression(TPA2016_AGC_OFF);  // Turn off AGC for the gain test
  audioamp.setReleaseControl(0);    // we also have to turn off the release to really turn off AGC
  // See Datasheet page 23 for value -> ms conversion table
  audioamp.setAttackControl(5);
  setTPAGain(30);
  setTPAenable(conf.TPA2016enabled);
  audioamp.setGain(conf.TPA2016Gain);
}

void initTCPS() { tcpserver.begin(conf.tcpPort); }

void handleUDPpacket(AsyncUDPPacket packet)
{
  s2("handleUDPpacket------------------------"); s2(crlf);
  s2(", Data: "); //  for (int i=0;i<packet.length();i++); s2(packet.data());
}

void initUDPS()
{
 /** if(udp.listen(udpPort)) {
      s2("UDP Listening on IP: ");  s2(WiFi.localIP());
      udp.onPacket([](AsyncUDPPacket packet) {
          //printhora();
          s2("UDP Packet Type: ");
          s2(packet.isBroadcast()?"Broadcast":packet.isMulticast()?"Multicast":"Unicast");
          s2(", From: "); s2(packet.remoteIP());
          s2(":"); s2(packet.remotePort());
          s2(", To: ");  s2(packet.localIP());
          s2(":");  s2(packet.localPort());
          s2(", Length: "); s2(packet.length());
          s2(", Data: "); Serial2.write(packet.data(), packet.length());
           S2(crlf);
          //reply to the client
          packet.printf("Got %u bytes of data", packet.length());
      });
  }**/
}

void initWS() { wsserver.listen(conf.wsPort); }

void initLyraT()
{
  sendVolumeLyraT(conf.audiovolume); 
  delay(200);
  sendFilterLyraT(conf.audiolower,conf.audioupper);
  delay(200);
  sendGainLyraT(conf.audiogain);
  delay(200);
  sendSpectrumAttLyraT(conf.spatt);
  delay(200);
  sendSpectrumSpanLyraT(conf.spspan);
  delay(200);

  delay(200);
}

void setup()
{
  initSerial2(115200);  
  EEPROM.begin(EEPROM_SIZE);
  initConf();
  conf.connMode=1;
  s2("========== sBitx32  Init =========");s2(crlf);
  Init_Cat(38400, SERIAL_8N1);  s2("  Serial 1 started");s2(crlf);
  s2("  Serial 2 started"); s2(crlf);
  s2("  Vers.:"); s2(FIRMWARE_VERSION_INFO); s2(crlf);
  delay(10);
  Serial2.print("CPU clock:"); Serial2.println(getCpuFrequencyMhz()); //Get CPU clock
  initTFT();          s2("TFT started");s2(crlf);
  DisplayVersionInfo(FIRMWARE_VERSION_INFO);
  initSPIFSS (true,true);  
  initPorts();        s2("Ports OK");  //s2(crlf);
  initSettings();    
  conf.attLevel=0;      //
  conf.ifShiftValue=0;   //
  byte auxconnMode=conf.connMode;
  conf.connMode=1;
 // showSettings();
  initI2C();          s2("I2C started");s2(crlf);
  initOscillators();  s2("Oscillators started");s2(crlf);
  initTone();         s2("Pin Tone OK");s2(crlf);
  setFrequency(conf.frequency);  
  initDS18B20();   
  initADS();          s2("ADS1115 started");s2(crlf);
  initATU();          s2("ATU started");s2(crlf);
  updateDisplay(1);
  initWiFi();  
  if (WiFi.isConnected())       
    initNetServices();
  initTPA2016();      s2("TPA2016 started"); s2(crlf);
  //initDecodeCW();     s2("initDecodeCW");s2(crlf);
  smetervalue=0;  maxsmeter=0;  minsmeter=32000;
  int8_t power;
  int auxerr=esp_wifi_get_max_tx_power(&power);
  Serial2.print("radio power:");Serial2.println(power);
  initLyraT();
  
  s2("END SETUP");s2(crlf);  
  s2("============================");s2(crlf);
  s2("Type 'h' to help"); s2(crlf); 
  s2("----------------------------");s2(crlf);
  conf.connMode=auxconnMode;
//#define VERIFY_TFT
#if defined VERIFY_TFT
#if defined ESP32
  Serial2.print("ESP32:"); Serial2.println("defined");
#endif  
  Serial2.print("BITX: "); Serial2.println(BITX);
  Serial2.print("TFT_RST: "); Serial2.println(TFT_RST);
  Serial2.print("TFT_DC: "); Serial2.println(TFT_DC);
  Serial2.print("TFT_MISO: "); Serial2.println(TFT_MISO);
  Serial2.print("TFT_MOSI: "); Serial2.println(TFT_MOSI);
  Serial2.print("TFT_SCLK: "); Serial2.println(TFT_SCLK);
  Serial2.print("TFT_CS: "); Serial2.println(TFT_CS);
  Serial2.print("TOUCH_CS: "); Serial2.println(TOUCH_CS);
#endif
}

void ICACHE_FLASH_ATTR leevaloresOW()
{
  sensors0.requestTemperatures();
  for (byte i=0; i<maxTemp; i++)  
    {
    if (conf.nprobe[i]>0)
      {
      int auxI=sensors0.getTempC(conf.probecode[i])*100;
      if (auxI>0)
        {
        MbR[i]=auxI;
        MbRant[i]=MbR[i];
        }
      }
    }
}

void readSmeter( ) 
{ 
  int16_t smeteradc=0;
  smeteradc=adsA.readADC_SingleEnded(SMETERp);    // es el valor leído del ADC sin convertir
  calSmeterReq=calSmeterReq || (smeteradc<conf.sMeterLevels[0]) || (smeteradc>conf.sMeterLevels[15]);
  smeteradc=smeteradc>smeterlast?((smeterlast*7+smeteradc*3))/10:   // valor ajustado para evitar variaciones rápidas
                                 ((smeterlast*9+smeteradc*1))/10;   // de VK2ETA
  smeterlast=smeteradc;     // guardar último valor leído
  if (smeteradc<=minsmeter) { minsmeter=smeteradc; }    // buscar mínimo      
  if (smeteradc>=maxsmeter) { maxsmeter=smeteradc; }    // buscar máximo    

  // convertir valor a escala 0-90
  byte i=0; boolean encontrado=false;
  while ((i<16) && (!encontrado))           // busca intervalo de 0 a 16
    {
    if (smeteradc<conf.sMeterLevels[i]) 
      encontrado=true;
    else
      i++;
    }
  if (i<15)
    smetervalue=((i-1)*6) + (smeteradc-conf.sMeterLevels[i])*6 / (conf.sMeterLevels[i]-conf.sMeterLevels[i-1]);
  else
    smetervalue=90; 

  if (WiFi.isConnected()) {
    //Send a packet
    sendData(tcpsmeter);
    //udpsmeter.beginPacket(udpAddress,conf.udpPortSmeter);
    //udpsmeter.printf("%lu",smetervalue);
    //udpsmeter.endPacket();
    }
}

void readCW() { cwcodevalue=adsB.readADC_SingleEnded(3); }

void task01() {
  tini=millis();
  if ((inTx==0) && (tftpage==0) && (conf.framemode==1))
    {
    displaySmeter(190,210,50,1); 
    }
  if (tftpage==23) { updateDisplay(0); }
  if ((tftpage==0) && (conf.framemode==0))
    {
    getSpectrumLyraT();
    displaySpectrum(10,135,256,70);
    displayWaterfall(10,140,256,MAXLINESWATERFALL);
    }
  mact01=millis();
}

void sendData(byte c)
{
  //if (keylock==1) return;
  char data[60];
  if (c==tcpsplitOn) { strcpy(data,itoa(conf.splitOn,buff,10)); }
  else if (c==tcpisUSB) { strcpy(data,itoa(conf.isUSB,buff,10)); }
  else if (c==tcpritOn) { strcpy(data,itoa(conf.ritOn,buff,10)); }
  else if (c==tcpcwMode) { strcpy(data,itoa(conf.cwMode,buff,10)); }
  else if (c==tcpvfoActive) { strcpy(data,itoa(conf.vfoActive,buff,10)); }
  else if (c==tcpisusbA) { strcpy(data,itoa(conf.isUSBA,buff,10)); }
  else if (c==tcpisusbB) { strcpy(data,itoa(conf.isUSBB,buff,10)); }
  else if (c==tcpcwmodeA) { strcpy(data,itoa(conf.cwModeA,buff,10)); }
  else if (c==tcpcwmodeB) { strcpy(data,itoa(conf.cwModeB,buff,10)); }
  else if (c==tcpCallSign) { strcpy(data,conf.CallSign); }
  else if (c==tcpfrequency) { strcpy(data,itoa(conf.frequency,buff,10)); }
  else if (c==tcpfrequencyA) { strcpy(data,itoa(conf.frequencyA,buff,10)); }
  else if (c==tcpfrequencyB) { strcpy(data,itoa(conf.frequencyB,buff,10)); }
  else if (c==tcpwifi) { strcpy(data,WiFi.isConnected()?"1":"0"); }
  else if (c==tcpscanst) { strcpy(data,itoa(scanF,buff,10)); }
  else if (c==tcpkeylock) { strcpy(data,itoa(keylock,buff,10)); }
  else if (c==tcptemp1) { strcpy(data,itoa(MbR[0],buff,10)); }
  else if (c==tcptemp2) { strcpy(data,itoa(MbR[1],buff,10)); }
  else if (c==tcptemp3) { strcpy(data,itoa(MbR[2],buff,10)); }
  else if (c==tcptunestep) { strcpy(data,itoa(conf.tuneStepIndex,buff,10)); }
  else if (c==tcpminsmeter) { strcpy(data,itoa(minsmeter,buff,10)); }
  else if (c==tcpmaxsmeter) { strcpy(data,itoa(maxsmeter,buff,10)); }
  else if (c==tcpattlevel) { strcpy(data,itoa(conf.attLevel,buff,10)); }
  else if (c==tcpifShiftVal) { strcpy(data,itoa(conf.ifShiftValue,buff,10)); }
  else if (c==tcpvtotvalue) { dtostrf(vtotvalue,8,2,data); }
  else if (c==tcpitotvalue) { dtostrf(itotvalue,8,2,data); }
  else if (c==tcpmemmode) { strcpy(data,itoa(conf.memMode,buff,10)); }
  else if (c==tcpsmeter) { strcpy(data,itoa(smetervalue,buff,10)); }
  else { strcpy(data,"999"); }
  if ((conf.connMode==1) || (conf.connMode==2))   // modos IP, enviar por Client
    {
    tcpclient.write(c); 
    if (c==tcpMEM) { tcpclient.write(buffmemo,sizeof(memo)); }
    else if (c==tcpALL) { tcpclient.write(buffconf,sizeof(conf)); }
    else { tcpclient.write(data); tcpclient.write('\n'); }
    }
}

void task10() {
  tini=millis();
  if (inTx==0) leevaloresOW();
  sendTemp();
  sendData(tcpminsmeter);
  sendData(tcpmaxsmeter);
  displayWiFiSt();
  //sendstatus(1);
  if (conf.mqttenabled) 
    {
    if (!PSclient.connected()) 
      mqttreconnect();
    if (PSclient.connect(mqttclientID.c_str())) 
      { 
      PSclient.publish(mqtttopicip, conf.myippub);
      PSclient.publish(mqtttopictcpport, itoa(conf.tcpPort,buff,10));
      PSclient.publish(mqtttopicwebport, itoa(conf.webPort,buff,10));
      PSclient.publish(mqtttopicudpSport, itoa(conf.udpPortSmeter,buff,10));
      PSclient.publish(mqtttopicudpFport, itoa(conf.udpPortFreq,buff,10));
      }
    } 
  mact10=millis();
}

void task3600() {
  tini=millis();
  senddweet();
  mact3600=millis();
}

void drawX(int x, int y)
{
  tft.drawLine(x-5, y-5, x+5, y+5, TFT_WHITE);
  tft.drawLine(x-5, y+5, x+5, y-5, TFT_WHITE);
}

void onDataReceived(String &data)
{
  s2(data);
}

void procesaWSrec(String datarec)
{
  if(datarec=="f")
    {
    s2(ltoa(conf.frequency,buff,10));
    wsclient.send(ltoa(conf.frequency,buff,10));     // send data
    }
  else 
    {
    s2(datarec);
    wsclient.send(datarec);     // send data
    }
  s2(crlf);
}

void sendstatus(byte todo)
{
  if (!tcpclient.connected()) return;
  if (todo==1) 
    {
    Serial2.print(0); 
    sendData(tcpritOn); Serial2.print(1);
    sendData(tcpsplitOn); Serial2.print(2);
    sendData(tcpisUSB); Serial2.print(3);
    sendData(tcpcwMode); Serial2.print(4);
    sendData(tcpvfoActive); Serial2.print(5);
    sendData(tcpisusbA); Serial2.print(6);
    sendData(tcpisusbB); Serial2.print(7);
    sendData(tcpCallSign);  Serial2.print(8);
    sendData(tcpfrequency);  Serial2.print(9);
    sendData(tcpfrequencyA);  Serial2.print(10);
    sendData(tcpfrequencyB);  Serial2.print(11);
    sendData(tcpscanst);  Serial2.print(12);
    sendData(tcpkeylock);  Serial2.print(13);
    sendData(tcptemp1);  Serial2.print(14);
    sendData(tcptemp2);  Serial2.print(15);
    sendData(tcptemp3);  Serial2.print(16);
    sendData(tcptunestep);  Serial2.print(17);
    sendData(tcpwifi);  Serial2.print(18);
    sendData(tcpattlevel);  Serial2.print(19);
    sendData(tcpifShiftVal);  Serial2.print(20);
    sendData(tcpvtotvalue); Serial2.print(21); 
    sendData(tcpitotvalue);  Serial2.print(22);
    }
}

void handleRecDataIP(char c, String data)
{
  byte b=(byte)c;
  if (c==tcpsplitOn) { setSPLIT(data.toInt()); }         // 3
  else if (c==tcpisUSB) { setUSB(data.toInt()); }        // 4
  else if (c==tcpcwMode) { setCW(data.toInt()); }        // 5
  else if (c==tcpritOn) { setRIT(data.toInt()); }        // 8
  else if (c==tcpvfoActive) { setVFO(data.toInt()); sendData(tcpfrequencyA); sendData(tcpfrequencyB); }    // 19
  else if (c==tcpbanddn) { setNextHamBandFreq(conf.frequency,-1); } // 40
  else if (c==tcpbandup) { setNextHamBandFreq(conf.frequency,1); }  // 41
  else if (c==tcpfreqdn) { setFreq(-1); }                // 42
  else if (c==tcpfrequp) { setFreq(1); }                 // 43
  else if (c==tcpfrequency) { conf.frequency=data.toInt(); setFreq(0); } // 45
  else if (c==tcptunestep) { setSTEP(data.toInt()); }    // 51
  else if (c==tcpwifi) { setWiFi(); }                    // 52
  else if (c==tcpreset) { ESP.restart(); }               // 53
  else if (c==tcpframe) { setFrame(); }                  // 54
  else if (c==tcpscanst) { setSCAN(data.toInt()); }      // 64
  else if (c==tcpkeylock) { setLOCK(data.toInt()); }     // 65
  else if (c==tcpattlevel) { setATT(data.toInt(),0); }   // 66
  else if (c==tcpifShiftVal) { setIFS(data.toInt(),0); } // 67
  else if (c==tcpmemmode) { setMemMode(data.toInt()); }  // 70
  else if (c==tcpmempos) { setmemPos(data.toInt()); }    // 71 
  else if (c==tcpippublic) 
  { 
    data.toCharArray(udpAddress,data.length()+1); 
    Serial2.print("Client Address:"); Serial2.println(udpAddress);
    }    // 72
  else if (c==tcpMEM) { s2("tcpMEM received");s2(crlf); sendData(tcpMEM); }  // 126
  else if (c==tcpALL) { s2("tcpALL received");s2(crlf); sendData(tcpALL); }  // 127
}

void handletcpS()
{
  tcpclient=tcpserver.available(); 
  if (tcpclient) 
    {
    remoteclientexist=true; displayNav();
    String auxS=tcpclient.remoteIP().toString();
    auxS.toCharArray (udpAddress, auxS.length()+1);
    printhora(); s2(" New client TCP: "); s2(tcpclient.remoteIP());s2(crlf);
    if ((conf.connMode==1) || (conf.connMode==2))   // modos IP, enviar por Client
      {
      webactive=0;
      //sendFreq()      ;
      printhora(); Serial2.print("sending CONF/MEM: ");
      tcpclient.write(tcpALL);   tcpclient.write(buffconf,sizeof(conf)); 
      tcpclient.write(tcpMEM);   tcpclient.write(buffmemo,sizeof(memo)); 
      Serial2.print(sizeof(conf)); Serial2.print("/"); Serial2.print(sizeof(memo)); 
      Serial2.println(" Done");
      printhora(); Serial2.print("sending status "); sendstatus(1);
      Serial2.println(" Done");
      }
    setLOCK(2);
    while (tcpclient.connected()) 
      {
      while (tcpclient.available()>0) 
        {
        unsigned long tini=millis();
        char c=tcpclient.read();
        String datarec = tcpclient.readStringUntil('\n'); 
        handleRecDataIP(c, datarec);
        }
      //sendFreq(); // no deberia ser necesario
      loopaux();
      }
    webactive=1;
    printhora(); s2(" Client TCP disconnected"); s2(crlf);
    remoteclientexist=false;
    setLOCK(0);
    displayNav();
    }
  else
    {
    loopaux();
    }
}

void handleWS()
{
  if (wsclient.available()) 
    {
    WebsocketsMessage wsmsg=wsclient.readBlocking();
    procesaWSrec(wsmsg.data()); 
    }
  if (wsserver.poll())
    {
    printhora();
    s2(" New client WS: "); s2(tcpclient.remoteIP());s2(crlf);
    wsclient=wsserver.accept();    // accept() bloquea
    }
}

int graphVal = 1;
int delta = 1;

void loopaux()
{
  countloop++;  
  tini=millis();
  if (btnDown())
    {
      Serial2.println(modefunction);
    delay(200);
    if (modefunction==0)
      {
      if (modeencoder==0)   
        {
        modeencoder=1;  // volume
        conf.audiovolume=getValByKnob(29, conf.audiovolume, 0, 100, 1, "Volume", 1);
        sendVolumeLyraT(conf.audiovolume);
        saveconf();
        }
      else if (modeencoder==1)   
        {
        modeencoder=2;  // gain
        conf.audiogain=getValByKnob(30, conf.audiogain, 1, 1000, 10, "Gain", 1);
        sendGainLyraT(conf.audiogain);
        saveconf();
        }
      else if (modeencoder==2)   
        {
        modeencoder=3;  // spectrum att
        conf.spatt=getValByKnob(31, conf.spatt, 1, 100, 1, "Sp. Att", 1);
        sendSpectrumAttLyraT(conf.spatt);
        saveconf();
        }
      else if (modeencoder==3)   
        {
        modeencoder=4;  // spectrum attle
        conf.spspan=getValByKnob(32, conf.spspan, 12, 24, 12, "Span khz", 1);
        sendSpectrumSpanLyraT(conf.spspan);
        saveconf();
        }
      else if (modeencoder>=4)   
        {
        modeencoder=0;
        updateDisplay(1);
        }
      return;   
      }
    else
      if (modeencoder==0)   
        {
        modeencoder=4;  // low filter
        conf.audiolower=getValByKnob(33, conf.audiolower, 100, conf.audioupper-100, 100, "Low filt", 1);
        sendFilterLyraT(conf.audiolower,conf.audioupper);
        saveconf();
        }
      else if (modeencoder==4)   
        {
        modeencoder=5;  // high filter
        conf.audioupper=getValByKnob(34, conf.audioupper, conf.audioupper+100, 5000, 100, "High Fil.", 1);
        sendFilterLyraT(conf.audiolower,conf.audioupper);
        saveconf();
        }
      else if (modeencoder==5)   
        {
        modeencoder=6;  // pitch
        conf.audiopitch=getValByKnob(35, conf.audiopitch, 100, 5000, 100, "Pitch.", 1);
        sendFilterLyraT(conf.audiolower,conf.audioupper);
        saveconf();
        } 
      else if (modeencoder>=6)   
        {
        modeencoder=0;
        updateDisplay(1);
        }
      return;   
    }
  handleSerial();  
  if (conf.ftpenable) { handleFTP(); }
  if (webactive==1) 
    { 
      handleWebclient(); 
    }
  if (conf.wsenable) { handleWS(); }
  if (conf.debugenable) { Debug.handle(); }
  if (isCWAutoMode==0)    //when CW AutoKey Mode, disable this process
    {
    if ((!txCAT) && (!txTFT)) { checkPTT(); }  
    handletfttouch();     // este proceso dura unos 11 ms
    }
  if (conf.cwMode!=0) 
    { 
    //decodeCW();
    cwKeyer(); 
    }
  if (tftpage==0)
    {
    //readSmeter();     // este proceso dura unos 9 ms
    if (inTx==1)  // TX
      { 
      displaySWR(0); 
      }
    else    // RX
      {
      if (conf.memMode==0)
        {
        if (isCWAutoMode == 0 || cwAutoDialType == 1)
          {
          if (scanF>0) 
            { 
            doScanF();  
            displayFrame();
            if (conf.scanmode>0)
              {
              if (smetervalue>conf.squelchval) { setSCAN(0); }
              }
            }
          else
            {
            if (conf.ritOn) {  doRIT();    }
            else 
              { 
              if (keylock==0) 
                {
                doTuningWithThresHold();   // este proceso dura unos 10 ms  
                }
              }
            }
          }
        }
      } //end of check TX Status
    //we check CAT after the encoder as it might put the radio into TX
    Check_Cat(inTx?1:0); // s2("Check_Cat");s2(crlf);
    }
  else if (tftpage==22) 
    { doMem(); }    // Mem Mode
  if((millis() > (mact01 + 100))) { task01(); }     // tareas que se hacen cada 0.1 segundo
  if((millis() > (mact1 + 1000))) { task1(); }      // tareas que se hacen cada segundo
  if((millis() > (mact10 + 10000))) { task10(); }   // tareas que se hacen cada 10 segundos
////////////////////////////////////////////
    // RemoteDebug handle
}

void loop()
{ 
  if (conf.tcpenable==1)
    {
    handletcpS();
    }
  else
    {
    loopaux();
    }

}
