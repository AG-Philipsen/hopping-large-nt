//Created: 28-11-2013
//Modified: Thu 24 Jul 2014 13:50:56 CEST
//Author: Jonas R. Glesaaen (jonas@glesaaen.com)

#ifndef TREE_SHAPE_PTR_H
#define TREE_SHAPE_PTR_H

#include<iostream>
#include<list>

#include"print_lib.h"

/*
 * This library saves a sort of BTree, which well, isn't a tree. Every node can
 * contain a value (but doesn't need to), it can also have any number of children
 * and a link to the right. If the node it a link of another node, they share
 * children, and it therefore stores none itself. When adding children to a link
 * node, one navigates to the "left" until one reaches the first link, and adds
 * to that node's child.
 *
 * The class has < and == operators which compares the stucture of two trees only,
 * not their contents. It also has an overloaded stream-operator. The < comparison 
 * operators compares number of children first, then number of links. It then
 * recursively compares the children nodes (no need to compare the link nodes
 * as they contain the same sub-structure as the left-most element)
 */

template <class Type = char>
struct Tree_Shape_Ptr{
	const Type *val;
	Tree_Shape_Ptr *parent;
	std::list<Tree_Shape_Ptr*> children;

	Tree_Shape_Ptr *link_right;
	Tree_Shape_Ptr *link_left;

	Tree_Shape_Ptr() : parent(nullptr), val(nullptr), link_right(nullptr), link_left(nullptr) {};
	Tree_Shape_Ptr(const Type &v) : parent(nullptr), val(&v), link_right(nullptr), link_left(nullptr) {};
	Tree_Shape_Ptr(const Tree_Shape_Ptr &);
	Tree_Shape_Ptr(Tree_Shape_Ptr&&);
	~Tree_Shape_Ptr();

	void cleanup();

	/* Iterators to iterate through the node's children */

	typedef typename std::list<Tree_Shape_Ptr*>::iterator child_iterator;
	typedef typename std::list<Tree_Shape_Ptr*>::const_iterator child_const_iterator;

	child_iterator begin();
	child_iterator end();
	child_const_iterator cbegin() const;
	child_const_iterator cend() const;

	Tree_Shape_Ptr* front();
	Tree_Shape_Ptr* back();

	/* Read-functions which returns the values of the left-most node */

	Tree_Shape_Ptr* get_parent();
	int link_size() const;
	int child_size() const;

	/* Children and Link insertions */

	Tree_Shape_Ptr* insert_child();
	Tree_Shape_Ptr* insert_child(const Type&);
	Tree_Shape_Ptr* insert_child(const Tree_Shape_Ptr &);

	Tree_Shape_Ptr* insert_link();
	Tree_Shape_Ptr* insert_link(const Type&);
	Tree_Shape_Ptr* insert_link(const Tree_Shape_Ptr &);

	/* Comparison operators */

	bool operator==(const Tree_Shape_Ptr&) const;
	bool operator<(const Tree_Shape_Ptr&) const;

	/* Copy- and move assignment operators */

	Tree_Shape_Ptr& operator=(const Tree_Shape_Ptr&);
	Tree_Shape_Ptr& operator=(Tree_Shape_Ptr&&);

	/* Stream printing and stream manipulators */

	static int& print_shape_only() {static int i = std::ios_base::xalloc(); return i;};
	static std::ios_base& shape_only(std::ios_base &io) {io.iword(print_shape_only()) = 1; return io;};
	static std::ios_base& shape_and_val(std::ios_base &io) {io.iword(print_shape_only()) = 0; return io;};
};

/// The copy constructor calls itself recursively at the creation of children and links

template <class Type>
inline Tree_Shape_Ptr<Type>::Tree_Shape_Ptr(const Tree_Shape_Ptr<Type> &l) : val(l.val), parent(nullptr), link_right(nullptr), link_left(nullptr) {

	if(l.link_right != nullptr){
		link_right = new Tree_Shape_Ptr<Type>(*l.link_right);
		link_right->link_left = this;
	}

	for(typename std::list< Tree_Shape_Ptr<Type>* >::const_iterator cit = l.children.begin(); cit != l.children.end(); cit++){
		children.push_back(new Tree_Shape_Ptr<Type>(**cit));
		children.back()->parent = this;
	}

}

/// The move constructor. Assigns 'this' the memory of rhs, and removes rhs' memory

template <class Type>
inline Tree_Shape_Ptr<Type>::Tree_Shape_Ptr(Tree_Shape_Ptr<Type> &&rhs) : val(rhs.val), parent(rhs.parent), link_right(rhs.link_right), link_left(rhs.link_left),
								children(std::move(rhs.children)) {
	rhs.val 	= nullptr;
	rhs.parent 	= nullptr;
	rhs.link_right 	= nullptr;
	rhs.link_left 	= nullptr;

}


