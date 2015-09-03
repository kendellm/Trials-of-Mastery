#ifndef SLLIST_H
#define SLLIST_H

#include <iostream>
using namespace std;

// forward declaration
template<typename Type> class SLLIter;

// class SLList
template<typename Type> class SLList
{
private:
	// the iterator is the list’s friend
	friend class SLLIter<Type>;
	unsigned int m_unSize;

	struct Node
	{
		Type element;
		Node * next;
	};
	Node * head;

	void RecursiveCopy(Node *newItem);

public:

	SLList();
	~SLList();
	SLList(const SLList<Type>& that);
	SLList<Type>& operator=(const SLList<Type>& that);
	void addHead(const Type &v);
	void clear();
	void insert(SLLIter<Type>& index, const Type& v);
	void remove(SLLIter<Type>& index);
	unsigned int size() const { return m_unSize; }

};

// class SLLIter
template<typename Type> class SLLIter
{
	// the list is the iterator’s friend
	friend class SLList<Type>;
	typename SLList<Type>::Node *prev;
	typename SLList<Type>::Node *curr;
	typename SLList<Type> * mylist;
public:
	SLLIter(SLList<Type>& listToIterate);
	void begin();
	bool end() const;
	SLLIter<Type>& operator++();
	Type& current() const;
};

template<typename Type>
SLList<Type>::SLList()
{
	head = nullptr;
	m_unSize = 0;
}

template<typename Type>
SLList<Type>::~SLList()
{
	while (head != nullptr)
	{
		Node * temp = head;
		head = head->next;
		delete temp;
	}
}

template<typename Type>
SLList<Type>::SLList(const SLList<Type>& that)
{
	head = nullptr;
	operator=(that);
}

template<typename Type>
SLList<Type>& SLList<Type>::operator=(const SLList<Type>& that)
{
	if (this != &that)
	{
		this->clear();
		RecursiveCopy(that.head);

	}
	return *this;
}

template<typename Type>
void SLList<Type>::addHead(const Type &v)
{
	Node *temp = new Node;
	temp->element = v;
	temp->next = head;
	head = temp;
	++m_unSize;
}

template<typename Type>
void SLList<Type>::clear()
{
	while (head != nullptr)
	{
		Node * temp = head;
		head = head->next;
		delete temp;
	}
	m_unSize = 0;
}

template<typename Type>
void SLList<Type>::insert(SLLIter<Type>& index, const Type& v)
{
	if (!head)
		return;

	if (index.prev == nullptr)
	{
		addHead(v);
		index.curr = head;
		return;
	}

	Node *temp = new Node;
	temp->element = v;
	index.prev->next = temp;
	temp->next = index.curr;
	index.curr = temp;
	++m_unSize;
}

template <typename Type>
void SLList<Type>::remove(SLLIter<Type>& index)
{

	if (!head)
		return;

	if (index.prev == nullptr) // remove the first item
	{
		head = index.curr->next;
		delete index.curr;
		index.curr = head;
		--m_unSize;
		return;
	}

	index.prev->next = index.curr->next;
	delete index.curr;
	index.curr = index.prev->next;
	--m_unSize;

}

template<typename Type>
void SLList<Type>::RecursiveCopy(Node *newItem)
{
	if (newItem == nullptr)
		return;

	RecursiveCopy(newItem->next);

	addHead(newItem->element);
}


// Iter Methods //////////////////////////////////////////////////////////////
template<typename Type>
SLLIter<Type>::SLLIter(SLList<Type>& listToIterate)
{
	curr = prev = nullptr;
	mylist = &listToIterate;
}

template<typename Type>
void SLLIter<Type>::begin()
{
	prev = nullptr;
	curr = mylist->head;
}

template <typename Type>
bool SLLIter<Type>::end() const
{
	if (nullptr == curr)
		return true;
	else
		return false;
}

template <typename Type>
SLLIter<Type>& SLLIter<Type>::operator++()
{
	if (curr == nullptr)
		return *this;

	prev = curr;
	curr = curr->next;
	return *this;
}

template<typename Type>
Type& SLLIter<Type>::current() const
{
	return curr->element;
}


#endif