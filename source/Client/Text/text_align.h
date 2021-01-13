// my_class.h
#ifndef TEXT_ALIGN_H // include guard
#define TEXT_ALIGN_H

#include <iostream>
#include <vector>
#include <string>
#include <sys/ioctl.h>
#include <unistd.h>
#include "Text_Color.h"

    //quick guildline
    //add #include"text_align.h"
    //add using namespace text_align;
    //call function align::cout_left_with_username( <username> , <message>);
    //              align::cout_right_with_username( <username> , <message>);

namespace text_align
{
    class align
    {
    public:
        static int cout_to_the_right(std::string message);
        static int cout_to_the_left(std::string message);
        static void cout_line();
        static void clear();
        static std::vector<std::string> divide_message_into_word( std::string long_message);
        static std::vector<std::string> combine_short_message( std::vector<std::string> divided_message);
        static void cout_vector(std::vector<std::string> vEctor);
        static int cout_vector_to_the_right(std::vector<std::string> vEctor);
        static int cout_vector_to_the_left(std::vector<std::string> vEctor);
        static int cout_right_with_username_vector(std::string username, std::vector<std::string> vEctor_message);
        static int cout_left_with_username_vector(std::string username, std::vector<std::string> vEctor_message);
        static int cout_right_with_username(std::string username, int color, std::string message);
        static int cout_left_with_username(std::string username, int color, std::string message);
        static std::vector<std::string> change_color_vector(std::vector<std::string>, int color);
    };
}

#endif /* TEXT_ALIGN_H */