void zoomzoom()
{
    // read current time-date
    RTC_Date tnow = ttgo->rtc->getDateTime();
    hh = tnow.hour;
    mm = tnow.minute;
    ss = tnow.second;
    dday = tnow.day;
    mmonth = tnow.month;
    yyear = tnow.year;

    if((mm == 15) && (qarter1 == 0))
    {
        qarter1 = 1;
        qarter2 = 0;
        qarter3 = 0;
        qarter4 = 0;

        seconds_counter = 0;
        // f ..-.
        digitalWrite(4, HIGH);
        delay(50); // "."
        digitalWrite(4, LOW);
        delay(400);
        digitalWrite(4, HIGH);
        delay(50); // "."
        digitalWrite(4, LOW);
        delay(400);
        digitalWrite(4, HIGH);
        delay(400); // "-"
        digitalWrite(4, LOW);
        delay(400);
        digitalWrite(4, HIGH);
        delay(50); // "."
        digitalWrite(4, LOW);
    }
    else if((mm == 30) && (qarter2 == 0))
    {
        qarter1 = 0;
        qarter2 = 1;
        qarter3 = 0;
        qarter4 = 0;

        seconds_counter = 0;
        // b -...
        digitalWrite(4, HIGH);
        delay(400); // "-"
        digitalWrite(4, LOW);
        delay(400);
        digitalWrite(4, HIGH);
        delay(50); // "."
        digitalWrite(4, LOW);
        delay(400);
        digitalWrite(4, HIGH);
        delay(50); // "."
        digitalWrite(4, LOW);
        delay(400);
        digitalWrite(4, HIGH);
        delay(50); // "."
        digitalWrite(4, LOW);
        
    }
    else if((mm == 45) && (qarter3 == 0))
    {
        qarter1 = 0;
        qarter2 = 0;
        qarter3 = 1;
        qarter4 = 0;

        seconds_counter = 0;
        // c -.-.
        
        digitalWrite(4, HIGH);
        delay(400); // "-"
        digitalWrite(4, LOW);
        delay(400);
        digitalWrite(4, HIGH);
        delay(50); // "."
        digitalWrite(4, LOW);
        delay(400);
        digitalWrite(4, HIGH);
        delay(400); // "-"
        digitalWrite(4, LOW);
        delay(400);
        digitalWrite(4, HIGH);
        delay(50); // "."
        digitalWrite(4, LOW);
    }
    else if((mm == 0) && (qarter4 == 0))
    {
        qarter1 = 0;
        qarter2 = 0;
        qarter3 = 0;
        qarter4 = 1;

        seconds_counter = 0;
        // l .-..
        digitalWrite(4, HIGH);
        delay(50); // "."
        digitalWrite(4, LOW);
        delay(400);
        digitalWrite(4, HIGH);
        delay(400); // "-"
        digitalWrite(4, LOW);
        delay(400);
        digitalWrite(4, HIGH);
        delay(50); // "."
        digitalWrite(4, LOW);
        delay(400);
        digitalWrite(4, HIGH);
        delay(50); // "."
        digitalWrite(4, LOW);
        
    }
}
