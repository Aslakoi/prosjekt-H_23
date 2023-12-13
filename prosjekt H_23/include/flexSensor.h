#ifndef FLEXSENSOR_H
#define FLEXSENSOR_H

const int flexPin = A0;
int ledArray[] = {7, 4, 3, 2, };
int flexVal = 0;

void flexSensor()
{
  flexVal = analogRead(flexPin);

  if (flexVal > 400)
  {
    for (int i = 0; i < 4; i++)
    {
      digitalWrite(ledArray[i], HIGH);
    }
  }
  else if (flexVal > 370)
  {
    for (int i = 0; i < 3; i++)
    {
      digitalWrite(ledArray[i], HIGH);
    }
    digitalWrite(ledArray[3], LOW);
  }
  else if (flexVal > 340)
  {
    for (int i = 0; i < 2; i++)
    {
      digitalWrite(ledArray[i], HIGH);
    }
    for (int i = 2; i < 4; i++)
    {
      digitalWrite(ledArray[i], LOW);
    }
  }
  else if (flexVal > 310)
  {
    for (int i = 0; i < 1; i++)
    {
      digitalWrite(ledArray[i], HIGH);
    }
    for (int i = 1; i < 4; i++)
    {
      digitalWrite(ledArray[i], LOW);
    }
  }
  else 
  {
    for (int i = 0; i < 4; i++)
    {
      digitalWrite(ledArray[i], LOW);
    }
  }
}

#endif