
void rain_setup() 
{
  pinMode(RAIN_SENSOR, INPUT);
}

bool get_rain() 
{
  return !digitalRead(RAIN_SENSOR);
}

