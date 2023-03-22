/*************************************************************************
  KD8CEC's uBITX Display Routine for TFT320240 SPI
  Uses SPI interface  
-----------------------------------------------------------------------------
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.

**************************************************************************/
#include "sbitx.h"

//======================================================================== 
//Begin of TFT320240 Library by EA4GZI
//========================================================================

TFT_eSPI_Button btVFO[2];          // buttons vfo
TFT_eSPI_Button btMEM[3];          // buttons mem
TFT_eSPI_Button btFreq[9];         // buttons frequency display
TFT_eSPI_Button btFreqTx[9];       // buttons frequency display TX at ritOn & SplitOn
TFT_eSPI_Button btMain[10];        // buttons main display
TFT_eSPI_Button btNav[5];          // buttons Nav display
TFT_eSPI_Button btCAL[5];          // buttons Calibration display
TFT_eSPI_Button btSME[5];          // buttons S-METER display
TFT_eSPI_Button btCON[5];          // buttons Connections display
TFT_eSPI_Button btTEMP[5];         // buttons Temps display
TFT_eSPI_Button btPORTS[5];        // buttons Ports display
TFT_eSPI_Button btFILT[5];         // buttons FILTERS display
TFT_eSPI_Button btOSC[6];          // buttons OSC display
TFT_eSPI_Button btFIL1[5];          // buttons FIL1 display
TFT_eSPI_Button btFIL2[5];          // buttons FIL2 display
TFT_eSPI_Button btATU[5];          // buttons ATU 
TFT_eSPI_Button btTPA[5];          // buttons TPA2016 
TFT_eSPI_Button btCW[5];           // buttons CW setting
TFT_eSPI_Button btSet[5];          // buttons Setting 
TFT_eSPI_Button btSetRad[5];       // buttons Setting radio
TFT_eSPI_Button btMemMan[5];       // buttons mem manager
TFT_eSPI_Button btNet[5];          // buttons Setting Net display
TFT_eSPI_Button btFlot[5];         // buttons flotantes
TFT_eSPI_Button btKEYER[8];        // buttons KEYER setting
TFT_eSPI_Button btSta[8];          // buttons Status (Wifi,Hour, Min, Sec, Rst, TR1, TR2, T5v)
TFT_eSPI_Button btYN[3];           // buttons OK / Cancel / Backspace
TFT_eSPI_Button btKey[50];         // buttons Key 0..9, A..Z & symbols
TFT_eSPI_Button btSel[4];          // botones barra estado: "< Cancel OK >"
TFT_eSPI_Button btMemN[7];         // botones lista memorias visibles, mempos
TFT_eSPI_Button btMemF[7];         // botones lista memorias visibles, frequency
TFT_eSPI_Button btSmeter[2];       // botones Smeter
TFT_eSPI_Button btMenuNav[20];     // botones selección pantalla
TFT_eSPI_Button btFilter[3];       // botones filtro audio
TFT_eSPI_Button btFunction[1];     // boton function

byte btMainact[10]={0,0,0,0,0,0,0,0,0,0};
uint16_t btMaincol[10]={TFT_RED,TFT_YELLOW,TFT_YELLOW,TFT_YELLOW,TFT_YELLOW,TFT_YELLOW,TFT_CYAN,TFT_ORANGE,TFT_YELLOW,TFT_YELLOW};
int16_t btMainposx[10]={ 0,64,255,255, 64,   0,128,192,128,192}; // position mains buttons
int16_t btMainposy[10]={70,70, 35,  0,105, 105, 70, 70,105,105}; // position mains buttons
int16_t btMaintam[10]={60,60, 65, 65,60, 60, 60, 60,60,60}; // position mains buttons
//char btMaintext[10][6]={"ATU","V/M","Band-","Band+","LSB","CW","RIT","SPL","IFS","ATT"};
char btMaintextbis[10][6]={"ATU","V/M","B-","B+","L","CW","RIT","SPL","IFS","ATT"};
int16_t btMainposxbis[10]={ 0,  54,    275,    275,    275,  275, 275,  275,  275,  275};  // position mains buttons
int16_t btMainposybis[10]={70,  70,     35,      0,    140,  175, 105,   70,  105,  105}; // position mains buttons
int16_t btMaintambis[10]={40,40, 40, 40,40, 40, 40, 40,40,40}; // tam mains buttons

byte btCWact[5]={1,1,1,1,1};
byte btCalact[5]={1,1,0,0,1};
byte btSmeact[5]={1,1,0,0,0};
byte btConact[5]={1,1,0,0,0};
byte btTempact[5]={1,1,1,0,0};
byte btPortsact[5]={1,1,1,1,1};
byte btFiltact[5]={1,1,1,0,0};
byte btOscact[6]={1,1,1,1,1,1};
byte btFil1act[5]={1,1,0,0,0};
byte btFil2act[5]={1,1,0,0,0};
byte btMemManact[5]={0,0,0,0,1};
byte btATUact[5]={1,1,1,0,0};
byte btTPAact[5]={1,1,1,1,0};
byte btSetact[5]={1,1,1,1,1};
byte btSetRadact[5]={1,1,1,1,1};
byte btNetact[5]={1,1,1,1,1};
byte btNavact[5]={1,1,1,1,0};
byte btFlotact[5]={1,1,1,1,1};
byte btYNact[3]={1,1,1};
byte btFilteract[3]={1,1,1};       // botones filtro audio
byte btFuntcionact[1]={1};     // botones ffunction
byte btMenuNavact[20]={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0};
char btMaintext[10][6]={"ATU","V/M","Band-","Band+","LSB","CW","RIT","SPL","IFS","ATT"};
char btCaltext[5][11]={"Calibrati.","Adj. BFO","xxx","xxx","Reset Fact"};
char btSmetext[5][11]={"Min. Value","Max. Value","xxx","xxx","xxx"};
char btContext[5][11]={"Conn. Mode","Serial2","xxx","xxx","xxx"};
char btFilttext[5][11]={"Opt. Mode","SSB lower","SSB upper","xxx","xxx"};
char btOsctext[6][11]={"GEN0","GEN1","GEN2","GEN0+","GEN1+","GEN2+"};
char btFil1text[5][11]={"Center Fr.","Wide Band","xxx","xxx","xxx"};
char btFil2text[5][11]={"Center Fr.","Wide Band","xxx","xxx","xxx"};
char btTemptext[5][11]={"TR1","TR2","+5v","xxx","xxx"};
char btPortstext[5][11]={"TCP","UDP-S","WS","WEB","UDP-F"};
char btATUtext[5][11]={"Enable","C1","C2","",""};
char btTPAtext[5][11]={"Enable","AGC Comp.","Max Gain","Gain",""};
char btCWtext[5][11]={"Key type","WPM","Sidetone","Delay Time","Start Time"};
char btKEYERtext[8][11]={"from","from","from","from","to","to","to","to"};
char btSettext[5][11]={"Language","CallSign","Latitude","Longitude","Time Zone"};
char btSetRadtext[5][11]={"TX range","SSB auto","Scan range","Scan mode","Resume (s)"};
char btMemMantext[5][11]={"xxx","xxx","xxx","xxx","Clear All"};
char btNettext[5][11]={"Auto Conn.","Scan SSID","Password","WiFi Mode","Static IP"};
char btNavtext[5][8]={"H","<",">","M","xxx"};
char btFlottext[5][6]={"Ent","Scan-","Scan+","Lock",">MEM"};
char btYNtext[3][6]={"OK","ESC","<--"};
char btFiltertext[3][6]={"<",">","|"};
char btFunctiontext[1][6]={"F"};
char btKeytextL[50][3]={"0","1","2","3","4","5","6","7","8","9",
                        "a","b","c","d","e","f","g","h","i","j",
                        "k","l","m","n","ñ","o","p","q","r","s",
                        "t","u","v","w","x","y","z","","","",
                        "-","=","_",".",",",";","/","(",")","U"};
char btKeytextU[50][3]={"0","1","2","3","4","5","6","7","8","9",
                        "A","B","C","D","E","F","G","H","I","J",
                        "K","L","M","N","Ñ","O","P","Q","R","S",
                        "T","U","V","W","X","Y","Z","", "", "",
                        "-","=","_",".",",",";","/","(",")","L"};
char keytypetext[3][11]={"Straigth","Iambic A","Iambic B"};
char btMenuNavtext[20][6]={"HOME","USER","RADIO","CW",
                           "Wifi","CALIB","KEYER","MEM",
                           "ATU","TPA","SMet","CONN",
                           "TEMP","PORTS","FILT.","OSC",
                           "F1","F2","19","20"};

char auxc[30], auxb[30];

void clearTFT()
{
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
}

void initTFT()
{
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(2);
  tft.drawString("Init...",0,0);
}

// The generic routine to display one line on the LCD 
void printLine(unsigned char linenmbr, const char *c) {
  tft.drawString(c,0,20*linenmbr);
}

void printLineF(char linenmbr, const __FlashStringHelper *c)
{
  int i;
  char tmpBuff[21];
  PGM_P p = reinterpret_cast<PGM_P>(c);  
  for (i = 0; i < 21; i++){
    unsigned char fChar = pgm_read_byte(p++);
    tmpBuff[i] = fChar;
    if (fChar == 0)
      break;
  }
  printLine(linenmbr, tmpBuff);
}

void clearLine2()
{
  printLine(0,"");
}

//End of Display Base Routines
//==================================================================================

void drawNumberB(long value,int x, int y, int w, int h, int backcolor, int textcolor, int size)    // draw Number as utton 
{
  TFT_eSPI_Button btN;          // button
  btN.initButtonUL(&tft,x,y,w,h,2,backcolor,textcolor,itoa(value,buff,10),size);
  btN.drawButton();
}

void drawStringB(char* cad,int x, int y, int w, int h, int backcolor, int textcolor, int size)    // draw Number as utton 
{
  TFT_eSPI_Button btN;          // button
  btN.initButtonUL(&tft,x,y,w,h,2,backcolor,textcolor,cad,size);
  btN.drawButton();
}

void displayTemp()
{
  tft.setTextDatum(MR_DATUM);     // derecha
  tft.setTextColor(TFT_WHITE);
  btSta[5].initButtonUL(&tft,100,220,20,20,2,MbR[0]>4000?TFT_ORANGE:MbR[0]>4500?TFT_RED:TFT_GREEN,TFT_BLACK,itoa(MbR[0]/100,buff,10),1);
  btSta[5].drawButton();
  btSta[6].initButtonUL(&tft,121,220,20,20,2,MbR[1]>4000?TFT_ORANGE:MbR[1]>4500?TFT_RED:TFT_GREEN,TFT_BLACK,itoa(MbR[1]/100,buff,10),1);
  btSta[6].drawButton();
  btSta[7].initButtonUL(&tft,141,220,20,20,2,MbR[2]>4500?TFT_ORANGE:MbR[2]>5000?TFT_RED:TFT_GREEN,TFT_BLACK,itoa(MbR[2]/100,buff,10),1);
  btSta[7].drawButton();
  tft.setTextDatum(ML_DATUM);     // izquierda
}

void displayWiFiSt()
{
  if ((tftpage>0) || (conf.framemode!=1)) return;
  btSta[0].initButtonUL(&tft,0,140,30,30,2,WiFi.isConnected()?TFT_GREEN:TFT_RED,TFT_BLACK,"WiFi",1);
  btSta[0].drawButton();
}

void displayRstBt()
{
  btSta[4].initButtonUL(&tft,0,175,30,30,2,TFT_WHITE,TFT_BLACK,"Rst",1);
  btSta[4].drawButton();
}

void displayTime()
{
  //if (!WiFi.isConnected()) return;
  tft.setTextDatum(MR_DATUM);     // derecha
  char auxh[3]=""; if (hour()<10) strcpy(auxh,"0"); strcat(auxh,itoa(hour(),buff,10)); 
  char auxm[3]=""; if (minute()<10) strcpy(auxm,"0"); strcat(auxm,itoa(minute(),buff,10)); 
  char auxs[3]=""; if (second()<10) strcpy(auxs,"0"); strcat(auxs,itoa(second(),buff,10));
  btSta[1].initButtonUL(&tft,165,220,30,18,2,TFT_BLACK,TFT_WHITE,auxh,2);
  btSta[1].drawButton();
  btSta[2].initButtonUL(&tft,194,220,30,18,2,TFT_BLACK,TFT_WHITE,auxm,2);
  btSta[2].drawButton();
  btSta[3].initButtonUL(&tft,223,220,30,18,2,TFT_BLACK,TFT_WHITE,auxs,2);
  btSta[3].drawButton();
  tft.setTextSize(2);  tft.setTextColor(TFT_WHITE);
  tft.setTextDatum(ML_DATUM);     // izquierda
}

void displayVI()
{
  tft.setTextSize(1); tft.setTextColor(TFT_WHITE); 
  tft.fillRect(40, 208, 80, 10, TFT_BLACK);
  tft.drawFloat(vtotvalue,1,40,213);
  tft.drawFloat(itotvalue,3,80,213);
}

void drawST(boolean flechas, boolean escok)     // barra navegación
{
  if (flechas)
    {
    btSel[0].initButtonUL(&tft,0,200,75,40,2,TFT_WHITE,TFT_BLACK,flecha[0],2);  btSel[0].drawButton();
    btSel[3].initButtonUL(&tft,245,200,75,40,2,TFT_WHITE,TFT_BLACK,flecha[3],2);  btSel[3].drawButton();
    }
  if (escok)
    {
    btSel[1].initButtonUL(&tft,80,200,75,40,2,TFT_WHITE,TFT_BLACK,"ESC",2);  btSel[1].drawButton();
    btSel[2].initButtonUL(&tft,165,200,75,40,2,TFT_WHITE,TFT_BLACK,"OK",2);  btSel[2].drawButton();
    }
}

void drawAP(byte nact)
{
  clearTFT();
  tft.setTextDatum(TL_DATUM);      // Set new datum
  for (int i=0; i<nAP; i++)
    if (WiFi.RSSI(i)>-90)
      {
      int y=i*20;
      WiFi.SSID(i).toCharArray(auxchar, 20);
      if (i==tftapactual) { tft.setTextColor(TFT_BLACK, TFT_WHITE);    }
      tft.drawNumber(i,0,y);
      tft.drawString(auxchar,20,y); 
      tft.drawNumber(WiFi.RSSI(i),240,y);
      tft.drawString("dbm",280,y); 
      tft.setTextColor(TFT_WHITE, TFT_BLACK);
      }
  drawST(true,true);
}

