// MyMap.h

// Skeleton for the MyMap class template.  You must implement the first six
// member functions.

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
			else
				insert(key, value);
		}
		
		// RETURN A POINTER TO CONST ValueType (unmodifiable ValueType)
		const ValueType* find(const KeyType& key) const;

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

		// INSERT INTO TREE
		void insert(KeyType key, ValueType value)
		{
			// TODO INSERT IN BST TREE
		}

};
