







const int max_journal_menu_items = 3; // number of items
String journal_menu_itemName[max_journal_menu_items] = {"Print", "Reset", "Back"}; // items names

uint8_t journal_menu() 
{
    int mSelect = 0; // The currently highlighted app
    int16_t x, y, tx, ty;
  
    boolean exitMenu = false; // used to stay in the menu until user selects app
  
    journal_menu_display(0); // display the list of Apps
  
    while (!exitMenu) 
    {
        if (ttgo->getTouch(x, y)) // If you have touched something...
        { 
    
            while (ttgo->getTouch(tx, ty)) {} // wait until you stop touching
  
            if (y <= 80) // top selected (back)
            { 
                exitMenu = true;
            }
            else
            {

      
                if (y >= 160) // bottom selected (reset)
                { 
                    exitMenu = true;
                    writeFile(SPIFFS, "/journal.txt", "\r\n");
                    
                }
          
                
                if (y > 80 && y < 160) // You selected the middle (print)
                { 
                  
                    exitMenu = true;
                    readFile(SPIFFS, "/journal.txt");
                }
            }
        }
    }
    //Return with mSelect containing the desired mode
    ttgo->tft->fillScreen(TFT_BLACK);
    return mSelect;
}

void journal_menu_display(int mSel) 
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
    ttgo->tft->println(journal_menu_itemName[curSel]);
  
    ttgo->tft->setTextSize(3);
    ttgo->tft->setTextColor(TFT_RED);
    ttgo->tft->setCursor(40, 110);
    ttgo->tft->println(journal_menu_itemName[mSel]);
  
    if (mSel == maxItems - 1) curSel = 0;
    else curSel = mSel + 1;
  
    ttgo->tft->setTextSize(2);
    ttgo->tft->setTextColor(TFT_GREEN);
    ttgo->tft->setCursor(50, 190);
    ttgo->tft->print(journal_menu_itemName[curSel]);
}