void selectapTFT()
{
  tftpage=21; 
  nAPact=0;
  tftapactual=0;
  clearTFT();
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(2);
  tft.drawString("Scanning...",20,20); 
  delay(1000);
  nAP=0;
  nAP=WiFi.scanNetworks(false, false);  
  if (nAP>maxAP) nAP=maxAP;   // 9 como máximo
  delay(1000);
  drawAP(tftapactual);
}

void displayStatus()
{
  displayTime();
  displayTemp();
  displayVI();
}

void displaySWR(byte tam)
{
  SWR=readSWR(1);
  if (conf.framemode==1)     // analog
    {
    if (tam==0)
      displaygauge(2,SWR*10,190,210,50,1,1,60,90,6);
    else
      displaygauge(2,SWR*10,160,190,110,1,1,60,180,6);
    }
}

void displayFreq(byte fast, byte fA, byte fB, byte fAux)
{
  if ((tftpage>0) && (tftpage!=22)) return;
  char freqpart[9][4]={"","","","","","","","",""};
  char freqpartSec[9][4]={"","","","","","","","",""};
  unsigned long f=conf.frequency;
  unsigned long fsec;
  int16_t colf;
  if (conf.ritOn==1) fsec=conf.ritTxFrequency;
  else if (conf.splitOn==1) fsec=conf.frequencyB;
  else fsec=conf.vfoActive==VFO_A?conf.frequencyB:conf.frequencyA;
  for (byte i=0;i<9;i++)    // separa cifras
    {
    long auxL=long(pow(10,8-i));
    if ((i==0)&&(f<100000000)) strcat(freqpart[i]," "); 
    else if ((i==1)&&(f<10000000)) strcat(freqpart[i]," "); 
    else strcat(freqpart[i],itoa((f/auxL)%10,buff,10));
    if ((i==0)&&(fsec<100000000)) strcat(freqpartSec[i]," "); 
    else if ((i==1)&&(fsec<10000000)) strcat(freqpartSec[i]," "); 
    else strcat(freqpartSec[i],itoa((fsec/auxL)%10,buff,10));
    }
  if (conf.ritOn==1) colf=TFT_CYAN; else if (conf.splitOn==1) colf=TFT_ORANGE; else colf=TFT_WHITE;
  tft.setTextSize(2); tft.setTextColor(colf);
  tft.setTextDatum(ML_DATUM);
  char auxchar[2][5]={"A","B"};
  
  if (fAux==1)
    {
    if (conf.vfoActive==VFO_B) { strcpy(auxchar[0],"B"); strcpy(auxchar[1],"A"); }
    btVFO[1].initButtonUL(&tft,3,3,30,30,2,TFT_BLACK,colf,auxchar[1],4);
    btVFO[1].drawButton();
    if ((conf.ritOn==1) || (conf.splitOn==1))
      {
      tft.drawString("TX",21,0);
      if (conf.splitOn==1) tft.drawString(conf.isUSBB==0?"LSB":"USB",80,0);
      }
    else if ((inTx==1) && (conf.ritOn==0) && (conf.splitOn==0))
      {
      tft.setTextColor(TFT_BLACK);
      tft.drawString("TX",21,0);
      if (conf.vfoActive==VFO_A)
        {
        tft.drawString(conf.cwModeB==0?"   ":"CW-",45,0);
        tft.drawString(conf.isUSBB==0?"LSB":"USB",80,0);
        }
      else
        {
        tft.drawString(conf.cwModeA==0?"   ":"CW-",45,0);
        tft.drawString(conf.isUSBA==0?"LSB":"USB",80,0);
        }
      }
    }
  if (fA==1)
    {
    for (byte i=0;i<9;i++)      // frecuencia principal  
      {
      if ((fast==0) || (strcmp(freqpartant[i],freqpart[i]) != 0))    // son diferentes
        {
        strcpy(freqpartant[i],freqpart[i]);
        if (i<6) btFreq[i].initButtonUL(&tft,35+26*i,00,26,40,2,TFT_BLACK,conf.tuneStepIndex==i?TFT_YELLOW:TFT_WHITE,freqpart[i],4);
        else btFreq[i].initButtonUL(&tft,71+20*i,8,20,30,2,TFT_BLACK,conf.tuneStepIndex==i?TFT_YELLOW:TFT_WHITE,freqpart[i],3);
        btFreq[i].drawButton();
        }
      }
    tft.fillCircle(110,32,2,TFT_WHITE);    // punto de miles
    }
  if ((conf.framemode!=0) && (fB==1))
    {
    for (byte i=0;i<9;i++)    // frecuencia secundaria 
      {
      btFreqTx[i].initButtonUL(&tft,140+12*i,40,12,20,2,TFT_BLACK,colf,freqpartSec[i],2);
      btFreqTx[i].drawButton();
      }
    tft.fillCircle(173,44,1,colf);    // punto de miles
    btVFO[0].initButtonUL(&tft,0,40,30,30,2,TFT_BLACK,TFT_WHITE,auxchar[0],3);
    btVFO[0].drawButton();
    }

}

void displayNav()   // botones navegación
{
  strcpy(btNavtext[1],tftpage==22?"Ed":"<");
  strcpy(btNavtext[2],tftpage==22?"Ok":">");
  strcpy(btNavtext[3],"M");
  btNav[0].initButtonUL(&tft,0,210,30,30,2,TFT_WHITE,TFT_BLACK,btNavtext[0],2);
  btNav[0].drawButton();
  for (byte i=1;i<5;i++) if (btNavact[i]==1)
    {
    btNav[i].initButtonUL(&tft,10+22*i,220,20,20,2,TFT_WHITE,TFT_BLACK,btNavtext[i],2);
    btNav[i].drawButton();
    }
}

void displayYN(byte en0, byte en1, byte en2)
{
  // botones Yes/Cancel
  btYNact[0]=en0; btYNact[1]=en1; btYNact[2]=en2;
  for (byte i=0;i<3;i++)
    if (btYNact[i]==1)
      {
      btYN[i].initButtonUL(&tft,100*i+5,205,90,30,2,TFT_WHITE,TFT_BLACK,btYNtext[i],2);
      btYN[i].drawButton();
      }
}

void displayCal()
{
  // botones setting
  for (byte i=0;i<5;i++) if (btCalact[i]==1)
    {
    btCAL[i].initButtonUL(&tft,0,35*i+30,160,30,2,TFT_WHITE,TFT_BLACK,btCaltext[i],2);
    btCAL[i].drawButton();
    }
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString("5 - ADJUST",0,0);
  tft.drawNumber(conf.calibration,180,40);
  tft.drawNumber(conf.usbCarrier,180,75);
}

void displayATU()
{
  // botones setting
  int backcolor=TFT_WHITE;
  for (byte i=0;i<5;i++) if (btATUact[i]==1)
    {
    if (i==0) backcolor=conf.ATUZM2enabled==1?TFT_YELLOW:TFT_WHITE;
    else backcolor=TFT_WHITE;
    btATU[i].initButtonUL(&tft,0,35*i+30,160,30,2,backcolor,TFT_BLACK,btATUtext[i],2);
    btATU[i].drawButton();
    }
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString("8 - ATU ZM-2",0,0);
  tft.drawString(conf.ATUZM2enabled==1?"YES":"NO",180,40);
}

void displayTPA2016()
{
  const char comptext[4][4]={"OFF","1:2","1:4","1:8"};
  // botones setting
  int backcolor=TFT_WHITE;
  for (byte i=0;i<5;i++) if (btTPAact[i]==1)
    {
    if (i==0) backcolor=conf.TPA2016enabled==1?TFT_YELLOW:TFT_WHITE;
    else if (i==1) backcolor=conf.TPA2016Compvalue>0?TFT_YELLOW:TFT_WHITE;
    else backcolor=TFT_WHITE;
    btTPA[i].initButtonUL(&tft,0,35*i+30,160,30,2,backcolor,TFT_BLACK,btTPAtext[i],2);
    btTPA[i].drawButton();
    }
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString("9 - TPA2016 Settings",0,0);
  tft.drawString(conf.TPA2016enabled==1?"YES":"NO",180,40);
  tft.drawString(comptext[conf.TPA2016Compvalue],180,75);
  tft.drawNumber(conf.TPA2016Compmaxgain,180,110);
  tft.drawNumber(conf.TPA2016Gain,180,145);
}

void displayMenuSmeter()
{
  for (byte i=0;i<5;i++) if (btSmeact[i]==1)
    {
    btSME[i].initButtonUL(&tft,0,35*i+30,160,30,2,TFT_WHITE,TFT_BLACK,btSmetext[i],2);
    btSME[i].drawButton();
    }
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString("10 - S-Meter Settings",0,0);
  tft.drawNumber(conf.sMeterLevels[0],180,40);
  tft.drawNumber(conf.sMeterLevels[15],180,75);

  tft.drawNumber(minsmeter,0,110);
  tft.drawString("<--->",130,110);
  tft.drawNumber(maxsmeter,250,110);
}

void displayCONNS()
{
  for (byte i=0;i<5;i++) if (btConact[i]==1)
    {
    btCON[i].initButtonUL(&tft,0,35*i+30,160,30,2,TFT_WHITE,TFT_BLACK,btContext[i],2);
    btCON[i].drawButton();
    }
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString("11 - Connections Settings",0,0);
  tft.drawString(connmodetext[conf.connMode],180,40);
  tft.drawString(serial2modetext[conf.serial2Mode],180,75);
}

void displayTEMP()
{
  for (byte i=0;i<5;i++) if (btTempact[i]==1)
    {
    btTEMP[i].initButtonUL(&tft,0,35*i+30,60,30,2,TFT_WHITE,TFT_BLACK,btTemptext[i],2);
    btTEMP[i].drawButton();
    }
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString("12 - Temp. Settings",0,0);
  tft.setTextSize(2);
  for (byte i=0;i<3;i++)
    {
    tft.drawNumber(conf.nprobe[i],65,45+(35*i));
    for (byte j=0;j<8;j++)
      tft.drawHex(conf.probecode[conf.nprobe[i]][j],80+(30*j),45+(35*i));
    }
  tft.setTextSize(2);
}

void displayPORTS()
{
  for (byte i=0;i<5;i++) if (btPortsact[i]==1)
    {
    int backcolor;
    if (i==0) backcolor=conf.tcpenable==0?TFT_WHITE:TFT_YELLOW;
    else if (i==1) backcolor=conf.udpenable==0?TFT_WHITE:TFT_YELLOW;
    else if (i==2) backcolor=conf.wsenable==0?TFT_WHITE:TFT_YELLOW;
    else if (i==3) backcolor=conf.webenable==0?TFT_WHITE:TFT_YELLOW;
    else if (i==4) backcolor=conf.webenable==0?TFT_WHITE:TFT_YELLOW;
    btPORTS[i].initButtonUL(&tft,0,35*i+30,160,30,2,backcolor,TFT_BLACK,btPortstext[i],2);
    btPORTS[i].drawButton();
    }
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString("13 - Ports Settings",0,0);
  tft.drawNumber(conf.tcpPort,180,40);
  tft.drawNumber(conf.udpPortSmeter,180,75);
  tft.drawNumber(conf.wsPort,180,110);
  tft.drawNumber(conf.webPort,180,145);
  tft.drawNumber(conf.udpPortFreq,180,180);
}

void displayFILTERS()
{
  for (byte i=0;i<5;i++) if (btFiltact[i]==1)
    {
    btFILT[i].initButtonUL(&tft,0,35*i+30,160,30,2,TFT_WHITE,TFT_BLACK,btFilttext[i],2);
    btFILT[i].drawButton();
    }
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString("14 - Filters Settings",0,0);
  tft.drawString(conf.optimalmode==0?"NORMAL":"OPTIMAL",180,40);
  tft.drawNumber(conf.audiolower,180,75);
  tft.drawNumber(conf.audioupper,180,110);
}

int hzppixel1=100;   // horizontal ratio
int pixelpdb1=20;   // vertical ratio

double Filter1function(double f, byte returndb)
{
    f=(f-45000000)/1000;         // Ajuste de escala.  f:frequency    db:db
    double db = -1.43708 * (pow(10,-7) * pow(f,6));
    db = db - 1.4428 * (pow(10,-6) * pow(f,5));
    db = db + 1.1239 * (pow(10,-4) * pow(f,4));
    db = db + 7.88   * (pow(10,-4) * pow(f,3));
    db = db - 5.11   * (pow(10,-2) * pow(f,2));
    db = db - 0.1612 * f  - 0.1422;
    if (returndb)
      return db;
    else
      return (40 - (db * pixelpdb1));
}

double Q12poles=4.0;    // polos
double Q15ripple=1.7;   // ripple
double Q13sup=(double(conf.fc2)+double(conf.df2)/2)/1000000;
double Q14inf=(double(conf.fc2)-double(conf.df2)/2)/1000000;
double Q8=pow(10,Q15ripple/10)-1;
double Q9=sqrt(Q13sup*Q14inf);
double R9=(Q13sup-Q14inf)/Q9;
int hzppixel2=13;   // horizontal ratio
int pixelpdb2=10;   // vertical ratio

double Filter2function(double f, byte returndb)
{
  double C2=double(f)/1000000;
  double G2=abs((C2/Q9)-(Q9/C2))/R9;    
  double H2, J2, L2, K2, M2, db;
  if (C2<Q14inf)
    {
    H2=log(G2+(sqrt(G2*G2-1)));
    J2=cosh(Q12poles*H2);
    L2=1;
    K2=log10(1+Q8*J2*J2);
    M2=log10(1+Q8*L2*L2);
    db=-10*K2;
    }
  else
    {
    if (C2<Q13sup)
      {
      H2=acos(G2);
      J2=cosh(Q12poles*H2);
      L2=cos(Q12poles*H2);
      K2=log10(1+Q8*J2*J2);
      M2=log10(1+Q8*L2*L2);
      db=-10*M2;
      }
    else
      {
      H2=log(G2+(sqrt(G2*G2-1)));
      J2=cosh(Q12poles*H2);
      L2=1;
      K2=log10(1+Q8*J2*J2);
      M2=log10(1+Q8*L2*L2);
      db=-10*K2;
      }
    }
  if (returndb)
    return db;
  else
    return (40 - (db * pixelpdb2));
}

