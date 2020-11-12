#pragma once
#include <iostream>
#include <string>

template <class T> class HeapNode
{
private:
	HeapNode<T>* prev;
	HeapNode<T>* next;
	HeapNode<T>* child;
	HeapNode<T>* parent;
	T value;
	int degree;
	template <class T> friend class FibonacciHeap;
public:
	HeapNode<T>* getPrev() { return prev; }
	HeapNode<T>* getNext() { return next; }
	HeapNode<T>* getChild() { return child; }
	HeapNode<T>* getParent() { return parent; }
	T getValue() { return value; }

	bool hasChildren() { return child; }
	bool hasParent() { return parent; }
};

template <class T> class FibonacciHeap
{
protected:
	HeapNode<T>* heap;
	int _size = 0;
public:
	FibonacciHeap()
	{
		heap = _empty();
	}

	virtual ~FibonacciHeap()
	{
		if (heap)
		{
			_deleteAll(heap);
		}
	}

	int size() { return _size; }

	HeapNode<T>* insert(T value)
	{
		HeapNode<T>* result = _createHeapNode(value);
		heap = _merge(heap, result);
		_size++;
		return result;
	}

	void merge(FibonacciHeap& other)
	{
		heap = _merge(heap, other.heap);
		other.heap = _empty();
	}

	bool isEmpty()
	{
		return heap == nullptr;
	}

	T getMinimum()
	{
		return heap->value;
	}

	T removeMinimum()
	{
		HeapNode<T>* old = heap;
		heap = _removeMinimum(heap);
		T value = old->value;
		delete old;
		_size--;
		return value;
	}

	HeapNode<T>* find(T value)
	{
		return _find(heap, value);
	}

private:
	HeapNode<T>* _empty()
	{
		return nullptr;
	}

	HeapNode<T>* _createHeapNode(T value)
	{
		HeapNode<T>* n = new HeapNode<T>;
		n->value = value;
		n->prev = n->next = n;
		n->degree = 0;
		n->child = nullptr;
		n->parent = nullptr;
		return n;
	}

	HeapNode<T>* _merge(HeapNode<T>* a, HeapNode<T>* b)
	{
		if (a == nullptr) return b;
		if (b == nullptr) return a;
		if (a->value > b->value)
		{
			HeapNode<T>* temp = a;
			a = b;
			b = temp;
		}

		HeapNode<T>* aNext = a->next;
		HeapNode<T>* bPrev = b->prev;
		a->next = b;
		b->prev = a;
		aNext->prev = bPrev;
		bPrev->next = aNext;
		return a;
	}

	void _deleteAll(HeapNode<T>* n)
	{
		if (n != nullptr)
		{
			HeapNode<T>* _child = n;
			do
			{
				HeapNode<T>* d = _child;
				_child = _child->next;
				_deleteAll(d->child);
				delete d;
			} while (_child != n);
		}
	}

	void _addChild(HeapNode<T>* parent, HeapNode<T>* child)
	{
		child->prev = child->next = child;
		child->parent = parent;
		parent->degree++;
		parent->child = _merge(parent->child, child);
	}

	void _unParentAll(HeapNode<T>* n)
	{
		if (n == nullptr) return;
		HeapNode<T>* _child = n;
		do
		{
			_child->parent = nullptr;
			_child = _child->next;
		} while (_child != n);
	}

	HeapNode<T>* _removeMinimum(HeapNode<T>* n)
	{
		_unParentAll(n->child);
		if (n->next == n)
		{
			n = n->child;
		}
		else
		{
			n->next->prev = n->prev;
			n->prev->next = n->next;
			n = _merge(n->next, n->child);
		}

		if (n == nullptr) return n;
		HeapNode<T>* trees[64] = { nullptr };

		while (true)
		{
			if (trees[n->degree] != nullptr)
			{
				HeapNode<T>* t = trees[n->degree];
				if (t == n) break;
				trees[n->degree] = nullptr;
				t->prev->next = t->next;
				t->next->prev = t->prev;
				if (n->value < t->value)
				{
					_addChild(n, t);
				}
				else
				{
					if (n->next == n)
					{
						t->next = t->prev = t;
					}
					else
					{
						n->prev->next = t;
						n->next->prev = t;
						t->next = n->next;
						t->prev = n->prev;
					}
					_addChild(t, n);
					n = t;
				}
				continue;
			}
			else
			{
				trees[n->degree] = n;
			}
			n = n->next;
		}
		HeapNode<T>* min = n;
		HeapNode<T>* startNode = n;
		do
		{
			if (n->value < min->value) min = n;
			n = n->next;
		} while (n != startNode);
		return min;
	}

	HeapNode<T>* _find(HeapNode<T>* heap, T value)
	{
		HeapNode<T>* n = heap;
		if (n == nullptr) return nullptr;
		do
		{
			if (n->value == value) return n;
			HeapNode<T>* result = _find(n->child, value);
			if (result) return result;
			n = n->next;
		} while (n != heap);
		return nullptr;
	}
};