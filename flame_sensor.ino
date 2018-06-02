

void flame_setup()
{
  pinMode(FLAME_SENSOR,INPUT);
}

bool get_flame()
{
  return !digitalRead(FLAME_SENSOR);

}