/// The destructor must also call itself recursively, deleting links and children

template <class Type>
inline Tree_Shape_Ptr<Type>::~Tree_Shape_Ptr(){

	val = nullptr;

	if(link_right != nullptr){
		delete link_right;
	}

	parent=nullptr; 

	for(Tree_Shape_Ptr *s : children){ 
		delete s;
	}
}

template <class Type>
inline void Tree_Shape_Ptr<Type>::cleanup(){

	val = nullptr;

	if(link_right != nullptr){
		delete link_right;
	}

	link_right = nullptr;
	parent = nullptr;

	for(Tree_Shape_Ptr<Type> *s : children){
		delete s;
	}

	children.clear();
}

template <class Type>
inline Tree_Shape_Ptr<Type>* Tree_Shape_Ptr<Type>::front(){

	Tree_Shape_Ptr* tree = this;
	while(tree->link_left != nullptr){
		tree = tree->link_left;
	}

	return tree->children.front();

}

template <class Type>
inline Tree_Shape_Ptr<Type>* Tree_Shape_Ptr<Type>::back(){

	Tree_Shape_Ptr* tree = this;
	while(tree->link_left != nullptr){
		tree = tree->link_left;
	}

	return tree->children.back();

}

/* -- The following functions returns parent/children of the left-most node -- */

template <class Type>
inline Tree_Shape_Ptr<Type>* Tree_Shape_Ptr<Type>::get_parent(){

	Tree_Shape_Ptr* tree = this;
	while(tree->link_left != nullptr){
		tree = tree->link_left;
	}

	return tree->parent;
}

template <class Type>
inline int Tree_Shape_Ptr<Type>::link_size() const{
	if(link_right == nullptr){
		return 1;
	}else{
		return link_right->link_size()+1;
	}
}

template <class Type>
inline int Tree_Shape_Ptr<Type>::child_size() const{

	const Tree_Shape_Ptr<Type>* tree = this;
	while(tree->link_left != nullptr){
		tree = tree->link_left;
	}

	return tree->children.size();
}

template <class Type>
inline typename Tree_Shape_Ptr<Type>::child_iterator Tree_Shape_Ptr<Type>::begin(){

	Tree_Shape_Ptr* tree = this;
	while(tree->link_left != nullptr){
		tree = tree->link_left;
	}

	return tree->children.begin();
}

template <class Type>
inline typename Tree_Shape_Ptr<Type>::child_iterator Tree_Shape_Ptr<Type>::end(){

	Tree_Shape_Ptr* tree = this;
	while(tree->link_left != nullptr){
		tree = tree->link_left;
	}

	return tree->children.end();
}

template <class Type>
inline typename Tree_Shape_Ptr<Type>::child_const_iterator Tree_Shape_Ptr<Type>::cbegin() const {

	const Tree_Shape_Ptr* tree = this;
	while(tree->link_left != nullptr){
		tree = tree->link_left;
	}

	return tree->children.cbegin();
}

template <class Type>
inline typename Tree_Shape_Ptr<Type>::child_const_iterator Tree_Shape_Ptr<Type>::cend() const {

	const Tree_Shape_Ptr* tree = this;
	while(tree->link_left != nullptr){
		tree = tree->link_left;
	}

	return tree->children.cend();
}

/* -- Adding children also happens at the left-most node -- */

template <class Type>
inline Tree_Shape_Ptr<Type>* Tree_Shape_Ptr<Type>::insert_child(){
	Tree_Shape_Ptr* tree = this;
	while(tree->link_left != nullptr){
		tree = tree->link_left;
	}

	tree->children.push_back(new Tree_Shape_Ptr<Type>);
	tree->children.back()->parent = this;
	return tree->children.back();
}

template <class Type>
inline Tree_Shape_Ptr<Type>* Tree_Shape_Ptr<Type>::insert_child(const Type &v){
	Tree_Shape_Ptr* tree = this;
	while(tree->link_left != nullptr){
		tree = tree->link_left;
	}

	tree->children.push_back(new Tree_Shape_Ptr<Type>(v));
	tree->children.back()->parent = this;
	return tree->children.back();
}


template <class Type>
inline Tree_Shape_Ptr<Type>* Tree_Shape_Ptr<Type>::insert_child(const Tree_Shape_Ptr<Type> &c){

	Tree_Shape_Ptr* tree = this;
	while(tree->link_left != nullptr){
		tree = tree->link_left;
	}

	tree->children.push_back(new Tree_Shape_Ptr<Type>(c));
	tree->children.back()->parent = this;
	return tree->children.back();
}

/* -- Similarly links are added to the right-most node -- */

