#ifndef DATAPARSER_H
#define DATAPARSER_H

#include <serialization/TokenStream.h>
#include <serialization/ObjectNode.h>
#include <istream>
#include <string>

namespace ce {

class DataParser {
    public:
        DataParser( std::istream& in );
        ~DataParser() = default;

        DataParser( const DataParser& )             = delete;
        DataParser& operator=( const DataParser& )  = delete;
        DataParser( DataParser&& )                  = delete;
        DataParser& operator=( DataParser&& )       = delete;

        void parse();

        const ObjectNode::node_map&     getNodes() const;
        /// \brief Get the specific node.
        ObjectNode::node_ptr            node( const std::string& field ) const;
        /// \brief Check if the parser encountered the specific object.
        bool                            contains( const std::string& field ) const;

    private:
        // METHODS
        // handles node assignment on file level
        ObjectNode::node_ptr assignment_();
        // traverses into a node's description
        // adds all described fields to the node
        // including nested nodes ( this function is recursive )
        // returns a node, containing all fields and nested nodes
        ObjectNode::node_ptr objectTraversal_( const std::string& parent );

        // FIELDS
        ObjectNode::node_map    nodeMap_;
        TokenStream             tstream_;
        std::istream&           in_;

};

}

#endif // DATAPARSER_H
