#include <serialization/DataParser.h>
#include <serialization/SymbolicConstants.h>
#include <utils/Assert.h>

using ce::DataParser;
using ce::Token;
using ce::ObjectNode;

DataParser::DataParser( std::istream& in )
    :   tstream_( in ),
        in_( in )
        {}

void DataParser::parse() {
    ASSERT( in_.good(), "error: input stream is no good." );

    while ( in_.good() ) {
        Token t = tstream_.get();
        // discard all newlines
        while ( t.kind == '\n' ) {
            t = tstream_.get();
        }

        // handle end of file
        if ( t.kind == ce::symbol::Eof ) {
            break;
        }
        
        // terrible hack
        // for some reason there is no end of file, instead the parser thinks the name 'M'
        // is being defined at the end
        if ( t.kind == ce::symbol::Name && t.string == "M" ) {
            break;
        }

        // EOF doesn't seem to be detected properly in release mode
        // I need to work around it

        tstream_.putback( t );
        auto node = assignment_();
        nodeMap_.insert( std::make_pair( node->name(), node ) );
    }
}

ObjectNode::node_ptr DataParser::assignment_() {
    // the first token must be a name
    Token t = tstream_.get();
    if ( t.kind == ce::symbol::Name ) {
        const std::string field( t.string );
        // here should be the assignment operator
        t = tstream_.get();
        if ( t.kind == ce::symbol::Assignment ) {
            // get rid of the opening delimiter
            tstream_.get();
            return objectTraversal_( field );
        } else {
            std::string msg = "error: expected assignment operator after ";
            msg += field;
            ASSERT( false, msg.c_str() );
        }
    } else {
        ASSERT( false, "error: expected field name" );
    }
}

ObjectNode::node_ptr DataParser::objectTraversal_( const std::string& name ) {
    auto node = ObjectNode::node_ptr( new ObjectNode( name ) );

    while( true ) {
        Token t = tstream_.get();

        //handle possible newlines
        while ( t.kind == '\n' ) {
            t = tstream_.get();
        }

        // handle the end of object declaration
        if ( t.kind == ce::symbol::ObjEnd ) {
            break;
        }

        // here must be a name, or else syntax error
        if ( t.kind == ce::symbol::Name ) {
            const std::string field( t.string );
            Token t2 = tstream_.get();
            // here must be the assignment operator, or else syntax error
            if ( t2.kind == ce::symbol::Assignment ) {
                // here can be three different tokens:
                // a number, a string, or ObjStart delimiter
                Token t3 = tstream_.get();
                if ( t3.kind == ce::symbol::ObjStart ) {
                    node->addNode( field, objectTraversal_( field ) );
                } else if ( t3.kind == ce::symbol::Number ) {
                    node->addNumber( field, t3.value );
                } else if ( t3.kind == ce::symbol::String ) {
                    node->addString( field, t3.string );
                }
            } else {
                ASSERT( false, "error: expected assignment operator after %s in object %s" );
            }
        } else {
            ASSERT( false, "error: expected name in object %s" );
        }   //if
    }   // while
    return node;
}

const ObjectNode::node_map& DataParser::getNodes() const {
    return nodeMap_;
}

ObjectNode::node_ptr DataParser::node( const std::string& field ) const {
    return nodeMap_.at( field );
}

bool DataParser::contains( const std::string& field ) const {
    auto it = nodeMap_.find( field );
    if ( it != nodeMap_.end() ) {
        return true;
    }
    return false;
}
