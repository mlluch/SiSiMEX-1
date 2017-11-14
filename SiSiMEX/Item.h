#pragma once
#include <Net.h>

/**
 * A single item and its properties.
 */
class Item
{
public:
	Item();
	Item(int id);
	~Item();

	int id() const { return _id; }

	// Serialization methods
	void Write(OutputMemoryStream &stream);
	void Read(InputMemoryStream &stream);

	// Comparison operator
	bool operator<(const Item &item) { return _id < item._id; }

private:
	int _id;
};

/**
 * A list of items.
 */
class ItemList
{
public:

	ItemList();
	~ItemList();

	void randomInitialization();

	void addItem(const Item &item);
	void removeItem(int itemId);

	const std::list<Item> &items() const { return _items; }

	ItemList getSpareItems() const;

	// Serialization methods
	void Write(OutputMemoryStream &stream);
	void Read(InputMemoryStream &stream);

private:

	std::list<Item> _items;
};
