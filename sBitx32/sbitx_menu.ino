/*
This source code started with Farhan's original source. The license rules are followed as well.
Calibration related functions kept the original source except for the minor ones. 
The part is collected in the last minute of this source.
Ian KD8CEC
 */
 
#include "sbitx.h"
#include "sbitx_eemap.h"

//Commonly called functions when exiting menus by KD8CEC

uint32_t getValByKnob(int valueType, int targetValue, int minValue, int maxValue, int incStep, const char* Title, int Sensitivity)
{
  int knob;
  int moveDetectStep=0;
  int antvalue=targetValue;
  if (targetValue<minValue) targetValue=minValue;
  if (targetValue>maxValue) targetValue=maxValue;
  TFT_eSPI_Button btaux;          // buttons aux
  clearTFT();
  tft.setTextSize(3); tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString(Title,5,20);
  displayYN(1,1,0);
  tft.setTextSize(3);  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  if ((valueType!=0) && (valueType!=2) && (valueType!=5) && (valueType!=6) && (valueType!=19))
    {
    btaux.initButtonUL(&tft,160,5,155,40,2,TFT_BLACK,TFT_WHITE,itoa(targetValue,buff,10),3);
    btaux.drawButton();
    }
  if (valueType==2)      // IFS
    { 
    displayIFS(1,0,0); 
    }
  else if (valueType==3)       // ajustar C1
    {
    displaygauge(3,targetValue,160,190,140,1,minValue,maxValue,180,12);
    displayneedle(targetValue,160,190,140,minValue,maxValue,180);
    setVCAP(CAP1, targetValue);
    }
  else if (valueType==4)       // ajustar C2
    {
    displaygauge(3,targetValue,160,190,140,1,minValue,maxValue,180,12);
    displayneedle(targetValue,160,190,140,minValue,maxValue,180);
    setVCAP(CAP2, targetValue);
    }
  else if (valueType==5)        // squelch
    {
    displaygauge(5,targetValue,160,190,140,1,minValue,maxValue,180,15);
    displayneedle(targetValue,160,190,140,minValue,maxValue,180);
    }
  else if (valueType==6)      // ATT
    {
    displayATT(1,0,0); 
    }
  else if ((valueType>=11) && (valueType<=18))        // analog KEYER setting
    {
    tft.setTextSize(2); tft.setTextColor(TFT_YELLOW, TFT_BLACK);
    if ((valueType==11)||(valueType==15)) tft.drawString("Press DOT to check value",0,70); 
    else if ((valueType<=12)||(valueType<=16)) tft.drawString("Press DASH to check value",0,70); 
    else if ((valueType<=13)||(valueType<=17)) tft.drawString("Press BOTH to check value",0,70); 
    else if ((valueType<=14)||(valueType<=18)) tft.drawString("Press ST to check value",0,70); 
    tft.drawString("Value keyer:",0,100); 
    tft.drawString("Suggested value:",0,130); 
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    }
  else if (valueType==19)   // timezone 
    {
    btaux.initButtonUL(&tft,215,5,100,40,2,TFT_BLACK,TFT_WHITE,timezonetext[targetValue],3);
    btaux.drawButton();
    }
  else if (valueType==20) { setFrequency(conf.frequency); }       // set filters
  else if (valueType==22) { si5351bx_setfreqNOLIMIT(0, targetValue);  }  // set OSC0
  else if (valueType==23) { si5351bx_setfreqNOLIMIT(1, targetValue);  }  // set OSC1
  else if (valueType==24) { si5351bx_setfreqNOLIMIT(2, targetValue);  }  // set OSC2
  else if (valueType==25) { si5351bx_setfreqNOLIMIT(0, targetValue);  }  // set OSC1 x1000
  else if (valueType==26) { si5351bx_setfreqNOLIMIT(1, targetValue);  }  // set OSC2 x1000
  else if (valueType==27) { si5351bx_setfreqNOLIMIT(2, targetValue);  }  // set OSC2 x1000
    
  while(!btnDown())
    {
    knob = enc_read();
    if (knob != 0)
      {
      moveDetectStep += (knob > 0 ? 1 : -1);
      if (moveDetectStep < (Sensitivity * -1)) {
        if (targetValue > minValue) targetValue -= incStep;
        moveDetectStep = 0;
        }
      else if (moveDetectStep > Sensitivity) {
        if (targetValue < maxValue) targetValue += incStep;
        moveDetectStep = 0;
        }
      if ((valueType!=0) && (valueType!=2) && (valueType!=5) && (valueType!=6))
        {
        btaux.initButtonUL(&tft,160,5,160,35,2,TFT_BLACK,TFT_WHITE,itoa(targetValue,buff,10),3);
        btaux.drawButton();
        }
      if (valueType==2)   // IFS
        {
        conf.ifShiftValue=targetValue; isIFShift=conf.ifShiftValue==0?0:1;
        //displayIFS(1,0,0);
        setFrequency(conf.frequency);
        displayBand2();
        }
      else if (valueType==3)        // ajustar C1
        {
        //cap1.write(targetValue);             // tell servo to go to position in variable '0'
        displayneedle(targetValue,160,190,140,minValue,maxValue,180);
        setVCAP(CAP1, targetValue);
        }
      else if (valueType==4)        // ajustar C2
        {
        //cap2.write(targetValue);             // tell servo to go to position in variable '0'
        displayneedle(targetValue,160,190,140,minValue,maxValue,180);
        setVCAP(CAP2, targetValue);
        }
      else if (valueType==5)        // squelch
        {
        if ((targetValue>=minValue) && (targetValue<=maxValue))
          displayneedle(targetValue,160,190,140,minValue,maxValue,180);
        }
      if (valueType==6)   // ATT
        {
        conf.attLevel=targetValue;
        setFrequency(conf.frequency);
        displayBand1(); 
        }
      else if (valueType==7)      // Max gain TPA2016
        { 
        setTPAMaxGain(targetValue);
        }
      else if (valueType==8)      // Gain TPA2016
        { 
        setTPAGain(targetValue);
        }
      else if (valueType==29) { sendVolumeLyraT(targetValue);  }          // set volume to LyraT
      else if (valueType==30) { sendGainLyraT(targetValue);  }            // set gain to LyraT
      else if (valueType==31) { sendSpectrumAttLyraT(targetValue);  }     // set spectrum att to LyraT
      else if (valueType==32) { sendSpectrumSpanLyraT(targetValue);  }    // set spectrum span to LyraT
      else if (valueType==33) {  }    // 
      else if (valueType==34) {  }    // 
      else if (valueType==35) {  }    // 
      }  
    if ((valueType>=11) && (valueType<=18))        // analog KEYER setting
      {
      tft.setTextSize(2); tft.setTextColor(TFT_YELLOW, TFT_BLACK);
      tft.drawString("     ",160,100); 
      int valaux=(analogRead(ANALOG_KEYER)/10)*10;
      tft.drawNumber(valaux,160,100); 
      tft.setTextSize(3);
      tft.drawString("     ",200,130); 
      if (valueType<=14) tft.drawNumber(valaux-200,200,130);
      else tft.drawNumber(valaux+200,200,130);
      tft.setTextSize(2); tft.setTextColor(TFT_WHITE, TFT_BLACK);
      }
    else if (valueType==19) 
      {
      btaux.initButtonUL(&tft,215,5,100,40,2,TFT_BLACK,TFT_WHITE,timezonetext[targetValue],3);
      btaux.drawButton();
      }
    else if (valueType==22) { si5351bx_setfreqNOLIMIT(0, targetValue);  }  // set OSC0
    else if (valueType==23) { si5351bx_setfreqNOLIMIT(1, targetValue);  }  // set OSC1
    else if (valueType==24) { si5351bx_setfreqNOLIMIT(2, targetValue);  }  // set OSC2
    else if (valueType==25) { si5351bx_setfreqNOLIMIT(0, targetValue);  }  // set OSC1 x1000
    else if (valueType==26) { si5351bx_setfreqNOLIMIT(1, targetValue);  }  // set OSC2 x1000
    else if (valueType==27) { si5351bx_setfreqNOLIMIT(2, targetValue);  }  // set OSC2 x1000
        
    int auxres=checkYN();
    if (auxres==0) 
      return targetValue;
    else if (auxres==1) 
      return antvalue;
    Check_Cat(0);  //To prevent disconnections
    }  
  //s2("targetValue:"); s2(targetValue);s2(crlf);
  return targetValue;
}