int FreqtoX1(unsigned long f, unsigned long f0)    
{
  if ((f >= f0) && (f <= f0+320*hzppixel1))
    return (f-f0)/hzppixel1;   // 1 pixel = hzppixel1 Hz 
  else
    return 0;
}

int FreqtoX2(unsigned long f, unsigned long f0)    
{
  //if ((f >= f0) && (f <= f0+320*hzppixel2))
    return (f-f0)/hzppixel2;   // 1 pixel = hzppixel2 Hz 
  //else
    //return 0;
}

int xinflast, xsuplast, xIFlast;
int yinflast, ysuplast, yIFlast;
void displayBand1()
{
  int xIF, xinf, xsup, yIF, yinf, ysup;
  double y;
  unsigned long finf, fsup, IF;
  IF=conf.optimalmode==1?IF1+conf.attLevel*100:conf.firstIF+conf.attLevel*100;
  if (conf.isUSB==1)
    {
    finf=IF-conf.audioupper;
    fsup=IF-conf.audiolower;
    }
  else
    {
    finf=IF+conf.audiolower;
    fsup=IF+conf.audioupper;
    }
  xIF=FreqtoX1(IF, conf.fc1-160*hzppixel1);
  xinf=FreqtoX1(finf, conf.fc1-160*hzppixel1);
  xsup=FreqtoX1(fsup, conf.fc1-160*hzppixel1);
  yIF = int(Filter1function(IF,0));
  yinf=int(Filter1function(conf.fc1-160*hzppixel1+xinf*hzppixel1,0));
  ysup=int(Filter1function(conf.fc1-160*hzppixel1+xsup*hzppixel1,0));
  
  if (xinflast!=999)
    {
    tft.fillRect(xinflast,140,xsuplast-xinflast,11,TFT_BLACK);    // clean prior band
    tft.drawRect(xinflast,40,xsuplast-xinflast,70,TFT_BLACK);     // level -3 db
    tft.drawLine(xinflast,40,xinflast,140,TFT_BLACK);
    tft.drawLine(xsuplast-1,40,xsuplast-1,140,TFT_BLACK);
    tft.drawLine(xIFlast, max(40,yIFlast), xIFlast, 150, TFT_BLACK);  // vertical lines
    }
  xinflast=xinf;  xsuplast=xsup;  xIFlast=xIF; 
  yinflast=yinf;  ysuplast=ysup;  yIFlast=yIF; 
  if (conf.isUSB==1)
    {
    tft.fillRect(xinf+10, 140, xsup-xinf-10, 10, TFT_GREEN);       // level -3 db
    tft.fillTriangle(xinf,145, xinf+10,140, xinf+10,149, TFT_GREEN);
    }
  else
    {
    tft.fillRect(xinf, 140, xsup-xinf-10, 10, TFT_GREEN);         // level -3 db
    tft.fillTriangle(xsup-1,145, xsup-10,140, xsup-10,149, TFT_GREEN);
    }
  tft.drawLine(xinf, max(40,yinf), xinf, 150, TFT_GREEN);      // xinf line
  tft.drawLine(xsup-1, max(40,ysup), xsup-1, 150, TFT_GREEN);  // xsup line
  tft.drawLine(xIF, max(40,int(Filter1function(IF,0))), xIF, 150, TFT_YELLOW);  // IF line
  for (int i=xinf-2;i<xsup+10;i++)   // plot curve
    {
    int auxy=int(Filter1function(conf.fc1-160*hzppixel1+i*hzppixel1,0));
    tft.drawLine(i,auxy,i,auxy,TFT_YELLOW);  
    }
  tft.drawLine(160,40,160,200,TFT_WHITE);   // vertical center line
  tft.drawLine(155,100,165,100,TFT_WHITE);  // -3db line

  tft.fillRect(200,180,70,15,TFT_BLACK);     // clean number IF1
  tft.fillRect(50,160,80,20,TFT_BLACK);      // clean number db inf    
  tft.fillRect(170,160,80,20,TFT_BLACK);     // clean number db sup
  tft.setTextSize(1);
  tft.drawNumber(IF,220,190);   // print IF1
  tft.setTextSize(2);
  tft.drawFloat(Filter1function(conf.fc1-160*hzppixel1+xinf*hzppixel1,1),1,50,170);    // print db finf
  tft.drawFloat(Filter1function(conf.fc1-160*hzppixel1+xsup*hzppixel1,1),1,190,170);   // print db fsupnf
  tft.drawString("dB",105,170);  tft.drawString("dB",250,170);

  tft.setTextSize(3); tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawFloat(Filter1function(IF,1),1,180,20);
  tft.drawString("dB",270,20);
  tft.fillRect(10,60,10,10,conf.attLevel!=0?TFT_YELLOW:TFT_BLACK);    // mark Attlevel<>0
}

void displayBand2()
{
  int xinf, xsup, xIF, yinf, ysup, yIF;
  int liminf, limsup;
  unsigned long finf, fsup, IF;
  IF=conf.optimalmode==1?IF2+conf.ifShiftValue*hzppixel2:conf.usbCarrier+conf.ifShiftValue*hzppixel2;
  finf=IF+conf.isUSB==1?conf.audiolower:-conf.audiolower;
  fsup=IF+conf.isUSB==1?conf.audioupper:-conf.audioupper;
  if (conf.isUSB==1)
    { finf=IF+conf.audiolower;   fsup=IF+conf.audioupper;   }
  else
    { finf=IF-conf.audiolower;   fsup=IF-conf.audioupper;   }

  liminf=conf.fc2-160*hzppixel2;    // frecuencia límite inferior de la curva
  xinf=FreqtoX2(finf, liminf);
  xsup=FreqtoX2(fsup, liminf);
  xIF=FreqtoX2(IF, liminf);
  Serial2.print(" finf:"); Serial2.print(finf);
  Serial2.print(" fsup:"); Serial2.print(fsup);
  Serial2.print(" IF:"); Serial2.print(IF);
  Serial2.print(" xIF:"); Serial2.print(xIF);
  Serial2.println();
  yinf=int(Filter2function(liminf+xinf*hzppixel2,0));
  ysup=int(Filter2function(liminf+xsup*hzppixel2,0));
  yIF=int(Filter2function(IF,0));
  // ajuste x,y
  if (conf.isUSB==1)
    {
    if (xIF<0) xIF=0;
    if (xIF>319) xIF=319;
    if (xinf<0) xinf=0;
    if (xsup>319) xsup=319;
    if (yinf>120) yinf=120;
    if (ysup>120) ysup=120;
    if (yIF>120) yIF=120;
    }
  else
    {
    if (xIF<0) xIF=0;
    if (xIF>319) xIF=319;
    if (xinf<0) xinf=0;
    if (xsup>319) xsup=319;
    if (yinf>120) yinf=120;
    if (ysup>120) ysup=120;
    if (yIF>120) yIF=120;
    }
  // clear prior band
  if (xinflast!=999)    // clear prior data
    {
    tft.fillRect(xinflast,140,xsuplast-xinflast+11,10,TFT_BLACK);    // clean prior band, rectangle+triangle
    tft.drawLine(xinflast,40,xinflast,150,TFT_BLACK);                // clear xinf line
    tft.drawLine(xsuplast,40,xsuplast,150,TFT_BLACK);                // clear xsup line
    tft.drawLine(xIFlast, max(40,yIF), xIFlast, 150, TFT_BLACK);     // clear vertical lines
    }
  xinflast=xinf;  xsuplast=xsup;  xIFlast=xIF; 
  yinflast=yinf;  ysuplast=ysup;  yIFlast=yIF; 

  // draw actual band
  if (conf.isUSB==1)
    {
    tft.fillRect(xinf, 140, xsup-xinf-10, 10, TFT_GREEN);         // level -3 db
    tft.fillTriangle(xsup-1,145, xsup-10,140, xsup-10,149, TFT_GREEN);
    }
  else
    {
    tft.fillRect(xinf+10, 140, xsup-xinf-10, 10, TFT_GREEN);       // level -3 db
    tft.fillTriangle(xinf,145, xinf+10,140, xinf+10,149, TFT_GREEN);
    }
  tft.drawLine(xinf, yinf, xinf, 150, TFT_GREEN);   // xinf line
  tft.drawLine(xsup, ysup, xsup, 150, TFT_GREEN);   // xsup line
  tft.drawLine(xIF, yIF, xIF, 150, TFT_YELLOW);     // IF line
  
  for (int i=xIF-1;i<xIF+2;i++)   // plot curve
    {
    int auxy=int(Filter2function(conf.fc2-160*hzppixel2+i*hzppixel2,0)); if (auxy>200) auxy=200;
    tft.drawLine(i,auxy,i,auxy,TFT_YELLOW);  
    }
  for (int i=xinf-1;i<xinf+2;i++)   // plot curve
    {
    int auxy=int(Filter2function(conf.fc2-160*hzppixel2+i*hzppixel2,0)); if (auxy>200) auxy=200;
    tft.drawLine(i,auxy,i,auxy,TFT_YELLOW);  
    }
  for (int i=xsup-1;i<xsup+5;i++)   // plot curve
    {
    int auxy=int(Filter2function(conf.fc2-160*hzppixel2+i*hzppixel2,0)); if (auxy>200) auxy=200;
    tft.drawLine(i,auxy,i,auxy,TFT_YELLOW);  
    }
  tft.drawLine(160,40,160,200,TFT_WHITE);   // vertical center line
  tft.drawLine(155,40+3*pixelpdb2,165,40+3*pixelpdb2,TFT_WHITE);  // -3db line
  tft.drawLine(155,40+6*pixelpdb2,165,40+6*pixelpdb2,TFT_WHITE);  // -6db line
  tft.drawLine(155,40+9*pixelpdb2,165,40+9*pixelpdb2,TFT_WHITE);  // -9db line

  tft.fillRect(200,180,70,15,TFT_BLACK);     // clean number IF1
  tft.fillRect(50,160,80,20,TFT_BLACK);      // clean number db inf    
  tft.fillRect(170,160,80,20,TFT_BLACK);     // clean number db sup
  tft.setTextSize(1);  tft.drawNumber(IF,220,190);   // print IF2
  tft.setTextSize(2);
  tft.drawFloat(Filter2function(conf.fc2-160*hzppixel2+xinf*hzppixel2,1),1,50,170);    // print db finf
  tft.drawFloat(Filter2function(conf.fc2-160*hzppixel2+xsup*hzppixel2,1),1,190,170);   // print db fsupnf
  tft.drawString("dB",105,170);  tft.drawString("dB",250,170);

  tft.setTextSize(3); tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawFloat(Filter2function(IF,1),1,180,20);
  tft.drawString("dB",270,20);
  tft.fillRect(10,60,10,10,conf.ifShiftValue!=0?TFT_YELLOW:TFT_BLACK);    // mark ifShiftValue<>0
}

void displayCurve1()      // Filter 45 MHz
{
  xinflast=999;  xsuplast=999;  xIFlast=999;
  yinflast=999;  ysuplast=999;  yIFlast=999;
  tft.drawLine(155,40+3*pixelpdb1,165,40+3*pixelpdb1,TFT_WHITE);   // -3db line
  tft.drawLine(155,40+6*pixelpdb1,165,40+6*pixelpdb1,TFT_WHITE);   // -6db line
  tft.drawLine(0,200,319,200,TFT_WHITE);     // base line
  tft.drawLine(160,40,160,200,TFT_WHITE);    // vertical center line
  unsigned long f0=conf.fc1-16000;
  tft.setTextSize(1);
  tft.drawString("Center:",60,190);
  tft.drawNumber(conf.fc1,100,190);
  tft.drawString("IF1:",185,190);
  for (int i=0;i<320;i++)   // plot curve
    tft.drawPixel(i,int(Filter1function(f0+i*100,0)),TFT_YELLOW);  
  displayBand1(); 
}

void displayCurve2()      // Filter 11.059 MHz
{  
  xinflast=999;  xsuplast=999;  xIFlast=999;
  yinflast=999;  ysuplast=999;  yIFlast=999;
  tft.drawLine(155,40+3*pixelpdb2,165,40+3*pixelpdb2,TFT_WHITE);   // -3db line
  tft.drawLine(155,40+6*pixelpdb2,165,40+6*pixelpdb2,TFT_WHITE);   // -6db line
  tft.drawLine(155,40+9*pixelpdb2,165,40+9*pixelpdb2,TFT_WHITE);   // -9db line
  tft.drawLine(20,200,299,200,TFT_WHITE);    // base line
  tft.drawLine(160,40,160,200,TFT_WHITE);    // vertical center line
  unsigned long f0=conf.fc2-160*hzppixel2;
  tft.setTextSize(1);
  tft.drawString("Center:",60,190);
  tft.drawNumber(conf.fc2,100,190);
  tft.drawString("IF2:",185,190);
  for (int i=0;i<320;i++)   // plot curve
    tft.drawPixel(i,int(Filter2function(f0+i*hzppixel2,0)),TFT_YELLOW);  
  displayBand2(); 
}

void displayFIL1()
{
  for (byte i=0;i<5;i++) if (btFil1act[i]==1)
    {
    btFIL1[i].initButtonUL(&tft,0,35*i+30,160,30,2,TFT_WHITE,TFT_BLACK,btFil1text[i],2);
    btFIL1[i].drawButton();
    }
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString("16 - Filter 1",0,0);
  tft.drawNumber(conf.fc1,180,40);
  tft.drawNumber(conf.df1,180,75);
}

void displayFIL2()
{
  for (byte i=0;i<5;i++) if (btFil2act[i]==1)
    {
    btFIL2[i].initButtonUL(&tft,0,35*i+30,160,30,2,TFT_WHITE,TFT_BLACK,btFil2text[i],2);
    btFIL2[i].drawButton();
    }
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString("17 - Filter 2",0,0);
  tft.drawNumber(conf.fc2,180,40);
  tft.drawNumber(conf.df2,180,75);
}

