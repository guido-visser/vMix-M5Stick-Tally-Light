void showTallyNum()
{
  screen = 2;
  resetScreen();
  if(screenRotation == 1 || screenRotation == 3){
    M5.Lcd.setCursor(65, 25);
  } else {
    M5.Lcd.setCursor(30, 70);
  }
  M5.Lcd.setTextSize(5);
  M5.Lcd.println(TALLY_NR);
}
