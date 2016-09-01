#include <cstdio>
#include <vector>

using namespace std;

#define LeftChild		0
#define RightChild	1

struct SplayTree
{
	vector < vector <int> > tree;
	vector <int> size;
	vector <int> parent;
	vector <int> Max;
	vector <int> dat;
	int cur;

	SplayTree(int n)
	{
		tree.resize(n + 1, vector <int>(2, NULL));
		size.resize(n + 1, NULL);
		parent.resize(n + 1, NULL);
		Max.resize(n + 1, NULL);
		dat.resize(n + 1, NULL);
		cur = NULL;
	}

	bool isLeftChild(int parentNode, int childNode)
	{
		return tree[parentNode][LeftChild] == childNode;
	}

	void rightRotate(int parentNode, int childNode)
	{
		int grandParNode = parent[parentNode];

		if (grandParNode != NULL)
		{
			if (isLeftChild(grandParNode, parentNode))tree[grandParNode][LeftChild] = childNode;
			else tree[grandParNode][RightChild] = childNode;
		}

		parent[childNode] = grandParNode;
		parent[parentNode] = childNode;
		tree[parentNode][LeftChild] = tree[childNode][RightChild];
		parent[tree[childNode][RightChild]] = parentNode;
		tree[childNode][RightChild] = parentNode;

		Max[childNode] = Max[parentNode];
	}

	void leftRotate(int parentNode, int childNode)
	{
		int grandParNode = parent[parentNode];

		if (grandParNode != NULL)
		{
			if (isLeftChild(grandParNode, parentNode))tree[grandParNode][LeftChild] = childNode;
			else tree[grandParNode][RightChild] = childNode;
		}

		parent[childNode] = grandParNode;
		parent[parentNode] = childNode;
		tree[parentNode][RightChild] = tree[childNode][LeftChild];
		parent[tree[childNode][LeftChild]] = parentNode;
		tree[childNode][LeftChild] = parentNode;

		Max[parentNode] = Max[tree[parentNode][RightChild]];
	}

	int splaying(int node)
	{
		while (parent[node] != NULL)
		{
			int parNode = parent[node];

			if (parent[parNode] == NULL)
			{
				if (isLeftChild(parNode, node))rightRotate(parNode, node);
				else leftRotate(parNode, node);
			}
			else
			{
				int grandParNode = parent[parNode];

				bool parNodeIsLeftChild = isLeftChild(grandParNode, parNode);
				bool nodeIsLeftChild = isLeftChild(parNode, node);

				if (parNodeIsLeftChild^nodeIsLeftChild)
				{
					if (parNodeIsLeftChild)
					{
						leftRotate(parNode, node);
						rightRotate(grandParNode, node);
					}
					else
					{
						rightRotate(parNode, node);
						leftRotate(grandParNode, node);
					}
				}
				else
				{
					if (parNodeIsLeftChild)
					{
						rightRotate(grandParNode, parNode);
						rightRotate(parNode, node);
					}
					else
					{
						leftRotate(grandParNode, parNode);
						leftRotate(parNode, node);
					}
				}
				
				size[grandParNode] = size[tree[grandParNode][LeftChild]] + size[tree[grandParNode][RightChild]] + 1;
			}

			size[parNode] = size[tree[parNode][LeftChild]] + size[tree[parNode][RightChild]] + 1;
			size[node] = size[tree[node][LeftChild]] + size[tree[node][RightChild]] + 1;
		}

		return node;
	}

	int find(int node, int key)
	{
		if (dat[node] == key)return splaying(node);
		else if (dat[node] < key)
		{
			if (tree[node][RightChild] == NULL)return NULL;
			else return find(tree[node][RightChild], key);
		}
		else
		{
			if (tree[node][LeftChild] == NULL)return NULL;
			else return find(tree[node][LeftChild], key);
		}
	}

	int insertion(int node, int key)
	{
		if (node == NULL)
		{
			++cur;
			dat[cur] = key;
			size[cur] = 1;
			Max[cur] = key;

			return cur;
		}

		bool smaller = dat[node] > key;

		if (tree[node][smaller ^ 1] == NULL)
		{
			tree[node][smaller ^ 1] = ++cur;
			dat[cur] = key;
			Max[cur] = key;
			parent[cur] = node;

			return splaying(cur);
		}
		else
		{
			return insertion(tree[node][smaller ^ 1], key);
		}
	}

	int findMax(int node)
	{
		if (tree[node][RightChild] == NULL)return splaying(node);
		else return findMax(tree[node][RightChild]);
	}

	int findMin(int node)
	{
		if (tree[node][LeftChild] == NULL)return splaying(node);
		else return findMin(tree[node][LeftChild]);
	}

	int join(int root1, int root2)
	{
		if (root1 == NULL && root2 == NULL)return NULL;
		if (root1 == NULL)
		{
			parent[root2] = NULL;
			return root2;
		}
		else if (root2 == NULL)
		{
			parent[root1] = NULL;
			return root1;
		}

		if (size[root1] < size[root2])
		{
			parent[root2] = NULL;
			root2 = findMin(root2);
			tree[root2][LeftChild] = root1;
			parent[root1] = root2;
			size[root2] += size[root1];

			return root2;
		}
		else
		{
			parent[root1] = NULL;
			root1 = findMax(root1);
			tree[root1][RightChild] = root2;
			parent[root2] = root1;
			size[root1] += size[root2];
			Max[root1] = Max[root2];

			return root1;
		}
	}

	int deletion(int node)
	{
		splaying(node);
		return join(tree[node][LeftChild], tree[node][RightChild]);
	}
};