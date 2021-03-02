#ifndef LIST_HPP
#define LIST_HPP

#include <memory>
#include <cstddef>
#include <limits>
#include "Utils/util.hpp"
#include "List/ListNode.hpp"
#include "List/ListIterator.hpp"

namespace ft
{

	template <typename T, typename Alloc = std::allocator<T> >
	class list
	{
	public :
		typedef T								value_type;
		typedef ListNode<value_type>			node;
		typedef Alloc							allocator_type;
		typedef value_type&						reference;
		typedef const value_type&				const_reference;
		typedef value_type*						pointer;
		typedef const value_type*				const_pointer;
		typedef ListIterator<T>					iterator;
		typedef ListReverseIterator<T>			reverse_iterator;
		typedef ListConstIterator<T>			const_iterator;
		typedef ListConstReverseIterator<T>		const_reverse_iterator;
		typedef ptrdiff_t						difference_type;
		typedef size_t							size_type;

	private :
		node*			_head;
		node*			_tail;
		allocator_type	_alloc;
		size_type		_size;

	private :
		template <typename Compare>
		void MergeSort(node** headRef, Compare comp)
		{
			node* head = *headRef;
			node* a = end()._ptr;
			node* b = end()._ptr;

			if (head == end()._ptr || head->_next == end()._ptr)
				return ;
			
			FrontBackSplit(head, &a, &b);
			MergeSort(&a, comp);
			MergeSort(&b, comp);
			*headRef = SortedMerge(a, b, comp);
		}

		template <typename Compare>
		node* SortedMerge(node* a, node* b, Compare comp)
		{
			node* result;

			if (a == end()._ptr)
				return (b);
			if (b == end()._ptr)
				return (a);

			if (comp(a->_data, b->_data)) {
				result = a;
				result->_next = SortedMerge(a->_next, b, comp);
			}
			else {
				result = b;
				result->_next = SortedMerge(a, b->_next, comp);
			}
			return (result);
		}

		void FrontBackSplit(node* source, node** frontRef, node** backRef)
		{
			node* fast = source;
			node* slow = source;
			while (fast != end()._ptr) {
				fast = fast->_next;
				if (fast != end()._ptr && fast->_next != end()._ptr) {
					slow = slow->_next;
					fast = fast->_next;
				}
			}
			*frontRef = source;
			*backRef = slow->_next;
			slow->_next = end()._ptr;
		}

	public :
		/*
		** default constructor
		** constructs an empty list
		*/
		explicit list(const allocator_type& alloc = allocator_type())
			: _alloc(alloc), _size(0)
		{
			this->_head = new node();
			this->_tail = new node();
			this->_head->_next = this->_tail;
			this->_tail->_prev = this->_head;
		}

		/*
		** fill constructor
		** fill list by n elements, equals to val
		*/
		explicit list(size_type n, const value_type& val = value_type(),
				      const allocator_type& alloc = allocator_type())
			: _alloc(alloc), _size(0)
		{
			this->_head = new node();
			this->_tail = new node();
			this->_head->_next = this->_tail;
			this->_tail->_prev = this->_head;
			assign(n, val);
		}

		/*
		** range constructor
		** fill list by range of iterator [first, last)
		*/
		template <typename InputIterator>
		list(InputIterator first, InputIterator last, const allocator_type& alloc = allocator_type(),
			 typename enable_if<!std::numeric_limits<InputIterator>::is_specialized>::type * = 0)
			: _alloc(alloc), _size(0)
		{
			this->_head = new node();
			this->_tail = new node();
			this->_head->_next = this->_tail;
			this->_tail->_prev = this->_head;
			assign(first, last);
		}

		/*
		** copy constructor
		** fill list by elements of list passed to param
		*/
		list(const list& x)
			: _alloc(x._alloc), _size(0)
		{
			this->_head = new node();
			this->_tail = new node();
			this->_head->_next = this->_tail;
			this->_tail->_prev = this->_head;
			assign(x.begin(), x.end());
		}

		/*
		** destructor
		** delete list and its elements
		*/
		~list()
		{
			clear();
			delete this->_head;
			delete this->_tail;
		}

