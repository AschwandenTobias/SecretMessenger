/*******************************************************************************
 * U8g2 latest unifont-14.0.02 all glyphs example
 * Please note this font is 2,250,360 in size and cannot fit in many platform.
 * This font is generated by U8g2 tools:
 * u8g2/tools/font/bdfconv/./bdfconv -v -f 1 -b 1 -m "0-1114111" unifont_jp-14.0.02.bdf -o u8g2_font_unifont_h_utf8.h -n u8g2_font_unifont_h_utf8
 * Hello world in multiple languages
 * https://codegolf.stackexchange.com/questions/146544/hello-world-in-multiple-languages
 ******************************************************************************/

/*******************************************************************************
 * Start of Arduino_GFX setting
 *
 * Arduino_GFX try to find the settings depends on selected board in Arduino IDE
 * Or you can define the display dev kit not in the board list
 * Defalult pin list for non display dev kit:
 * Arduino Nano, Micro and more: CS:  9, DC:  8, RST:  7, BL:  6, SCK: 13, MOSI: 11, MISO: 12
 * ESP32 various dev board     : CS:  5, DC: 27, RST: 33, BL: 22, SCK: 18, MOSI: 23, MISO: nil
 * ESP32-C3 various dev board  : CS:  7, DC:  2, RST:  1, BL:  3, SCK:  4, MOSI:  6, MISO: nil
 * ESP32-S2 various dev board  : CS: 34, DC: 35, RST: 33, BL: 21, SCK: 36, MOSI: 35, MISO: nil
 * ESP32-S3 various dev board  : CS: 40, DC: 41, RST: 42, BL: 48, SCK: 36, MOSI: 35, MISO: nil
 * ESP8266 various dev board   : CS: 15, DC:  4, RST:  2, BL:  5, SCK: 14, MOSI: 13, MISO: 12
 * Raspberry Pi Pico dev board : CS: 17, DC: 27, RST: 26, BL: 28, SCK: 18, MOSI: 19, MISO: 16
 * RTL8720 BW16 old patch core : CS: 18, DC: 17, RST:  2, BL: 23, SCK: 19, MOSI: 21, MISO: 20
 * RTL8720_BW16 Official core  : CS:  9, DC:  8, RST:  6, BL:  3, SCK: 10, MOSI: 12, MISO: 11
 * RTL8722 dev board           : CS: 18, DC: 17, RST: 22, BL: 23, SCK: 13, MOSI: 11, MISO: 12
 * RTL8722_mini dev board      : CS: 12, DC: 14, RST: 15, BL: 13, SCK: 11, MOSI:  9, MISO: 10
 * Seeeduino XIAO dev board    : CS:  3, DC:  2, RST:  1, BL:  0, SCK:  8, MOSI: 10, MISO:  9
 * Teensy 4.1 dev board        : CS: 39, DC: 41, RST: 40, BL: 22, SCK: 13, MOSI: 11, MISO: 12
 ******************************************************************************/
#include <U8g2lib.h>
#include <Arduino_GFX_Library.h>

#define GFX_BL DF_GFX_BL // default backlight pin, you may replace DF_GFX_BL to actual backlight pin

/* More dev device declaration: https://github.com/moononournation/Arduino_GFX/wiki/Dev-Device-Declaration */
#if defined(DISPLAY_DEV_KIT)
Arduino_GFX *gfx = create_default_Arduino_GFX();
#else /* !defined(DISPLAY_DEV_KIT) */

/* More data bus class: https://github.com/moononournation/Arduino_GFX/wiki/Data-Bus-Class */
Arduino_DataBus *bus = create_default_Arduino_DataBus();

/* More display class: https://github.com/moononournation/Arduino_GFX/wiki/Display-Class */
Arduino_GFX *gfx = new Arduino_ILI9341(bus, DF_GFX_RST, 3 /* rotation */, false /* IPS */);

#endif /* !defined(DISPLAY_DEV_KIT) */
/*******************************************************************************
 * End of Arduino_GFX setting
 ******************************************************************************/

/* more fonts at: https://github.com/moononournation/ArduinoFreeFontFile.git */

