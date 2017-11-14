#pragma once
#include "Item.h"
#include <memory>

class Node
{
public:
	Node();
	~Node();

	void initialize();

	ItemList &itemList() { return _itemList; }
	const ItemList &itemList() const { return _itemList; }

private:

	ItemList _itemList; /**< All items in this node. */
};

using NodePtr = std::shared_ptr<Node>;
