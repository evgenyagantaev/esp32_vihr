// The basic Time Display GUI
// if you are just updating the colon, fullUpdate =0
// if you want to update the complete display, fullUpdate =1
// This helps reduce flicker


void displayTime() 
{

    char buf[128];

    ttgo->tft->setTextSize(1);

    ttgo->tft->setTextColor(TFT_YELLOW, TFT_BLACK);
    snprintf(buf, sizeof(buf), "%s", ttgo->rtc->formatDateTime());
    ttgo->tft->drawString(buf, 5, 5, 7);
    snprintf(buf, sizeof(buf), "%s", ttgo->rtc->formatDateTime(PCF_TIMEFORMAT_DD_MM_YYYY));
    ttgo->tft->drawString(buf, 55, 60, 4);
    ttgo->tft->setTextColor(TFT_GREEN, TFT_BLACK);
    snprintf(buf, 128, "Bat: %.2fV %.2fmA      ", power->getBattVoltage() / 1000.0, power->getBattDischargeCurrent());
    ttgo->tft->drawString(buf, 0, 90, 4);
    snprintf(buf, 128, "Ext:  %.2fV", power->getVbusVoltage() / 1000.0);
    ttgo->tft->drawString(buf, 0, 115, 4);
    snprintf(buf, 128, "%.2fmA %.2fmA       ", power->getVbusCurrent(), power->getBattChargeCurrent());
    ttgo->tft->drawString(buf, 0, 140, 4);
    snprintf(buf, 128, "Stp: %5d WT: %5.1f           ", step_save, current_weight);
    ttgo->tft->setTextColor(TFT_RED, TFT_BLACK);
    ttgo->tft->drawString(buf, 0, 170, 4);
    snprintf(buf, 128, "Pll: %2d Psh: %2d            ", current_pull, current_push);
    ttgo->tft->drawString(buf, 0, 194, 4);
    //**********************************************************************
    ttgo->tft->setTextColor(TFT_YELLOW, TFT_BLACK);
    ttgo->tft->drawString(diagnostics, 0, 218, 4);
    //**********************************************************************
}
