#include <serialization/TokenStream.h>
#include <serialization/SymbolicConstants.h>
#include <utils/Assert.h>
#include <cctype>
#include <cstdio>   //for sprintf

#include <iostream>

using ce::TokenStream;
using ce::Token;

TokenStream::TokenStream( std::istream& in )
    :   buffer_(),
        in_( in )
        {}

Token TokenStream::get() {
    if ( !buffer_.empty() ) {
        Token t = buffer_.front();
        buffer_.pop();
        return t;
    }

    char ch;

    //check for end of file
    if ( in_.eof() ) {
        return Token( ce::symbol::Eof );
    }

    in_ >> ch;

    checkForComments_( &ch );

    switch( ch ) {
        case '\n':
        case ce::symbol::Assignment:
        case ce::symbol::ObjStart:
        case ce::symbol::ObjEnd:
            return Token( ch ); //let each operator represent itself
        case EOF:
            return Token( ce::symbol::Eof );
        //numeric literal can start with a point
        case '.':
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9': {
            //stick the leading digit or point of number back, and read it
            //from stream as a floating point literal
            in_.putback( ch );
            double val;
            in_ >> val;
            return Token( ce::symbol::Number, val );
        }

        // handle string
        case ce::symbol::StrDelimiter: {
            std::string str;
            while ( in_.get( ch ) && ch != ce::symbol::StrDelimiter ) {
                str += ch;
            }
            return Token( ce::symbol::String, str );
        }

        default: {
            if ( isalpha( ch ) ) {
                std::string str;
                str += ch;
                // this defines a string:
                // it must begin with an alphanumeric character
                // after that, it can contain alphabetical characters,
                // numbers, or underscores
                while ( in_.get( ch ) &&
                       ( (isalpha( ch ) || isdigit( ch ) ) || ch == '_' ) ) {
                    str += ch;
                }
                in_.putback( ch );
                return Token( ce::symbol::Name, str );
            } else {
                // this is a horrific hack
                // on windows, EOF doesn't seem to be detected.
                // instead, return an EOF when we start to encounter garbage
                return Token( ce::symbol::Eof );
            }
        }
    }
}

void TokenStream::putback( const Token& token ) {
    buffer_.push( token );
}

void TokenStream::checkForComments_( char* ch ) {
    while( true ) {
        if ( *ch == ce::symbol::Comment ) {
            eatComment_();
        } else {
            break;
        }
        // get the next char
        // prevent operator>>() from eating the newline
        /*if ( in_.peek() == '\n' ) {
            *ch = in_.get();
        } else {
            in_ >> *ch;
        }*/
        in_ >> *ch;
    }
}

void TokenStream::eatComment_() {
    char ch;
    while ( in_.get( ch ) && ch != '\n' );
}
