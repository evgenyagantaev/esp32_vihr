// Step control application menu


const int maxItems = 3; // number of items
String itemName[maxItems] = {"Reset", "Back", "Back"}; // items names

uint8_t step_control_menu() 
{
    int mSelect = 0; // The currently highlighted app
    int16_t x, y, tx, ty;
  
    boolean exitMenu = false; // used to stay in the menu until user selects app
  
    set_step_menu_display(0); // display the list of Apps
  
    while (!exitMenu) 
    {
        if (ttgo->getTouch(x, y)) // If you have touched something...
        { 
    
          while (ttgo->getTouch(tx, ty)) {} // wait until you stop touching
    
          if (y >= 160) // bottom selected
          { 
              exitMenu = true;
          }
    
          if (y <= 80) // top selected
          { 
              exitMenu = true;
          }
          if (y > 80 && y < 160) // You selected the middle
          { 
              // reset step counter
              sensor->resetStepCounter();
              preferences.begin("twatch", false);
              preferences.putUInt("step_save", 0);
              preferences.end();
              step_counter_initial = 0;
              step_save = 0;
              exitMenu = true;
          }
        }
    }
    //Return with mSelect containing the desired mode
    ttgo->tft->fillScreen(TFT_BLACK);
    return mSelect;
}

void set_step_menu_display(int mSel) 
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
    ttgo->tft->println(itemName[curSel]);
  
    ttgo->tft->setTextSize(3);
    ttgo->tft->setTextColor(TFT_RED);
    ttgo->tft->setCursor(40, 110);
    ttgo->tft->println(itemName[mSel]);
  
    if (mSel == maxItems - 1) curSel = 0;
    else curSel = mSel + 1;
  
    ttgo->tft->setTextSize(2);
    ttgo->tft->setTextColor(TFT_GREEN);
    ttgo->tft->setCursor(50, 190);
    ttgo->tft->print(itemName[curSel]);
}
