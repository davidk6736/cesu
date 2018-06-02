
void LCD_setup()
{
  Serial.println("1");
  lcd.init();                 //initialize the lcd
  Serial.println("2");
  lcd.backlight();            //open the backlight 
  Serial.println(F("LCD Initialized"));
}

void LCDset1() 
{
  lcd.setCursor(0,0);          // set the cursor to column 1, line 0
}

void LCDset2()
{
  lcd.setCursor(0,1);          // set the cursor to column 1, line 2
}   

