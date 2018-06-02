

unsigned char get_humidity() 
{
 return dht.readHumidity();
}

unsigned char get_temp()
{
  return dht.readTemperature(true);
}

