#include "text_align.h"
#include <iomanip>
#include <iostream>
#include <string>
#include <iterator>
#include <sstream>

using namespace text_align;
using namespace Text_Color;
using namespace std;
 


/*print message at the right of the screen*/
int align::cout_to_the_right (string message)
{
    ///////////////////////////////////////
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    //printf ("lines %d\n", w.ws_row);
    // printf ("columns %d\n", w.ws_col);
    /*------get window width------*/
    //////////////////////////////////////
    cout << "|";
    cout << std::right << std::setw(w.ws_col) <<  message;
    cout << "|";
    return 0;
}

/*print message at the left of the screen*/
int align::cout_to_the_left (string message)
{   
    ///////////////////////////////////////
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    //printf ("lines %d\n", w.ws_row);
    // printf ("columns %d\n", w.ws_col);
    /*------get window width------*/
    //////////////////////////////////////
    cout << "|";
    cout << std::left << std::setw(w.ws_col)  <<  message;
    cout << "|";
    return 0;
}


/*write a straight line*/
void align::cout_line()
{
    for(int i = 1; i <= 72; i++) {cout << "-";};
}

void align::clear()
{
    int temp1 = system("clear");
}

/*print vector values*/
void align::cout_vector( vector<string> vEctor)
{
    unsigned int i;
    for ( i = 0 ; i < vEctor.size(); i++)
        {
            cout << vEctor.at(i) << endl;
        }
}

/*divide message into vector of string word*/
vector<string> align::divide_message_into_word( string long_message)
{
    

    // std::string sentence = "And I feel fine...";
    vector<string> divided_message;
    istringstream iss(long_message);
    copy(istream_iterator<string>(iss),
         istream_iterator<string>(),
         back_inserter(divided_message));

    // for ( int i = 0; i < divided_message.size(); i++)
    //     {
    //         cout << divided_message.at(i) << endl;
    //     }

    return divided_message;
}

/*combine words into short string to display*/
vector<string> align::combine_short_message( vector<string> divided_message)
{
    string temp = "";                   //luu       tin nhan ngan duoi 35 ky tu
    vector<string> short_message;       //luu chuoi tin nhan ngan duoi 35 ky tu
    
    
    int size_string = 0;                           //medium loop
    unsigned int count_vertor_divided_message = 0;   //small loop
    
    
    unsigned int size_divided_message = divided_message.size(); //kich thuoc vector chu word

    for (count_vertor_divided_message = 0; count_vertor_divided_message < size_divided_message; count_vertor_divided_message ++)
    //                                     count_vertor_divided_message <               22          
    {
        //size string <= 35(length of the text each line)
        if (size_string <= 35)
        {
            //temporal string saves line of text <= 35 character
            temp += (divided_message.at(count_vertor_divided_message) + " ");
            //update size of temporal string
            size_string += (divided_message.at(count_vertor_divided_message).size() + 1);
        }
        else
        {   
            temp += (divided_message.at(count_vertor_divided_message) + " ");
            short_message.push_back(temp);
            //create new temporal string to save the next line
            temp = "";
            size_string = 0;
        }
    }    
    short_message.push_back(temp);
    return short_message;
}

int align::cout_vector_to_the_right( vector<string> vEctor)
{

    ///////////////////////////////////////
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    //printf ("lines %d\n", w.ws_row);
    //printf ("columns %d\n", w.ws_col);
    /*------get window width------*/
    //////////////////////////////////////

    string message;
    unsigned int count, vEctor_size = vEctor.size();
    for(count = 0; count < vEctor_size; count++)
    {   
        message = vEctor[count];
        cout << " ";    /*first line character*/
        //for(int i = 1; i<= 35 ; i++) cout << " ";
        cout << std::right << std::setw(w.ws_col - 4) << message;
        cout << " ";    /*last line character*/
        cout << endl;
    }
    return 0;
}

int align::cout_vector_to_the_left( vector<string> vEctor)
{
    ///////////////////////////////////////
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    //printf ("lines %d\n", w.ws_row);
    // printf ("columns %d\n", w.ws_col);
    /*------get window width------*/
    //////////////////////////////////////

    string message;
    unsigned int count, vEctor_size = vEctor.size();
    for(count = 0; count < vEctor_size; count++)
    {   
        message = vEctor[count];
        cout << " ";    /*first line character*/
        cout << std::left << std::setw( w.ws_col - 5 ) << message;
        cout << " ";    /*last line character*/
        cout << endl;
    }
    return 0;
}

/*print out message in lines with user name*/
int align::cout_right_with_username_vector(string username, vector<string> vEctor_message)
{   
    //////////////////////////////////////
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    //printf ("lines %d\n", w.ws_row);
    // printf ("columns %d\n", w.ws_col);

    //////////////////////////////////////
    cout << " ";    /*first line character*/
    cout << std::right << std::setw( w.ws_col ) <<  username ;
    cout << " ";    /*last line character*/
    cout << endl;
    cout_vector_to_the_right(vEctor_message);
    
    

    return 0;
}

int align::cout_left_with_username_vector(string username, vector<string> vEctor_message)
{
    ///////////////////////////////////////
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    //printf ("lines %d\n", w.ws_row);
    // printf ("columns %d\n", w.ws_col);
    /*------get window width------*/
    //////////////////////////////////////


    cout << " ";    /*first line character*/
    cout << std::left  << std::setw( w.ws_col ) <<  username;
    cout << " ";    /*last line character*/
    cout << endl;
    cout_vector_to_the_left(vEctor_message);
    
    return 0;
}

/*print message with username at the right of the screen*/
int align::cout_right_with_username (string username, int color, string message)
{
    vector<string> vector1, vector2;

    vector1 = align::divide_message_into_word(message);
    vector2 = align::combine_short_message(vector1);
   
    /*change color text*/
    vector1 = align::change_color_vector(vector2, color);
    username = "  " + username + "  ";
    username = color_text::color_Message_Backgound(username, 0); /*user name white back ground*/

    cout_right_with_username_vector(username, vector1);

    return 0;
}

/*print message with username at the left of the screen*/
int align::cout_left_with_username (string username, int color, string message)
{
    vector<string> vector1, vector2;

    vector1 = align::divide_message_into_word(message);
    vector2 = align::combine_short_message(vector1);
    
    /*change color text*/
    vector1 = align::change_color_vector(vector2, color);
    username = "  " + username + "  ";
    username = color_text::color_Message_Backgound(username, 0); /*user name white back ground*/

    cout_left_with_username_vector(username, vector1);
    return 0;
}

vector<string> align::change_color_vector(vector<string> vEctor, int color)
{
    string temp;
    vector<string> temp_vector;
    unsigned int i;

    for (i = 0; i < vEctor.size(); i++)
    {
        temp = vEctor[i];
        temp = color_text::color_Message( temp , color );
        temp_vector.push_back(temp);
    }

    return temp_vector;
}