#include <vector>
#include <stack>

using namespace std;

struct tarjan
{
	int vSize;
	int vCnt;
	int sccCnt;
	vector <int> discovered, sccId;
	vector <vector <int> > sccSet;
	vector <vector <int> > adj;
	vector <vector <int> > sccAdj;
	stack <int> st;

	tarjan(int n)
	{
		vSize = n;
		discovered.resize(n + 1, 0);
		sccId.resize(n + 1, 0);
		adj.resize(n + 1);
		sccCnt = 0;
		vCnt = 0;
	}

	int dfs(int u)
	{
		int ret = discovered[u] = ++vCnt;
		st.push(u);

		for (int i = 0; i < adj[u].size(); i++)
		{
			int v = adj[u][i];
			if (discovered[v] == 0)ret = min(ret, dfs(v));
			else if (sccId[v] == 0)ret = min(ret, discovered[v]);
		}

		if (ret == discovered[u])
		{
			sccCnt++;
			while (1)
			{
				int t = st.top();
				st.pop();
				sccId[t] = sccCnt;
				if (t == u)break;
			}
		}

		return ret;
	}

	void scc()
	{
		for (int i = 1; i <= vSize; i++)
			if (discovered[i] == 0)dfs(i);
	}

	void fillSccSet()
	{
		sccSet.resize(sccCnt + 1);
		for (int i = 1; i <= vSize; i++)sccSet[sccId[i]].push_back(i);
	}

	void fillSccAdj()
	{
		sccAdj.resize(sccCnt + 1);

		for (int i = 1; i <= vSize; i++)
		{
			for (int j = 0; j < adj[i].size(); j++)
			{
				int v = adj[i][j];

				if (sccId[i] == sccId[v])continue;
				sccAdj[sccId[i]].push_back(sccId[v]);
			}
		}

		for (int i = 1; i <= sccCnt; i++)
		{
			sort(sccAdj[i].begin(), sccAdj[i].end());
			sccAdj[i].erase(unique(sccAdj[i].begin(), sccAdj[i].end()), sccAdj[i].end());
		}
	}
};

