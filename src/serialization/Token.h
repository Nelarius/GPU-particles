#ifndef TOKEN_H_INCLUDED
#define TOKEN_H_INCLUDED

#include <string>

namespace ce {

/// \brief A token can represent a symbol, or a value, or a string
struct Token {
    char        kind;
    float       value;
    std::string string;

    Token( char ch )
    :   kind( ch ),
        value( 0.0 ),
        string( "" )
        {}

    Token( char ch, double val )
    :   kind( ch ),
        value( val ),
        string( "" )
        {}

    Token( char ch, const std::string& str )
    :   kind( ch ),
        value( 0.0 ),
        string( str )
        {}
};

}



#endif // TOKEN_H_INCLUDED
