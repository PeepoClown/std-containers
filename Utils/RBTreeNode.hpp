#ifndef RBTREENODE_HPP
#define RBTREENODE_HPP

#include <cstddef>

#define RED true // YELLOW
#define BLACK false // BLUE

namespace ft
{

	template <typename T>
	class TreeNode
	{
	public :
		template <typename K, typename Compare, bool isMulti>
		friend class Tree;

		template <typename K>
		friend class TreeIterator;
		template <typename K>
		friend class TreeReverseIterator;
		template <typename K>
		friend class TreeConstIterator;
		template <typename K>
		friend class TreeConstReverseIterator;

		typedef T			value_type;
		typedef TreeNode*	node_pointer;

	private :
		value_type		_data;
		node_pointer	_parent;
		node_pointer	_left;
		node_pointer	_right;
		bool 			_color;

	public :
		explicit TreeNode(const value_type& val = value_type(), node_pointer parent = NULL,
		                  node_pointer left = NULL, node_pointer right = NULL, bool color = RED)
			: _data(val), _parent(parent), _left(left), _right(right), _color(color)
		{ }

		TreeNode(const TreeNode& node)
				: _data(node._data), _parent(node._parent), _left(node._left),
				  _right(node._right), _color(node._color)
		{ }

		TreeNode& operator= (const TreeNode& node)
		{
			this->_data = node._data;
			this->_parent = node._parent;
			this->_left = node._left;
			this->_right = node._right;
			this->_color = node._color;
			return (*this);
		}

		~TreeNode()
		{ }
	};

}

#endif