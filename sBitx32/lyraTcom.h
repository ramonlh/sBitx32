
#define BUFFER_I2C_LEN 255;

typedef struct {    // datos I2C lyraT
      uint8_t comtype;
      uint16_t min_f;    // lim inferior filtro
      uint16_t max_f;    // lim superior filtro
      uint16_t gain;    // lim superior filtro
} datalyratype;
      datalyratype datalyra;
      uint8_t *bufflyra = (uint8_t *) &datalyra; // acceder a datalyratype como bytes

int sendtoLyraT(int valueType)
{
  if (valueType > 0)
    {
    Serial2.print("Send to LyraT:"); Serial2.println(datalyra.comtype);
    Wire.beginTransmission(LYRAT_ADDRESS);  
    Wire.write(bufflyra, sizeof(datalyra));                  
    Wire.endTransmission();
    Wire.requestFrom(LYRAT_ADDRESS, 255, 1);
    Serial2.print("Rec form LyraT:"); 
    while(Wire.available()) {
      byte c = Wire.read();    // Receive a byte as character
      Serial2.print(c);        // Print the character
      }
    Serial2.println(); 
    }
  return 0;
}

void setFilterLyraT(uint16_t min_f, uint16_t max_f)
{
    datalyra.comtype=1;   // set filter
    datalyra.min_f=min_f;
    datalyra.max_f=max_f;  
    sendtoLyraT(datalyra.comtype);
}

void setGainLyraT(uint16_t gain)
{
    datalyra.comtype=2;   // set gain
    datalyra.gain=gain;  
    sendtoLyraT(datalyra.comtype);
}

void getSpectrumLyraT()
{
    datalyra.comtype=3;   // set gain
    sendtoLyraT(datalyra.comtype);
}
