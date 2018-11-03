/*
    maximum-weighted matching algorithm : Hungarian method �Դϴ�.
    �ð����⵵ : O(n^3)
*/

#include <cstdio>
#include <vector>

using namespace std;

struct data{

    int n;
    vector < vector <int> > weight;

    data(int _n){
        n = _n;
        weight.resize(n + 1);
        for(int i = 1; i <= n; i++){
            weight[i].resize(n + 1);
        }
    }
};

struct HungarianAlgorithm{

    vector <int> labelX, labelY;
    vector <int> slackY;
    vector <int> matchX, matchY;
    vector <bool> neighbor, T, S, visited;

    bool NequalT;
    int n;
    int M;
    int ans;
    int root;
    const int INF = 0x7fffffff;
    const int NINF = 0x80000000;
    data & dat;

    HungarianAlgorithm(data & _dat):dat(_dat){
        n = dat.n;

        step1();
        bool toStep2 = true;
        while(1){
            if(toStep2 && step2())break;

            generateNeighbor();
            NequalT = true;
            for(int y = 1; y <= n; y++)
                if(neighbor[y] != T[y]){
                    NequalT = false;
                    break;
                }

            if(NequalT)step3();
            else toStep2 = step4();
        }

        ans = 0;
        for(int i = 1; i <= n; i++)
            ans += labelX[i] + labelY[i];
    }

    void step1(){
        labelX.resize(n + 1, 0);
        labelY.resize(n + 1, 0);
        // slackY�� S�� ���Ե� ��� x, �׸��� ��� y�� ���ؼ� min{labelX[x] + labelY[y] - weight[x][y]} �� �����Ѵ�.
        slackY.resize(n + 1, INF);

        // �� �ʱ�ȭ
        for(int x = 1; x <= n; x++)
            for(int y = 1; y <= n; y++)
                labelX[x] = max(labelX[x], dat.weight[x][y]);

        // �ʱ� ��Ī M�� ���Ѵ�.
        matchX.resize(n + 1, -1);
        matchY.resize(n + 1, -1);
        M = 0;
        for(int x = 1; x <= n; x++){
            visited = vector <bool> (n + 1, false);
            if(augment(x))M++;
        }
    }

    // augmenting path�� ã�´�.
    bool augment(int x){
        if(visited[x])return false;
        visited[x] = true;

        for(int y = 1; y <= n; y++)
            if(dat.weight[x][y] == labelX[x] + labelY[y])
                if(matchY[y] == -1 || augment(matchY[y])){
                    matchX[x] = y;
                    matchY[y] = x;
                    return true;
                }

        return false;
    }

    bool step2(){
        // ����Ʈ ��Ī�̸� �ܰ���� �����Ѵ�.
        if(M == n)return true;

        // S,T ������ �ʱ�ȭ �Ѵ�. S ���տ��� free vertex�� ���� �ְ� �װ��� Alternating tree�� ��Ʈ�� �ȴ�.
        S = vector <bool> (n + 1, false);
        for(int x = 1; x <= n; x++)
            if(matchX[x] == -1){
                S[x] = true;
                root = x;
                break;
            }

        T = vector <bool> (n + 1, false);

        // slackY ���� �ʱ�ȭ �Ѵ�.
        for(int y = 1; y <= n; y++)
            slackY[y] = labelX[root] + labelY[y] - dat.weight[root][y];

        return false;
    }

    // S������ �̿� ������ ��� ã�´�.
    void generateNeighbor(){
        neighbor = vector <bool> (n + 1, false);
        for(int y = 1; y <= n; y++)
            if(slackY[y] == 0)
                neighbor[y] = true;
    }

    void step3(){
        int alpha = INF;

        // T�� ������ ���� ���� �� ���� ���� alpha���� ���Ѵ�.
        for(int y = 1; y <= n; y++)
            if(!T[y])alpha = min(alpha, slackY[y]);

        // S�� ���� ���� ���ؼ��� alpha �� ���ش�.
        for(int x = 1; x <= n; x++)
            if(S[x])labelX[x] -= alpha;

        // T�� ���ϴ� ���� ���ؼ��� alpha�� �����ְ� slackY���� ���ش�.
        for(int y = 1; y <= n; y++)
            if(T[y])labelY[y] += alpha;
            else slackY[y] -= alpha;
    }

    bool step4(){
        // candY�� T�� ������ �����鼭 S������ �̿��� ���̴�.
        int candY;
        for(int y = 1; y <= n; y++)
            if(neighbor[y] && !T[y]){
                candY = y;
                break;
            }

        // candY�� free�ϴٸ� ������θ� ã���ְ� �ܰ�4�� ��ġ�� �ܰ�2�� ����.
        if(matchY[candY] == -1){
            visited = vector <bool> (n + 1, false);
            if(augment(root))M++;
            return true;
        }

        // candY�� free���� �ʴٸ� T���տ� ���Խ�Ű�� ��Ī�Ǿ� �ִ� x���� S���տ� ���Խ�Ų��. �׸��� slackY���� ������Ʈ ���ش�.
        S[matchY[candY]] = true;
        T[candY] = true;

        for(int y = 1; y <= n; y++)
            slackY[y] = min(slackY[y], labelX[matchY[candY]] + labelY[y] - dat.weight[matchY[candY]][y]);

        return false;
    }
};


int main(){
    freopen("input.txt","r",stdin);

    int n;
    scanf("%d",&n);

    data dat(n);

    int maxN = 0;
    for(int i = 0; i < n; i++)
        for(int j = 0; j < n; j++){
                scanf("%d", &dat.weight[i + 1][j + 1]);
                maxN = max(maxN, dat.weight[i + 1][j + 1]);
            }

    for(int i = 0; i < n; i++)
        for(int j = 0; j < n; j++)
            dat.weight[i + 1][j + 1] = maxN - dat.weight[i + 1][j + 1];

    HungarianAlgorithm ha(dat);

    printf("%d\n", -(ha.ans - n*maxN));

    return 0;
}
