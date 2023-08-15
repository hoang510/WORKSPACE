#include<iostream>

using namespace std;
int sumDigits(int n)
{
    int result = 0;
    while (n != 0)
    {
        result += n % 10;
        n /= 10;
    }

    return result;
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    freopen("SOMM.INP", "r", stdin);
    freopen("SOMM.OUT", "w", stdout);

    int p, q;
    cin >> p >> q;
    int result = 0;
    for (int i = p; i <= q; i++)
    {
        if (sumDigits(i) % 3 == 0)
            result++;
    }

    cerr << result;

    return 0;
}