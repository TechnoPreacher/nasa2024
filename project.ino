
#include <Servo.h>

Servo myservo;  // create servo object to control a servo
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#define PIN        16 // On Trinket or Gemma, suggest changing this to 1

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 4 // Popular NeoPixel ring size
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);



void setup() {

  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  // END of Trinket-specific code.

  pixels.begin();
    pixels.fill(pixels.Color(255, 255, 255));

    pixels.show();   // Send the updated pixel colors to the hardware.


  myservo.attach(14);  // attaches the servo on pin 9 to the servo object
              // tell servo to go to position in variable 'pos'
Serial.begin(9600);
}

// Параметры фильтра Калмана
float Q = 0.1;  // Процессный шум
float R = 1.0;  // Шум измерений
float P = 1.0;  // Начальное значение ошибки ковариации
float K = 0.0;  // Коэффициент Калмана
float X = 0.0;  // Оценка (инициализируем нулем)



int sensorValue;

float kalmanFilter(float measurement) {
  // Предсказание
  P = P + Q;

  // Вычисляем коэффициент Калмана
  K = P / (P + R);

  // Обновляем оценку X с учетом нового измерения
  X = X + K * (measurement - X);

  // Обновляем ошибку ковариации
  P = (1 - K) * P;

  // Возвращаем фильтрованное значение
  return X;
}


void loop() { 
   sensorValue = analogRead(A1);  // Считываем данные с аналогового порта
  float filteredValue = kalmanFilter(sensorValue);  // Применяем фильтр Калмана
//filteredValue=sensorValue;
filteredValue=   map(filteredValue, 0, 1023, 0, 100);
  // Отправляем фиктивные значения для закрепления вертикального диапазона
  Serial.print("20 ");  // Нижняя граница диапазона
  Serial.print("60 "); // Верхняя граница диапазонаint mappedValue = map(sensorValue, 0, 1023, 0, 100);  // Приводим данные к диапазону 0-100
 //Serial.println(sensorValue);
     Serial.println(100-filteredValue);
    delay(10);
 
      myservo.write(map(analogRead(A3),10,1000,92,80)); 
 //Serial.println(map(analogRead(A3),10,1020,92,86));
}