void displayOSC()
{
  for (byte i=0;i<3;i++)
    {
    btOSC[i].initButtonUL(&tft,0,35*i+30,75,30,2,TFT_WHITE,TFT_BLACK,btOsctext[i],2);
    btOSC[i].drawButton();
    btOSC[i+3].initButtonUL(&tft,80,35*i+30,75,30,2,TFT_WHITE,TFT_BLACK,btOsctext[i+3],2);
    btOSC[i+3].drawButton();
    }
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString("15 - OSC Settings",0,0);
  tft.drawNumber(conf.gen0,180,40);
  tft.drawNumber(conf.gen1,180,75);
  tft.drawNumber(conf.gen2,180,110);
  si5351bx_setfreqNOLIMIT(0, conf.gen0);  // set OSC0
  si5351bx_setfreqNOLIMIT(1, conf.gen1);  // set OSC1
  si5351bx_setfreqNOLIMIT(2, conf.gen2);  // set OSC2
}

void displayCWSet()
{
  // botones setting
  for (byte i=0;i<5;i++) if (btCWact[i]==1)
    {
    btCW[i].initButtonUL(&tft,0,35*i+30,160,30,2,TFT_WHITE,TFT_BLACK,btCWtext[i],2);
    btCW[i].drawButton();
    }
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString("3 - CW setting",0,0);
  tft.drawString(keytypetext[conf.cwKeyType],180,40);
  tft.drawNumber(1200/conf.cwSpeed,180,75);
  tft.drawNumber(conf.sideTone,180,110);
  tft.drawNumber(conf.cwDelayTime,180,145);
  tft.drawNumber(conf.delayBeforeCWStartTime,180,180);
}

void displayKEYERSet()    // botones KEYER setting
{
  for (byte i=0;i<4;i++) 
    {
    btKEYER[i].initButtonUL(&tft,50,35*i+30,60,30,2,TFT_WHITE,TFT_BLACK,btKEYERtext[i],2);
    btKEYER[i].drawButton();
    btKEYER[i+4].initButtonUL(&tft,180,35*i+30,60,30,2,TFT_WHITE,TFT_BLACK,btKEYERtext[i+4],2);
    btKEYER[i+4].drawButton();
    }
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString("6 - analog KEYER setting",0,0);
  tft.drawString("DOT",0,45);
    tft.drawNumber(conf.cwAdcDotFrom,120,45);
    tft.drawNumber(conf.cwAdcDotTo,250,45);
  tft.drawString("DASH",0,80);
    tft.drawNumber(conf.cwAdcDashFrom,120,80);
    tft.drawNumber(conf.cwAdcDashTo,250,80);
  tft.drawString("BOTH",0,115);
    tft.drawNumber(conf.cwAdcBothFrom,120,115);
    tft.drawNumber(conf.cwAdcBothTo,250,115);
  tft.drawString("ST",0,150);
    tft.drawNumber(conf.cwAdcSTFrom,120,150);
    tft.drawNumber(conf.cwAdcSTTo,250,150);
}

void displayKey(byte mode) // 0: all keys UP, 1: all keys LW, 2:alfa only UP, 3:alfa only LW, 4:numeric only 
{ 
  if ((mode<=1) || (mode==4))
    for (byte i=0;i<10;i++)
      {
      btKey[i].initButtonUL(&tft,31*(i%10),35+(31*int(i/10)),30,30,2,TFT_WHITE,TFT_BLACK,btKeytextU[i],2);
      btKey[i].drawButton();
      }
  if ((mode==0) || (mode==2))
    for (byte i=10;i<50;i++)
      {
      btKey[i].initButtonUL(&tft,31*(i%10),35+(31*int(i/10)),30,30,2,TFT_WHITE,TFT_BLACK,btKeytextU[i],2);
      btKey[i].drawButton();
      }
  if ((mode==1) || (mode==2))
    for (byte i=10;i<50;i++)
      {
      btKey[i].initButtonUL(&tft,31*(i%10),35+(31*int(i/10)),30,30,2,TFT_WHITE,TFT_BLACK,btKeytextL[i],2);
      btKey[i].drawButton();
      }
  displayYN(1,1,1);
}

void displayUSERSet()
{
  // botones setting
  for (byte i=0;i<5;i++) if (btSetact[i]==1)
    {
    btSet[i].initButtonUL(&tft,0,35*i+30,160,30,2,TFT_WHITE,TFT_BLACK,btSettext[i],2);
    btSet[i].drawButton();
    }
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString("1 - USER setting",0,0);
  tft.drawString(conf.lang==0?"Español":"English",180,40);
  tft.drawString(conf.CallSign,180,75);
  tft.drawNumber(conf.latitud,180,110);
  tft.drawNumber(conf.longitud,180,145);
  tft.drawString(timezonetext[conf.timezone],180,180);
}

void displaySetRad()
{
  // botones setting radio
  int backcolor=TFT_WHITE;
  for (byte i=0;i<5;i++) if (btSetRadact[i]==1)
    {
    if (i==1) backcolor=conf.autoMode==1?TFT_YELLOW:TFT_WHITE;
    else backcolor=TFT_WHITE;
    btSetRad[i].initButtonUL(&tft,0,35*i+30,160,30,2,backcolor,TFT_BLACK,btSetRadtext[i],2);
    btSetRad[i].drawButton();
    }
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString("2 - RADIO Setup",0,0);
  tft.drawString(conf.TXall==0?"Ham Bands":"All Range",180,40);
  tft.drawString(conf.autoMode==0?"NO":"YES",180,75);
  tft.drawString(conf.scanallf==0?"Ham Bands":"All Range",180,110);
  tft.drawString(conf.scanmode==0?"No Stop":conf.scanmode==1?"Stop-Resume":"Stop",180,145);
  tft.drawNumber(conf.scandelay,180,180);
}

void displayMemMan()
{
  for (byte i=0;i<5;i++)  if (btMemManact[i]==1)
    {
    btMemMan[i].initButtonUL(&tft,0,35*i+30,160,30,2,TFT_WHITE,TFT_BLACK,btMemMantext[i],2);
    btMemMan[i].drawButton();
    }
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString("7 - Manage Memories",0,0);
}

void displayMemList()   // tftpage=22
{
  tft.setTextSize(1); 
  tft.setTextColor(TFT_WHITE,TFT_BLACK);
  tft.drawString("  n      Name             VFO L/U CW Rt Spl", 0, 74);
  tft.setTextSize(2); 
  for (int i=0; i<6; i++)
    {
    if (memlin==i)
      {
      btMemN[i].initButtonUL(&tft,0,82+20*i,30,20,2,TFT_WHITE,TFT_BLACK,itoa(mempos+i-memlin,buff,10),2);
      btMemN[i].drawButton();
      tft.setTextColor(TFT_BLACK,TFT_WHITE);
      }
    else
      {
      btMemN[i].initButtonUL(&tft,0,82+20*i,30,20,2,TFT_BLACK,TFT_WHITE,itoa(mempos+i-memlin,buff,10),2);
      btMemN[i].drawButton();
      tft.setTextColor(TFT_WHITE,TFT_BLACK);
      }
    tft.drawString("                       ",35,92+20*i);    
    if (memo.act[mempos+i-memlin]==1)
      {
      tft.drawString(memo.descr[mempos+i-memlin],35,92+20*i);    
      tft.drawString(memo.vfoActive[mempos+i-memlin]==VFO_A?"A":"B",160,92+20*i);    
      tft.drawString(memo.isUSB[mempos+i-memlin]==0?"L":"U",180,92+20*i);    
      tft.drawString(memo.cwMode[mempos+i-memlin]==0?" ":"C",200,92+20*i);    
      tft.drawString(memo.ritOn[mempos+i-memlin]==0?" ":"R",220,92+20*i);    
      tft.drawString(memo.splitOn[mempos+i-memlin]==0?" ":"S",240,92+20*i);    
      }
    }
}

void clearMsg(int x,int y,int w, int h)
{
  tft.fillRect(x, y, w, h, TFT_BLACK);
}

void displayMsg(char* msg1,char* msg2,char* msg3,int x,int y,int w, int h)
{
  // Clear the screen areas
  tft.setTextColor(TFT_WHITE, TFT_BLACK); tft.setTextSize(1);
  tft.fillRect(x, y, w, h, TFT_BLACK);
  tft.drawRect(x, y, w, h, TFT_WHITE);
  uint8_t td = tft.getTextDatum(); // Get current datum
  tft.setTextDatum(TL_DATUM);      // Set new datum
  tft.drawString(msg1, x+5, y+5, 2); // Message in font 2
  tft.drawString(msg2, x+5, y+25, 2); // Message in font 2
  tft.drawString(msg3, x+5, y+45, 2); // Message in font 2
  tft.setTextDatum(td); // Restore old datum
  displayNav();     // botones navegación "< H > "
}

const float degtoradf=0.017453;        // 3.1416/180;  coef. para convertir a radianes 
int xant=190; int yant=210;
int xantR=190; int yantR=210;
int backcol=TFT_BLACK; int textcol=TFT_WHITE; 
int dialcol=TFT_YELLOW; int needlecol=TFT_GREEN;

void displayneedle(int value,int xcen,int ycen,int rad,int min,int max,int angle)
{
  float factorg=float((max-min))/float(angle); // unidades por grado
  int grados=(90+angle/2)-((value-min)/factorg);
  float radi=grados*degtoradf;
  int x=xcen+(rad*8/10)*cos(radi);
  int y=ycen-(rad*8/10)*sin(radi);
  tft.drawLine(xcen,ycen,xant,yant,backcol); 
    tft.drawLine(xcen+1,ycen,xant+1,yant,backcol); 
  tft.drawLine(xcen,ycen,x,y,needlecol); 
    tft.drawLine(xcen+1,ycen,x+1,y,needlecol); 
  xant=x; yant=y;
}

void displaygauge(byte tipo,int value,int xcen,int ycen,int rad,byte fondo,int min,int max,int angle,int ndiv)
{
  displayneedle(value,xcen,ycen,rad,min,max,angle);
  if (angle>180) angle=180; if (value<min) value=min; if (value>max) value=max;
  int x0=rad>=100?0:xcen-rad*7/6; 
  int y0=rad>100?0:ycen-rad*7/5;
  int wx=rad>100?320:rad*7/3;
  int wy=rad>100?240:rad*3/2;
  tft.drawRect(x0,y0,wx,wy,textcol);
  tft.fillCircle(xcen,ycen,2,needlecol);
  int dgrdiv=angle/ndiv;      // grados por división
  for (byte i=0; i<ndiv+1;i++) 
    if (i%2==0)
      {
      int dgr=(90+angle/2)-dgrdiv*i;
      float drad=dgr*degtoradf;
      float auxcos=cos(drad);
      float auxsin=sin(drad);
      int x=xcen+(rad*(rad>100?9:10)/10)*auxcos;
      int y=ycen-(rad*(rad>100?9:10)/10)*auxsin;
      int colorline=value==conf.squelchval?TFT_WHITE:TFT_YELLOW;
      tft.drawLine(x-5*auxcos,y+5*auxsin,x,y,colorline);
      tft.drawLine(x-5*auxcos+1,y+5*auxsin,x+1,y,colorline);
      tft.setTextColor(textcol, backcol); tft.setTextSize(rad<100?1:2);
      if (tipo==0) { tft.drawNumber(min+(max-min)*i/ndiv, x-5+5*auxcos, y-10); }     // generic
      else if (tipo==1)   // smeter
        { 
        if (calSmeterReq) tft.setTextColor(TFT_RED, backcol);  
        tft.drawString(conf.smeterTit[i], x+(rad/8)*auxcos, y-rad*auxsin/5); 
        }
      else if (tipo==2) { tft.drawNumber(min+i,x+(rad/10)*auxcos, y-rad*auxsin/8); } // SWR
      else if (tipo==3) { tft.drawNumber(i*15,x+(rad/10)*auxcos, y-rad*auxsin/8); }  // CAP1 & CAP2
      else if (tipo==5) { tft.drawString(conf.smeterTit[i],x+(rad/10)*auxcos, y-rad*auxsin/8); }  // adjust squelch level 
      }   
  if (tipo==1)    // S-meter
    {
    char auxC[4]="OFF";
    if (conf.scanmode==1) 
      {
      strcpy(auxC,"SR"); 
      }
    else if (conf.scanmode==2)
      {
      strcpy(auxC,"St");  
      }
    btSmeter[0].initButtonUL(&tft,xcen-(rad+7),ycen-16,30,20,2,conf.scanmode==0?TFT_LIGHTGREY:conf.scanmode==1?TFT_GREEN:TFT_ORANGE,TFT_BLACK,auxC,1);
    btSmeter[0].drawButton();
    btSmeter[1].initButtonUL(&tft,xcen+rad-23,ycen-16,30,20,2,TFT_ORANGE,TFT_BLACK,"Lev.",1);
    btSmeter[1].drawButton();
    }
  else if (tipo==2)  // SWR
    { 
      tft.drawString("SWR",x0+5,y0+65); 
      tft.drawFloat(SWR,1,x0+90,y0+65); 
    }   
}

