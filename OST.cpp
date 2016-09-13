#include "OST.h"
#include <iostream>

const OST_color_type OST_node::RED = false;
const OST_color_type OST_node::BLACK = true;
OST_node* OST_node::NIL = new OST_node(OST_node::BLACK);

int main()
{
	Order_statistic_tree tree;
	tree.insert(19);
	tree.insert(7);
	tree.insert(16);
	tree.insert(41);
	tree.insert(21);
	tree.insert(17);
	tree.insert(35);
	tree.insert(3);
	tree.insert(30);
	tree.insert(26);
	tree.insert(20);
	tree.insert(28);
	tree.insert(12);
	tree.insert(14);
	tree.insert(39);
	tree.insert(21);
	tree.insert(38);
	tree.insert(10);
	tree.insert(14);
	tree.insert(47);

	tree.print_tree();
	tree.print_list();

	int i;
	std::cin >> i;
	return 0;
}