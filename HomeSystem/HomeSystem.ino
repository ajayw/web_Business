#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <TFT_eSPI.h>
#include <time.h>

// WiFi 连接信息
const char* ssid = "iQOO12";
const char* password = "womima2013.";

// OpenWeatherMap API 信息（替换为你的 API Key 和城市 ID）
const char* apiKey = "a32b831995d66f7c5563e9d2a1ed5278";
const char* cityID = "1795565";  
const char* weatherUrl = "http://api.openweathermap.org/data/2.5/weather?id=1795565&appid=a32b831995d66f7c5563e9d2a1ed5278&units=metric&lang=zh_cn";

// NTP 服务器
const char* ntpServer = "pool.ntp.org";
long  gmtOffset_sec = 8 * 3600;  // UTC+8
int daylightOffset_sec = 0;

unsigned long lastWeatherUpdate = 0;  // 记录上次更新时间
const unsigned long weatherUpdateInterval = 1800000;  // 30分钟（毫秒）

// 触摸按钮坐标（灯开关）
#define BUTTON_X 100
#define BUTTON_Y 200
#define BUTTON_RADIUS 30

bool lightState = false;  // 灯光状态
TFT_eSPI tft = TFT_eSPI();  // TFT 屏幕对象

void setup() {
    Serial.begin(115200);
    tft.init();
    tft.setRotation(3);
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextSize(2);

    // 连接 WiFi
    connectWiFi();

    // 获取天气数据
    getWeatherData();
    delay(3000);
    // 配置 NTP 时间
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

    // 绘制 UI
    drawUI();
}

void loop() {
    // 更新时间
    displayTime();

    // 每30分钟更新一次天气
    if (millis() - lastWeatherUpdate > weatherUpdateInterval) {
        getWeatherData();  // 获取并更新天气数据
        lastWeatherUpdate = millis();  // 记录更新时间
    }

    // 检测触摸事件
    checkTouch();
}

// 连接 WiFi
void connectWiFi() {
    WiFi.begin(ssid, password);
    tft.setCursor(10, 10);
    tft.print("连接WiFi...");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi 连接成功!");
    tft.fillScreen(TFT_BLACK);
}

// 获取天气数据
void getWeatherData() {
    HTTPClient http;
    http.begin(weatherUrl);
    int httpCode = http.GET();

    if (httpCode > 0) {
        String payload = http.getString();
        DynamicJsonDocument doc(1024);
        deserializeJson(doc, payload);
        
        String weatherDesc = doc["weather"][0]["description"].as<String>();
        float temperature = doc["main"]["temp"];
        
        tft.setCursor(10, 50);
        tft.printf("天气: %s", weatherDesc.c_str());
        tft.setCursor(10, 80);
        tft.printf("温度: %.1f C", temperature);
        Serial.print("天气：");
        Serial.println(weatherDesc.c_str());
        Serial.print("温度:");
        Serial.println(temperature);
    }
    http.end();
}

// 绘制 UI 界面
void drawUI() {
    // 画灯光开关按钮
    drawButton(lightState);
}

// 显示当前时间
void displayTime() {
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
        Serial.println("获取时间失败");
        return;
    }

    char timeStr[16];
    strftime(timeStr, sizeof(timeStr), "%H:%M:%S", &timeinfo);
    tft.fillRect(10, 120, 100, 20, TFT_BLACK);  // 清除旧时间
    tft.setCursor(10, 120);
    tft.print(timeStr);
    // Serial.print("时间：");
    // Serial.println(timeStr);
}

// 画开关按钮
void drawButton(bool state) {
    uint16_t color = state ? TFT_GREEN : TFT_RED;
    tft.fillCircle(BUTTON_X, BUTTON_Y, BUTTON_RADIUS, color);
}

// 检测触摸事件（需要电阻屏或触摸支持）
void checkTouch() {
    uint16_t x, y;
    if (tft.getTouch(&x, &y)) {
        if ((x - BUTTON_X) * (x - BUTTON_X) + (y - BUTTON_Y) * (y - BUTTON_Y) <= BUTTON_RADIUS * BUTTON_RADIUS) {
            lightState = !lightState;  // 切换状态
            drawButton(lightState);
            Serial.println(lightState ? "灯光打开" : "灯光关闭");
        }
    }
}
