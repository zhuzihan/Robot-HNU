void turnBackPlus()
{
    wait(50);
    setZZero();
    initAngle = getAngle();
    newAngle = initAngle;
    Serial.print("initAngle:  ");
    Serial.println(initAngle);
    while(abs(initAngle - newAngle) < 170)
    {
        newAngle = getAngle();
        RUN(-60,60);
        wait(50);
        Serial.print("newAngle:  ");
        Serial.println(newAngle);
        Serial.println(abs(initAngle - newAngle));
    }
    RUN(0,0);
    wait(200);

    RUN(60,-60);
    wait(150);
    
    RUN(0,0);
    wait(200);
    int turnCount = 0;
    int finalAngle = getAngle();
    Serial.print("finalAngle:  ");
    Serial.println(finalAngle);
    int fixAngle = 0;
    fixAngle = finalAngle - 180;
    int fixInitAngle = getAngle();
    int fixNewAngle = fixInitAngle;
    if (fixAngle > 0){
        while(1)
        {
            fixNewAngle = getAngle();
            if(abs(fixNewAngle - fixInitAngle) > fixAngle)
            {
                RUN(0,0);
                delay(50);
                break;
            }
            RUN(30, -30);
            wait(30);
            RUN(0, 0);
            wait(70);
        }
        finalAngle = getAngle();
        if((finalAngle - initAngle) < 176.5){
            RUN(-30, 30);
            wait(50);
        }
        if((finalAngle - initAngle) > 177 && (finalAngle - initAngle) < 180){
            RUN(-25, 25);
            wait(30);
        }
        RUN(0, 0);
    }else{
        while(1)
        {
            fixNewAngle = getAngle();
            if(abs(fixNewAngle - fixInitAngle) > abs(fixAngle))
            {
                RUN(0,0);
                delay(50);
                break;
            }
            RUN(-30, 30);
            wait(30);
            RUN(0, 0);
            wait(70);
        }
        RUN(30, -30);
        wait(50);
        RUN(0, 0);
    }
    wait(200);
}

float getAngle()
{
    JY901.GetAngle();
    float angle = (float)JY901.stcAngle.Angle[2]/32768*180;
    return angle < 0 ? 360 + angle : angle;
}
void setZZero()
{
    Serial.write(JY901_set_zero_hex,5);
    delay(50);
    Serial.write(JY901_save_hex,5);
    delay(50);
}

unsigned char JY901_set_zero_hex[5] = {0xff,0xaa,0x01,0x04,0x00};
unsigned char JY901_save_hex[5] = {0xff,0xaa,0x00,0x00,0x00};