void displaySWRcross()
{
  SWR=readSWR(1);
 // float valueF=vFORc; float valueR=vREFc;   // voltage
  float fescala=2;
  float valueF=wFORc*fescala; float valueR=wREFc*fescala;   // power
  int angle=80; int difangle=45; int ndiv=10;
  int difx=40; int radio=170;
  int xcenF=160+difx; int xcenR=160-difx; int ycen=190;
  if (valueF<0) valueF=0; if (valueR>10) valueR=10;
  int wx=320; int wy=240;
  tft.drawRect(0,0,wx,wy,textcol);
  tft.setTextSize(2); tft.drawString("SWR",3,10);    // SWR
  tft.drawString("F:",3,220); tft.drawString("R:",230,220); 
  tft.drawString("SWR:",120,220); tft.drawFloat(SWR,1,170,220);
  int dgrdiv=angle/ndiv;           // grados por división
  // escala FOR
  tft.fillCircle(xcenF,ycen,2,needlecol);
  for (byte i=0; i<ndiv;i++)  // rótulos
    {
    int dgr=(90+difangle+angle/2)-dgrdiv*i;
    float drad=dgr*degtoradf; float auxcos=cos(drad); float auxsin=sin(drad);
    int x=xcenF+(radio*9/10)*auxcos;
    int y=ycen-(radio*9/10)*auxsin;
    tft.drawLine(x-5*auxcos,y+5*auxsin,x,y,TFT_YELLOW);
    tft.drawLine(x-5*auxcos+1,y+5*auxsin,x+1,y,TFT_YELLOW);
    tft.setTextColor(textcol, backcol); tft.setTextSize(1);
    if (i<8) tft.drawNumber(i,x+(radio/ndiv)*auxcos, y-radio*auxsin/8); // SWR
    }   
  // aguja FOR
  float factorg=float((ndiv-0))/float(angle); // unidades por grado
  int grados=(90+difangle+angle/2)-(valueF/factorg);
  float radi=grados*degtoradf;
  int x=xcenF+(radio*8/10)*cos(radi);
  int y=ycen-(radio*8/10)*sin(radi);
  tft.drawLine(xcenF,190,xant,yant,backcol);  tft.drawLine(xcenF+1,190,xant+1,yant,backcol); 
  tft.drawLine(xcenF,ycen,x,y,backcol); tft.drawLine(xcenF+1,ycen,x+1,y,needlecol); 
  tft.setTextSize(2); tft.drawFloat(valueF,1,40,220);
  xant=x; yant=y;

  // escala REF
  tft.fillCircle(xcenR,ycen,2,needlecol);
  for (byte i=0; i<ndiv;i++)  // rótulos
    {
    int dgr=(90-difangle+angle/2)-dgrdiv*i;
    float drad=dgr*degtoradf; float auxcos=cos(drad); float auxsin=sin(drad);
    int x=xcenR+(radio*9/10)*auxcos;
    int y=ycen-(radio*9/10)*auxsin;
    tft.drawLine(x-5*auxcos,y+5*auxsin,x,y,TFT_YELLOW);
    tft.drawLine(x-5*auxcos+1,y+5*auxsin,x+1,y,TFT_YELLOW);
    tft.setTextColor(textcol, backcol); tft.setTextSize(1);
    if (i>=8) tft.drawNumber(ndiv-i,x+(radio/10)*auxcos, y-radio*auxsin/8); // SWR
    }   
  
  // aguja REF
  grados=(90-difangle+angle/2)-(angle-(valueR/factorg));
  radi=grados*degtoradf;
  x=xcenR+(radio*8/10)*cos(radi);
  y=ycen-(radio*8/10)*sin(radi);
  tft.drawLine(xcenR,190,xantR,yantR,backcol); tft.drawLine(xcenR+1,190,xantR+1,yantR,backcol); 
  tft.drawLine(xcenR,ycen,x,y,backcol); tft.drawLine(xcenR+1,ycen,x+1,y,needlecol); 
  tft.setTextSize(2); tft.drawFloat(valueR,1,270,220);
  xantR=x; yantR=y;
}

void displaySmeter(int x, int y, int r, byte fondo)
{
  readSmeter();
  displaygauge(1,smetervalue,x,y,r,fondo,0,90,90,15);
}

int ifShiftValueant=0;

void displayIFS(byte tam,int x, int y)
{
  if (tam==0)   // tamaño pequeño
    {
    int auxvalue=conf.ifShiftValue/160+12;
    for (byte i=0; i<25; i++)
      {
      tft.fillRect(x+i*3,y,2,20,(auxvalue>=i-1)&&(auxvalue<=i+1)?conf.ifShiftValue!=0?TFT_CYAN:TFT_LIGHTGREY:TFT_ORANGE);
      }
    tft.setTextSize(1); tft.setTextColor(TFT_WHITE);
    tft.drawString("IFS",x,y-5);
    tft.drawNumber(conf.ifShiftValue,x+30,y-5);
    }
  else      // tamaño grande
    {
    displayCurve2(); 
    }
}

void displayATT(byte tam,int x, int y)
{
  if (tam==0)   // pequeño
    {
    for (byte i=0; i<25;i++)
      tft.fillRect(x+i*3,y, 2, 20, conf.attLevel*60/250>i*3?conf.attLevel!=0?TFT_CYAN:TFT_LIGHTGREY:TFT_ORANGE);
    tft.setTextSize(1); tft.setTextColor(TFT_WHITE);
    tft.drawString("ATT",x,y-5);
    tft.drawNumber(conf.attLevel,x+30,y-5);
    }
  else    // grande
    {
    displayCurve1();
    }
}

void displayNet()
{
  // botones setting Net
  btNet[0].initButtonUL(&tft,0,35*0+30,160,30,2,conf.autoWiFi==1?TFT_YELLOW:TFT_WHITE,TFT_BLACK,btNettext[0],2);
  btNet[0].drawButton();
  for (byte i=1;i<4;i++) if (btNetact[i]==1)
    {
    btNet[i].initButtonUL(&tft,0,35*i+30,160,30,2,TFT_WHITE,TFT_BLACK,btNettext[i],2);
    btNet[i].drawButton();
    }
  btNet[4].initButtonUL(&tft,0,35*4+30,160,30,2,conf.staticIP==1?TFT_YELLOW:TFT_WHITE,TFT_BLACK,btNettext[4],2);
  btNet[4].drawButton();
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString("4 - WiFi setting",0,0);
  tft.drawString(conf.autoWiFi==1?"YES":"NO",180,40);
  tft.drawString(conf.ssidSTA,180,75);
  tft.drawString(conf.passSTA,180,110);
  tft.drawString(wifimodetext[conf.wifimode],180,145);
  tft.drawString(conf.staticIP==1?itoa(conf.EEip[3],buff,10):"NO",180,180);
}

void displayFlot()
{
  //char btFlottext[5][6]={"Ent","Scan-","Scan+","Lock",">MEM"};
  // botones flotantes
  if (conf.framemode==0)
    {
    btFlot[1].initButtonUL(&tft,275,105,40,30,2,scanF==1?TFT_CYAN:TFT_WHITE,TFT_BLACK,"S-",2);
    btFlot[1].drawButton();
    btFlot[2].initButtonUL(&tft,275,70,40,30,2,scanF==2?TFT_CYAN:TFT_WHITE,TFT_BLACK,"S+",2);
    btFlot[2].drawButton();
    }
  if (conf.framemode==1)
    {
    btFlot[0].initButtonUL(&tft,255,140,65,30,2,TFT_WHITE,TFT_BLACK,btFlottext[0],2);
    btFlot[0].drawButton();
    btFlot[1].initButtonUL(&tft,255,105,65,30,2,scanF==1?TFT_CYAN:TFT_WHITE,TFT_BLACK,btFlottext[1],2);
    btFlot[1].drawButton();
    btFlot[2].initButtonUL(&tft,255,70,65,30,2,scanF==2?TFT_CYAN:TFT_WHITE,TFT_BLACK,btFlottext[2],2);
    btFlot[2].drawButton();
    btFlot[3].initButtonUL(&tft,255,210,65,30,2,keylock>0?TFT_RED:TFT_WHITE,TFT_BLACK,btFlottext[3],2);
    btFlot[3].drawButton();
    btFlot[4].initButtonUL(&tft,255,175,65,30,2,TFT_WHITE,TFT_BLACK,btFlottext[4],2);
    btFlot[4].drawButton();
    }
}
void displaybtFilter()      // botones filtro
{
  if (conf.framemode==0)
    {
    char buff[5];
    itoa(conf.audiolower, buff, 10);
    btFilter[0].initButtonUL(&tft,10,40,30,15,2,modeencoder==4?TFT_YELLOW:TFT_WHITE,TFT_BLACK,buff,1);
    btFilter[0].drawButton();
    itoa(conf.audioupper, buff, 10);
    btFilter[1].initButtonUL(&tft,230,40,30,15,2,modeencoder==5?TFT_YELLOW:TFT_WHITE,TFT_BLACK,buff,1);
    btFilter[1].drawButton();
    itoa(conf.audiopitch, buff, 10);
    btFilter[2].initButtonUL(&tft,126,40,30,15,2,modeencoder==6?TFT_YELLOW:TFT_WHITE,TFT_BLACK,buff,1);
    btFilter[2].drawButton();
    }
}

void displaybtFunction()      // botones function
{
  if (conf.framemode==0)
    {
    btFunction[0].initButtonUL(&tft,275,210,40,30,2,modefunction==1?TFT_YELLOW:TFT_WHITE,TFT_BLACK,btFunctiontext[0],2);
    btFunction[0].drawButton();
    }
}

void displayMain()
{
  if (tftpage==0) {
    initButtons();
    // botones superiores
    // char btMaintext[10][6]={"ATU","V/M","B-","B+","LSB","CW","RIT","SPL","IFS","ATT"};
    if (conf.framemode==0) 
      {
      for (byte i=2;i<6;i++)
        {
        btMain[i].initButtonUL(&tft,btMainposxbis[i],btMainposybis[i],btMaintambis[i],30,2,btMainact[i]==1?btMaincol[i]:TFT_WHITE,TFT_BLACK,btMaintextbis[i],2);
        btMain[i].drawButton();
        }
      }
    else if (conf.framemode==1) 
      {
      for (byte i=0;i<10;i++)
        {
        btMain[i].initButtonUL(&tft,btMainposx[i],btMainposy[i],btMaintam[i],30,2,btMainact[i]==1?btMaincol[i]:TFT_WHITE,TFT_BLACK,btMaintext[i],2);
        btMain[i].drawButton();
        }
      }
    }
}

void displayFreqs()
{
  tft.setTextSize(2);  tft.setTextColor(TFT_WHITE,TFT_BLACK);
  tft.drawString("OSC0",0,153); drawNumberB(OSC0,55,140,100,22,TFT_BLACK,TFT_WHITE,2); 
  tft.drawString("OSC1",0,173); drawNumberB(OSC1,55,160,100,22,TFT_BLACK,TFT_YELLOW,2); 
  tft.drawString("OSC2",0,193); drawNumberB(OSC2,55,180,100,22,TFT_BLACK,TFT_YELLOW,2); 
  tft.setTextColor(TFT_WHITE,TFT_BLACK);
  tft.drawString("Cal",170,193); drawNumberB(conf.calibration,230,180,100,22,TFT_BLACK,TFT_WHITE,2); 
}

byte tabspectrum[MAXLINESWATERFALL][BUFFER_I2C_LEN];
uint32_t colorniv[10]={TFT_BLACK, TFT_NAVY, TFT_BLUE, TFT_CYAN, TFT_CYAN, TFT_GREEN, TFT_GREEN, TFT_YELLOW, TFT_ORANGE, TFT_RED};

void displayWaterfall(int x, int y, int w, int h)
{
  if (waterfallenable==0) return;
  // traza waterfall
  for (int j=0; j<h; j++)    // líneas
    {
    int jplusy=j+y;
    for (int i=0; i<BUFFER_I2C_LEN; i++)    // puntos de la línea
      {
      tft.drawPixel(i+x, jplusy, colorniv[tabspectrum[j][i]/1]); 
      }
    }
}

void displaySpectrum(int x, int y, int w, int h, int resol)
{
  int xwdiv2=x+w/2;
  int yminus15=y-15;
  int yminus25=y-25;
  resol=2;
  tft.setTextSize(1);
  drawStringB("Khz",x+60,y-13,30,12,TFT_BLACK,TFT_WHITE,1);
  drawStringB("Khz",x+180,y-13,30,12,TFT_BLACK,TFT_WHITE,1);
  drawNumberB(conf.frequency/1000-6*resol,x,y-13,40,12,TFT_BLACK,TFT_WHITE,1);
  drawNumberB(conf.frequency/1000,x+w/2-15,y-13,40,12,TFT_BLACK,TFT_WHITE,1);
  drawNumberB(conf.frequency/1000+6*resol,x+w-40,y-13,40,12,TFT_BLACK,TFT_WHITE,1);
  tft.fillRect(xwdiv2+audiolowerold/(50*resol), y-h, (audioupperold-audiolowerold)/(50*resol), h-15,TFT_BLACK);
  tft.fillRect(xwdiv2+conf.audiolower/(50*resol), y-h, (conf.audioupper-conf.audiolower)/(50*resol), h-15,TFT_DARKGREY);
  tft.drawLine(xwdiv2+audiopitchold/(50*resol),y-h,x+w/2+audiopitchold/(50*resol),y-15,TFT_BLACK);
  for (int i=0; i<12; i++)    // líneas bajas cada 1 kHz
    {
    int ipor10=i*10;
    tft.drawFastVLine(xwdiv2+ipor10, yminus25, 10, TFT_DARKGREY);
    tft.drawFastVLine(xwdiv2-ipor10, yminus25, 10, TFT_DARKGREY);
    }
  int yaux = y-16;
  for (int i=0; i<w; i++)     // gráfica
    { 
      int xaux = x+i;
      tft.drawFastVLine(xaux,yaux-spvalold[i],spvalold[i],TFT_BLACK);
      tft.drawFastVLine(xaux,yaux-spval[i],spval[i],TFT_GREEN);
    } 
  tft.drawLine(x+w/2,y-h-5,x+w/2,y-15,TFT_WHITE);
  tft.drawLine(x,y-15,x+w,y-15,TFT_WHITE);
  tft.drawLine(x+w/2+conf.audiopitch/(50*resol),y-h,x+w/2+conf.audiopitch/(50*resol),y-15,TFT_RED);
  // calcula valores waterfall
  for (int i=0; i<MAXLINESWATERFALL-1; i++)   // scroll lines down
    {
    int MAXWFminusiminus1=MAXLINESWATERFALL-i-1;
    int MAXWFminusiminus2=MAXWFminusiminus1-1;
    for (int j=0; j<BUFFER_I2C_LEN; j++)
      tabspectrum[MAXWFminusiminus1][j] = tabspectrum[MAXWFminusiminus2][j];   
    }
  for (int i=0; i<BUFFER_I2C_LEN; i++)
    {
    tabspectrum[0][i] = (byte)(spval[i]);  
    }
}

void displayFrame()
{
  if (conf.framemode==0) 
    {
      displaySpectrum(10,135,256,70,1);
      displayWaterfall(10,138,256,MAXLINESWATERFALL);
      displaybtFilter();
      displaybtFunction();
}
  else if (conf.framemode==1) displaySmeter(190,210,50,1);
  else if (conf.framemode==2) displayFreqs();
}

