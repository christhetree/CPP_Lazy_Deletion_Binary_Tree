// Author: Christopher Mitcheltree
// Date: Fall 2013

#ifndef LAZY_DELETION_NODE_H
#define LAZY_DELETION_NODE_H

#include <utility>

#define nullptr 0

template <typename Type>
class Lazy_deletion_tree;

template <typename Type>
class Lazy_deletion_node {
	private:
		Type element;
		Lazy_deletion_node<Type> *left_tree;
		Lazy_deletion_node<Type> *right_tree;
		bool erased;

	public:
		Lazy_deletion_node(Type const & = Type());

		Lazy_deletion_node<Type> *left() const;
		Lazy_deletion_node<Type> *right() const;
		Type retrieve() const;
		int height() const;
		std::pair<Type, bool> front() const;
		std::pair<Type, bool> back() const;
		bool member(Type const &) const;

		bool insert(Type const &);
		bool erase(Type const &);
		void clear();
		void clean(Lazy_deletion_node *&);

	friend class Lazy_deletion_tree<Type>;
};

// Constructor: creates a new instance of a lazy deletion node.
template <typename Type>
Lazy_deletion_node<Type>::Lazy_deletion_node(Type const &obj):
element(obj),
left_tree(nullptr),
right_tree(nullptr),
erased(false) {
	// Does nothing.
}

// Left Accessor: returns a pointer to the left subtree of this lazy deletion node.
template <typename Type>
Lazy_deletion_node<Type> * Lazy_deletion_node<Type>::left() const {
	return left_tree;
}

// Right Accessor: returns a pointer to the right subtree of this lazy deletion node.
template <typename Type>
Lazy_deletion_node<Type> * Lazy_deletion_node<Type>::right() const {
	return right_tree;
}

// Retrieve Accessor: returns the object being stored in this lazy deletion node.
template <typename Type>
Type Lazy_deletion_node<Type>::retrieve() const {
	return element;
}

// Height Accessor: returns the height of the subtree with this lazy deletion node as its root.
template <typename Type>
int Lazy_deletion_node<Type>::height() const {
	if(this == nullptr) {
		return -1; // Empty subtree has a height of -1.
	}
	else {
		return(1 + std::max(left()->height(),right()->height())); // Height is the maximum path starting from the root node of the subtree.
	}
}

// Member Accessor: returns if the object is in and unerased in the subtree with this lazy deletion node as its root node.
template <typename Type>
bool Lazy_deletion_node<Type>::member(Type const &obj) const {
	if(this == nullptr) {
		return false;
	}
	else {
		if(retrieve() == obj && !erased) { // Object must not be tagged as erased.
			return true;
		}
		else if(obj < retrieve()) {
			return(left()->member(obj)); // Recursion.
		}
		else {
			return(right()->member(obj)); // Recursion.
		}
	}
}	

// Front Accessor: returns the minimum unerased object in the subtree that has this lazy deletion node as its root node.
template <typename Type>
std::pair<Type, bool> Lazy_deletion_node<Type>::front() const {
	if(this == nullptr) {
		return std::pair<Type, bool>(Type(), false); // Return false if subtree is empty.
	}

	std::pair<Type, bool> result = left()->front(); // First attempt to find minimum object in left subtree of subtree.

	if(result.second) {
		return result;
	}

	if(!erased) {
		return std::pair<Type, bool>(retrieve(), true);
	}

	result = right()->front(); // If minimum object is not in left subtree of subtree, attempt to find it in the right subtree of the subtree.

	if(result.second) {
		return result;
	}

	return std::pair<Type, bool>(Type(), false);
}