		/*
		** assignation operator overload
		** same as copy constructor but returns list
		*/
		list& operator= (const list& x)
		{
			clear();
			delete this->_head;
			delete this->_tail;
			this->_head = new node();
			this->_tail = new node();
			this->_head->_next = this->_tail;
			this->_tail->_prev = this->_head;
			assign(x.begin(), x.end());
			return (*this);
		}

		/*
		** iterators access methods
		** returns an iterator to begin/end of list
		*/
		iterator begin()
		{ return (iterator(this->_head->_next)); }

		iterator end()
		{ return (iterator(this->_tail)); }

		reverse_iterator rbegin()
		{ return (reverse_iterator(this->_tail->_prev)); }

		reverse_iterator rend()
		{ return (reverse_iterator(this->_head)); }

		const_iterator begin() const
		{ return (const_iterator(this->_head->_next)); }

		const_iterator end() const
		{ return (const_iterator(this->_tail)); }

		const_reverse_iterator rbegin() const
		{ return (const_reverse_iterator(this->_tail->_prev)); }

		const_reverse_iterator rend() const
		{ return (const_reverse_iterator(this->_head)); }

		/*
		** empty
		** returns true if list is empty, otherwise false
		*/
		bool empty() const
		{ return (!this->_size); }

		/*
		** size
		** returns list size
		*/
		size_type size() const
		{ return (this->_size); }

		/*
		** max_size
		** returns max size of elements that list can store
		*/
		size_type max_size() const
		{ return (std::numeric_limits<size_type>::max() / sizeof(node)); }

		/*
		** front in 2 versions
		** returns value that stored in first list item
		*/
		reference front()
		{ return (this->_head->_next->_data); }

		const_reference front() const
		{ return (this->_head->_next->_data); }

		/*
		** back in 2 versions
		** returns value that stored in last list item
		*/
		reference back()
		{ return (this->_tail->_prev->_data); }

		const_reference back() const
		{ return (this->_tail->_prev->_data); }

		template <class InputIterator>
		void assign(InputIterator first, InputIterator last,
					typename enable_if<!std::numeric_limits<InputIterator>::is_specialized>::type * = 0)
		{
			clear();
			insert(begin(), first, last);
		}
		void assign(size_type n, const value_type& val)
		{
			clear();
			insert(begin(), n, val);
		}

		void push_front(const value_type& val)
		{ insert(begin(), val); }

		void pop_front()
		{ erase(begin()); }

		void push_back(const value_type& val)
		{ insert(end(), val); }

		void pop_back()
		{ erase(--end()); }

		iterator insert(iterator position, const value_type& val)
		{
			node* item = new node(val);
			item->_next = position._ptr;
			item->_prev = position._ptr->_prev;
			position._ptr->_prev->_next = item;
			position._ptr->_prev = item;
			this->_size++;
			return (iterator(item));
		}

		void insert(iterator position, size_type n, const value_type& val)
		{
			iterator curr = position;
			for (size_type i = 0; i < n; i++)
				curr = ++insert(curr, val);
		}

		template <typename InputIterator>
		void insert(iterator position, InputIterator first, InputIterator last,
					typename enable_if<!std::numeric_limits<InputIterator>::is_specialized>::type * = 0)
		{
			iterator curr = position;
			for (iterator it = first; it != last; it++)
				curr = ++insert(curr, *it);
		}

		iterator erase(iterator position)
		{
			if (this->_size <= 0)
				return (iterator());

			iterator result(position._ptr->_next);
			position._ptr->_prev->_next = position._ptr->_next;
			position._ptr->_next->_prev = position._ptr->_prev;
			delete position._ptr;
			this->_size--;
			return (result);
		}

		iterator erase(iterator first, iterator last)
		{
			node* curr = first._ptr;
			while (curr != last._ptr) {
				node* tmp_next = curr->_next;
				erase(iterator(curr));
				curr = tmp_next;
			}
			return (iterator(curr));
		}

		void swap(list& x)
		{
			ft::swap(this->_head, x._head);
			ft::swap(this->_tail, x._tail);
			ft::swap(this->_size, x._size);
		}

