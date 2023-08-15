#include <vector>
#include <iostream>
#include <numeric>

// #define DEBUG_MODE

using namespace std;

struct InstantSum
{
public:
    InstantSum() {}
    InstantSum(const vector<long long> &inputArray)
    {
        size = inputArray.size();
        this->data.resize(size);
        partial_sum(inputArray.begin(), inputArray.end(), data.begin());
    }

    inline long long sum(int l, int r)
    {
        if (l < 0 || l >= data.size())
            throw "Invalid Argument";
        return data[min(r, size - 1)] - (l > 0 ? data[l - 1] : 0);
    }

    inline long long sumRound()
    {
        return data.back();
    }

private:
    vector<long long> data;
    int size;
};

typedef char Digit;

class Modulus
{
public:
    Modulus(int modulo)
    {
        this->modulo = modulo;
    }
    typedef int Number;

    inline int plus(Number a, Number b)
    {
        Number result = a + b;
        if (result >= modulo)
            result -= modulo;
        return result;
    }

    inline int minus(Number a, Number b)
    {
        Number result = a - b;
        if (result < 0)
            result += modulo;
        return result;
    }

    inline pair<int, Number> ref(Digit a)
    {
        static auto data = buildDigitsRef();
        return data[a];
    }

    auto buildFrequencyTable(int n)
    {
        vector<InstantSum> data(n);

        vector<long long> a(1, 1);
        for (int i = 0; i < n; i++)
        {
            data[i] = InstantSum(buildA(a));
        }
        return data;
    }

    long long countMid(int i, Digit l, Digit r)
    {
        static auto data = buildFrequencyTable(10);
        auto r_l = ref(l);
        auto r_r = ref(r);

        if (i == -1)
        {
            return (r_r.first - r_l.first) + (r_l.second == 0);
        }

        return data[i].sumRound() * (r_r.first - r_l.first) + data[i].sum(minus(0, r_r.second), minus(0, r_l.second));
    }

    long long countBottom(const vector<Digit> &a, int i, int remain)
    {
        return (i > 0 ? countBottom(a, i - 1, minus(remain, ref(a[i]).second)) : 0) +
               (a[i] != 9 ? countMid(i - 1, a[i] + 1, 9) : 0);
    }

    long long countTop(const vector<Digit> &a, int i, int remain)
    {
        return (i > 0 ? countTop(a, i - 1, minus(remain, ref(a[i]).second)) : 0) +
               (a[i] != 0 ? countMid(i - 1, 0, a[i] - 1) : 0);
    }

    long long countBoth(const vector<Digit> &a, const vector<Digit> &b, int i, int remain)
    {
        if (a[i] == b[i])
            return countBoth(a, b, i - 1, minus(remain, ref(a[i]).second));

        return countBottom(a, i - 1, minus(remain, ref(a[i]).second)) +
               countMid(i - 1, a[i] + 1, b[i] - 1) +
               countTop(b, i - 1, minus(remain, ref(b[i]).second));
    }

private:
    vector<pair<int, Number>> buildDigitsRef()
    {
        vector<pair<int, Number>> data(10);
        for (int i = 0; i < 10; i++)
        {
            data[i] = {i / modulo, i % modulo};
        }

        return data;
    }

    vector<long long> buildA(const vector<long long> &a)
    {
        vector<long long> result(min(int(a.size()) + 9, modulo), 0);
        for (Digit i = 0; i < 10; i++)
        {
            for (Number j = 0; j < a.size(); j++)
            {
                result[plus(ref(i).second, j)] += a[j];
            }
        }

        return result;
    }
    int modulo;
};

auto splitNumber(int n)
{
    vector<Digit> data(10);
    int i = 0;
    while (n != 0)
    {
        data[i] = n % 10;
        n /= 10;
        i++;
    }
    return data;
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    freopen("SOMM.INP", "r", stdin);
    freopen("SOMM.OUT", "w", stdout);
#ifdef DEBUG_MODE
    freopen("err.log", "w", stderr);
#endif
    int p, q, k;
    cin >> p >> q >> k;
    auto l = splitNumber(p);
    auto r = splitNumber(q);

    cout << Modulus(k).countBoth(l, r, 9, 0);

    return 0;
}