void displayMenuNav()
{
  for (byte j=0;j<5;j++)
    {
    for (byte i=0;i<4;i++) if (btMenuNavact[(j*4)+i]==1)
      {
      btMenuNav[(j*4)+i].initButtonUL(&tft,80*i,35*j,70,30,2,i+j==0?TFT_YELLOW:TFT_WHITE,TFT_BLACK,btMenuNavtext[(j*4)+i],2);
      btMenuNav[(j*4)+i].drawButton();
      }
    }
}

void updateDisplay(byte alldata) {
  boolean localdebug=false;
  clearTFT();
  if (tftpage==0)   // Main page
    {
    if (alldata==1) 
      {
      displayMain();    // 10 botones principales
      displayNav();     // botones navegación "< H > "
      displayFlot();    // botones "Ent","Scan-","Scan+","Lock",">VFO""
      displayStatus();  // time, status
      if (conf.framemode==0)      // 
        {
        displayFrame();
        }
      else if (conf.framemode==1)      // analog meters
        {
        if (inTx==0) { displaySmeter(190,210,50,1);}
        displayIFS(0,40,185);
        displayATT(0,40,150);
        displayWiFiSt();
        displayRstBt();
        }
      else if (conf.framemode==2) 
        { displayFreqs(); }  // frequencies
      }
    if (modeencoder==0)
      {
      displayFreq(0,1,1,1);   // tunning
      }
    else if (modeencoder==4)  //
      {
      displayFreq(0,1,1,1);   // filter low
      }
    else if (modeencoder==5)  // filter high
      {
      }
    else if (modeencoder==6)  // pitch
      {
      }
    }
  else if (tftpage==1) { displayUSERSet(); displayNav(); }   // Setup User
  else if (tftpage==2) { displaySetRad();  displayNav(); }   // Setup radio
  else if (tftpage==3) { displayCWSet(); displayNav();   }   // CW setting
  else if (tftpage==4) { displayNet(); displayNav();     }   // WiFi
  else if (tftpage==5) { displayCal(); displayNav();     }   // Calibration
  else if (tftpage==6) { displayKEYERSet(); displayNav(); }   // analog KEYER setting
  else if (tftpage==7) { displayMemMan(); displayNav();  }   // Mem manage
  else if (tftpage==8) { displayATU(); displayNav();     }   // ATU
  else if (tftpage==9) { displayTPA2016(); displayNav(); }   // Audio amplifier TPA2016
  else if (tftpage==10) { displayMenuSmeter(); displayNav(); }   // S-Meter
  else if (tftpage==11) { displayCONNS(); displayNav();  }   // Connections
  else if (tftpage==12) { displayTEMP(); displayNav();  }   // Temperatures
  else if (tftpage==13) { displayPORTS(); displayNav();  }   // Ports
  else if (tftpage==14) { displayFILTERS(); displayNav();  }   // Ports
  else if (tftpage==15) { displayOSC(); displayNav();  }   // Ports
  else if (tftpage==16) { displayFIL1(); displayNav();  }   // Filter 1
  else if (tftpage==17) { displayFIL2(); displayNav();  }   // Filter 2
  else if (tftpage==21) {  }   // select AP
  else if (tftpage==22)    // Mem display and select
    {
    Serial2.println("tftpage=22");
    displayFreq(0,1,1,1);
    displayYN(1,1,0);
    displayMemList(); 
    setMEMtoVFO(mempos); 
    }
  else if (tftpage==23)    // test ports
    {
    tft.setTextSize(2);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.drawString("23 - Test Ports",0,0);
    
    tft.drawString("ADSA",0,40);
    for (byte i=0; i<2; i++) tft.drawNumber(adsA.readADC_SingleEnded(i),60+i*80,40);
    for (byte i=2; i<4; i++) tft.drawNumber(adsA.readADC_SingleEnded(i),60+(i-2)*80,60);
    
    tft.drawString("ADSB",0,80);
    for (byte i=0; i<2; i++) tft.drawNumber(adsB.readADC_SingleEnded(i),60+i*80,80);
    for (byte i=2; i<4; i++) tft.drawNumber(adsB.readADC_SingleEnded(i),60+(i-2)*80,100);
    }
  else if (tftpage==24) {  displayMenuNav();  }
}

void DisplayVersionInfo(const __FlashStringHelper * fwVersionInfo)
{
  tft.drawString("sBitx32 Version:",0,20);
  tft.drawString(fwVersionInfo,220,20);
}

void  initButtons()
{
  btMainact[0]=inTx==1?1:0;         // RX / TX
  btMainact[1]=0;                   // V/M
  btMainact[2]=0;                   // Band Down
  btMainact[3]=0;                   // Band Up
  btMainact[4]=conf.isUSB==1?1:0;   // LSB / USB
  strcpy(btMaintext[0],inTx==1?"TX":"ATU");          // TX / ATU (only at RX
  strcpy(btMaintext[4],conf.isUSB==1?"USB":"LSB");   // LSB / USB
  strcpy(btMaintextbis[4],conf.isUSB==1?"U":"L");   // LSB / USB
  btMainact[5]=conf.cwMode>0?1:0;   // CW
  btMainact[6]=conf.ritOn==1?1:0;   // RIT
  btMainact[7]=conf.splitOn==1?1:0; // Split
  btMainact[8]=conf.ifShiftValue!=0?1:0; // IFS
  btMainact[9]=conf.attLevel!=0?1:0; // ATT
}

int checkYN()
{
  uint16_t x, y;
  if (tft.getTouch(&x, &y))
    {
    if (tft.getRotation()==1) { y=tft.height()-y;}
    else if (tft.getRotation()==3) { x=tft.width()-x; y=tft.height()-y;}
    for (byte i=0;i<3;i++)
      if (btYN[i].contains(x,y)) { delay(100); return i;  } 
    }
  return -1;
}

long getNumberTFT(long valini, byte len, char *tunits)
{
  clearTFT();
  tft.setTextSize(4); tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString(tunits,200,0);
  displayKey(4);
  strcpy(auxtft,itoa(valini,buff,10));
  tft.setTextSize(4); tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString(auxtft,0,0);
  uint16_t x, y;
  while (true) 
    {
    if (tft.getTouch(&x, &y))
      {
      if (tft.getRotation()==1) { y=tft.height()-y;}
      else if (tft.getRotation()==3) { x=tft.width()-x; y=tft.height()-y;}
      for (byte i=0; i<10; i++)    // check KeyN buttons
        {
        if (btKey[i].contains(x,y)) 
          {
          //delay(100);
          if (strlen(auxtft)<len)
            {
            strcat(auxtft, itoa(i,buff,10));
            tft.setTextSize(4); tft.setTextColor(TFT_WHITE, TFT_BLACK);
            tft.drawString(auxtft,0,0);
            }
          }
        }
      int result=checkYN();
      if (result==0) { return atol(auxtft);   }   // OK
      else if (result==1) { return -1; }     // Cancel
      else if (result==2)                    // Backspace
        { 
        if (strlen(auxtft)>0) auxtft[strlen(auxtft)-1]=0; 
        tft.drawString("                    ",0,0);  
        tft.drawString(auxtft,0,0); }     
      }
    }
}

int getCharTFT(char *valini, byte tam)
{
  byte modekeyer=0;
  clearTFT();
  displayKey(modekeyer);
  uint16_t x, y;
  strcpy(auxtft,valini);
  tft.setTextSize(4); tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString(auxtft,0,0);
  while (true) 
    {
    if (tft.getTouch(&x, &y))
      {
      if (tft.getRotation()==1) { y=tft.height()-y;}
      else if (tft.getRotation()==3) { x=tft.width()-x; y=tft.height()-y;}
      for (byte i=0; i<50; i++)    // check KeyA buttons
        {
        if (btKey[i].contains(x,y)) 
          {
          if (i==49)    // cambio Lower/Upper 
            {
            if (modekeyer==0) modekeyer=1; else modekeyer=0;
            displayKey(modekeyer);
            }
          else
            {
            if (strlen(auxtft)<tam)
              {
              if (modekeyer==0)
                strcat(auxtft, btKeytextU[i]);
              else
                strcat(auxtft, btKeytextL[i]);
              tft.setTextSize(4); tft.setTextColor(TFT_WHITE, TFT_BLACK);
              tft.drawString(auxtft,0,0);
              }
            }
          delay(100);
          }
        }
      int result=checkYN();
      if (result==0) { return strlen(auxtft);   }        // OK
      else if (result==1) { return -1; }  // Cancel
      else if (result==2)                 // Backspace
        { 
        if (strlen(auxtft)>0) auxtft[strlen(auxtft)-1]=0; 
        tft.drawString("                    ",0,0);  
        tft.drawString(auxtft,0,0); }     
      }
    }
}

void editMEM()
{
  
}

void checkNavButtons(uint16_t x, uint16_t y)
{
  for (byte i=0; i<5;i++)     // navigation buttons
    {
    if (btNav[i].contains(x,y)) 
      {
      delay(100);
      if (i==0)         // Home, "H"
        { 
        if (tftpage==15) setFrequency(conf.frequency);
        if (tftpage==0) { setFrame(); }
        else tftpage=0;
        }               // main page
      else if (i==1)       // flecha izquierda "<"  
        { 
        if (tftpage==15) setFrequency(conf.frequency);
        tftpage=tftpage>0?tftpage-1:MAX_PAGES; // prior page
        }     
      else if (i==2)        // flecha derecha ">"
        { 
        if (tftpage==15) setFrequency(conf.frequency);
        tftpage=tftpage<MAX_PAGES?tftpage+1:0;  // next page
        }
      else if (i==3) { if (tftpage==15)  setFrequency(conf.frequency); tftpage=24; }
      else if (i==4) {  }
      updateDisplay(1);
      }
   }
}

void checkMenuButtons(uint16_t x, uint16_t y)
{
  for (byte i=0; i<20;i++)     // menu pages buttons
    {
    if (btMenuNav[i].contains(x,y)) 
      {
      delay(50);
      if ((i>=0) && (i<=19)) if (btMenuNavact[i]==1) { tftpage=i; }       
      updateDisplay(1);
      }
   }
}

void setWiFi()
{
   if (WiFi.isConnected())
     { 
     WiFi.disconnect();  
     displayWiFiSt();
     }
   else
     {
     initWiFi();
     if ((conf.wifimode==1) || (conf.wifimode==3))  
       connectSTA();
     if (conf.wifimode>0) 
       initNetServices();
     }
}

void checkStaButtons(uint16_t x, uint16_t y)
{
  for (byte i=0; i<5;i++)   // buttons status line
    {
     if (btSta[i].contains(x,y)) 
       {
       if (i==0) { setWiFi(); }            // WiFi
       else if (i==1) {     }
       else if (i==2) {     }
       else if (i==3) {     }
       else if (i==4) { ESP.restart(); }    // reset
       updateDisplay(1);
       }
    }
}

void checkSelButtons(uint16_t x, uint16_t y)
{
  for (byte i=0;i<4;i++)  // botones Cancel,OK,N,Backspace
    { 
    if (btSel[i].contains(x,y))  
      { 
      if (i==0) { tftapactual--; if (tftapactual<0) tftapactual=nAP-1; drawAP(tftapactual);}
      else if (i==1) { tftpage=4; updateDisplay(1); }
      else if (i==2) { WiFi.SSID(tftapactual).toCharArray(conf.ssidSTA, 20); saveconf(); tftpage=4; updateDisplay(1); }
      else if (i==3) { tftapactual++; if (tftapactual>nAP-1) tftapactual=0; drawAP(tftapactual);}
      }  
    }
}

void startScan(byte dir)
{
  if ((conf.actualBand==99) && (conf.scanallf==0))
    {
    tftErrormsg("OUT OF BAND","Modify parameter","Scan mode");
    tftpage=2;
    updateDisplay(1);
    return;
    }
  setSCAN(scanF==dir?0:dir);
  //astframemode=conf.framemode;
  //conf.framemode=0;
}

void checkFlotButtons(uint16_t x, uint16_t y)
{
  for (byte i=0; i<5; i++)    // check float buttons
    { 
    if (btFlot[i].contains(x,y)) 
      {
      if (i==3) { setLOCK(keylock==0?1:0); delay(50); }    // lock key
      if (keylock==1) return; 
      if (i==0)     // ent frequency
        {
        setSCAN(0);
        long auxL=getNumberTFT(conf.frequency/1000,5,"Khz");  // retorna con OK o Cancel
        if (auxL!=-1) { setFrequency(auxL*1000); }
        updateDisplay(1);
        }
      else if (i==1) { startScan(1); }     // scan down
      else if (i==2) { startScan(2); }     // scan up
      else if (i==4)        // VFO to Mem 
        { 
        if (conf.memMode==0)   // VFO mode
          { 
          conf.lastmempos=mempos; 
          saveVFOtoMem();
          updateDisplay(1); 
          }
        else               // Mem mode
          { 
          mempos=conf.lastmempos; 
          setMEMtoVFO(mempos); 
          }          
        saveconf();
        }     
      }
    }
}

void checkFreqButtons(uint16_t x, uint16_t y)
{
  for (byte i=0; i<9; i++)    // check step buttons
    { 
    if (btFreq[i].contains(x,y)) 
      { 
      setSTEP(i);
      if (i<=5) conf.frequency=conf.frequency/1000 *1000;  
      else if (i==6) conf.frequency=conf.frequency/100 *100;  
      else if (i==7) conf.frequency=conf.frequency/10 *10;  
      displayFreq(0,1,1,1); 
      } 
    }
}

void checkFilterButtons(uint16_t x, uint16_t y)
{
  for (byte i=0; i<3; i++)    // check step buttons
    { 
    if (btFilter[i].contains(x,y)) 
      { 
      if (i==0) 
        { 
        if (modeencoder==4) modeencoder=0; else modeencoder=4;
        }
      else if (i==1) 
        { 
        if (modeencoder==5) modeencoder=0; else modeencoder=5;
        }
      else if (i==2)
        {
        if (modeencoder==6) modeencoder=0; else modeencoder=6;
        }
      tftpage=0; 
      updateDisplay(1);
      } 
    }
}
void checkFunctionButtons(uint16_t x, uint16_t y)
{
  for (byte i=0; i<1; i++)    // check function button
    { 
    if (btFunction[i].contains(x,y)) 
      { 
      if (i==0) 
        { 
        if (modefunction==0) modefunction=1; else modefunction=0;
        }
      tftpage=0; 
      updateDisplay(1);
      } 
    }
}