		void resize(size_type n, value_type val = value_type())
		{
			if (n <= this->_size) {
				size_type size_diff = this->_size - n;
				for (size_type i = 0; i < size_diff; i++)
					pop_back();
			}
			else {
				for (size_type i = this->_size; i < n; i++)
					push_back(val);
			}
		}

		void clear()
		{
			while (!empty())
				pop_back();
		}

		void splice(iterator position, list& x, iterator i)
		{
			node* inserted_elem = i._ptr;
			node* inserted_elem_next = i._ptr->_next;
			node* inserted_elem_prev = i._ptr->_prev;
			node* position_elem = position._ptr;

			inserted_elem_prev->_next = inserted_elem_next;
			inserted_elem_next->_prev = inserted_elem_prev;
			position_elem->_prev->_next = inserted_elem;
			inserted_elem->_prev = position_elem->_prev;
			position_elem->_prev = inserted_elem;
			inserted_elem->_next = position_elem;

			this->_size++;
			x._size--;
		}

		void splice(iterator position, list& x, iterator first, iterator last)
		{
			node* first_elem = first._ptr;
			node* last_elem = last._ptr->_prev;
			node* last_elem_next = last._ptr;
			node* position_elem = position._ptr;

			first_elem->_prev->_next = last_elem_next;
			last_elem_next->_prev = first_elem->_prev;
			position_elem->_prev->_next = first_elem;
			first_elem->_prev = position_elem->_prev;
			position_elem->_prev = last_elem;
			last_elem->_next = position_elem;

			this->_size += ft::distance(first, last);
			x._size -= ft::distance(first, last);
		}

		void splice(iterator position, list& x)
		{ splice(position, x, x.begin(), x.end()); }

		void remove(const value_type& val)
		{
			node* curr = this->_head->_next;
			while (curr != this->_tail) {
				node* tmp_next = curr->_next;
				if (!ft::less(curr->_data, val) && !ft::less(val, curr->_data)) {
					node* tmp = curr;
					tmp->_prev->_next = tmp->_next;
					tmp->_next->_prev = tmp->_prev;
					delete tmp;
					this->_size--;
				}
				curr = tmp_next;
			}
		}

		template <typename Predicate>
		void remove_if(Predicate pred)
		{
			node* curr = this->_head->_next;
			while (curr != this->_tail) {
				node* tmp_next = curr->_next;
				if (pred(curr->_data)) {
					node* tmp = curr;
					tmp->_prev->_next = tmp->_next;
					tmp->_next->_prev = tmp->_prev;
					delete tmp;
					this->_size--;
				}
				curr = tmp_next;
			}
		}

		/*
		** unique in 2 version
		** remove sequence of elements equal or predictable, leaving only first of them
		*/
		void unique()
		{
			node* curr = this->_head->_next;
			node* next = curr;
			while (next != this->_tail->_prev) {
				next = next->_next;
				if (!ft::less(curr->_data, next->_data) && !ft::less(next->_data, curr->_data)) {
					node* tmp = next;
					tmp->_prev->_next = tmp->_next;
					tmp->_next->_prev = tmp->_prev;
					delete tmp;
					this->_size--;
					next = curr;
				}
				else
					curr = curr->_next;
			}
		}

		template <typename BinaryPredicate>
		void unique(BinaryPredicate binary_pred)
		{
			node* curr = this->_head->_next;
			node* next = curr;
			while (next != this->_tail->_prev) {
				next = next->_next;
				if (binary_pred(curr->_data, next->_data)) {
					node* tmp = next;
					tmp->_prev->_next = tmp->_next;
					tmp->_next->_prev = tmp->_prev;
					delete tmp;
					this->_size--;
					next = curr;
				}
				else
					curr = curr->_next;
			}
		}

