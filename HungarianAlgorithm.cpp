/*
    maximum-weighted matching algorithm : Hungarian method 입니다.
    시간복잡도 : O(n^3)
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
        // slackY는 S에 포함된 모든 x, 그리고 모든 y에 대해서 min{labelX[x] + labelY[y] - weight[x][y]} 를 만족한다.
        slackY.resize(n + 1, INF);

        // 라벨 초기화
        for(int x = 1; x <= n; x++)
            for(int y = 1; y <= n; y++)
                labelX[x] = max(labelX[x], dat.weight[x][y]);

        // 초기 매칭 M을 구한다.
        matchX.resize(n + 1, -1);
        matchY.resize(n + 1, -1);
        M = 0;
        for(int x = 1; x <= n; x++){
            visited = vector <bool> (n + 1, false);
            if(augment(x))M++;
        }
    }

    // augmenting path를 찾는다.
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
        // 퍼펙트 매칭이면 단계들을 종료한다.
        if(M == n)return true;

        // S,T 집합을 초기화 한다. S 집합에는 free vertex를 집어 넣고 그것이 Alternating tree의 루트가 된다.
        S = vector <bool> (n + 1, false);
        for(int x = 1; x <= n; x++)
            if(matchX[x] == -1){
                S[x] = true;
                root = x;
                break;
            }

        T = vector <bool> (n + 1, false);

        // slackY 값을 초기화 한다.
        for(int y = 1; y <= n; y++)
            slackY[y] = labelX[root] + labelY[y] - dat.weight[root][y];

        return false;
    }

    // S집합의 이웃 노드들을 모두 찾는다.
    void generateNeighbor(){
        neighbor = vector <bool> (n + 1, false);
        for(int y = 1; y <= n; y++)
            if(slackY[y] == 0)
                neighbor[y] = true;
    }

    void step3(){
        int alpha = INF;

        // T에 속하지 않은 점들 중 가장 작은 alpha값을 구한다.
        for(int y = 1; y <= n; y++)
            if(!T[y])alpha = min(alpha, slackY[y]);

        // S에 속한 점에 대해서는 alpha 를 빼준다.
        for(int x = 1; x <= n; x++)
            if(S[x])labelX[x] -= alpha;

        // T에 속하는 점에 대해서는 alpha를 더해주고 slackY에는 빼준다.
        for(int y = 1; y <= n; y++)
            if(T[y])labelY[y] += alpha;
            else slackY[y] -= alpha;
    }

    bool step4(){
        // candY는 T에 속하지 않으면서 S집합의 이웃한 점이다.
        int candY;
        for(int y = 1; y <= n; y++)
            if(neighbor[y] && !T[y]){
                candY = y;
                break;
            }

        // candY가 free하다면 증가경로를 찾아주고 단계4를 마치고 단계2로 간다.
        if(matchY[candY] == -1){
            visited = vector <bool> (n + 1, false);
            if(augment(root))M++;
            return true;
        }

        // candY가 free하지 않다면 T집합에 포함시키고 매칭되어 있는 x값은 S집합에 포함시킨다. 그리고 slackY값을 업데이트 해준다.
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
