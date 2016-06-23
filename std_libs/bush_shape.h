//Created: 06-02-2014
//Modified: Mon 10 Feb 2014 14:51:14 CET
//Author: Jonas R. Glesaaen (jonas@glesaaen.com)

/*
 * Class Descriptions:
 *
 * The implementation of a "bush" structure, which is tree-like, but is not a recursive structure. The bush has a "root" and
 * "branches" (nodes), but the branches themselves cannot branch further. The nodes of the Bush_Shape "tree" has type "Bush_Node",
 * which itself has two members. An ID of type int, and a tempated value. The node-structure works so that:
 * 	- Comparison operators only compare the ID
 * 	- Modifier operators only affect the node's value.
 *
 * If a node is added to the bush without a specified ID, it is given a unique one, while if it is pushed back with a specified
 * ID, it is checked for uniqueness:
 * 	- The ID should be unique
 * 	- If the ID is not unique, the values must be the same
 *
 * When merging two bushes (B merged with A), then B's root is added as a node to A, and all nodes of B will have B's root added
 * to them ( node += root), before being added to A's nodes. This functionality might be extended later if the need is there.
 *
 */

#ifndef BUSH_SHAPE_H
#define BUSH_SHAPE_H

#include<utility>
#include<fstream>

#include<set>

#include"error.h"

template <class Type>
struct Bush_Node{
	int id;
	Type val;

	Bush_Node() {};
	Bush_Node(const Type &value) : val(value) {};
	Bush_Node(Type &&value) : val(std::move(value)) {};
	Bush_Node(int idd, const Type &value) : id(idd), val(value) {};
	Bush_Node(int idd, Type &&value) : id(idd), val(std::move(value)) {};
	Bush_Node(const Bush_Node &n) : id(n.id), val(n.val) {};
	Bush_Node(Bush_Node &&n) : id(std::move(n.id)), val(std::move(n.val)) {};
	~Bush_Node() {};

	Bush_Node& operator=(const Bush_Node &n) {id = n.id; val = n.val; return *this;};
	Bush_Node& operator=(Bush_Node &&n) {std::swap(id,n.id); std::swap(val,n.val); return *this;};

	/* Comparison operators only compare IDs */
	bool operator<(const Bush_Node &n) const {return id < n.id;};
	bool operator==(const Bush_Node &n) const {return id == n.id;};

	/* Modification operators only modify values */
	Bush_Node operator+(const Bush_Node &n) const {Bush_Node res(*this); res.val += n.val; return res;};
	Bush_Node& operator+=(const Bush_Node &n) {val += n.val; return *this;};
};

template <class Type>
struct Bush_Shape{
	Bush_Node<Type> root;
	std::set< Bush_Node<Type> > nodes;

	Bush_Shape() {};
	Bush_Shape(const Type &r) : root(0,r) {};
	Bush_Shape(Type &&r) : root(0,std::move(r)) {};
	Bush_Shape(int id, const Type &r) : root(id,r) {};
	Bush_Shape(int id, Type &&r) : root(id,std::move(r)) {};
	Bush_Shape(const Bush_Shape &b) : root(b.root), nodes(b.nodes) {};
	Bush_Shape(Bush_Shape &&b) : root(std::move(b.root)), nodes(std::move(b.nodes)) {};
	~Bush_Shape() {};

	Bush_Shape& operator=(const Bush_Shape &b) {root = b.root; nodes = b.nodes;};
	Bush_Shape& operator=(Bush_Shape &&b) {std::swap(root, b.root); std::swap(nodes, b.nodes);};

	//Check whether the node satisfy the uniqueness criterion
	bool compatible(const Bush_Node<Type> &n) const;
	typename std::set< Bush_Node<Type> >::iterator in_nodes(int id) const {return nodes.find(Bush_Node<Type>(id,Type()));};

	void add_node(const Type &n);
	void add_node(Type &&n);
	void add_node(int,const Type &n);
	void add_node(int,Type &&n);
	void add_node(const Bush_Node<Type>&);
	void add_node(Bush_Node<Type> &&);