// Back Accessor: returns the maximum unerased object in the subtree that has this lazy deletion node as its root node.
// This is a copy of the previous method except with front replaced with back and left and right switched.
template <typename Type>
std::pair<Type, bool> Lazy_deletion_node<Type>::back() const {
	if(this == nullptr) {
		return std::pair<Type, bool>(Type(), false); // Return false if subtree is empty.
	}

	std::pair<Type, bool> result = right()->back(); // First attempt to find maximum object in right subtree of subtree.

	if(result.second) {
		return result;
	}

	if(!erased) {
		return std::pair<Type, bool>(retrieve(), true);
	}

	result = left()->back(); // If maximum object is not in right subtree of subtree, attempt to find it in the left subtree of the subtree.

	if(result.second) {
		return result;
	}

	return std::pair<Type, bool>(Type(), false);
}

// Insert Mutator: inserts an object into the subtree or returns false if the object is already inserted.
template <typename Type>
bool Lazy_deletion_node<Type>::insert(Type const &obj) {
	if(this == nullptr) { // Inserting object into an empty subtree.
		element = obj;
		return true;
	}
	else if(retrieve() == obj) { 
		if(erased) { // Inserting existing object that has been tagged erased.
			erased = false;
			return true;
		}
		else { // Inserting an existing object.
			return false;
		}
	}
	else if(obj < retrieve()) { // Inserting object that is less than the current node.
		if(left() == nullptr) { // Insert object into left leafnode of current node.
			left_tree = new Lazy_deletion_node<Type>(obj);
			return true;
		}
		else {
			return(left()->insert(obj)); // Recursion.
		}
	}
	else { // Inserting object that is greater than the current node.
		if(right() == nullptr) { // Insert object into right leafnode of current node.
			right_tree = new Lazy_deletion_node<Type>(obj);
			return true;
		}
		else {
			return(right()->insert(obj)); // Recursion.
		}
	}
}

// Erase Mutator: erases an object from the subtree, or returns false if the object is already erased.
template <typename Type>
bool Lazy_deletion_node<Type>::erase(Type const &obj) {
	if(this == nullptr) { // Cannot erase from an empty subtree or if reached an empty leaf node.
		return false;
	}
	else if(retrieve() == obj) { // Found object to be erased in the subtree.
		if(!erased) { // Perform erase tagging.
			erased = true;
			return true;
		}
		else { // Cannot erase an already erased object.
			return false;
		}
	}
	else if(obj < retrieve()) { // Object being erased is less than object in the current node.
			return(left()->erase(obj)); // Recursion.
	}
	else { // Object being erased is greater than object in the current node.
		return(right()->erase(obj)); // Recursion.
	}
}

// Clear Mutator: deletes all nodes in the subtree.
template <typename Type>
void Lazy_deletion_node<Type>::clear() {
	if(this == nullptr) {
		return; // Cannot delete a node that is a nullptr (already deleted).
	}
	else {
		if(left() != nullptr) // Check if left subtree of this node is already cleared.
			left()->clear(); // Recursion.
		if(right() != nullptr) // Check if right subtree of this node is already cleared.
			right()->clear(); // Recursion.
		
		delete this;
	}
}

// Clean Mutator: deletes all erased tagged nodes in the subtree with this lazy deletion node as its root node.
template <typename Type>
void Lazy_deletion_node<Type>::clean(Lazy_deletion_node *&ptr_to_this) {
	if(this == nullptr) {
		return; // Cannot clean an empty subtree or a node that is a nullptr (already deleted).
	}
	else if(erased) { // Found node to be cleaned.
		if((right()->front()).second) { 
			element = (right()->front()).first; // If possible replace this node with the front of its right subtree.
			erased = false;
			right()->erase(element); // Erase copied node (front of right subtree).
		}
		else if((left()->back()).second) {
			element = (left()->back()).first; // Else if possible replace this node with the back of its left subtree.
			erased = false;
			left()->erase(element); // Erase copied node (back of left subtree).
		}
		else {
			// Do nothing.
		}
	}
	else {
		// Do nothing;
	}

	left()->clean(left_tree); // Recursion.
	right()->clean(right_tree); // Recursion.

	if(erased) { // If this node has not been replaced by a node from one of the subtrees then delete it.
		ptr_to_this = nullptr;
		delete this;
	}
}

#endif
