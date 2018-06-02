


void gpsSetup() 
{

  GPS.begin(9600);                                    //turning on the streaming of data by GPS object to the mySerial software serial port

  GPS.sendCommand("$PGCMD,33,0*6D");                  //this command to the GPS module turns off the feature GPS module that sends information about Anttena
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_10HZ);         //this command sets the update rate of the GPS module to 10 hertz
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);       //this command tells the GPS module to send only the RMC sentence and GGA sentence
  Serial.println("GPS Set-up");
  
 }


void runGPS()                                         //implementation of the readGPS() function
{
  int numData = 2;

    clearGPS();                                       //clears the buffer of the GPS object(buffer of the software serial port?)
    
    
    gps_serial.flush();
    
    
    while(!GPS.newNMEAreceived())                    //loops and reads the characters sent from the GPS module unti we have a good NMEA sentence
    {
      GPS.read();                                    //reads the current charachter sent from the GPS module  
    }

     GPS.parse(GPS.lastNMEA());                     //parses the last good NMEA sentence 
  //   NMEA1=GPS.lastNMEA();                        //saves the last NMEA sentence into the variable NMEA1 

     
     while(!GPS.newNMEAreceived())                  //loops and reads the characters sent from the GPS module unti we have a good NMEA sentence
    {
      GPS.read();                                   //reads the current charachter sent from the GPS module  
    }

    GPS.parse(GPS.lastNMEA());                      //parses the last good NMEA sentence 
  //  NMEA2=GPS.lastNMEA();                         //seves the last NMEA sentence into the variable NMEA2

 
}



void clearGPS()                                     //immplementing the clearGPS() function which clears the buffer of GPS 
{
 
  while(!GPS.newNMEAreceived())                     //loops and reads the characters sent from the GPS module unti we have a good NMEA sentence
    {
      GPS.read();                                   //reads the current charachter sent from the GPS module  
    }
    GPS.parse(GPS.lastNMEA());

    while(!GPS.newNMEAreceived())                   //loops and reads the characters sent from the GPS module unti we have a good NMEA sentence
    {
      GPS.read();                                   //reads the current charachter sent from the GPS module  
    }
    GPS.parse(GPS.lastNMEA());

    while(!GPS.newNMEAreceived())                   //loops and reads the characters sent from the GPS module unti we have a good NMEA sentence
    {
      GPS.read();                                   //reads the current charachter sent from the GPS module  
    }
    GPS.parse(GPS.lastNMEA());
}


