// MyMap.h

// Skeleton for the MyMap class template.  You must implement the first six
// member functions.

#include <string>
#include <cctype>
#include <typeinfo>

template<typename KeyType, typename ValueType>
class MyMap
{
	public:
		
		// CONSTRUCTOR
		MyMap(): m_size(0), root(nullptr){}
		
		// DESTRUCTOR
		~MyMap() {clear();}
		
		// CLEAR MAP
		void clear()
		{
			destroyTree(root);
			m_size = 0;
		}
		
		// RETURN SIZE
		int size() const {return m_size;}
		
		// INSERT INTO/MODIFY TREE
		void associate(const KeyType& key, const ValueType& value)
		{
			ValueType* val = find(key);
			if (val != nullptr) // key already exists
				*val = value;
			else // put in a new key
			{
				insert(key, value);
				m_size++;
			}
		}
		
		// RETURN A POINTER TO CONST ValueType (unmodifiable ValueType)
		const ValueType* find(const KeyType& key) const
		{
			Node* nodeToFind = root;
			KeyType modifiedKey = key;
			lowercase(modifiedKey); // lowercase key if it's a string
			if (search(modifiedKey, nodeToFind))
				return nodeToFind->value;
			else
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
			Node* n;
			n->key = key;
			n->value = value;
			n->left = n->right = nullptr;

			// First node in tree
			if (root == nullptr)
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
				} else if (key > curr->key) //greater than current
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
		bool search(KeyType key, Node* &ptr)
		{
			if (ptr == nullptr)
				return false;
			else if (key == ptr->key)
				return true;
			else if (key < ptr->key)
				return search(key, ptr->left);
			else
				return search(key, ptr->right);
		}

		// making key type lowercase if it's a string
		void lowercase(KeyType& key)
		{
			if (typeid(key) == typeid(std::string)) // is the type a string?
			{
				std::string modifiedKey = dynamic_cast<std::string>(key);
				for (int i = 0; i < modifiedKey.size(); i++)
				{
					modifiedKey.at(i) = tolower(modifiedKey.at(i));
				}

				key = modifiedKey;
			}
		}
};
