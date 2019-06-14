/*
    @program IM920モジュールを用いてArduino間の無線通信を行うライブラリ(受信用)
    @date 2018/10/25
    @author Watanabe Rui
*/

#pragma once

#include<arduino.h>
#include<SoftwareSerial.h>

const uint8_t IM920_BUFFER_SIZE = 80;
const uint8_t HEAD = 12;  //読み取る先頭のインデックス

class IM920Receiver
{
  private:

    SoftwareSerial serial;
    const uint8_t RX, TX;
    uint8_t dataSize;
    uint8_t  *data;

    uint8_t *currentLine, *lastLine;
    uint8_t line1[IM920_BUFFER_SIZE], line2[IM920_BUFFER_SIZE];
    uint16_t index, lineLength;
    bool isChanged; //改行

  public:

    IM920Receiver(const uint8_t RX, const uint8_t TX, uint8_t size);
    ~IM920Receiver();

    // 初期化関数(setup内で呼び出す)
    void init();
    void setData(uint8_t num, uint8_t val);
    uint8_t getData(int num);
    void getData(uint8_t ary[]);
    void show();
    void reset();
    void update();

    char readSingleData();
    void getLastLine(char buff[IM920_BUFFER_SIZE]);
    bool isLineChanged();
    bool isReceived();
};

/*
    コンストラクタ
    @param [const uint8_t] RX：RXピン
    @param [const uint8_t] TX：TXピン
    @param [uint8_t] size：送るデータのサイズ
*/
IM920Receiver::IM920Receiver(const uint8_t RX, const uint8_t TX, uint8_t size): serial(RX, TX), dataSize(size)
{
  data = new uint8_t[dataSize];
  reset();  //一旦0に初期化

  currentLine = line1;
  lastLine = line2;

  index = 0;
  lineLength = 0;
  isChanged = false;
}

// デストラクタ
IM920Receiver::~IM920Receiver()
{
  delete[] data;
}

// 初期化関数(setup内で呼び出し)
void IM920Receiver::init()
{
  serial.begin(19200);  //ボーレートの設定

  serial.print("ECIO\r\n"); //キャラクタ入出力モード
  serial.print("STRT 1\r\n"); //高速通信モード[50kbps]
  serial.print("SRID 272E\r\n");

  delay(1000);
}

char IM920Receiver::readSingleData()
{
  char c = 0;

  if (serial.available())
    c = serial.read();
  else
    return c;

  index = min(IM920_BUFFER_SIZE - 2, index);
  currentLine[index] = c;
  index++;

  if (c == '\n')
  {
    currentLine[index] = '\0';

    uint8_t *temp;
    temp = currentLine;
    currentLine = lastLine;
    lastLine = temp;

    lineLength = index;
    index = 0;
    isChanged = true;
  }
  return c;
}

void IM920Receiver::getLastLine(char buff[IM920_BUFFER_SIZE])
{
  isChanged = false;
  for (int i = 0; i < lineLength; i++)
    buff[i] = lastLine[i];
  buff[lineLength] = '\0';
}

bool IM920Receiver::isLineChanged()
{
  return isChanged;
}

// アップデート関数(データを受信し，変換)
void IM920Receiver::update()
{
  char buff[IM920_BUFFER_SIZE];
    
  while (isReceived())
  {
    readSingleData();
    if (isLineChanged())
      getLastLine(buff);
  }

  int j = 0, k = 0;
  char tmp[3];

  for (int i = HEAD; buff[i] != '\0'; i++)
  {
    if (buff[i] == ' ')
    {
      tmp[j] = '\0';
      data[k++] = atoi(tmp);
      j = 0;
    }
    tmp[j++] = buff[i];
  }
}

uint8_t IM920Receiver::getData(int num)
{
  return data[num - 1];
}

void IM920Receiver::getData(uint8_t ary[])
{
  for (int i = 0; i < dataSize; i++)
    ary[i] = data[i];
}

bool IM920Receiver::isReceived()
{
  return serial.available();
}

// データの中身を表示
void IM920Receiver::show()
{
  for (int i = 0; i < dataSize; i++) {
    Serial.print(data[i]);
    Serial.print("\t");
  }
  Serial.println();
}

// データの中身をリセット
void IM920Receiver::reset()
{
  for (int i = 0; i < dataSize; i++)
    data[i] = 0;
}
