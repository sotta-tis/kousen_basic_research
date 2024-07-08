#include <WiFi.h>
#define LED1 13
#define LED2 12
#include <ESP32Servo.h>//サーボのためのライブラリ
Servo penguin;//penguinという名前をつけたの

const char *ssid="OPPO Reno5 A";          //  *** 書き換え必要 ***
const char *password="j2stfi6r";    //  *** 書き換え必要（8文字以上）***

WiFiServer server(80);

void setup() {
  pinMode(LED1,OUTPUT);
  pinMode(LED2,OUTPUT);
penguin.attach(5);//PWMにつなぐ。[~]マークの3,5,6,9,10,11。今回は6pin

  Serial.begin(250000);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("connecting");
  }
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop()
{
    WiFiClient client = server.available(); // クライアントからの要求を待つ
    //Serial.print("*");
    if (client)
    { // if you get a client,
      Serial.println("New Client.");  //シリアル・モニタに"New Client."を表示
      String currentLine = "";        // 入力用の文字列をクリア
      while (client.connected())
      {
        //クライアントと接続中は繰り返す処理
        if (client.available())
        {
          // 受信データがあるとき
          char c = client.read();     // 1文字読み取る
          Serial.write(c);            // シリアル・モニタに書き出す
          if (c == '\n')
          {
            // 改行の文字をチェックし空行なら応答を返す
            if (currentLine.length() == 0)
            {
                // HTTP headers always start with a response code
                // (e.g. HTTP/1.1 200 OK)
                // and a content-type so the client knows what's
                // coming, then a blank line:
                client.println("HTTP/1.1 200 OK");  // 要求を正常に
                                                    // 受け取ったときの応答
                client.println("Content-type:text/html");
                client.println();
                // 次の2行がブラウザに表示される
                client.print("<meta name='viewport' content='width=device-width' />");
                client.print("<style>body { text-align: center; } div { margin: 8px auto; } button{ font-size: 32px; padding: 8px; width: 120px; margin: 8px; background-color: blue; color: white; }</style>");
                client.print("<div><a href='/H'><button>open</button></a></div>");
                client.print("<div><a href='/L'><button>close</button></a></div>");
                client.print("<div><a href='/N'><button>middle</button></a></div>");
                // The HTTP response ends with another blank line:
                client.println();
                // break out of the while loop:
                break;
            }
            else
            {
              // if you got a newline, then clear currentLine:
              Serial.print("readline : ");
              Serial.print(currentLine);
              Serial.println();
              currentLine = "";
            }
          }
          else if (c != '\r')
          { // if you got anything else but
                              // a carriage return character,
            currentLine += c; // add it to the end of
                              // the currentLine
          }
          // Check to see if the client request was
          // "GET /H" or "GET /L":
          if (currentLine.endsWith("GET /H"))
          {
            digitalWrite(LED1,HIGH);
            penguin.write(180);
            digitalWrite(LED2,LOW);
          }
          if (currentLine.endsWith("GET /L"))
          {
            digitalWrite(LED1,LOW);
            digitalWrite(LED2,HIGH);
            penguin.write(0);//0度の位置まで回転
          }if (currentLine.endsWith("GET /N"))
          {
            digitalWrite(LED1,HIGH);
            digitalWrite(LED2,HIGH);
            penguin.write(90);//90度の位置まで回転
          }
        }

      }
      // close the connection:
      client.stop();
      Serial.println("Client Disconnected.");
    }
}