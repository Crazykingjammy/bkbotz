// Queue.h : 
//////////////////////////////////////////////////////////////////////////
// Author : Francesco Rinaldi
// Created: Many many moons ago
// Purpose: doubly Linked List / Iterator class Implementation
//////////////////////////////////////////////////////////////////////////

#ifndef _LLIST_H_
#define _LLIST_H_

// forward declaration
template <typename Type> class LLIter;

// A Linked List.
template <typename Type>
class Queue
{
	// friend declaration
	friend class LLIter<Type>;

private:

	// Define a Node Structure privately.
	// Only LList and LLIter can use this.
	struct Node
	{
		Type element;
		Node *next;
		Node *prev;
	};

	Node *head; // always keeps track of the beginning
	// of the Linked List
	Node *tail; // always keeps track of the end of the 
	// of the linked list

	// dont forget a counter
	int Max;
public:

	// ctr : create the list as empty initially
	Queue ()
	{
		head = tail = NULL;
		Max = 0;		
	}

	// dtr : destroy any memory list is using
	~Queue ()
	{
		clear ();
	}

	// isEmpty : is list empty? (true/false)
	bool isEmpty () { return (head == NULL); }

	// enqueue : enqueue to beginning of singly linked list.
	void enqueue (const Type &v)
	{


		Node *n = new Node;

		n->element = v;
		n->next = head;
		n->prev = NULL;	
		if (!head)
		{
			
			head = tail = n;

		}
		else
		{
			
			head->prev = n;
			head = n;
		}
		
		Max++;
	}

	// Remove something from the beginnin' o' our list.
	//
	// In:	t	Where to store the data.
	//
	// Out:	t	The value.
	//
	// Return:	false if we're empty.
	template <typename Type>
	bool dequeue(Type &t)
	{
		if (!head)
			// We're empty.
			return false;

		// Give 'em their data back:
		t = tail->element;
		Node *temp = tail;
        tail = tail->prev;
		
		if (tail)
		{
			tail->next = NULL;
			delete temp;
		}
		else
		{
			delete temp;
			head = tail = NULL;
		}

		// One less!
		Max--;
		return true;
	}

	int getMax()
	{
		return Max;
	}

	// clear : destroys all of the nodes.
	//         also called by dtr
	void clear ()
	{
		while (!isEmpty())
		{
			Node *n = head;
			head = head->next;
			delete n;
		}
		tail = NULL;
		Max = 0;
	}
};

// A Linked List Index. (Like Node *i, but encapsulated)
template <typename Type>
class LLIter
{
private:

	// declare a pointer to a Node.
	// use 'typename' since using SRO and template in
	// declaration, as of VC.NET 2003
	typename Queue<Type>::Node *i;

	// the list to walk through
	Queue<Type> *theList;

public:

	// ctr : create the iterator object
	LLIter (Queue<Type> &whichList)
	{
		theList = &whichList;
	}

	// begin : sets the iterator to the head of the list.
	void begin () 
	{
		i = theList->head; 
	}

	// end : tests to see if we hit the end of the list?
	bool end () 
	{ 
		return (i == NULL); 
	}

	// start at end
	void startEnd()
	{
		i = theList->tail;
	}

	// operator++ : set i = i->next
	LLIter<Type> &operator++ ()
	{
		i = i->next;
		return *this;
	}

	// operator-- : set i = i->prev
	LLIter<Type> &operator--()
	{
		i = i->prev;
		return *this;
	}

	// current : return the current item in the list
	//           (i->element)
	Type &current ()
	{
		return i->element;
	}
};

#endif