String helloWorldStrings[] = {
"Hello Wêreld!", // Afrikaans
"Përshendetje Botë!", // Albanian
"ሰላም ልዑል!", // Amharic
"مرحبا بالعالم!", // Arabic
"Բարեւ աշխարհ!", // Armenian
"Kaixo Mundua!", // Basque
"Прывітанне Сусвет!", // Belarussian
"ওহে বিশ্ব!", // Bengali
"Здравей свят!", // Bulgarian
"Hola món!", // Catalan
"Moni Dziko Lapansi!", // Chichewa
"世界你好！", // Chinese
"Pozdrav svijete!", // Croatian
"Ahoj světe!", // Czech
"Hej Verden!", // Danish
"Hallo Wereld!", // Dutch
"Hello World!", // English
"Tere maailm!", // Estonian
"Hei maailma!", // Finnish
"Bonjour monde!", // French
"Hallo wrâld!", // Frisian
"გამარჯობა მსოფლიო!", // Georgian
"Hallo Welt!", // German
"Γειά σου Κόσμε!", // Greek
"Sannu Duniya!", // Hausa
"שלום עולם!", // Hebrew
"नमस्ते दुनिया!", // Hindi
"Helló Világ!", // Hungarian
"Halló heimur!", // Icelandic
"Ndewo Ụwa!", // Igbo
"Halo Dunia!", // Indonesian
"Ciao mondo!", // Italian
"こんにちは世界！", // Japanese
"Сәлем Әлем!", // Kazakh
"សួស្តី​ពិភពលោក!", // Khmer
"Салам дүйнө!", // Kyrgyz
"ສະ​ບາຍ​ດີ​ຊາວ​ໂລກ!", // Lao
"Sveika pasaule!", // Latvian
"Labas pasauli!", // Lithuanian
"Moien Welt!", // Luxemburgish
"Здраво свету!", // Macedonian
"Hai dunia!", // Malay
"ഹലോ വേൾഡ്!", // Malayalam
"Сайн уу дэлхий!", // Mongolian
"မင်္ဂလာပါကမ္ဘာလောက!", // Myanmar
"नमस्कार संसार!", // Nepali
"Hei Verden!", // Norwegian
"سلام نړی!", // Pashto
"سلام دنیا!", // Persian
"Witaj świecie!", // Polish
"Olá Mundo!", // Portuguese
"ਸਤਿ ਸ੍ਰੀ ਅਕਾਲ ਦੁਨਿਆ!", // Punjabi
"Salut Lume!", // Romanian
"Привет мир!", // Russian
"Hàlo a Shaoghail!", // Scots Gaelic
"Здраво Свете!", // Serbian
"Lefatše Lumela!", // Sesotho
"හෙලෝ වර්ල්ඩ්!", // Sinhala
"Pozdravljen svet!", // Slovenian
"¡Hola Mundo!", // Spanish, Leading '¡' optional
"Halo Dunya!", // Sundanese
"Salamu Dunia!", // Swahili
"Hej världen!", // Swedish
"Салом Ҷаҳон!", // Tajik
"สวัสดีชาวโลก!", // Thai
"Selam Dünya!", // Turkish
"Привіт Світ!", // Ukrainian
"Salom Dunyo!", // Uzbek
"Chào thế giới!", // Vietnamese
"Helo Byd!", // Welsh
"Molo Lizwe!", // Xhosa
"העלא וועלט!", // Yiddish
"Mo ki O Ile Aiye!", // Yoruba
"Sawubona Mhlaba!" // Zulu
};

void setup(void)
{
    gfx->begin();
    gfx->fillScreen(BLACK);
    gfx->setUTF8Print(true); // enable UTF8 support for the Arduino print() function

#ifdef GFX_BL
    pinMode(GFX_BL, OUTPUT);
    digitalWrite(GFX_BL, HIGH);
#endif

    gfx->setCursor(0, 14);
    gfx->setFont(u8g2_font_unifont_h_utf8);
    gfx->println("Hello world in multiple languages");

    delay(2000); // 2 seconds
}

void loop()
{
    gfx->setCursor(random(gfx->width() / 4), random(gfx->height() - 32) + 16);
    gfx->setTextColor(random(0xffff), random(0xffff));
    gfx->setTextSize(random(2) + 2 /* x scale */, random(2) + 2 /* y scale */);
    gfx->println(helloWorldStrings[random(74)]);

    delay(500); // 0.5 second
}