void checkVFOButtons(uint16_t x, uint16_t y)
{
  for (byte i=0; i<2;i++)    // check buttons VFO
    {
    if (btVFO[i].contains(x,y)) 
      {
      setVFO(conf.vfoActive==VFO_A?VFO_B:VFO_A);
      updateDisplay(1);
      return;
      }
    }
}

void checkSmeterButtons(uint16_t x, uint16_t y)
{
  for (byte i=0; i<2;i++)    // check buttons function
    {
    if (btSmeter[i].contains(x,y)) 
      {
      if (i==0)
        {
        conf.scanmode=conf.scanmode<2?conf.scanmode+1:0;
        displaySmeter(190,210,50,0);
        }
      else if (i==1)
        {
        conf.squelchval=getValByKnob(5,conf.squelchval,0,90,6,"Squelch",1);
        updateDisplay(1);
        }
      saveconf();
      }
    }
}

void setMemMode(uint8_t value)
{
  conf.memMode=value;
  Serial2.println("setMemMode:"); Serial2.println(conf.memMode);
  if (conf.memMode==1)   // mem mode
    {
    tftpage=22;
    saveFREQ();
    }
  else
    {
    tftpage=0;
    restFREQ();
    }
  updateDisplay(1);
  sendData(tcpmemmode);
}
void setmemPos(uint8_t pos)
{
  mempos = pos;
  Serial2.print("mempos="); Serial2.println(mempos);
  updateDisplay(1);
}

void checkMainButtons(uint16_t x, uint16_t y)
{
  for (byte i=0; i<10;i++)    // check buttons function
    {
    if (btMain[i].contains(x,y)) 
      {
      if (i==0)   // ATU
        { 
        if (inTx==1) 
          { txTFT=false; stopTx(); }  // RX
        else 
          { 
           txTFT=true;
           startTx(TX_CW,1); 
           cwKeydown();
           sintATU();
           delay_background(conf.ATUdelay*1000,9,1);
           cwKeyUp();
           stopTx();
           txTFT=false;
           }  // TX
        strcpy(btMaintext[0],inTx==0?"ATU":"TX");
        updateDisplay(1);
        }   
      else if (i==1)    // V/M
        { 
        setMemMode(conf.memMode==0?1:0);
        }
      else if (i==2)    //Prior Band
        { 
        strcpy(btMaintext[4],conf.isUSB==1?"USB":"LSB"); 
        setNextHamBandFreq(conf.frequency,-1);
        setRIT(0); 
        displayFreq(1,1,1,1);
        } 
      else if (i==3)    //Next Band
        { 
        strcpy(btMaintext[4],conf.isUSB==1?"USB":"LSB"); 
        setNextHamBandFreq(conf.frequency, 1); 
        setRIT(0);
        displayFreq(1,1,1,1);
        }  
      else if (i==4)   // LSB-USB 
        { 
        setUSB(conf.isUSB==0?1:0);
        }
      else if (i==5)   // CW 
        {
        setCW(conf.cwMode>0?0:1);
        setFrequency(conf.frequency);
        }
      else if (i==6) { setRIT(conf.ritOn==0?1:0); }   // RIT
      else if (i==7) { setSPLIT(conf.splitOn==0?1:0); }   // SPL
      else if (i==8)    // IFS
        {
        conf.ifShiftValue = getValByKnob(2, conf.ifShiftValue, -2000, 2000, 1, "IF Shift", 2);
        isIFShift=conf.ifShiftValue!=0?1:0; 
        btMainact[i]=conf.ifShiftValue!=0?1:0;
        setIFS(conf.ifShiftValue,1);
        }
      else if (i==9)    // ATT
        {
        conf.attLevel = getValByKnob(6, conf.attLevel, -300, 300, 1, "ATT Level", 1);
        btMainact[i]=conf.attLevel==0?0:1;
        setATT(conf.attLevel,1);
        }
      if (i>0) saveconf();
      }
    }
}

void checkSetButtons(uint16_t x, uint16_t y)
{
  for (byte i=0; i<5;i++)
  {
   if (btSet[i].contains(x,y)) 
     {
     if (i==0) 
       { conf.lang=conf.lang==0?1:0; }           // language
     else if (i==1)                              // CallSign
       { 
       int auxI=getCharTFT(conf.CallSign,10); 
       if (auxI !=-1) { strcpy(conf.CallSign,auxtft);  }
       }   
     else if (i==2)           // Latitude
       {  
       long auxL=getNumberTFT(conf.latitud,10,"");  
       if (auxL!=-1) { conf.latitud=auxL; }
       }                         
     else if (i==3)               // Longitude
       {  
       long auxL=getNumberTFT(conf.longitud,10,"");  
       if (auxL!=-1) { conf.longitud=auxL; }
       }                         // Latitude
     else if (i==4) 
       {  
       conf.timezone=getValByKnob(19, conf.timezone, 0, 38, 1, "Time Zone", 1);
       }                         // 
     saveconf(); 
     updateDisplay(1);
     }
  }
}

void checkSetRadButtons(uint16_t x, uint16_t y)
{
  for (byte i=0; i<5;i++)
  {
   if (btSetRad[i].contains(x,y)) 
     {
     if (i==0) { conf.TXall=conf.TXall==0?1:0; }    // TX range
     else if (i==1) { conf.autoMode=conf.autoMode==0?1:0; }    // SSB auto                         
     else if (i==2) { conf.scanallf=conf.scanallf==0?1:0; }   // scan mode
     else if (i==3) { if (conf.scanmode<2) conf.scanmode++; else conf.scanmode=0; }    // scan mode                      
     else if (i==4) { conf.scandelay=getValByKnob(1,conf.scandelay,1,60,1,"Resume del. (s)",2); }  // resume delay                
     saveconf(); 
     updateDisplay(1);
     }
  }
}

void checkNetButtons(uint16_t x, uint16_t y)
{
  for (byte i=0; i<5;i++)
    {
    if (btNet[i].contains(x,y)) 
      {
      if (i==0)      { conf.autoWiFi=conf.autoWiFi==0?1:0;  }   // AutoConnect
      else if (i==1) { selectapTFT(); }   // SSID
      else if (i==2) { if (getCharTFT(conf.passSTA,10) !=-1) strcpy(conf.passSTA ,auxtft); }  // Password
      else if (i==3) { if (conf.wifimode<3) conf.wifimode++; else conf.wifimode=0; }        // WiFi Mode
      else if (i==4) { conf.staticIP=conf.staticIP==0?1:0; }                                //  Static IP
      saveconf();  
      if (i!=1) updateDisplay(1);
      }
    }
}

void checkCalButtons(uint16_t x, uint16_t y)
{
  for (byte i=0; i<5;i++)
    {
     if (btCAL[i].contains(x,y)) 
       {
       if (i==0)      { setupVFO(); }       // CAL
       else if (i==1) { setupBFO();  }      // BFO
       else if (i==2) {  }    
       else if (i==3) {  }      // 
       else if (i==4) { initConf(); saveconf(); ESP.restart(); }      // 
       updateDisplay(1);
       }
    }
}

void checkCONButtons(uint16_t x, uint16_t y)
{
  for (byte i=0; i<5;i++)
    {
     if (btCON[i].contains(x,y)) 
       {
       if (i==0)      { setupconnmode(); }   // Conn Mode
       else if (i==1) { setupserial2mode();  }   // Serial2 mode
       else if (i==2) {  }      // 
       else if (i==3) {  }      // 
       else if (i==4) {  }      // 
       updateDisplay(1);
       }
    }
}

void setTempProbe(byte n)
{
  conf.nprobe[n]=conf.nprobe[n]<nTemp-1?conf.nprobe[n]+1:0;
}

void checkTEMPButtons(uint16_t x, uint16_t y)
{
  for (byte i=0; i<5;i++)
    {
     if (btTEMP[i].contains(x,y)) 
       {
       if (i<=2)      
         { 
         conf.nprobe[i]++;
         // setTempProbe(i); 
         }   // 
       else if (i==3) {  }      // 
       else if (i==4) {  }      // 
       updateDisplay(1);
       }
    }
}

void checkPORTSButtons(uint16_t x, uint16_t y)
{
  for (byte i=0; i<5;i++)
    {
     if (btPORTS[i].contains(x,y)) 
       {
       if (i==0)      { conf.tcpenable=conf.tcpenable==0?1:0; }   // 
       else if (i==1) { conf.udpenable=conf.udpenable==0?1:0; }      // 
       else if (i==2) { conf.wsenable=conf.wsenable==0?1:0; }      // 
       else if (i==3) { conf.webenable=conf.webenable==0?1:0; }      // 
       else if (i==4) {  }      // 
       updateDisplay(1);
       }
    }
}

void checkMemManButtons(uint16_t x, uint16_t y)
{
  for (byte i=0; i<5;i++)
    {
     if (btMemMan[i].contains(x,y)) 
       {
       if (i==0)      {  }   // 
       else if (i==1) {  }   // 
       else if (i==2) {  }      // 
       else if (i==3) {  }      // 
       else if (i==4) { deleteMemo(); }       //  Clear all memories
       updateDisplay(1);
       }
    }
}

void checkATUButtons(uint16_t x, uint16_t y)
{
  for (byte i=0; i<5;i++)
  {
   if (btATU[i].contains(x,y)) 
     {
     if (i==0) { conf.ATUZM2enabled=conf.ATUZM2enabled==0?1:0; }         // 
     else if (i==1) { setupATU(1); }    // 
     else if (i==2) { setupATU(2); }    //                          
     else if (i==3) {  }    //                     
     else if (i==4) {  }    //          
     saveconf(); 
     updateDisplay(1);
     }
  }
}

  
//  audioamp.setLimitLevelOn();// See Datasheet page 22 for value -> dBV conversion table
//  audioamp.setLimitLevel(25);  // range from 0 (-6.5dBv) to 31 (9dBV)
                               // or turn off with setLimitLevelOff()
//  audioamp.setAttackControl(5);// See Datasheet page 23 for value -> ms conversion table
//  audioamp.setHoldControl(0);// See Datasheet page 24 for value -> ms conversion table
//  audioamp.setReleaseControl(11);// See Datasheet page 24 for value -> ms conversion table

void setTPAenable(boolean enablevalue)
{   // los dos canales a la vez
  audioamp.enableChannel(enablevalue, enablevalue);
}  

void setTPAComp()
{   
//#define TPA2016_AGC_OFF 0x00 ///<  Turn off AGC
//#define TPA2016_AGC_2 0x01   ///< AGC compression ratio 1:2
//#define TPA2016_AGC_4 0x02   ///< AGC compression ratio 1:4
//#define TPA2016_AGC_8 0x03   ///< AGC compression ratio 1:8
  audioamp.setAGCCompression(conf.TPA2016Compvalue);
  if (conf.TPA2016Compvalue==0) audioamp.setReleaseControl(0); 
  else   audioamp.setReleaseControl(11);
}  

void setTPAGain(uint8_t gain) // de -28 a +30,  (0-12)
{
  audioamp.setGain(gain);   setTPAenable(false); setTPAenable(true);
}

void setTPAMaxGain(uint8_t maxgain) // de -28 a +30,  (0-12)
{
  audioamp.setAGCMaxGain(maxgain);
}

void checkTPAButtons(uint16_t x, uint16_t y)
{
  tft.drawString(conf.TPA2016enabled==1?"YES":"NO",180,40);
  tft.drawString(conf.TPA2016Compvalue==0?"AGC OFF":itoa(conf.TPA2016Compvalue,buff,10),180,75);
  tft.drawNumber(conf.TPA2016Compmaxgain,180,110);
  for (byte i=0; i<5;i++)
    {
    if (btTPA[i].contains(x,y)) 
      {
      if (i==0)     // ON/OFF
        { conf.TPA2016enabled=conf.TPA2016enabled==0?1:0; }    
      else if (i==1)     // compression value
        { 
        conf.TPA2016Compvalue=conf.TPA2016Compvalue<3?conf.TPA2016Compvalue+1:0; // 0-3 
        setTPAComp(); 
        } 
      else if (i==2)     // maxgain value
        { 
        conf.TPA2016Compmaxgain=getValByKnob(7, conf.TPA2016Compmaxgain, 0, 12, 1, "Max Gain", 2);// 0-12 
        setTPAMaxGain(conf.TPA2016Compmaxgain);
        } 
      else if (i==3)     // gain value
        { 
        conf.TPA2016Gain=getValByKnob(8,conf.TPA2016Gain,-28,30,1,"Gain", 2);// -28 a +30 
        s2("conf.TPA2016Gain:"); s2(conf.TPA2016Gain); s2(crlf);
        setTPAGain(conf.TPA2016Gain);
        } 
      saveconf(); 
      updateDisplay(1);
      }
    }
}

void checkSMEButtons(uint16_t x, uint16_t y)
{
  tft.drawString(conf.TPA2016enabled==1?"YES":"NO",180,40);
  tft.drawString(conf.TPA2016Compvalue==0?"AGC OFF":itoa(conf.TPA2016Compvalue,buff,10),180,75);
  tft.drawNumber(conf.TPA2016Compmaxgain,180,110);
  for (byte i=0; i<5;i++)
    {
    if (btSME[i].contains(x,y)) 
      {
      if (i==0) { conf.sMeterLevels[0]=getValByKnob(1, conf.sMeterLevels[0], 0, 20000, 100, "MIN.", 3); }    
      else if (i==1) { conf.sMeterLevels[15]=getValByKnob(1, conf.sMeterLevels[15], 0, 20000, 100, "MAX.", 3); }    
      else if (i==2) {  } 
      else if (i==3) {  }    // 
      else if (i==4) {  }    // 
      calcSmeterScale();
      saveconf(); 
      updateDisplay(1);
      }
    }
}

