#ifndef TOKENSTREAM_H_INCLUDED
#define TOKENSTREAM_H_INCLUDED

#include <serialization/Token.h>
#include <istream>
#include <queue>

namespace ce {

class TokenStream {
    public:
        /// \brief Construct the Tokenstream from a reference to the source input stream
        TokenStream( std::istream& in );
        ~TokenStream() = default;

        TokenStream( const TokenStream& )               = delete;
        TokenStream& operator=( const TokenStream& )    = delete;
        TokenStream( TokenStream&& )                    = delete;
        TokenStream& operator=( TokenStream&& )         = delete;

        Token get();

        void putback( const Token& t );

    private:

        // METHODS
        void checkForComments_( char* );
        void eatComment_();

        // FIELDS
        std::queue<Token>   buffer_;
        std::istream&       in_;
};

}   //namespace ce


#endif // TOKENSTREAM_H_INCLUDED
