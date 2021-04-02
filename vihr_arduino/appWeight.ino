// save current weight

void appWeight() 
{
    int16_t x, y;
    
    float weight = appSetNumber();

    char buf[128];

    ttgo->tft->setTextSize(1);

    ttgo->tft->setTextColor(TFT_YELLOW, TFT_BLACK);
    snprintf(buf, sizeof(buf), "weight = %5.1f", weight);
    ttgo->tft->drawString(buf, 5, 5, 4);

    if((int)weight != 0)
    {
        preferences.begin("twatch", false);
        preferences.putUInt("weight", (uint32_t)(weight*10));
        preferences.end();
    
        // read current time-date
        RTC_Date tnow = ttgo->rtc->getDateTime();
        hh = tnow.hour;
        mm = tnow.minute;
        ss = tnow.second;
        dday = tnow.day;
        mmonth = tnow.month;
        yyear = tnow.year;
    
        char message[128];
    
        snprintf(message, 128, "%04d.%02d.%02d %02d:%02d wei %4.1f\r\n", yyear, mmonth, dday, hh, mm, weight);
        appendFile(SPIFFS, "/journal.txt", message);
        //**********************************************************************
        snprintf(diagnostics, sizeof(diagnostics), "jrnl: wei %4.1f                ", weight);
        //**********************************************************************
    }

    while (!ttgo->getTouch(x, y)) {} // Wait for touch to exit
    
  
    while (ttgo->getTouch(x, y)) {}  // Wait for release to return to the clock
    
    ttgo->tft->fillScreen(TFT_BLACK); // Clear screen
}
