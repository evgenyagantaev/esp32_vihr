#include "FS.h"
#include "SPIFFS.h"

//using namespace fs;

void appGraph() 
{
    int i, j;
  
    int16_t x, y;
    char buf[128];

    int months[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    int hh, mm, yyear, mmonth, dday; // 
    int base_hh, base_mm, base_yyear, base_mmonth, base_dday; //

    int state = 0;
    // 0 - idle (black)
    // 1 - slp  (skyblue)
    // 2 - stp  (green)
    // 3 - job  (yellow)
    // 4 - wgt  (red)
    // 5 - push/pull (orange)

    //*
    base_yyear = 1970;
    base_mmonth = 1;
    base_dday = 1;
    base_hh = 0;
    base_mm = 0;
    //*/

    /*
    base_yyear = 2021;
    base_mmonth = 3;
    base_dday = 3;
    base_hh = 15;
    base_mm = 6;
    //*/

    ttgo->tft->setTextSize(1);
    ttgo->tft->setTextColor(TFT_YELLOW, TFT_BLACK);
    snprintf(buf, sizeof(buf), "Graph application");
    ttgo->tft->drawString(buf, 5, 5, 4);

    ttgo->tft->fillScreen(TFT_BLACK);

    /*
    

    for(i=0; i<40; i++)
    {
        for(j=0; j<240; j++)
        {
            ttgo->tft->fillRect(i*6, j, 5, 1, TFT_SKYBLUE);
            Serial.println("draw");
            //delay(50);
        }
    }
    //*/

    //*
    fs::File file = SPIFFS.open("/journal.txt");
    if(!file || file.isDirectory())
    {
        Serial.println("- failed to open file for reading");
        return;
    }

    int sleep_start = 0;
    int step_start = 0;
    int job_start = 0;
    int no_redraw = 0;

    int base_minutes_calculated = 0;
    int base_minutes = 0;
    int start_hh, start_mm;
    int do_job = 1;
    while(do_job)
    {
        //Serial.write(file.read());
        //delay(2000);

        int buf_index = 0;
        int read_string = 1;
        while(read_string)
        {
            if(file.available())
            {
                char ch = file.read();
                buf[buf_index] = ch;
                buf_index++;
                if((ch == '\n') && buf_index > 16)
                {
                    buf[buf_index] = 0;
                    read_string = 0;
                    Serial.print(buf);
                    sscanf(buf, "%d.%d.%d %d:%d", &yyear, &mmonth, &dday, &hh, &mm);
                    int minutes = 0;
                    minutes += ((yyear - base_yyear) * 365 + (int)((yyear - base_yyear)/4)) * 24 * 60;
                    int days = 0;
                    for(int i=0; i< (mmonth-1); i++)
                    {
                        days += months[i];
                    }
                    if((mmonth>2) && (yyear%4 == 0))
                        days++;
                    days += dday;
                    minutes += days * 24 *60;
                    minutes += hh *60;
                    minutes += mm;
                    if(base_minutes_calculated == 0)
                    {
                        base_minutes_calculated = 1;
                        base_minutes = minutes;
                        start_hh = hh;
                        start_mm = mm;
                    }
                    minutes = minutes - base_minutes + start_hh *60 + start_mm;

                    int norm_minutes = minutes / 6;

                    if(strstr(buf, "org") != NULL)
                    {
                        if(strstr(buf, "slp") != NULL)
                        {
                            sleep_start = norm_minutes;
                        }
                        else if(strstr(buf, "wak") != NULL)
                        {
                            int l;
                            for(l=sleep_start; l<norm_minutes; l++)
                            {
                                i = l / 240;
                                j = l % 240;
                                ttgo->tft->fillRect(i*6, j, 5, 1, TFT_SKYBLUE);
                            }
                        }
                    }
                    else if(strstr(buf, "sta") != NULL)
                    {
                        if((norm_minutes - no_redraw) > 3)
                        {
                            i = norm_minutes / 240;
                            j = norm_minutes % 240;
                            ttgo->tft->fillRect(i*6, j, 5, 1, TFT_GREEN);
                            step_start = norm_minutes;
                        }
                        else
                        {
                            step_start = norm_minutes + 3;
                        }

                        // esli hodba prervala raboty
                        if(job_start > 0)
                        {
                            int l;
                            for(l=job_start; l<norm_minutes; l++)
                            {
                                i = l / 240;
                                j = l % 240;
                                ttgo->tft->fillRect(i*6, j, 5, 1, TFT_BLUE);
                                Serial.println("JOB****************************");
                            }
                            job_start = 0;
                            state = 0;  // idle state
                        }
                    }
                    else if(strstr(buf, "fin") != NULL)
                    {
                        sprintf(buf, "%d   %d", step_start, norm_minutes);
                        Serial.println(buf);
                        int l;
                        for(l=step_start; l<norm_minutes; l++)
                        {
                            i = l / 240;
                            j = l % 240;
                            ttgo->tft->fillRect(i*6, j, 5, 1, TFT_GREEN);
                            Serial.println("STEP****************************");
                        }
                    }
                    else if(strstr(buf, "wei") != NULL)
                    {
                        i = norm_minutes / 240;
                        j = norm_minutes % 240;
                        ttgo->tft->fillRect(i*6, j, 5, 3, TFT_RED);
                        no_redraw = norm_minutes;
                    }
                    else if((strstr(buf, "pus") != NULL) || (strstr(buf, "pul") != NULL))
                    {
                        i = norm_minutes / 240;
                        j = norm_minutes % 240;
                        ttgo->tft->fillRect(i*6, j, 5, 3, TFT_ORANGE);
                        no_redraw = norm_minutes;
                    }

                    if(strstr(buf, "job") != NULL)
                    {
                        if(state != 3) // not in job state
                        {
                            state = 3; // job state
                            job_start = norm_minutes;
                        }
                    }

                    if((strstr(buf, "idle") != NULL) && (job_start > 0))
                    {
                        int l;
                        for(l=job_start; l<norm_minutes; l++)
                        {
                            i = l / 240;
                            j = l % 240;
                            ttgo->tft->fillRect(i*6, j, 5, 1, TFT_BLUE);
                            Serial.println("JOB****************************");
                        }
                        job_start = 0;
                        state = 0;  // idle state
                    }
                    
                    sprintf(buf, "%d.%d.%d %d:%d ---> %d\r\n", yyear, mmonth, dday, hh, mm, minutes);
                    Serial.println(buf);
                    
                    
                    
                    
                }
            }
            else
            {
                read_string = 0;
                do_job = 0;
            }
        }
        
    }

    //*/
    

    while (!ttgo->getTouch(x, y)) {} // Wait for touch to exit
    
  
    while (ttgo->getTouch(x, y)) {}  // Wait for release to return to the clock
    
    ttgo->tft->fillScreen(TFT_BLACK); // Clear screen
}