	//Merge bush b into this bush
	void merge_bush(const Bush_Shape &b);
};

class Bush_Shape_Error : public Error{
public:
	Bush_Shape_Error(const char *inerr) : Error(inerr,"bush_shape") {};
};

template <class Type>
inline bool Bush_Shape<Type>::compatible(const Bush_Node<Type> &b) const {

	if(root.id == b.id and !(root.val == b.val)){
		return false;
	}

	auto it = nodes.find(b);

	if(it != nodes.end() and !(it->val == b.val)){
		return false;
	}

	return true;
}

template <class Type>
inline void Bush_Shape<Type>::add_node(const Type &n){

	int new_id;
	if(nodes.empty()){
		new_id = root.id + 1;
	}else{
		new_id = nodes.rbegin()->id + 1;
	}
	
	nodes.insert(Bush_Node<Type>(new_id,n));
	//nodes.emplace_hint(nodes.end(),new_id,n); //Wait for C++11
}

template <class Type>
inline void Bush_Shape<Type>::add_node(Type &&n){

	int new_id;
	if(nodes.empty()){
		new_id = root.id + 1;
	}else{
		new_id = nodes.rbegin()->id + 1;
	}
	
	nodes.insert(Bush_Node<Type>(new_id,std::move(n)));
	//nodes.emplace_hint(nodes.end(),new_id,std::move(n)); //Wait for C++11
}

template <class Type>
inline void Bush_Shape<Type>::add_node(int id, const Type &n){

	if( id == root.id and !(n == root.val) ){
		throw Bush_Shape_Error("In function add_node(int,Type):\n"
				       "Adding merging two nodes which aren't compatible.");
	}

	auto it = nodes.find(Bush_Node<Type>(id,Type()));

	if( it != nodes.end() and !(n == it->val) ){
		throw Bush_Shape_Error("In function add_node(int,Type):\n"
				       "Adding merging two nodes which aren't compatible.");
	}
	nodes.insert(Bush_Node<Type>(id,n));
	//nodes.emplace(id,n); //Wait for C++11
}

template <class Type>
inline void Bush_Shape<Type>::add_node(int id, Type &&n){

	if( id == root.id and !(n == root.val) ){
		throw Bush_Shape_Error("In function add_node(int,Type):\n"
				       "Adding merging two nodes which aren't compatible.");
	}


	auto it = nodes.find(Bush_Node<Type>(id,Type()));

	if( it != nodes.end() and !(n == it->val) ){
		throw Bush_Shape_Error("In function add_node(int,Type):\n"
				       "Adding merging two nodes which aren't compatible.");
	}

	nodes.insert(Bush_Node<Type>(id,std::move(n)));
	//nodes.emplace(id,std::move(n)); //Wait for C++11
}

template <class Type>
inline void Bush_Shape<Type>::add_node(const Bush_Node<Type> &n){

	if(!compatible(n)){
		throw Bush_Shape_Error("In function add_node(Bush_Node<Type>):\n"
				       "Adding merging two nodes which aren't compatible.");
	}

	nodes.insert(n);
}

template <class Type>
inline void Bush_Shape<Type>::add_node(Bush_Node<Type> &&n){

	if(!compatible(n)){
		throw Bush_Shape_Error("In function add_node(Bush_Node<Type>):\n"
				       "Adding merging two nodes which aren't compatible.");
	}

	nodes.insert(std::move(n));
}

template <class Type>
inline void Bush_Shape<Type>::merge_bush(const Bush_Shape<Type> &b){

	add_node(b.root);

	for(const Bush_Node<Type> &n : b.nodes){
		add_node(n + b.root);
	}
}

template <class Type>
inline std::ostream& operator<<(std::ostream &os, const Bush_Node<Type> &n){
	os << n.val << " (" << n.id << ")";
	return os;
}

template <class Type>
inline std::ostream& operator<<(std::ostream &os, const Bush_Shape<Type> &b){

	os << "|" << b.root << "| -> {";

	for(const Bush_Node<Type> &n : b.nodes){
		os << n << ",";
	}

	os << "}";

	return os;
}

#endif /* BUSH_SHAPE_H */
