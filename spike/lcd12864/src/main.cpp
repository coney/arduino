#include <Arduino.h>
#include "LCD12864.h"

void setup();
void loop();
#line 1 "src/sketch.ino"

#define ASCII2BIN(val, bit) (((val) >> (bit)) & 1)
#define ASCII2BINARGS(val) ASCII2BIN(val, 7), ASCII2BIN(val, 6), \
    ASCII2BIN(val, 5), ASCII2BIN(val, 4), ASCII2BIN(val, 3), \
    ASCII2BIN(val, 2), ASCII2BIN(val, 1), ASCII2BIN(val, 0)


void lcd_init() 
{
    LCDA.Initialise();
    delay(100);
    LCDA.Render();
}

void lcd_out_text()
{
    LCDA.RenderScreenBuffer(1);

    for (int i = 0; i < 64; ++i)
    {
        char buf[3];
        
        LCDA.Draw(false, i, 0);
        delay(1);

        sprintf(buf, "%02d", i);
        LCDA.setPins(1,0,ASCII2BINARGS(buf[0]));
        LCDA.setPins(1,0,ASCII2BINARGS(buf[1]));
        delay(200);
    }
}

void lcd_out_text2()
{
    LCDA.Draw(false, 0, 0);
    delay(1);

    for (int y = 0; y < 8; ++y)
    {
        for (int x = 0; x < 16; ++x)
        {
            LCDA.setPins(1,0,ASCII2BINARGS('#'));
        }
    }
}

void lcd_fill()
{
    LCDA.Draw(true,0,0);
    delay(1);
    LCDA.FillRectangle(0, 128, 128, 64);
    LCDA.RenderScreenBuffer(1);
    LCDA.RenderScreenBuffer(2);
}


void setup()
{
    lcd_init();
    //lcd_fill();
    //lcd_out_text();
    lcd_out_text2();
}

void loop()
{


}