template <class Type>
inline Tree_Shape_Ptr<Type>* Tree_Shape_Ptr<Type>::insert_link(){

	Tree_Shape_Ptr<Type>* tree = this;
	while(tree->link_right != nullptr){
		tree = tree->link_right;
	}

	tree->link_right = new Tree_Shape_Ptr<Type>;
	tree->link_right->link_left = this;
	return tree->link_right;
}

template <class Type>
inline Tree_Shape_Ptr<Type>* Tree_Shape_Ptr<Type>::insert_link(const Type &v){
	
	Tree_Shape_Ptr<Type>* tree = this;
	while(tree->link_right != nullptr){
		tree = tree->link_right;
	}

	tree->link_right = new Tree_Shape_Ptr<Type>(v);
	tree->link_right->link_left = this;
	return tree->link_right;
}

template <class Type>
inline Tree_Shape_Ptr<Type>* Tree_Shape_Ptr<Type>::insert_link(const Tree_Shape_Ptr<Type> &l){

	Tree_Shape_Ptr<Type>* tree = this;
	while(tree->link_right != nullptr){
		tree = tree->link_right;
	}

	tree->link_right = new Tree_Shape_Ptr<Type>(l);
	tree->link_right->link_left = this;
	return tree->link_right;

}

/// The == operator checks if the node has the same number of children, and the same number of
/// links. It then recursively checks the children.

template <class Type>
inline bool Tree_Shape_Ptr<Type>::operator==(const Tree_Shape_Ptr<Type> &s) const {

	if(children.size() != s.children.size()){
		return false;
	}

	if(link_size() != s.link_size()){
		return false;
	}

	typename std::list< Tree_Shape_Ptr<Type>* >::const_iterator it1 =   children.begin();
	typename std::list< Tree_Shape_Ptr<Type>* >::const_iterator it2 = s.children.begin();

	while(it1 != children.end()){

		if( !(**it1 == **it2) ){
			return false;
		}

		++it1; ++it2;
	}

	return true;
}

/// The < operator works similarly to the == operator, only checking shape.

template <class Type>
inline bool Tree_Shape_Ptr<Type>::operator<(const Tree_Shape_Ptr<Type> &s) const {

	if(children.size() != s.children.size()){
		return children.size() < s.children.size();
	}

	int ls = link_size(), sls = s.link_size();

	if(ls != sls){
		return ls < sls;
	}

	typename std::list< Tree_Shape_Ptr<Type>* >::const_iterator it1 =   children.begin();
	typename std::list< Tree_Shape_Ptr<Type>* >::const_iterator it2 = s.children.begin();

	while(it1 != children.end()){

		if( !(**it1 == **it2) ){
			return (**it1 < **it2);
		}

		++it1; ++it2;
	}

	return false;
}

/// The copy- and move-assignment operators

template <class Type>
inline Tree_Shape_Ptr<Type>& Tree_Shape_Ptr<Type>::operator=(const Tree_Shape_Ptr<Type> &rhs){

	cleanup();

	val = rhs.val;

	if(rhs.link_right == nullptr){
		link_right = nullptr;
	}else{
		link_right = new Tree_Shape_Ptr<Type>;
		*link_right = *rhs.link_right;
		link_right->link_left = this;
	}

	for(typename std::list< Tree_Shape_Ptr<Type>* >::const_iterator cit = rhs.children.begin(); cit != rhs.children.end(); cit++){
		children.push_back(new Tree_Shape_Ptr<Type>);
		*children.back() = **cit;
		children.back()->parent = this;
	}

	return *this;
}

template <class Type>
inline Tree_Shape_Ptr<Type>& Tree_Shape_Ptr<Type>::operator=(Tree_Shape_Ptr<Type> &&rhs){

	std::swap(val, 		rhs.val);
	std::swap(parent, 	rhs.parent);
	std::swap(link_right, 	rhs.link_right);
	std::swap(link_left, 	rhs.link_left);
	std::swap(children, 	rhs.children);

	return *this;
}

/// Prints links and children recursively, links first then children. Links are printed with
/// a '-' in front, and children are wrapped in paranthesis. If the string has been 
/// manipulated using Tree_Shape_Ptr<Type>::shape_only, or it has not been given a value,
/// it prints a '*' instead.

template <class Type>
inline std::ostream& operator<<(std::ostream &os, const Tree_Shape_Ptr<Type> &s){

	if(s.link_left != nullptr){
		os << " - ";
	}
	
	if(s.val == nullptr or os.iword(Tree_Shape_Ptr<Type>::print_shape_only()) == 1){
		os << "*";
	}else{
		os << *(s.val);
	}

	if(s.link_right != nullptr){
		os << *(s.link_right);
	}

	if(s.link_left != nullptr){
		return os;
	}

	if(!s.children.empty()){
		os << " ( ";
		for(const Tree_Shape_Ptr<Type> *c : s.children){
			os << *c;
		}
		os << ")";
	}

	os << " ";

	return os;
}

#endif
