#ifndef COLOR_TEXT // include guard
#define COLOR_TEXT

#include<iostream>


using namespace std;

namespace Text_Color
{
    class color_text
    {

        //          foreground background
        // black        30         40
        // red          31         41
        // green        32         42
        // yellow       33         43
        // blue         34         44
        // magenta      35         45
        // cyan         36         46
        // white        37         47

        // reset             0  (everything back to normal)
        // bold/bright       1  (often a brighter shade of the same colour)
        // underline         4
        // inverse           7  (swap foreground and background colours)
        // bold/bright off  21
        // underline off    24
        // inverse off      27

        //Brief explain:
        //"\033[24;31m" + text + "\033[0m\n"
        //"\033[" + underline off + color + "m" + string text + "\033[" + additional choice(this case, 0-back to normal)

        public:
            static string color_White_Text( string text)
            {
                string colored_text = "\033[24;37m" + text + "\033[0m";
                return colored_text;
            }
        
        public:
            static string color_Red_Text( string text)
            {
                string colored_text = "\033[24;31m" + text + "\033[0m";
                return colored_text;
            }

        public:
            static string color_Green_Text( string text)
            {
                string colored_text = "\033[24;32m" + text + "\033[0m";
                return colored_text;
            }

        public:
            static string color_Blue_Text( string text)
            {
                string colored_text = "\033[24;34m" + text + "\033[0m";
                return colored_text;
            }

        public:
            static string color_White_Underline_Text( string text)
            {
                string colored_text = "\033[4;37m" + text + "\033[0m";
                return colored_text;
            }

        public:
            static string color_Red_Underline_Text( string text)
            {
                string colored_text = "\033[4;31m" + text + "\033[0m";
                return colored_text;
            }

        public:
            static string color_Green_Underline_Text( string text)
            {
                string colored_text = "\033[4;32m" + text + "\033[0m";
                return colored_text;
            }

        public:
            static string color_Blue_Underline_Text( string text)
            {
                string colored_text = "\033[4;34m" + text + "\033[0m";
                return colored_text;
            }

        public:
            static string color_White_Bold_Text( string text)
            {
                string color_text = "\033[1;24;37m" + text + "\033[0m";
                return color_text;
            }

        public:
            static string color_Red_Bold_Text( string text)
            {
                string color_text = "\033[1;24;31m" + text + "\033[0m";
                return color_text;
            }

        public:
            static string color_Green_Bold_Text( string text)
            {
                string color_text = "\033[1;24;32m" + text + "\033[0m";
                return color_text;
            }

        public:
            static string color_Blue_Bold_Text( string text)
            {
                string color_text = "\033[1;24;34m" + text + "\033[0m";
                return color_text;
            }

        public:
            static string color_White_Bold_Underline_Text( string text)
            {
                string color_text = "\033[1;4;37m" + text + "\033[0m";
                return color_text;
            }

        public:
            static string color_Red_Bold_Underline_Text( string text)
            {
                string color_text = "\033[1;4;31m" + text + "\033[0m";
                return color_text;
            }

        public:
            static string color_Green_Bold_Underline_Text( string text)
            {
                string color_text = "\033[1;4;32m" + text + "\033[0m";
                return color_text;
            }

        public:
            static string color_Blue_Bold_Underline_Text( string text)
            {
                string color_text = "\033[1;4;34m" + text + "\033[0m";
                return color_text;
            }
        
        //function call to color message:
        //
        //add:
        //#include "Text_Color.h"
        //using namespace Text_Color;
        //
        //call:
        //color_text::color_Message( <message> , <color> );

        public:
            static string color_Message(string message, int color)
            {
                // choose color base on color_id
                // color_id = 0        white(default)
                // color_id = 1        red
                // color_id = 2        blue
                // color_id = 3        green
                
                

                switch (color){
                    case 0:
                        message = color_White_Text(message);
                        
                        break;
                    case 1:
                        message = color_Red_Text(message);
                        
                        break;
                    case 2:
                        message = color_Blue_Text(message);
                        
                        break;
                    case 3:
                        message = color_Green_Text(message);
                        
                        break;
                    default:   
                        message = color_White_Text(message);    
                        
                }
                return message;
            }


        //          foreground background
        // black        30         40
        // red          31         41
        // green        32         42
        // yellow       33         43
        // blue         34         44
        // magenta      35         45
        // cyan         36         46
        // white        37         47

        // reset             0  (everything back to normal)
        // bold/bright       1  (often a brighter shade of the same colour)
        // underline         4
        // inverse           7  (swap foreground and background colours)
        // bold/bright off  21
        // underline off    24
        // inverse off      27
        public:
            static string color_Blue_Background_Text( string text)
            {
                string color_text = "\033[1;24;37;44m" + text + "\033[0m";
                return color_text;
            }
            
        public:
            static string color_Red_Background_Text( string text)
            {
                string color_text = "\033[1;24;37;41m" + text + "\033[0m";
                return color_text;
            }

        public:
            static string color_Green_Background_Text( string text)
            {
                string color_text = "\033[1;24;37;42m" + text + "\033[0m";
                return color_text;
            }

        public:
            static string color_White_Background_Text( string text)
            {
                string color_text = "\033[1;24;30;47m" + text + "\033[0m";
                return color_text;
            }

        public:
            static string color_Message_Backgound(string text, int color)
            {   
                switch (color){
                    case 0:
                        text = color_White_Background_Text(text);
                        
                        break;
                    case 1:
                        text = color_Red_Background_Text(text);
                        
                        break;
                    case 2:
                        text = color_Blue_Background_Text(text);
                        
                        break;
                    case 3:
                        text = color_Green_Background_Text(text);
                        
                        break;
                    default:   
                        text = color_White_Background_Text(text);    
                        
                }
                return text;
            }

    };
}

#endif /* COLOR_TEXT */