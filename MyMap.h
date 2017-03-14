// MyMap.h

// Skeleton for the MyMap class template.  You must implement the first six
// member functions.

#ifndef MYMAP
#define MYMAP

// Packages to include
#include <cctype>
#include <typeinfo>
#include <iostream>
#include <string>
using namespace std;

// Templated class
template<typename KeyType, typename ValueType>

class MyMap
{
	public:
		// CONSTRUCTOR
		MyMap():m_size(0), root(nullptr){}
		
		// DESTRUCTOR
		~MyMap() {clear(); delete root;}
		
		// CLEAR MAP
		void clear()
		{
			destroyTree(root);
			root = nullptr;
			m_size = 0;
		}
		
		// RETURN SIZE
		int size() const {return m_size;}
		
		// INSERT INTO/MODIFY TREE
		void associate(const KeyType& key, const ValueType& value)
		{
			ValueType* val = find(key);
			if (val != nullptr) // key already exists
			{
				*val = value;
			} else // put in a new key
			{
				insert(key, value);
				m_size++;
			}
		}
		
		// RETURN A POINTER TO CONST ValueType (unmodifiable ValueType)
		const ValueType* find(const KeyType& key) const
		{
			Node* nodeToFind = root;
			if (search(key, nodeToFind))
			{
				ValueType* val = &nodeToFind->value;
				return val;
			} else
				return nullptr;
		}

		// RETURN A POINTER TO MODIFIABLE ValueType (modifiable ValueType)
		ValueType* find(const KeyType& key)
		{
			return const_cast<ValueType*>(const_cast<const MyMap*>(this)->find(key));
		}

		 // C++11 syntax for preventing copying and assignment
		MyMap(const MyMap&) = delete;
		MyMap& operator=(const MyMap&) = delete;

	private:
		int m_size;
		
		// NODE DEFINITION
		struct Node {
			Node* left;
			Node* right;
			KeyType key;
			ValueType value;
		};

		// NODE ROOT
		Node* root;

		// DESTROY TREE
		void destroyTree(Node* leaf)
		{
			if (leaf != nullptr)
			{
				destroyTree(leaf->left);
				destroyTree(leaf->right);
				delete leaf;
			}
		}

		// INSERT INTO TREE (GUARANTEED TO NOT ALREADY EXIST)
		void insert(KeyType key, ValueType value)
		{
			// Node
			Node* n = new Node;
			n->key = key;
			n->value = value;
			n->left = nullptr;
			n->right = nullptr;

			// First node in tree
			if (size() == 0)
			{
				root = n;
				return;
			}

			Node* curr = root;

			for (;;)
			{
				if (key == curr->key) return; // this case should not occur, repeats

				if (key < curr->key) // less than current
				{
					if (curr->left != nullptr) // look deeper in left
						curr = curr->left;
					else
					{
						curr->left = n;
						return;
					}
				} else if (curr->key < key) //greater than current
				{
					if (curr->right != nullptr) // look deeper in right
						curr = curr->right;
					else
					{
						curr->right = n;
						return;
					}
				}
			}
		}

		// SEARCH FOR CERTAIN KEY
		bool search(const KeyType key, Node*& ptr) const
		{
			if (ptr == nullptr) // reached null without
				return false;
			else if (key == ptr->key)
				return true;
			else if (key < ptr->key)
			{
				ptr = ptr->left;
				return search(key, ptr);
			}
			else
			{
				ptr = ptr->right;
				return search(key, ptr);
			}
		}
};




/*
// WORKING TEST CODE (ILLEGAL TO TURN IN)

#include <map>  // YOU MUST NOT USE THIS HEADER IN CODE YOU TURN IN

// In accordance with the spec, YOU MUST NOT TURN IN THIS CLASS TEMPLATE,
// since you are not allowed to use any STL associative containers, and
// this implementation uses std::map.

// This code is deliberately obfuscated.

// If you can not get your own MyMap class template working, you may use
// this one during development in order to let you proceed with implementing
// the other classes for this project; you can then go back to working on
// fixing your own MyMap class template.

template <typename KeyType, typename ValueType>
class MyMap
{
public:
	MyMap(const MyMap &other) = delete;
	MyMap &operator=(const MyMap &other) = delete;
typedef KeyType O;using O1=int;void clear(){l01.clear();}
using l0=ValueType;MyMap(){}using l10=O const;O1 size()
const{return l01.size();}using ll0=l0 const;using Const=
MyMap<O,l0>;void associate(l10&Using,ll0&first){l01[
Using]=first;}using l1=std::map<O,l0>;using l00=Const
const;ll0*find(l10&l11)const{auto first=l01.find(l11);
return(first!=l01.end()?&first->second:0);}l0*find(l10&
l01){return(l0*)(*(l00*)(this)).find(l01);}private:l1
l01;
};
*/



#endif // MYMAP
