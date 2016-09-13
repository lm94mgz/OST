#include <iostream>

typedef bool OST_color_type;
typedef int OST_value_type;

struct OST_node {
	OST_color_type color;
	static const OST_color_type RED;
	static const OST_color_type BLACK;
	static OST_node *NIL;

	OST_value_type key;
	int size;
	OST_node* left;
	OST_node* right;
	OST_node* parent;
	OST_node* next;
	OST_node* prev;

	OST_node(OST_color_type c = RED) : color(c), size(0)
	{ 
		size = 0;
		left = 0;
		right = 0;
		parent = 0;
		next = 0;
		prev = 0;
	}

	void print()
	{
		if (this == NIL)
		{
			return;
		}
		std::cout << "key : " << key << "; size : " << size << std::endl;

		left->print();
		right->print();
	}

	OST_node* search(OST_value_type k)
	{
		OST_node* ret = this;
		while (ret != 0 && k != ret->key)
		{
			if (k < ret->key)
			{
				ret = ret->left;
			}
			else
			{
				ret = ret->right;
			}
		}
		return ret;
	}
};

class Order_statistic_tree {
private:
	OST_node *root;
	OST_node list_head;

public:
	Order_statistic_tree()
	{
		root = OST_node::NIL;
		list_head.prev = OST_node::NIL;
		list_head.next = OST_node::NIL;
	}

	OST_node* minimum()
	{
		return list_head.next;
	}

	OST_node* maximum()
	{
		return list_head.prev;
	}

	void insert(OST_value_type key)
	{
		OST_node *node = new OST_node();
		node->key = key;
		insert_node(node);
	}

	void delete_by_key(OST_value_type key)
	{
		OST_node *node = root->search(key);
		node = delete_node(node);
		delete node;
	}

	int rank(OST_value_type key)
	{
		OST_node *node = root->search(key);
		return rank_node(node);
	}

	void print_tree()
	{
		root->print();
	}

	void print_list()
	{
		OST_node *node = list_head.next;
		while (node != &list_head)
		{
			std::cout << "key : " << node->key << "; size : " << node->size << std::endl;
			node = node->next;
		}
	}
private:
	// 向树内合适的位置插入一个结点
	void insert_node(OST_node *node)
	{
		OST_node *y = OST_node::NIL;
		OST_node *x = root;
		while (x != OST_node::NIL)
		{
			y = x;
			x->size += 1;
			if (node->key < x->key)
			{
				x = x->left;
			}
			else
			{
				x = x->right;
			}
		}

		node->parent = y;
		if (y == OST_node::NIL)
		{
			root = node;

			node->prev = node->next = &list_head;
			list_head.prev = list_head.next = node;
		}
		else if (node->key < y->key)
		{
			y->left = node;

			node->prev = y->prev;
			node->next = y;
			y->prev->next = node;
			y->prev = node;
		}
		else
		{
			y->right = node;

			node->next = y->next;
			node->prev = y;
			y->next->prev = node;
			y->next = node;
		}

		node->size = 1;
		node->left = OST_node::NIL;
		node->right = OST_node::NIL;
		node->color = OST_node::RED;
		insert_node_fixup(node);
	}

	// 删除树内的一个结点
	OST_node* delete_node(OST_node *node)
	{
		OST_node *y;
		if (node->left == OST_node::NIL || node->right == OST_node::NIL)
		{
			y = node;
		}
		else
		{
			y = node->next;
		}

		OST_node *x;
		if (y->left == OST_node::NIL)
		{
			x = y->left;
		}
		else
		{
			x = y->right;
		}
		
		x->parent = y->parent;
		if (y->parent == OST_node::NIL)
		{
			root = x;
		}
		else if (y == y->parent->left)
		{
			y->parent->left = x;
		}
		else
		{
			y->parent->right = x;
		}
		if (y != node)
		{
			node->key = y->key;
		}

		x->prev = y->prev;
		y->prev->next = x;
		
		OST_node *p = node;
		while (p != OST_node::NIL)
		{
			p->size -= 1;
			p = p->parent;
		}

		if (y->color == OST_node::BLACK)
		{
			delete_node_fixup(x);
		}
		return y;
	}

