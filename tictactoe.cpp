#include<bits/stdc++.h>
using namespace std;
#define ar array
#define sz(x) (int)x.size()
#define board vector<vector<char> >
mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
int ran(int l, int r)
{
    return l + rng() % (r - l + 1);
}
int pw3[] = {1, 3, 9, 27, 81, 243, 729, 2187, 6561};
int n;
vector<ar<int, 3> > track;
board flip(const board &a)
{
    board b(n + 1, vector<char>(n + 1));
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= n; j++) 
            b[i][j] = a[i][n - j + 1];
    return b;
}       
void spin(board &a) //counter clockwise
{
    board b(n + 1, vector<char>(n + 1));
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= n; j++)
        {
            b[i][j] = a[j][n - i + 1];
        }
    a = b;
}
int Hash(const board &a)
{
    int mask = 0;
    for (int i = 1; i <= n; i++)
    for (int j = 1; j <= n; j++)
    if (a[i][j] == 'X') mask += pw3[(i - 1) * 3 + j - 1];
    else if (a[i][j] == 'O') mask += 2 * pw3[(i - 1) * 3 + j - 1];
    return mask;
}
int findbox(board &a, int &cntspin, board &ans)
{
    board b = flip(a);
    int state = 19683;
    for (int i = 1; i <= 4; i++)
    {
        spin(b);
        spin(a);
        int h1 = Hash(a), h2 = Hash(b);
        if (state > h1) 
        {
            state = h1;
            cntspin = i;
            ans = a;
        }
        if (state > h2)
        {
            state = h2;
            cntspin = -i;
            ans = b;
        }
    }
    return state;
}
int check(const board &a)
{
    for (int i = 1; i <= n; i++)
    {
        int sum = 0, sum2 = 0;
        for (int j = 1; j <= n; j++)
        {
            if (a[i][j] == 'X') sum++;
            else if (a[i][j] == 'O') sum--;
            if (a[j][i] == 'X') sum2++;
            else if (a[j][i] == 'O') sum2--;
        }
        if (sum == n || sum2 == n) return 1;
        if (sum == -n || sum2 == -n) return -1;
    }
    int sum = 0, sum2 = 0;
    for (int i = 1; i <= n; i++) 
    {
        if (a[i][i] == 'X') sum++;
        else if (a[i][i] == 'O') sum--;
        if (a[i][n - i + 1] == 'X') sum2++;
        else if (a[i][n - i + 1] == 'O') sum2--;
    }
    if (sum == n || sum2 == n) return 1;
    if (sum == -n || sum2 == -n) return -1;
    return 0;
}   
void in(const board &a)
{
    for (int i = 1; i <= 100; i++) cout << '-';
    cout << '\n' << "  ";
    for (int i = 1; i <= n; i++) cout << i << ' ';
    cout << '\n';
    for (int i = 1; i <= n; i++)
    {
        cout << i << ' ';
        for (int j = 1; j <= n; j++) cout << a[i][j] << ' ';
        cout << '\n';
    }
    cout << '\n';
}
bool endgame(int &luot, const board &a)
{
    int ok = check(a);
    if (ok == 1)
    {
        in(a);
        cout << "X WINS" << '\n';
        return 1;
    }
    if (ok == -1)
    {
        in(a);
        cout << "O WINS" << '\n';
        return 1;
    }
    luot++;
    if (luot > n * n)
    {
        in(a);
        cout <<"DRAW GAME" << '\n';
        return 1;
    }
    return 0;
}
void pvp()
{
    while(1)
    {
        cout << "\nTO PLAY NEW GAME ENTER THE SIZE OF THE BOARD\n";
        while(cin >> n)
        {
            if (n >= 10) cout << "I don't like big boards" << '\n';
            else break;
        }
        int luot = 1;
        board a(n + 1, vector<char>(n + 1, '-'));
        while (1)
        {

            in(a);
            if (luot % 2) cout << "X'S TURN" << '\n';
            else cout << "O'S TURN" << '\n';

            int x, y;
            while(1)
            {
                cin >> x >> y;
                if (x > 0 && y > 0 && x <= n && y <= n && a[x][y] == '-') break;
                else cout << "INPUT NOT VALID" << '\n';
            }

            if (luot % 2) a[x][y] = 'X';
            else a[x][y] = 'O';

            if (endgame(luot, a)) break;
        }
    }
}
struct Ecanem
{
    vector<ar<int, 2> > moves[19683];
    void play(board &a, int &x, int &y)
    {
        int cntspin;
        board ans;
        int state = findbox(a, cntspin, ans);
        if (moves[state].empty())
        {
            // cout << "EMPTY : " << state << '\n';
            for (int i = 1; i <= n; i++)
                for (int j = 1; j <= n; j++)
                    if (ans[i][j] == '-') moves[state].push_back({i, j});   
        }
        int i = ran(0, sz(moves[state]) - 1);
        x = moves[state][i][0];
        y = moves[state][i][1];
        track.push_back({state, x, y});
        moves[state][i] = moves[state].back();
        moves[state].pop_back();
        int ok = 0;
        if (cntspin < 0)
        {
            ok = 1;
            cntspin *= -1;
        }
        for (int j = 1; j <= cntspin; j++)
        {
            int nx = y;
            int ny = n - x + 1;
            x = nx;
            y = ny;
        }
        if (ok) y = n - y + 1;
        if (a[x][y] != '-') 
        {
            in(ans);
            cout << x << ' ' << y << " is different from ";
            cout << cntspin << ' ' << track.back()[1] << ' ' << track.back()[2] << '\n';
        }
    }
};
void dummy(board &a, int &x, int &y)
{
    vector<ar<int, 2> > tmp;
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= n; j++)
            if (a[i][j] == '-') tmp.push_back({i, j});
    int i = ran(0, sz(tmp) - 1);
    x = tmp[i][0];
    y = tmp[i][1];  
}
void pvc()
{
    n = 3;
    ifstream fin;
    fin.open("howtoplay.txt");
    Ecanem GG;
    int state, cnt = 0, game = 0, win, draw, lose;
    fin >> game >> win >> draw >> lose;
    while(fin >> state)
    {
        cnt++;
        int numberofmoves;
        fin >> numberofmoves;
        GG.moves[state].resize(numberofmoves);
        for (ar<int, 2> &i : GG.moves[state]) 
        {
            fin >> i[0] >> i[1];
        }
        // cout << state << ' ' << sz(GG.moves[state]) << '\n';
    }
    fin.close();
    cout << "NUMBER OF STORED STATES: " << cnt << '\n';
    while(1)
    {   
        game++;
        cout << "\nGAME " << game << '\n';
        // if (game > 100) break;
        int luot = 1;
        board a(4, vector<char>(4, '-'));
        track.clear();
        while (1)
        {
            if (luot != 1) in(a);
            int x, y;
            if (luot % 2 == 0) 
            {
                // dummy(a, x, y);
                cout << "HUMAN'S TURN" << '\n';
                while(1)
                {
                    cin >> x >> y;
                    if (x > 0 && y > 0 && x <= n && y <= n && a[x][y] == '-') break;
                    else cout << "INPUT NOT VALID" << '\n';
                }
            }
            else 
            {
                GG.play(a, x, y);
                cout << "BOT PLAYS " << x << ' ' << y << '\n';
            }
            if (luot % 2 == 0) a[x][y] = 'O';
            else a[x][y] = 'X';

            if (endgame(luot, a)) break;
        }
        int ok = check(a);
        if (ok == 0) // if draw game put the beads back and add 1 more
        {
            draw++;
            for (ar<int, 3> i : track) 
            for (int j = 0; j < 2; j++) GG.moves[i[0]].push_back({i[1], i[2]});
        }
        else if (ok == 1) // if com wins put the beads back and add 3 more
        {
            win++;
            for (ar<int, 3> i : track) 
            for (int j = 0; j < 4; j++) GG.moves[i[0]].push_back({i[1], i[2]});
        }
        else lose++;
        cout << "\nPRESS 1 to continue playing or 0 to exit: ";
        int playnextgame;
        cin >> playnextgame;
        if (playnextgame == 0) break;
    }
    cout << "WIN: " << win << " DRAW: " << draw << " LOSE: " << lose << '\n';
    ofstream fout;
    fout.open("howtoplay.txt");
    fout << game << ' ' << win << ' ' << draw << ' ' << lose << '\n';
    for (int i = 0; i < 19683; i++)
        if (!GG.moves[i].empty())
        {
            fout << i << ' ' << sz(GG.moves[i]) << '\n';
            for (ar<int, 2> j : GG.moves[i]) fout << j[0] << ' ' << j[1] << '\n';
        }
    fout.close();
}
signed main()
{
    int solokoem;
    cout << "PRESS 1 TO PVP OR PRESS 2 TO PVC: ";
    cin >> solokoem;
    if (solokoem == 1) pvp();
    else pvc();
    return 0;
}