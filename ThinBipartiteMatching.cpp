#include <vector>

using namespace std;

// A -> B로 가는 정점집합 A와 정점집합 B의 이분매칭에서, A의 원소가 B의 원소 최대 2개와
// 인접한 경우만을 다루는 이분매칭 솔루션이다. union-find 로 해결한다.
struct ThinBipartiteMaching
{
	vector <int> f,v;
	vector < vector <int> > adj;
	int Un, Vn;

	ThinBipartiteMaching(int _Un, int _Vn):Un(_Un), Vn(_Vn)
	{
		f.resize(Vn + 1);
		v.resize(Vn + 1);
		adj.resize(Un + 1);
	}

	int find(int u)
	{
		if (f[u] == u)return u;
		return f[u] = find(f[u]);
	}

	void unite(int a, int b)
	{
		f[find(a)] = find(b);
	}

	int findAugmentPath(int a, int b)
	{
		if (!v[a])
		{
			v[a] = 1;
			unite(a, b);
		}
		else if (!v[b])
		{
			v[b] = 1;
			unite(b, a);
		}
		else if (!v[find(a)])
		{
			v[find(a)] = 1;
			unite(a, b);
		}
		else if (!v[find(b)])
		{
			v[find(b)] = 1;
			unite(b, a);
		}
		else return 0;

		return 1;
	}

	int matching()
	{
		int ret = 0;
		for (int i = 1; i <= Vn; i++)f[i] = i, v[i] = 0;

		for (int i = 1; i <= Un; i++)
		{
			if (adj[i].size() == 0)continue;

			if (adj[i].size() == 1)ret += findAugmentPath(adj[i][0], adj[i][0]);
			else if(adj[i].size() == 2)ret += findAugmentPath(adj[i][0], adj[i][1]);
		}

		return ret;
	}
};