	// 向树内插入结点后，调整树以保持红黑树的性质
	void insert_node_fixup(OST_node *node)
	{
		OST_node *uncle;
		while (node->parent->color == OST_node::RED)
		{
			if (node->parent == node->parent->parent->left)
			{
				uncle = node->parent->parent->right;
				if (uncle->color == OST_node::RED)
				{
					node->parent->color = OST_node::BLACK;
					uncle->color = OST_node::BLACK;
					node->parent->parent->color = OST_node::RED;
					node = node->parent->parent;
				}
				else
				{
					if (node == node->parent->right)
					{
						node = node->parent;
						left_rotate(node);
					}

					node->parent->color = OST_node::BLACK;
					node->parent->parent->color = OST_node::RED;
					right_rotate(node->parent->parent);
				}
			}
			else
			{
				uncle = node->parent->parent->left;
				if (uncle->color == OST_node::RED)
				{
					node->parent->color = OST_node::BLACK;
					uncle->color = OST_node::BLACK;
					node->parent->parent->color = OST_node::RED;
					node = node->parent->parent;
				}
				else
				{
					if (node == node->parent->left)
					{
						node = node->parent;
						right_rotate(node);
					}

					node->parent->color = OST_node::BLACK;
					node->parent->parent->color = OST_node::RED;
					left_rotate(node->parent->parent);
				}
			}
		}
		root->color = OST_node::BLACK;
	}

	// 向树内删除结点后，调整树以保持红黑树的性质
	void delete_node_fixup(OST_node *node)
	{
		OST_node *uncle;
		while (node != root && node->color == OST_node::BLACK)
		{
			if (node == node->parent->left)
			{
				uncle = node->parent->right;
				if (uncle->color == OST_node::RED)
				{
					uncle->color = OST_node::BLACK;
					node->parent->color = OST_node::RED;
					left_rotate(node->parent);
					uncle = node->parent->right;
				}
				if (uncle->left->color == OST_node::BLACK && uncle->right->color == OST_node::BLACK)
				{
					uncle->color = OST_node::RED;
					node = node->parent;
				}
				else
				{
					if (uncle->right->color == OST_node::BLACK)
					{
						uncle->left->color = OST_node::BLACK;
						uncle->color = OST_node::RED;
						right_rotate(uncle);
						uncle = node->parent->right;
					}
					uncle->color = node->parent->color;
					node->parent->color = OST_node::BLACK;
					uncle->right->color = OST_node::BLACK;
					left_rotate(node->parent);
					node = root;
				}
			}
			else
			{
				uncle = node->parent->left;
				if (uncle->color == OST_node::RED)
				{
					uncle->color = OST_node::BLACK;
					node->parent->color = OST_node::RED;
					right_rotate(node->parent);
					uncle = node->parent->left;
				}
				if (uncle->left->color == OST_node::BLACK && uncle->right->color == OST_node::BLACK)
				{
					uncle->color = OST_node::RED;
					node = node->parent;
				}
				else
				{
					if (uncle->left->color == OST_node::BLACK)
					{
						uncle->right->color = OST_node::BLACK;
						uncle->color = OST_node::RED;
						left_rotate(uncle);
						uncle = node->parent->left;
					}
					uncle->color = node->parent->color;
					node->parent->color = OST_node::BLACK;
					uncle->left->color = OST_node::BLACK;
					right_rotate(node->parent);
					node = root;
				}
			}
		}
		node->color = OST_node::BLACK;
	}

	// 对树内的一个结点执行左旋操作
	void left_rotate(OST_node *x)
	{
		OST_node *y = x->right;
		x->right = y->left;
		if (y->left != OST_node::NIL)
		{
			y->left->parent = x;
		}

		y->parent = x->parent;
		if (x->parent == OST_node::NIL)
		{
			root = y;
		}
		else if (x == x->parent->left)
		{
			x->parent->left = y;
		}
		else
		{
			x->parent->right = y;
		}
		y->left = x;
		x->parent = y;
		y->size = x->size;
		x->size = x->left->size + x->right->size + 1;
	}

	// 对树内的一个结点执行右旋操作
	void right_rotate(OST_node *x)
	{
		OST_node *y = x->left;
		x->left = y->right;
		if (y->right != OST_node::NIL)
		{
			y->right->parent = x;
		}

		y->parent = x->parent;
		if (x->parent == OST_node::NIL)
		{
			root = y;
		}
		else if (x == x->parent->left)
		{
			x->parent->left = y;
		}
		else
		{
			x->parent->right = y;
		}
		y->right = x;
		x->parent = y;
		y->size = x->size;
		x->size = x->left->size + x->right->size + 1;
	}

	// 确定树中一个结点的秩
	int rank_node(OST_node *node)
	{
		int r = node->left->size + 1;
		OST_node *y = node;
		while (y != root)
		{
			if (y == y->parent->right)
			{
				r += y->parent->left->size + 1;
			}
			y = y->parent;
		}
		return r;
	}
};