void setupVFO(){
  clearTFT();
  int knob = 0;
  //round off the the nearest khz
  conf.frequency = (conf.frequency/1000l)* 1000l;
  setFrequency(conf.frequency);
  delay(100);
  tft.drawNumber(conf.calibration,180,40);
  tft.drawString("You should have a signal",0,130);
  tft.drawString("exactly at      Khz",0,150);
  tft.drawNumber(conf.frequency/1000l,135,150);
  tft.drawString("Rotate to zerobeat",0,170);
  displayYN(1,1,0);
  int result=-1;
  while (result==-1)
    {
    tft.fillRect(190, 40, 160, 30, TFT_BLACK);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.drawNumber(conf.calibration,180,40);
    if (btnDown()) result=0; else result=checkYN();
    knob = enc_read();
    if (knob != 0)
      {
      conf.calibration += knob * 875;
      OSC0 = conf.usbCarrier;
      si5351bx_setfreq(0, OSC0);  //set back the carrrier oscillator anyway, cw tx switches it off  
      si5351_set_calibration(conf.calibration);
      setFrequency(conf.frequency);
      }
    }
  if (result==0)
    {
    saveconf();
    initOscillators();
    si5351_set_calibration(conf.calibration);
    }
  setFrequency(conf.frequency);    
}

