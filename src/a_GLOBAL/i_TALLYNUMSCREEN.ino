void showTallyNum()
{
  screen = 2;
  resetScreen();

  if (C_PLUS == 2) M5.Lcd.setTextSize(6);
  else if (C_PLUS) M5.Lcd.setTextSize(5);
  else M5.Lcd.setTextSize(4);

  String s = String(TALLY_NR);

  int w = M5.Lcd.textWidth(s);
  int x = (M5.Lcd.width() - w) / 2;
  int y = (M5.Lcd.height() / 2) - 20;   // small optical offset

  M5.Lcd.setCursor(x, y);
  M5.Lcd.print(s);
}
