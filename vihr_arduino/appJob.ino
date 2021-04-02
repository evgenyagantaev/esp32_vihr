// save current weight

void appJob() 
{
    int16_t x, y;
    
    int job_selected = job_menu();
    
    char buf[128];

    const int max_job_menu_items = 7; // number of items
    String job_menu_itemName[max_job_menu_items] = {"Vihr", "E spusk", "T-watch", "Texts", "Face-id", "Idle", "Back"}; // items names


    ttgo->tft->setTextSize(1);

    ttgo->tft->setTextColor(TFT_YELLOW, TFT_BLACK);
    snprintf(buf, sizeof(buf), "job - %s", job_menu_itemName[job_selected]);
    ttgo->tft->drawString(buf, 5, 5, 4);

    
    if(job_selected != (max_job_menu_items - 1))
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
      
        if(job_selected == (max_job_menu_items - 2))  // idle item
        {
            snprintf(message, 128, "%04d.%02d.%02d %02d:%02d byt idle\r\n", yyear, mmonth, dday, hh, mm);
            //**********************************************************************
            snprintf(diagnostics, sizeof(diagnostics), "jrnl: byt idle                ");
            //**********************************************************************
        }
        else
        {
            snprintf(message, 128, "%04d.%02d.%02d %02d:%02d job %s\r\n", yyear, mmonth, dday, hh, mm, job_menu_itemName[job_selected]);
            //**********************************************************************
            snprintf(diagnostics, sizeof(diagnostics), "jrnl: job %s                ", job_menu_itemName[job_selected]);
            //**********************************************************************

        }
        
        appendFile(SPIFFS, "/journal.txt", message);

    }

    while (!ttgo->getTouch(x, y)) {} // Wait for touch to exit
    
  
    while (ttgo->getTouch(x, y)) {}  // Wait for release to return to the clock
    
    ttgo->tft->fillScreen(TFT_BLACK); // Clear screen
}
