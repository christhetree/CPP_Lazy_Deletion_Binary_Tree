// Author: Christopher Mitcheltree
// Date: Fall 2013

#ifndef LAZY_TREE_H
#define LAZY_TREE_H

#include <queue>
#include "Lazy_deletion_node.h"

#define nullptr 0

template <typename Type>
class Lazy_deletion_tree {
	private:
		Lazy_deletion_node<Type> *root_node;
		int count;

	public:
		Lazy_deletion_tree();
		~Lazy_deletion_tree();

		bool empty() const;
		int size() const;
		int height() const;
		Type front() const;
		Type back() const;
		bool member( Type const & ) const;
		void breadth_first_traversal() const;

		bool insert( Type const & );
		bool erase( Type const & );
		void clean();
		void clear();
};

// Constructor: creates a new instance of the lazy deletion tree data structure.
template <typename Type>
Lazy_deletion_tree<Type>::Lazy_deletion_tree():
root_node(nullptr),
count(0) {
	// Only initializes member variables
}

// Deconstructor: returns any memory that was allocated by the lazy deletion tree constructor.
template <typename Type>
Lazy_deletion_tree<Type>::~Lazy_deletion_tree() {
	clear(); // Calling clear on the root node will delete all nodes of the lazy deletion tree.
}

// Breadth First Traversal Accessor: performs a breadth first traversal and prints objects in order that they're visited.
template <typename Type>
void Lazy_deletion_tree<Type>::breadth_first_traversal() const {
	if(root_node == nullptr) {
		return;
	}

	// Read up on the STL queue at http://www.cplusplus.com/reference/queue/queue/
	std::queue< Lazy_deletion_node<Type> *> queue;

	queue.push(root_node);

	// Push nodes on the queue, then print object, then pop it off and push on its children. Repeat to display entire tree.
	while(!queue.empty()) {
		Lazy_deletion_node<Type> *current = queue.front();
		
		if(current->erased) {
			std::cout << current->retrieve() << "x "; // Print x next to object if it is marked as erased.
		}
		else {
			std::cout << current->retrieve() << " ";
		}

		queue.pop();
		
		if(current->left() != nullptr) // Check if there is a left child.
			queue.push(current->left());

		if(current->right() != nullptr) // Check if there is a right child.
			queue.push(current->right());
	}
}

// Empty Accessor: returns true if the size of the lazy deletion tree is 0.
template <typename Type>
bool Lazy_deletion_tree<Type>::empty() const {
	return(size() == 0);
}

// Size Accessor: returns how many unerased objects are in the lazy deletion tree.
template <typename Type>
int Lazy_deletion_tree<Type>::size() const {
	return count;
}

// Height Accessor: returns the height of the lazy deletion tree (also including erased nodes).
template <typename Type>
int Lazy_deletion_tree<Type>::height() const {
	return(root_node->height());
}

// Member Accessor: returns true if the object can be found in the lazy deletion tree and is not erased.
template <typename Type>
bool Lazy_deletion_tree<Type>::member(Type const &obj) const {
	return(root_node->member(obj));
}

// Front Accessor: returns the minimum unerased element of the lazy deletion tree, false if there is none.
template <typename Type>
Type Lazy_deletion_tree<Type>::front() const {
	if(empty()) { 
		throw underflow(); // Throw underflow incase the tree is empty.
	}
	else {
		return((root_node->front()).first);
	}
}

// Back Accessor: returns the maximum unerased element of the lazy deletion tree, false if there is none.
template <typename Type>
Type Lazy_deletion_tree<Type>::back() const {
	if(empty()) {
		throw underflow(); // Throw underflow incase the tree is empty.
	}
	else {
		return((root_node->back()).first);
	}
}

// Insert Mutator: inserts an object into the lazy deletion tree or returns false if the object is already inserted.
template <typename Type>
bool Lazy_deletion_tree<Type>::insert(Type const &obj) {
	if(root_node == nullptr) { 
		++count;
		root_node = new Lazy_deletion_node<Type>(obj); // If no root node exists yet, create a new one.
		return true;
	}
	else if(root_node->insert(obj)) {
		++count;
		return true;
	}
	else {
		return false; // Object is already in the tree.
	}
}

// Erase Mutator: erases an object from the lazy deletion tree, or returns false if the object is already erased.
template <typename Type>
bool Lazy_deletion_tree<Type>::erase(Type const &obj) {
	if(root_node == nullptr) {
		return false; // Tree is empty, therefore nothing can be erased.
	}
	else if(root_node->erase(obj)) {
		--count;
		return true;
	}
	else {
		return false; // Object has already been erased.
	}
}

// Clear Mutator: deletes all the nodes in the lazy deletion tree.
template <typename Type>
void Lazy_deletion_tree<Type>::clear() {
	if(root_node == nullptr) {
		return;
	}
	else {
		root_node->clear(); // Deletes all nodes in the tree.
		root_node = nullptr; // Must set the root_node to a nullptr after deleting all nodes.
		count = 0;
	}
}

// Clean Mutator: deletes all objects of the lazy deletion tree that have been tagged as erased.
template <typename Type>
void Lazy_deletion_tree<Type>::clean() {
	root_node->clean(root_node);
}

#endif
