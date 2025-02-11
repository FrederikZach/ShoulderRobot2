#include <Pixy2.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver ServoSet = Adafruit_PWMServoDriver();

// This is the main Pixy object 
Pixy2 pixy;

int MoveV = 290;  //Variable für Sichelservos (Vertikal)
int MoveH = 380;  //Variable für Sichelservo (Horizontal)

void setup()
{
  //Pixy2
  Serial.begin(115200);
  //Serial.print("Starting...\n");
  
  pixy.init();
  
  //Servomotor
  ServoSet.begin();
  ServoSet.setPWMFreq(50);

  //Ausrichtung der Großen Servos
  //Vertikale Rotation Servo PWM > Kamera Seite runter (290 gerade Ausrichtung)
  ServoSet.setPWM(1, 0, 290);
      
  //Horizontale Rotation Servo PWM > Rechtsdrehung (380 gerade ausrichtung)
  ServoSet.setPWM(0, 0, 380);
  delay(2000);
}

void loop()
{ 
  int i; 
  pixy.ccc.getBlocks();
  
  
  // Falls Schulter erkannt wird
  if (pixy.ccc.numBlocks)
  {
    for (i=0; i<pixy.ccc.numBlocks; i++)
    {
      //HORIZONTALE

      //Pan-Tilt PixyCam2 Seite
      //Positionsabfrage der Objekts über die Pixy2 x-Achse
      int pos_x = pixy.ccc.blocks[i].m_x; 
      pos_x = pos_x + pixy.ccc.blocks[i].m_width/2;           //Durch 2 Teilen damit immer die Mitte des Referenzpunktes angesteuert wird
    
      //PWM umwandlung
      int PWM_X = pos_x;
      PWM_X = 450+40-PWM_X/6;
  
      
      //Servomotor ausrichtung x-Achse
      ServoSet.setPWM(4, 0, PWM_X);

      //Pan-Tilt andere Seite
      //PWM umwandlung
      PWM_X = 760-PWM_X;
      Serial.print("x: ");
      Serial.print(PWM_X);
      Serial.print('\n');
  
      //Servomotor ausrichtung x-Achse
      ServoSet.setPWM(9, 0, PWM_X);

      
      
      //SICHELBEWEGUNGEN
      //Horizontalbewegung Sichelmotor
      //Linksjustierung
      if(pos_x > 280)
      {
        MoveH = MoveH - 1;    //Durch veringerung des Wertes erfolgt eine Linsbewegung
        ServoSet.setPWM(0, 0, MoveH);
      }

      //Rechtsjustierung
      if(pos_x < 100)
      {
        MoveH = MoveH + 1;    //Durch steigerung des Wertes erfolgt eine Linsbewegung
        ServoSet.setPWM(0, 0, MoveH);
      } 
      

      //VERTIKALE 
      
      //Positionsabfrage der Objekts über die Pixy2 y-Achse
      int pos_y = pixy.ccc.blocks[i].m_y;
      pos_y = pos_y + pixy.ccc.blocks[i].m_height/2;
      
      //PWM umwandlung
      int PWM_Y = pos_y;
      PWM_Y = 230+PWM_Y/5.7;

      //Servomotor ausrichtung y-Achse
      ServoSet.setPWM(5, 0, PWM_Y);

      //PanTilt andere Seite
      //PWM umwandlung
      PWM_Y = PWM_Y-138;
      Serial.print("y: ");
      Serial.print(PWM_Y);
      Serial.print('\n');

      //Servomotor ausrichtung y-Achse
      ServoSet.setPWM(8, 0, PWM_Y);

      
      
      //SICHELBEWEGUNGEN
      //Vertikalbewegung Sichelmotor
      //Aufwärts PixyCam2 Seite
      if(pos_y > 200)
      {
        MoveV = MoveV - 1;    //Durch veringerung des Wertes erfolgt eine Aufwärtsbewegung (PixyCam2 Seite)
        ServoSet.setPWM(1, 0, MoveV);
      }

      //Abwärts
      if(pos_y < 60)
      {
        MoveV = MoveV + 1;    //Durch steigerung des Wertes erfolgt eine Abwärtsbewegung (PixyCam2 Seite)
        ServoSet.setPWM(1, 0, MoveV);
      } 
    }
  }  
}
