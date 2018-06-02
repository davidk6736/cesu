


bool dataLogSetup() 
{  
  dataLogger=SD.open("data.txt",FILE_WRITE);              //creating a File object that writes data on the "fieldData.txt" file on the SD card and assigning this file object to dataLogger variable
  if (dataLogger)                                         // if   the "fieldData.txt" file does not exist it will create it and if it does exist the dataLogger object will append the data to it
  {
    dataLogger.print(F("temperature,humidity,pressure,lpg,co,smoke,flame,rain,latDeg,latMin,longDeg,longMin"));
    dataLogger.print("\n");
  }
    return (SD.begin(CHIP_SELECT));
}






void writeFile()
{ 
     //File dataLogger=SD.open("data.txt",FILE_WRITE);  //creating a File object that writes data on the "fieldData.txt" file on the SD card and assigning this file object to dataLogger variable
                                                        // if   the "fieldData.txt" file does not exist it will create it and if it does exist the dataLogger object will append the data to it
     if (dataLogger)                                    //if the file has opened successfuly this statement is true else this statement is false 
     {  
        if (loopCount == 0)
        {
          SD.open("data.txt",FILE_WRITE);
          Serial.println(F("File opened successfuly"));                                              
        }
        dataLogger.print(temp);
        dataLogger.print(",");
        dataLogger.print(humidity);
        dataLogger.print(",");
        dataLogger.print(pres);
        dataLogger.print(",");
        dataLogger.print(lpg);
        dataLogger.print(",");
        dataLogger.print(co);
        dataLogger.print(",");
        dataLogger.print(smoke);
        dataLogger.print(",");
        dataLogger.print(flame);
        dataLogger.print(",");
        dataLogger.print(rain);
        dataLogger.print(",");
        dataLogger.print(GPS.latitude,4);
        dataLogger.print(",");
        dataLogger.print(GPS.lat);
        dataLogger.print(",");
        dataLogger.print(GPS.longitude,4);
        dataLogger.print(",");
        dataLogger.print(GPS.lon);
      //  dataLogger.print(time);
      //  dataLogger.print(date);
        dataLogger.print("\n");
            }

    if (loopCount==6) dataLogger.close();
     delay(100);
}




