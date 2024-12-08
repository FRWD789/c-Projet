#pragma once
template <typename T>
struct list
{
	list* _next;
	T _element;
	list(T val ={}) :_next(nullptr), _element(val) {}
};


template <typename T>
class LinkedList {




	list<T>* _head;

public:
	LinkedList()  : _head(nullptr) {};
	~LinkedList();
	void push(T& val);
	void removeById(T& val);
	list<T>* getHead() const;



};

template<typename T>
inline LinkedList<T>::~LinkedList()
{
	if (_head) {
		list<T>* temp = nullptr;
		while (_head) {
			temp = _head;
			_head = _head->_next;
			delete temp;
		}
	}
}

template<typename T>
inline void LinkedList<T>::push(T& val)
{
	list<T>* toAdd = new list<T>(val);
	if (_head == nullptr) {
		_head = toAdd;
		return;
	}
	list<T>* curr = _head;
	while (curr->_next) {
		curr = curr->_next;
	}
	curr->_next = toAdd;

}

template<typename T>
inline void LinkedList<T>::removeById(T& val)
{
	if (_head) {
		list<T>* curr = _head;
		if (_head->_element == val) {
			list<T>* temp = _head;
			_head = _head->_next;
			delete temp;
			temp = nullptr;
			return;
		}
		while (curr->_next!= nullptr)
		{
			if (curr->_next->_element == val) {
				list<T>* temp = curr->_next;
				curr->_next = temp->_next;
				delete temp;
				temp = nullptr;
				break;
			}
			curr = curr->_next;
		}
	}
}

template<typename T>
inline list<T>* LinkedList<T>::getHead() const
{
	return _head;
}
