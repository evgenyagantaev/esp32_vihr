


const int max_sleep_menu_items = 3; // number of items
String sleep_menu_itemName[max_sleep_menu_items] = {"Sleep", "Wake up", "Back"}; // items names

uint8_t sleep_menu() 
{
    int mSelect = 0; // The currently highlighted app
    int16_t x, y, tx, ty;
  
    boolean exitMenu = false; // used to stay in the menu until user selects app
  
    sleep_menu_display(0); // display the list of Apps
  
    while (!exitMenu) 
    {
        if (ttgo->getTouch(x, y)) // If you have touched something...
        { 
    
            while (ttgo->getTouch(tx, ty)) {} // wait until you stop touching
  
            if (y <= 80) // top selected
            { 
                exitMenu = true;
            }
            else
            {

                // read current time-date
                RTC_Date tnow = ttgo->rtc->getDateTime();
                hh = tnow.hour;
                mm = tnow.minute;
                ss = tnow.second;
                dday = tnow.day;
                mmonth = tnow.month;
                yyear = tnow.year;

                char message[128];
      
                if (y >= 160) // bottom selected (wake up)
                { 
                    exitMenu = true;
                    snprintf(message, 128, "%04d.%02d.%02d %02d:%02d org wak\r\n", yyear, mmonth, dday, hh, mm);
                    appendFile(SPIFFS, "/journal.txt", message);
                    //**********************************************************************
                    snprintf(diagnostics, sizeof(diagnostics), "jrnl: org wak                ");
                    //**********************************************************************
                }
          
                
                if (y > 80 && y < 160) // You selected the middle (sleep)
                { 
                    exitMenu = true;
                    snprintf(message, 128, "%04d.%02d.%02d %02d:%02d org slp\r\n", yyear, mmonth, dday, hh, mm);
                    appendFile(SPIFFS, "/journal.txt", message);
                    //**********************************************************************
                    snprintf(diagnostics, sizeof(diagnostics), "jrnl: org slp                ");
                    //**********************************************************************
                    // reset step counter
                    // reset push counter
                    // reset pull counter
                    // power off
                }
            }
        }
    }
    //Return with mSelect containing the desired mode
    ttgo->tft->fillScreen(TFT_BLACK);
    return mSelect;
}

void sleep_menu_display(int mSel) 
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
    ttgo->tft->println(sleep_menu_itemName[curSel]);
  
    ttgo->tft->setTextSize(3);
    ttgo->tft->setTextColor(TFT_RED);
    ttgo->tft->setCursor(40, 110);
    ttgo->tft->println(sleep_menu_itemName[mSel]);
  
    if (mSel == maxItems - 1) curSel = 0;
    else curSel = mSel + 1;
  
    ttgo->tft->setTextSize(2);
    ttgo->tft->setTextColor(TFT_GREEN);
    ttgo->tft->setCursor(50, 190);
    ttgo->tft->print(sleep_menu_itemName[curSel]);
}
