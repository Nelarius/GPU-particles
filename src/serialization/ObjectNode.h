#ifndef OBJECTNODE_H_INCLUDED
#define OBJECTNODE_H_INCLUDED

#include <memory>
#include <map>
#include <string>

namespace ce {

/// \brief A node in an object hierarchy.
/// The ObjectNode can store numbers, strings, or other ObjectNodes.
class ObjectNode {
    public:
        typedef std::map<const std::string, float>                  number_map;
        typedef std::map<const std::string, const std::string>      string_map;
        typedef std::shared_ptr<ce::ObjectNode>                     node_ptr;
        typedef std::map<const std::string, node_ptr>               node_map;
        typedef std::map<const std::string, node_ptr>::iterator     node_iterator;

        explicit ObjectNode( const std::string& name );
        ~ObjectNode() = default;

        void addNode( const std::string&, node_ptr node );
        void addNumber( const std::string&, double value );
        void addString( const std::string&, const std::string& );

        /// \brief Get the number value of the field.
        float               number( const std::string& field ) const;
        /// \brief Get the string value of the field.
        const std::string&  string( const std::string& field ) const;
        /// \brief Get the node pointer of the field.
        node_ptr            node( const std::string& field ) const;

        /// \brief Check to see if the field is a number
        bool isNumber( const std::string& field ) const;

        /// \brief Check to see if the field is a string
        bool isString( const std::string& field ) const;

        /// \brief Check to see if the field is an ObjectNode
        bool isObjectNode( const std::string& field ) const;

        /// \brief Check whether the node contains the name.
        bool contains( const std::string& name ) const;

        /// \brief Get the name of this node.
        const std::string& name() const;

        const number_map&   getNumbers() const;
        const string_map&   getStrings() const;
        const node_map&     getNodes() const;

    private:
        const std::string   name_;
        number_map          numbers_;
        string_map          strings_;
        node_map            nodes_;

};

}   //namespace ce


#endif // OBJECTNODE_H_INCLUDED
