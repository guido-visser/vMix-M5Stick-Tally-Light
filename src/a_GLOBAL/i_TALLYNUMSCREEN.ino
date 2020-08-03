void showTallyNum()
{
  screen = 2;
  resetScreen();
  M5.Lcd.setTextSize(5);
  M5.Lcd.println(TALLY_NR);
}