void setupBFO(){
  clearTFT();
  int knob = 0;
  tft.drawNumber(conf.usbCarrier,180,70);
  tft.drawString("Set BFO",0,130);
  tft.drawString("Press TUNE to Save",0,170);
  displayYN(1,1,0);
  OSC0 = conf.usbCarrier;
  si5351bx_setfreq(0, OSC0);
  int result=-1;
  while (result==-1)
    {
    tft.fillRect(190, 70, 160, 30, TFT_BLACK);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.drawNumber(conf.usbCarrier,180,70);
    if (btnDown()) result=0; else result=checkYN();
    knob = enc_read();
    if (knob != 0)
      {
      conf.usbCarrier += 10 * knob;
      OSC0 = conf.usbCarrier;
      si5351bx_setfreq(0, OSC0);
      setFrequency(conf.frequency);
      delay(100);
      }
  }
  saveconf();
  OSC0 = conf.usbCarrier;
  si5351bx_setfreq(0, OSC0);          
  setFrequency(conf.frequency);    
}

void setupFirstIF(){
  clearTFT();
  int knob = 0;
  tft.drawNumber(conf.firstIF,180,70);
  tft.drawString("Set First IF",0,130);
  tft.drawString("Press TUNE to Save",0,170);
  displayYN(1,1,0);
  int result=-1;
  while (result==-1)
    {
    tft.fillRect(190, 70, 160, 30, TFT_BLACK);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.drawNumber(conf.firstIF,180,70);
    if (btnDown()) result=0; else result=checkYN();
    knob = enc_read();
    if (knob != 0)
      {
      conf.firstIF += 100 * knob;
      setFrequency(conf.frequency);
      delay(100);
      }
  }
  saveconf();
  setFrequency(conf.frequency);    
}


void setupATU(byte C){
  if (C==1) 
    { 
      //conf.posATUC1=getValByKnob(3, conf.posATUC1, 0, 180, 1, "C1", 1); 
      conf.posATUC1=getValByKnob(3, conf.posATUC1, 0, 4050, 50, "C1", 1); 
    }
  else 
    { 
      //conf.posATUC2=getValByKnob(4, conf.posATUC2, 0, 180, 1, "C2", 1); 
      conf.posATUC2=getValByKnob(4, conf.posATUC2, 0, 4050, 50, "C2", 1); 
    }
  saveconf(); 
  return;
}

void setupconnmode()
{
  conf.connMode=conf.connMode<4?conf.connMode+1:0;
  saveconf();
}

void setupserial2mode()
{
  conf.serial2Mode=conf.serial2Mode==0?1:0;
  saveconf();
}

/* Take a deep breath, math(ematics) ahead
 * The 25 mhz oscillator is multiplied by 35 to run the vco at 875 mhz
 * This is divided by a number to generate different frequencies.
 * If we divide it by 875, we will get 1 mhz signal
 * So, if the vco is shifted up by 875 hz, the generated frequency of 1 mhz is shifted by 1 hz (875/875)
 * At 12 Mhz, the carrier will needed to be shifted down by 12 hz for every 875 hz of shift up of the vco
 */

 //this is used by the si5351 routines in the ubitx_5351 file
//extern int32_t calibration;
extern uint32_t si5351bx_vcoa;
extern int enc_read(void);
