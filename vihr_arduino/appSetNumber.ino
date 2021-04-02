// Set the time - no error checking, you might want to add it

float appSetNumber() 
{

    //Set up the interface buttons
  
    ttgo->tft->fillScreen(TFT_WHITE);
    ttgo->tft->fillRoundRect(2, 2, 98, 34, 4, TFT_BLACK);
    ttgo->tft->fillRoundRect(102, 2, 136, 34, 4, TFT_RED);
    
    ttgo->tft->fillRoundRect(2, 39, 77, 49, 6,  TFT_BLACK);
    ttgo->tft->fillRoundRect(81, 39, 79, 49, 6,  TFT_BLACK);
    ttgo->tft->fillRoundRect(161, 39, 77, 49, 6,  TFT_BLACK);
    
    ttgo->tft->fillRoundRect(2, 89, 77, 49, 6,  TFT_BLACK);
    ttgo->tft->fillRoundRect(81, 89, 79, 49, 6,  TFT_BLACK);
    ttgo->tft->fillRoundRect(161, 89, 77, 49, 6,  TFT_BLACK);
    
    ttgo->tft->fillRoundRect(2, 139, 77, 49, 6,  TFT_BLACK);
    ttgo->tft->fillRoundRect(81, 139, 79, 49, 6,  TFT_BLACK);
    ttgo->tft->fillRoundRect(161, 139, 77, 49, 6,  TFT_BLACK);
    
    ttgo->tft->fillRoundRect(2, 189, 77, 49, 6,  TFT_BLACK);
    ttgo->tft->fillRoundRect(81, 189, 79, 49, 6,  TFT_BLACK);
    ttgo->tft->fillRoundRect(161, 189, 77, 49, 6,  TFT_GREEN);
    
    ttgo->tft->setTextColor(TFT_WHITE);
    ttgo->tft->drawString("cancel",  127, 2, 2);
    ttgo->tft->drawNumber(1, 30, 48, 2);
    ttgo->tft->drawNumber(2, 110, 48, 2);
    ttgo->tft->drawNumber(3, 190, 48, 2);
    ttgo->tft->drawNumber(4, 30, 98, 2);
    ttgo->tft->drawNumber(5, 110, 98, 2);
    ttgo->tft->drawNumber(6, 190, 98, 2);
    ttgo->tft->drawNumber(7, 30, 148, 2);
    ttgo->tft->drawNumber(8, 110, 148, 2);
    ttgo->tft->drawNumber(9, 190, 148, 2);
    ttgo->tft->drawNumber(0, 30, 198, 2);
    ttgo->tft->setTextSize(4);
    ttgo->tft->setCursor(110, 198);
    ttgo->tft->print(",");
    ttgo->tft->setTextColor(TFT_BLACK);
    ttgo->tft->setTextSize(3);
    ttgo->tft->setCursor(180, 203);
    ttgo->tft->print(">>");
    ttgo->tft->setTextSize(2);
    ttgo->tft->setCursor(0, 0);
  
    ttgo->tft->setTextColor(TFT_WHITE);
    int wl = 0; // Track the current digit selected
    float current_number = 0;  // 
    int after_point = false;
  
    
  
    while ((wl != 13) && (wl != 11)) 
    {
        wl = get_digit();
        if (wl != -1) 
        {
            if(wl == 11)  // cancel
                current_number = 0;
                
            else if(wl == 12)  
                after_point = true;
            else
            {
                if((wl>= 0) && (wl<=9))
                {
                    if(!after_point)
                        current_number = current_number*10.0 + wl;
                    else
                        current_number = current_number + ((float)wl/10.0);
                        
                    
                }
            }

            digitalWrite(4, HIGH);
            delay(10);
            digitalWrite(4, LOW);
            print_entered_number(current_number, after_point); // Display the current entered number
            while (get_digit() != -1) {}  // wait for untouching
        }
        
    }
    while (get_digit() != -1) {}  // wait for untouching
    
    ttgo->tft->fillScreen(TFT_BLACK);

    return current_number;
}

// prtTime will display the current selected time and highlight
// the current digit to be updated in yellow

void print_entered_number(float number, bool fractional) 
{
    char buf[128];
  
    ttgo->tft->fillRoundRect(2, 2, 98, 34, 4, TFT_BLACK);
    ttgo->tft->setTextColor(TFT_WHITE);
    ttgo->tft->setTextSize(1);
    if(!fractional)
        snprintf(buf, sizeof(buf), "%d", (int)number);
    else
      snprintf(buf, sizeof(buf), "%5.1f", number);
    ttgo->tft->drawString(buf, 5, 5, 4);
    
}

// get_digit takes care of translating where we pressed into
// a number that was pressed. Returns -1 for no press
// and 13 for DONE and 11 for CANCEL and 12 for comma

int get_digit() 
{
    int16_t x, y;
    
    if (!ttgo->getTouch(x, y))
    {
        return - 1;
    }
    else
    {
        if (y < 35) // cancel line
        {
            return 11;
        }
        else if ((y>=35) && (y < 85)) // top line
        {
            if (x < 80) return 1;
            else if (x > 160) return 3;
            else return 2;
        }
        else if ((y>=85) && (y < 135)) // middle line
        {
            if (x < 80) return 4;
            else if (x > 160) return 6;
            else return 5;
        }
        else if ((y>=135) && (y < 185)) // third line
        {
            if (x < 80) return 7;
            else if (x > 160) return 9;
            else return 8;
        }
        else     // bottom line
        {
            if (x < 80) return 0;   // 
            else if (x > 160) return 13; // done
            else return 12;  // comma
        }
    }

    //while (ttgo->getTouch(x, y)) {} // wait for user to release
    
}