		void merge(list& x)
		{
			if (&x == this)
				return ;

			node* curr = this->_head->_next;
			node* other_curr = x._head->_next;
			while (curr != this->_tail && other_curr != x._tail) {
				while (curr != this->_tail && ft::less(curr->_data, other_curr->_data))
					curr = curr->_next;
				node* other_curr_next = other_curr->_next;
				splice(iterator(curr), x, iterator(other_curr));
				other_curr = other_curr_next;
			}
			if (other_curr != x._tail)
				splice(iterator(curr), x, iterator(other_curr), x.end());
		}

		template <class Compare>
		void merge(list& x, Compare comp)
		{
			if (&x == this)
				return ;

			node* curr = this->_head->_next;
			node* other_curr = x._head->_next;
			while (curr != this->_tail && other_curr != x._tail) {
				while (curr != this->_tail && comp(curr->_data, other_curr->_data))
					curr = curr->_next;
				node* other_curr_next = other_curr->_next;
				splice(iterator(curr), x, iterator(other_curr));
				other_curr = other_curr_next;
			}
			if (other_curr != x._tail)
				splice(iterator(curr), x, iterator(other_curr), x.end());
		}

		void sort()
		{
			MergeSort(&this->_head->_next, (ft::Less<T>()));
			node* prev = this->_head;
			node* tmp = begin()._ptr;
			while (tmp != end()._ptr) {
				tmp->_prev = prev;
				prev = tmp;
				tmp = tmp->_next;
			}
			this->_tail->_prev = prev;
		}

		template <typename Compare>
		void sort(Compare comp)
		{
			MergeSort(&this->_head->_next, comp);
			node* prev = this->_head;
			node* tmp = begin()._ptr;
			while (tmp != end()._ptr) {
				tmp->_prev = prev;
				prev = tmp;
				tmp = tmp->_next;
			}
			this->_tail->_prev = prev;
		}

		/*
		** reverse
		** reverse all list elements
		*/
		void reverse()
		{
			node* curr = this->_head->_next;
			while (curr != this->_tail) {
				ft::swap(curr->_prev, curr->_next);
				curr = curr->_prev;
			}

			ft::swap(this->_head->_prev, this->_head->_next);
			ft::swap(this->_tail->_prev, this->_tail->_next);
			ft::swap(this->_head, this->_tail);
		}
	};

	/*
	** compare operators overload
	*/
	template <class T, class Alloc>
	bool operator== (const list<T, Alloc>& lhs, const list<T, Alloc>& rhs)
	{
		if (lhs.size() != rhs.size())
			return (false);
		typename list<T>::iterator it_lhs = lhs.begin(), it_rhs = rhs.begin();
		for ( ; it_lhs != lhs.end(); it_lhs++, it_rhs++) {
			if (*it_lhs != *it_rhs)
				return (false);
		}
		return (true);
	}

	template <class T, class Alloc>
	bool operator!= (const list<T, Alloc>& lhs, const list<T, Alloc>& rhs)
	{ return (!(lhs == rhs)); }

	template <class T, class Alloc>
	bool operator< (const list<T, Alloc>& lhs, const list<T, Alloc>& rhs)
	{
		size_t min_size = ft::min(lhs.size(), rhs.size());
		typename list<T>::iterator it_lhs = lhs.begin(), it_rhs = rhs.begin();
		for (size_t i = 0; i < min_size; i++, it_lhs++, it_rhs++) {
			if (*it_lhs < *it_rhs)
				return (true);
			else if (*it_lhs > *it_rhs)
				return (false);
		}
		return (lhs.size() < rhs.size());
	}

	template <class T, class Alloc>
	bool operator> (const list<T, Alloc>& lhs, const list<T, Alloc>& rhs)
	{ return (rhs < lhs); }

	template <class T, class Alloc>
	bool operator<= (const list<T, Alloc>& lhs, const list<T, Alloc>& rhs)
	{ return (!(rhs < lhs)); }

	template <class T, class Alloc>
	bool operator>= (const list<T, Alloc>& lhs, const list<T, Alloc>& rhs)
	{ return (!(lhs < rhs)); }

	/*
	** swap
	** identical to list::swap
	*/
	template <class T, class Alloc>
	void swap(list<T, Alloc>& x, list<T, Alloc>& y)
	{ x.swap(y); }

}

#endif
