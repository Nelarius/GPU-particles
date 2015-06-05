#include <serialization/ObjectNode.h>
#include <utils/Assert.h>
#include <exception>
#include <cstdlib>  // for std::exit()
#include <iostream>

using ce::ObjectNode;

ObjectNode::ObjectNode( const std::string& name )
    :   name_( name )
        {}

void ObjectNode::addNode( const std::string& name, node_ptr node ) {
    nodes_.insert( std::make_pair( name, node ) );
}

void ObjectNode::addNumber( const std::string& name, double value ) {
    numbers_.insert( std::make_pair( name, value) );
}

void ObjectNode::addString( const std::string& name, const std::string& str ) {
    strings_.insert( std::make_pair( name, str ) );
}

bool ObjectNode::isNumber( const std::string& name ) const {
    auto it = numbers_.find( name );
    if ( it != numbers_.end() ) {
        return true;
    }
    return false;
}

bool ObjectNode::isString( const std::string& name ) const {
    auto it = strings_.find( name );
    if ( it != strings_. end() ) {
        return true;
    }
    return false;
}

bool ObjectNode::isObjectNode( const std::string& name ) const {
    auto it = nodes_.find( name );
    if ( it != nodes_.end() ) {
        return true;
    }
    return false;
}

bool ObjectNode::contains( const std::string& name ) const {
    if ( isNumber( name ) || isObjectNode( name ) || isString( name ) ) {
        return true;
    }
    return false;
}

float ObjectNode::number( const std::string& field ) const {
    #ifdef DEBUG
    //ASSERT( isNumber(field), std::sprintf("error: no such number %s", field.c_str()) );
    #endif // DEBUG
    return numbers_.at( field );
}

const std::string& ObjectNode::string( const std::string& field ) const {
    #ifdef DEBUG
    //ASSERT( isString(field), std::sprintf("error: no such string %s", field.c_str()) );
    #endif // DEBUG
    return strings_.at( field );
}

ObjectNode::node_ptr ObjectNode::node( const std::string& field ) const {
    #ifdef DEBUG
    //ASSERT( isObjectNode( field ), std::sprintf("error: no such node %s", field.c_str()) );
    #endif
    return nodes_.at( field );
}

const std::string& ObjectNode::name() const {
    return name_;
}

const ObjectNode::number_map& ObjectNode::getNumbers() const {
    return numbers_;
}

const ObjectNode::string_map& ObjectNode::getStrings() const {
    return strings_;
}

const ObjectNode::node_map& ObjectNode::getNodes() const {
    return nodes_;
}
