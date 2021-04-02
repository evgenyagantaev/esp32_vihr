

const int max_job_menu_items = 7; // number of items
String job_menu_itemName[max_job_menu_items] = {"Vihr", "E spusk", "T-watch", "Texts", "Face-id", "Idle", "Back"}; // items names

uint8_t job_menu() 
{
    int maxApp = max_job_menu_items;
  
    int mSelect = 0; // The currently highlighted app
    int16_t x, y, tx, ty;
  
    boolean exitMenu = false; // used to stay in the menu until user selects app
  
    job_menu_display(0); // display the list of Apps
  
    while (!exitMenu) 
    {
        if (ttgo->getTouch(x, y)) // If you have touched something...
        { 
    
          while (ttgo->getTouch(tx, ty)) {} // wait until you stop touching
    
          if (y >= 160) // you want the menu list shifted up
          { 
              mSelect += 1;
              if (mSelect == maxApp) mSelect = 0;
              job_menu_display(mSelect);
          }
    
          if (y <= 80) // you want the menu list shifted down
          { 
              mSelect -= 1;
              if (mSelect < 0) mSelect = maxApp - 1;
              job_menu_display(mSelect);
          }
          if (y > 80 && y < 160) // You selected the middle
          { 
              exitMenu = true;
          }
        }
    }
    //Return with mSelect containing the desired mode
    ttgo->tft->fillScreen(TFT_BLACK);
    return mSelect;
}

void job_menu_display(int mSel) 
{

    int curSel = 0;
    // Display mode header
    ttgo->tft->fillScreen(TFT_BLUE);
    ttgo->tft->fillRect(0, 80, 239, 80, TFT_BLACK);
  
    // Display apps
    if (mSel == 0) curSel = max_job_menu_items - 1;
    else curSel = mSel - 1;
  
    ttgo->tft->setTextSize(2);
    ttgo->tft->setTextColor(TFT_GREEN);
    ttgo->tft->setCursor(50, 30);
    ttgo->tft->println(job_menu_itemName[curSel]);
  
    ttgo->tft->setTextSize(3);
    ttgo->tft->setTextColor(TFT_RED);
    ttgo->tft->setCursor(40, 110);
    ttgo->tft->println(job_menu_itemName[mSel]);
  
    if (mSel == max_job_menu_items - 1) curSel = 0;
    else curSel = mSel + 1;
  
    ttgo->tft->setTextSize(2);
    ttgo->tft->setTextColor(TFT_GREEN);
    ttgo->tft->setCursor(50, 190);
    ttgo->tft->print(job_menu_itemName[curSel]);
}
