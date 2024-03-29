#ifndef SET_HPP
#define SET_HPP

#include <memory>
#include <cstddef>
#include <limits>
#include "util.hpp"
#include "RBTree.hpp"

namespace ft
{

	template <typename T, typename Compare = std::less<T>,
			  typename Alloc = std::allocator<T> >
	class set : public Tree<T, Compare, false>
	{
	public :
		typedef T																		key_type;
		typedef T																		value_type;
		typedef Compare																	key_compare;
		typedef Compare																	value_compare;
		typedef Alloc																	allocator_type;
		typedef value_type&						 										reference;
		typedef const value_type&														const_reference;
		typedef value_type*																pointer;
		typedef const value_type*														const_pointer;
		typedef typename Tree<value_type, key_compare, false>::iterator					iterator;
		typedef typename Tree<value_type, key_compare, false>::reverse_iterator			reverse_iterator;
		typedef typename Tree<value_type, key_compare, false>::const_iterator			const_iterator;
		typedef typename Tree<value_type, key_compare, false>::const_reverse_iterator	const_reverse_iterator;
		typedef ptrdiff_t																difference_type;
		typedef size_t																	size_type;
		typedef Tree<value_type, key_compare, false>									tree;
		typedef TreeNode<value_type>													node;

	private :
		key_compare		_cmp;
		allocator_type	_alloc;

	public :
		explicit set(const key_compare& cmp = key_compare(),
					 const allocator_type& alloc = allocator_type())
			: tree(cmp), _cmp(cmp), _alloc(alloc)
		{ }

		template <typename InputIterator>
		set(InputIterator first, InputIterator last, const key_compare& cmp = key_compare(),
			const allocator_type& alloc = allocator_type())
			: tree(cmp), _cmp(cmp), _alloc(alloc)
		{ this->insert(first, last); }

		set(const set& x)
			: tree(x._cmp), _cmp(x._cmp), _alloc(x._alloc)
		{ this->insert(x.begin(), x.end()); }

		set& operator= (const set& x)
		{
			this->_cmp = x._cmp;
			tree::clear();
			this->insert(x.begin(), x.end());
			return (*this);
		}

		virtual ~set()
		{ }

		bool empty() const
		{ return (!this->size()); }

		size_type max_size() const
		{ return(std::numeric_limits<size_type>::max() / sizeof(node)); }

		std::pair<iterator, bool> insert(const value_type& val)
		{ return (tree::insert(val)); }

		iterator insert(iterator position, const value_type& val)
		{
			static_cast<void>(position);
			return (tree::insert(val).first);
		}

		template <typename InputIterator>
		void insert(InputIterator first, InputIterator last,
					typename enable_if<!std::numeric_limits<InputIterator>::is_specialized>::type * = 0)
		{
			while (first != last) {
				tree::insert(*first);
				first++;
			}
		}

		void erase(iterator position)
		{ tree::erase(position); }

		size_type erase(const value_type& val)
		{
			size_type result = tree::count(val);
			if (result == 1)
				tree::erase(tree::find(val));
			return (result);
		}

		void erase(iterator first, iterator last)
		{
			iterator curr = first, next;
			while (curr != last) {
				next = ++curr;
				--curr;
				tree::erase(curr);
				curr = next;
			}
		}

		void swap(set& x)
		{
			tree::swap(x.getTree());
			ft::swap(this->_cmp, x._cmp);
		}

		void clear()
		{ tree::clear(); }

		key_compare key_comp() const
		{ return (this->_cmp); }

		value_compare value_comp() const
		{ return (this->_cmp); }

		iterator find(const value_type& val) const
		{ return (tree::find(val)); }

		size_type count(const value_type& val) const
		{ return (tree::count(val)); }

		iterator lower_bound(const value_type& val) const
		{
			for (iterator it = tree::begin(); it != tree::end(); it++) {
				if (!this->_cmp(*it, val))
					return (it);
			}
			return (tree::end());
		}

		iterator upper_bound(const value_type& val) const
		{
			for (iterator it = tree::begin(); it != tree::end(); it++) {
				if (this->_cmp(val, *it))
					return (it);
			}
			return (tree::end());
		}

		std::pair<iterator, iterator> equal_range(const value_type& val) const
		{ return (std::pair<iterator, iterator>(lower_bound(val), upper_bound(val))); }
	};

}

#endif