void checkCWButtons(uint16_t x, uint16_t y)
{
  for (byte i=0; i<5;i++)
    {
     if (btCW[i].contains(x,y)) 
       {
       if (i==0) {conf.cwKeyType=conf.cwKeyType==2?0:conf.cwKeyType+1; }   // Key type
       else if (i==1) 
         { 
         int wpm=1200/conf.cwSpeed;
         wpm=getValByKnob(1, wpm, 3, 50, 1, "WPM", 3);
         conf.cwSpeed = 1200/wpm;
         }   
       else if (i==2) { conf.sideTone=getValByKnob(1, conf.sideTone, 100, 2000, 10, "Tone", 2); }    
       else if (i==3) { conf.cwDelayTime=getValByKnob(1, conf.cwDelayTime, 0, 250, 10, "Delay", 2); }      
       else if (i==4) { conf.delayBeforeCWStartTime=getValByKnob(1, conf.delayBeforeCWStartTime, 0, 250, 10, "Start time", 2); }
       saveconf(); 
       updateDisplay(1);
       }
    }
}

void checkKEYERButtons(uint16_t x, uint16_t y)
{
  for (byte i=0; i<8;i++)
    {
    if (btKEYER[i].contains(x,y)) 
      {
      if (i==0) {conf.cwAdcDotFrom=getValByKnob(11, conf.cwAdcDotFrom, 100, 3000, 10, "DOT from", 2);}   
      else if (i==4) {conf.cwAdcDotTo=getValByKnob(15, conf.cwAdcDotTo, 100, 3000, 10, "DOT to", 2);}   
      else if (i==1) {conf.cwAdcDashFrom=getValByKnob(12, conf.cwAdcDashFrom, 100, 3000, 10, "DASH from", 2);}   
      else if (i==5) {conf.cwAdcDashTo=getValByKnob(16, conf.cwAdcDashTo, 100, 3000, 10, "DASH to", 2);}   
      else if (i==2) {conf.cwAdcBothFrom=getValByKnob(13, conf.cwAdcBothFrom, 100, 3000, 10, "BOTH from", 2);}  
      else if (i==6) {conf.cwAdcBothTo=getValByKnob(17, conf.cwAdcBothTo, 100, 3000, 10, "BOTH to", 2);}  
      else if (i==3) {conf.cwAdcSTFrom=getValByKnob(14, conf.cwAdcSTFrom, 100, 3000, 10, "ST from", 2);}  
      else if (i==7) {conf.cwAdcSTTo=getValByKnob(18, conf.cwAdcSTTo, 100, 3000, 10, "ST to", 2);}  
      saveconf(); 
      updateDisplay(1);
      }
    }
}

void checkFILTButtons(uint16_t x, uint16_t y)
{
  for (byte i=0; i<5;i++)
    {
    if (btFILT[i].contains(x,y)) 
      {
      if (i==0) { conf.optimalmode=conf.optimalmode<1?conf.optimalmode+1:0; }    // 
      else if (i==1) 
        { 
        audiolowerold=conf.audiolower;
        conf.audiolower=getValByKnob(1, conf.audiolower, 0, 1500, 10, "SSB lower", 3); 
        sendFilterLyraT(conf.audiolower,conf.audioupper);
        }    
      else if (i==2) 
        { 
        audioupperold=conf.audioupper;
        conf.audioupper=getValByKnob(1, conf.audioupper, 1600, 5000, 10, "SSB upper.", 3); 
        sendFilterLyraT(conf.audiolower,conf.audioupper);
        }    
      saveconf(); 
      updateDisplay(1);
      }
    }
}

void checkFIL1Buttons(uint16_t x, uint16_t y)
{
  for (byte i=0; i<5;i++)
    {
    if (btFIL1[i].contains(x,y)) 
      {
      if (i==0) { conf.fc1=getValByKnob(20, conf.fc1, 44995000, 45005000, 10, "F1 Cent.", 3); }    
      else if (i==1) { conf.df1=getValByKnob(20, conf.df1, 0, 30000, 10, "F1 WB.", 3); }    
      saveconf(); 
      updateDisplay(1);
      }
    }
}

void checkFIL2Buttons(uint16_t x, uint16_t y)
{
  for (byte i=0; i<5;i++)
    {
    if (btFIL2[i].contains(x,y)) 
      {
      if (i==0) { conf.fc2=getValByKnob(21, conf.fc2, 11055000, 11058000, 10, "F2 Cent.", 3); }    
      else if (i==1) { conf.df2=getValByKnob(21, conf.df2, 0, 5000, 10, "F2 WB.", 3); }    
      saveconf(); 
      updateDisplay(1);
      }
    }
}

void checkOSCButtons(uint16_t x, uint16_t y)
{
  for (byte i=0; i<6;i++)
    {
    if (btOSC[i].contains(x,y)) 
      {
      if (i==0)      { conf.gen0=getValByKnob(22, conf.gen0, 450000, 105000000, 1000, "GEN0", 3); }    
      else if (i==1) { conf.gen1=getValByKnob(23, conf.gen1, 450000, 105000000, 1000, "GEN1", 3); }    
      else if (i==2) { conf.gen2=getValByKnob(24, conf.gen2, 450000, 105000000, 1000, "GEN2", 3); }    
      else if (i==3) { conf.gen0=getValByKnob(25, conf.gen0, 450000, 105000000, 1000000, "GEN0+", 3); }    
      else if (i==4) { conf.gen1=getValByKnob(26, conf.gen1, 450000, 105000000, 1000000, "GEN1+", 3); }    
      else if (i==5) { conf.gen2=getValByKnob(27, conf.gen2, 450000, 105000000, 1000000, "GEN2+", 3); }    
      saveconf(); 
      updateDisplay(1);
      }
    }
}

void setRIT(byte value) 
  { 
  conf.ritOn=value; 
  btMainact[6]=value;
  sendData(tcpritOn);   // reenvía estado a cliente
  if (conf.ritOn==1) 
    { 
    setSPLIT(0); 
    conf.ritTxFrequency = conf.frequency;
    }
  //setFrequency(conf.ritTxFrequency);
  displayMain();
  displayFreq(1,1,1,1);
  }
  
void setSPLIT(byte value) 
  { 
  conf.splitOn=value;
  btMainact[7]=value;
  sendData(tcpsplitOn); 
  if (conf.splitOn==1) 
    { 
    setRIT(0); 
    setFrequency(conf.ritTxFrequency);
    }
  displayMain();
  displayFreq(1,1,1,1);
  }

void setCW(byte value) 
  { 
  conf.cwMode=value; 
  sendData(tcpcwMode); 
  if (conf.vfoActive==VFO_A) conf.cwModeA=conf.cwMode; else conf.cwModeB=conf.cwMode;
  btMainact[5]=(conf.cwMode>0);
  displayMain(); 
  }

void setUSB(byte value) 
  {
  if (value==conf.isUSB)  return;
  conf.isUSB=value; sendData(tcpisUSB); 
  if (conf.vfoActive==VFO_A) conf.isUSBA=conf.isUSB; else conf.isUSBB=conf.isUSB;
  btMainact[4]=conf.isUSB==1?1:0; 
  strcpy(btMaintext[4],conf.isUSB==1?"USB":"LSB"); 
  displayMain(); 
  setFrequency(conf.frequency);
  displayFreq(1,1,1,1); 
  displayFrame();
}
  
void setVFO(byte value) 
  { 
  conf.vfoActive=value; sendData(tcpvfoActive); 
  if (conf.vfoActive==VFO_A) 
    {
    conf.isUSB=conf.isUSBA; 
    conf.frequency = conf.frequencyA;
    }
  else 
    {
    conf.isUSB=conf.isUSBB;
    conf.frequency = conf.frequencyB;
    }
  setUSB(conf.vfoActive==VFO_A?conf.isUSBA:conf.isUSBB);
  setCW(conf.vfoActive==VFO_A?conf.cwModeA:conf.cwModeB);
  setFrequency(conf.frequency);
  displayFreq(1,1,1,1); 
  }

void setFrame()
{
  conf.framemode=conf.framemode<2?conf.framemode+1:0;
  updateDisplay(1);
  saveconf();
}

void setSCAN(byte value)
{
  if (value==scanF) return;
  scanF=value; sendData(tcpscanst); 
  displayFlot();
}

void setLOCK(byte value)
{
  if (value==2) 
    {
    strcpy(btFlottext[3],"Rem.");
    value=1;
    }
  else
    strcpy(btFlottext[3],"Lock");
  keylock=value; sendData(tcpkeylock); 
  displayFlot();
}

void setATT(int value, byte local)
{
  conf.attLevel=value;
  btMaincol[9]=conf.attLevel!=0?TFT_YELLOW:TFT_WHITE; // ATT
  setFrequency(conf.frequency);
  if (local==1) {
    updateDisplay(1);
    sendData(tcpattlevel);
    }
  else if (local==0)
    {
    displayMain();
    displayATT(0,40,150);
    }
}

void setIFS(int value, byte local)
{
  conf.ifShiftValue=value;
  btMaincol[8]=conf.ifShiftValue!=0?TFT_YELLOW:TFT_WHITE; // IFS
  setFrequency(conf.frequency);
  if (local==1) {
    updateDisplay(1);
    //sendData(tcpifShiftVal);
  }
  else
    {
    btMaincol[8]=conf.ifShiftValue!=0?TFT_YELLOW:TFT_WHITE; // IFS
    displayMain();
    displayIFS(0,40,185);
    }
}

void setSTEP(byte value)
{
  conf.tuneStepIndex=value; sendData(tcptunestep); 
  displayFreq(1,1,1,1);  
}

void saveFREQ()     // save freq values
{
  Serial2.print("saveFREQ");
  Serial2.print("   freq: "); Serial2.println(conf.frequency);
  
  vfoActiveAct=conf.vfoActive; // VFO 
  isUSBAct=conf.isUSB;         // mode
  cwModeAct=conf.cwMode;       // CW mode
  ritOnAct=conf.ritOn;         // RIT
  splitOnAct=conf.splitOn;     // SPL
  frequencyAct=conf.frequency; // frequency
  ritTxFrequencyAct=conf.ritTxFrequency;  // frequency TX Rit
  splitOnAct=conf.splitOn;     // isUSB TX split mode
  cwModeAct=conf.cwMode;       // CW mode TX split
  ftxsplAct=conf.frequencyB;   // frequency TX Split
}

void restFREQ()     // restore freq values
{
  conf.vfoActive=vfoActiveAct;      // VFO 
  conf.isUSB=isUSBAct;       // mode
  conf.cwMode=cwModeAct;      // CW mode
  conf.ritOn=ritOnAct;       // RIT
  conf.splitOn=splitOnAct;     // SPL
  conf.frequency=frequencyAct;  // frequency
  conf.ritTxFrequency=ritTxFrequencyAct;  // frequency TX Rit
  conf.splitOn=splitOnAct;      // isUSB TX split mode
  conf.cwMode=cwModeAct;     // CW mode TX split
  conf.frequencyB=ftxsplAct;          // frequency TX Split
}

void handletfttouch()
{
  uint16_t x, y;
  if (tft.getTouch(&x, &y))
    { 
    if (tft.getRotation()==1) { y=tft.height()-y;}
    else if (tft.getRotation()==3) { x=tft.width()-x; y=tft.height()-y;}
    if (tftpage==0)   // main page
      {
      if (scanF>0) { scanF=0; updateDisplay(1); return; }
      checkFlotButtons(x,y);
      if (keylock==1) return;
      checkFilterButtons(x,y);
      checkFunctionButtons(x,y);
      checkFreqButtons(x,y);
      checkMainButtons(x,y);
      checkVFOButtons(x,y);
      checkSmeterButtons(x,y); 
      checkNavButtons(x,y); 
      checkStaButtons(x,y);
      }
    else if (tftpage==1) { checkSetButtons(x,y); checkNavButtons(x,y); checkStaButtons(x,y); }    // Setup page
    else if (tftpage==2) { checkSetRadButtons(x,y); checkNavButtons(x,y); checkStaButtons(x,y); } // radio setting page
    else if (tftpage==3) { checkCWButtons(x,y); checkNavButtons(x,y); checkStaButtons(x,y); }     // CW setting page
    else if (tftpage==4) { checkNetButtons(x,y); checkNavButtons(x,y); checkStaButtons(x,y); }    // WiFi page
    else if (tftpage==5) { checkCalButtons(x,y); checkNavButtons(x,y); checkStaButtons(x,y); }    // Calibration page
    else if (tftpage==6) { checkKEYERButtons(x,y); checkNavButtons(x,y); checkStaButtons(x,y); }  // KEYER setting page
    else if (tftpage==7) { checkMemManButtons(x,y); checkNavButtons(x,y); checkStaButtons(x,y); } // memories manager
    else if (tftpage==8) { checkATUButtons(x,y); checkNavButtons(x,y); checkStaButtons(x,y); }    // ATU page
    else if (tftpage==9) { checkTPAButtons(x,y); checkNavButtons(x,y); checkStaButtons(x,y); }    // TPA2016 page
    else if (tftpage==10) { checkSMEButtons(x,y); checkNavButtons(x,y); checkStaButtons(x,y); }    // S_METER page
    else if (tftpage==11) { checkCONButtons(x,y); checkNavButtons(x,y); checkStaButtons(x,y); }    // CONNS page
    else if (tftpage==12) { checkTEMPButtons(x,y); checkNavButtons(x,y); checkStaButtons(x,y); }  // TEMP page
    else if (tftpage==13) { checkPORTSButtons(x,y); checkNavButtons(x,y); checkStaButtons(x,y); }  // PORTS page
    else if (tftpage==14) { checkFILTButtons(x,y); checkNavButtons(x,y); checkStaButtons(x,y); }  // FILTERS page
    else if (tftpage==15) { checkOSCButtons(x,y); checkNavButtons(x,y); checkStaButtons(x,y); }  // OSC page
    else if (tftpage==16) { checkFIL1Buttons(x,y); checkNavButtons(x,y); checkStaButtons(x,y); }  // Filter1 page
    else if (tftpage==17) { checkFIL2Buttons(x,y); checkNavButtons(x,y); checkStaButtons(x,y); }  // Filter2 page
    else if (tftpage==21) { checkSelButtons(x,y); }    // Select AP
    else if (tftpage==22) 
      { 
      tftpage=0; 
      if (checkYN()==1)  // ESC
        {
        restFREQ();
        }
      updateDisplay(1); 
      }    // 
    else if (tftpage==24) { checkMenuButtons(x,y); checkNavButtons(x,y); checkStaButtons(x,y); }    // 
    delay(20);
    } 
}
