// Step control application menu


const int max_push_menu_items = 3; // number of items
String push_menu_itemName[max_push_menu_items] = {"Add", "Reset", "Back"}; // items names

uint8_t push_menu() 
{
    int mSelect = 0; // The currently highlighted app
    int16_t x, y, tx, ty;
  
    boolean exitMenu = false; // used to stay in the menu until user selects app
  
    push_menu_display(0); // display the list of Apps
  
    while (!exitMenu) 
    {
        if (ttgo->getTouch(x, y)) // If you have touched something...
        { 
    
          while (ttgo->getTouch(tx, ty)) {} // wait until you stop touching
    
          if (y >= 160) // bottom selected
          { 
              exitMenu = true;
              // reset push counter
              preferences.begin("twatch", false);
              preferences.putUInt("push", 0);
              preferences.end();
          }
    
          if (y <= 80) // top selected
          { 
              exitMenu = true;
          }
          if (y > 80 && y < 160) // You selected the middle (add)
          { 
              exitMenu = true;
              preferences.begin("twatch", false);
              uint32_t push_summ = preferences.getUInt("push", 0);
              int new_push = (uint32_t)(appSetNumber());

              if(new_push != 0)
              {
                  push_summ += new_push;
                  preferences.putUInt("push", push_summ);
                  preferences.end();
                  
                  // write in journal
                  // read current time-date
                  RTC_Date tnow = ttgo->rtc->getDateTime();
                  hh = tnow.hour;
                  mm = tnow.minute;
                  ss = tnow.second;
                  dday = tnow.day;
                  mmonth = tnow.month;
                  yyear = tnow.year;
                
                  char message[128];
                
                  snprintf(message, 128, "%04d.%02d.%02d %02d:%02d fit psh %d\r\n", yyear, mmonth, dday, hh, mm, new_push);
                  appendFile(SPIFFS, "/journal.txt", message);
                  //**********************************************************************
                  snprintf(diagnostics, sizeof(diagnostics), "jrnl: fit psh %d                ", new_push);
                  //**********************************************************************
              }
          }
        }
    }
    //Return with mSelect containing the desired mode
    ttgo->tft->fillScreen(TFT_BLACK);
    return mSelect;
}

void push_menu_display(int mSel) 
{

    int curSel = 0;
    // Display mode header
    ttgo->tft->fillScreen(TFT_BLUE);
    ttgo->tft->fillRect(0, 80, 239, 80, TFT_BLACK);
  
    // Display apps
    if (mSel == 0) curSel = maxItems - 1;
    else curSel = mSel - 1;
  
    ttgo->tft->setTextSize(2);
    ttgo->tft->setTextColor(TFT_GREEN);
    ttgo->tft->setCursor(50, 30);
    ttgo->tft->println(push_menu_itemName[curSel]);
  
    ttgo->tft->setTextSize(3);
    ttgo->tft->setTextColor(TFT_RED);
    ttgo->tft->setCursor(40, 110);
    ttgo->tft->println(push_menu_itemName[mSel]);
  
    if (mSel == maxItems - 1) curSel = 0;
    else curSel = mSel + 1;
  
    ttgo->tft->setTextSize(2);
    ttgo->tft->setTextColor(TFT_GREEN);
    ttgo->tft->setCursor(50, 190);
    ttgo->tft->print(push_menu_itemName[curSel]);
}
