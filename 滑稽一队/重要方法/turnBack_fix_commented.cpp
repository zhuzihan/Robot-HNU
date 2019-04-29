/*************************************************
Function: 带修正的180度掉头函数
Description: 掉头180度并修正罗盘偏差
Calls: // 被本函数调用的函数清单
Called By: // 调用本函数的函数清单
Input: // 输入参数说明，包括每个参数的作
// 用、取值说明及参数间关系。
    initAngle - 掉头前的角度值
    newAngle - 掉头中的角度值
    finalAngle - 掉头后的角度值
    fixInitAngle - 修正前角度
    fixAngle - 待修正角度数
    fixNewAngle - 修正中角度
Output: // 对输出参数的说明。
Return: // 函数返回值的说明
Others: // 其它说明
*************************************************/
void turnBackPlus() // 带修正的180度掉头函数
{
    wait(50);
    setZZero();
    initAngle = getAngle(); 
    newAngle = initAngle;
    Serial.print("initAngle:  ");
    Serial.println(initAngle);

    // 转180度
    while(abs(initAngle - newAngle) < 170)
    {
        newAngle = getAngle();
        
        RUN(-60,60);
        wait(50);

        // 输出掉头中的实时角度
        Serial.print("newAngle:  ");
        Serial.println(newAngle);
        
        // 输出掉头中的实时角度差值
        Serial.println(abs(initAngle - newAngle));
    }

    // 电机后摇硬值补偿
    RUN(0,0);
    wait(200);

    // 转向补偿
    RUN(60,-60);
    wait(150);
    
    RUN(0,0);
    wait(200);

    int turnCount = 0;
    int finalAngle = getAngle();

    // 输出掉头后的角度值
    Serial.print("finalAngle:  ");
    Serial.println(finalAngle);

    // 开始修正

    int fixAngle = 0;// 待修正角度数
    fixAngle = finalAngle - 180; // 待修正角度数 = 掉头后角度 - 180
    int fixInitAngle = getAngle();
    int fixNewAngle = fixInitAngle;

    // 反向修正
    if (fixAngle > 0){ // 掉头后角度 - 180 > 0
        while(1)
        {
            fixNewAngle = getAngle();

            // 若修正完成退出修正
            if(abs(fixNewAngle - fixInitAngle) > fixAngle)
            {
                RUN(0,0);
                delay(50);
                break;
            }

            // 反向转动修正
            RUN(30, -30);
            wait(30);

            // 电机后摇硬值补偿
            RUN(0, 0);
            wait(70);
        }

        // 获取修正后角度 - finalAngle
        finalAngle = getAngle();

        // 若修正后角度 - 调头前角度 < 176.5，说明修正过度，再正向微调一次
        if((finalAngle - initAngle) < 176.5){
            RUN(-30, 30);
            wait(50);
        }

        // 176.5 < 若修正后角度 - 调头前角度 < 180 ，说明修正略微过度，再正向小幅微调一次
        if((finalAngle - initAngle) > 177 && (finalAngle - initAngle) < 180){
            RUN(-25, 25);
            wait(30);
        }
        RUN(0, 0);
    //正向修正，fixAngle < 0，说明未转到位
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

            // 正向转动修正
            RUN(-30, 30);
            wait(30);

            // 电机后摇硬值补偿
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