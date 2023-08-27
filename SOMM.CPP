#include <vector>
#include <iostream>
#include <numeric>

#define DEBUG_MODE

using namespace std;

struct InstantSum
{
public:
    InstantSum() {}
    InstantSum(const vector<long long> &inputArray) : size(inputArray.size())
    {
        this->data.resize(size);
        partial_sum(inputArray.begin(), inputArray.end(), data.begin());
    }

    inline long long sum(int l, int r) const
    {
        const auto &data = this->data;
        return data[min(r, size - 1)] - (l > 0 ? data[l - 1] : 0);
    }

    inline long long at(int i) const
    {
        return this->sum(i, i);
    }

    inline long long sumRound() const
    {
        return data.back();
    }
#ifdef DEBUG_MODE
    void print(string Label)
    {
        cerr << Label << ": ";
        for (auto i : data)
        {
            cerr << i << ' ';
        }
        cerr << '\n';
    }
#endif
private:
    vector<long long> data;
    int size;
};

typedef char Digit;

class Modulus
{
public:
    typedef int Number;

    Modulus(int modulo) : modulo(modulo) {}

    inline int plus(Modulus::Number a, Modulus::Number b) const
    {
        Modulus::Number result = a + b;
        if (result >= modulo)
            result -= modulo;
        return result;
    }

    inline int minus(Modulus::Number a, Modulus::Number b) const
    {
        Modulus::Number result = a - b;
        if (result < 0)
            result += modulo;
        return result;
    }

protected:
    const int modulo;
};

#ifdef DEBUG_MODE
string toNumber(vector<Digit> a, int i)
{
    string result = "";
    for (int j = 0; j <= i; j++)
    {
        result = to_string(a[j]) + result;
    }

    return result;
}

#endif

class Solve : private Modulus
{
public:
    Solve(int modulo) : Modulus(modulo), ref(buildDigitsRef()), frequencyTable(buildFrequencyTable(10)) {}

    long long countMid(int layer, Digit l, Digit r, Number remain) const
    {
        if (!(l >= 0 && l <= r && r <= 9))
            return 0;

        if (layer == 0)
        {
            long long result = 0;
            for (int i = l; i <= r; i++)
            {
                result += plus(remain, ref[i].second) == 0;
            }
            return result;
        }

        long long result = 0;
        for (int i = l; i <= r; i++)
        {
            result += frequencyTable[layer - 1].at(minus(0, plus(remain, ref[i].second)));
        }

        return result;
    }

    long long countBottom(const vector<Digit> &a, int i, int remain) const
    {
        long long bottom = (i > 0 ? countBottom(a, i - 1, plus(remain, ref[a[i]].second)) : 0);
        long long mid = (countMid(i, a[i] + (i > 0), 9, remain));

#ifdef DEBUG_MODE
        cerr << '\n'
             << " - Left side: " << toNumber(a, i) << '\n';
        cerr << "Remain: " << remain << '\n';
        cerr << "Bottom: " << bottom << '\n';
        cerr << "Mid: " << mid << "\n";
#endif
        return bottom + mid;
    }

    long long countTop(const vector<Digit> &a, int i, int remain) const
    {

        long long top = (i > 0 ? countTop(a, i - 1, plus(remain, ref[a[i]].second)) : 0);
        long long mid = (countMid(i, 0, a[i] - (i > 0), remain));

#ifdef DEBUG_MODE
        cerr << '\n'
             << " - Right side: " << toNumber(a, i) << '\n';
        cerr << "Remain: " << remain << '\n';
        cerr << "Top: " << top << '\n';
        cerr << "Mid: " << mid << "\n";
#endif
        return top + mid;
    }

    long long countBoth(const vector<Digit> &a, const vector<Digit> &b, int i, int remain) const
    {
        if (a[i] == b[i])
            return countBoth(a, b, i - 1, plus(remain, ref[a[i]].second));
        long long mid = (countMid(i, a[i] + 1, b[i] - 1, remain));
        long long bottom(i > 0 ? countBottom(a, i - 1, plus(remain, ref[a[i]].second)) : 0);
        long long top = (i > 0 ? countTop(b, i - 1, plus(remain, ref[b[i]].second))
                               : 0);

#ifdef DEBUG_MODE
        cerr << '\n'
             << " - Both side: " << toNumber(a, i) << ' ' << toNumber(b, i) << '\n';
        cerr << "Remain: " << remain << '\n';
        cerr << "Top: " << top << '\n';
        cerr << "Mid: " << mid << "\n";
        cerr << "Bottom: " << bottom << '\n';
#endif
        return top + mid + bottom;
    }

private:
    vector<InstantSum> buildFrequencyTable(int n) const
    {
        vector<InstantSum> data(n);

        vector<long long> a(1, 1);
        for (int i = 0; i < n; i++)
        {
            a = move(buildA(a));
            data[i] = InstantSum(a);
#ifdef DEBUG_MODE
            data[i].print("Data " + to_string(i));
#endif
        }
        return data;
    }

    vector<pair<int, Number>> buildDigitsRef() const
    {
        vector<pair<int, Number>> data(10);
        for (int i = 0; i < 10; i++)
        {
            data[i] = {i / modulo, i % modulo};
        }

        return data;
    }

    vector<long long> buildA(const vector<long long> &a) const
    {
        vector<long long> result(modulo, 0);
        for (Digit i = 0; i < 10; i++)
        {
            for (Number j = 0; j < a.size(); j++)
            {
                result[plus(ref[i].second, j)] += a[j];
            }
        }

        return result;
    }

    const vector<pair<int, Number>> ref;
    const vector<InstantSum> frequencyTable;
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

    cout << Solve(k).countBoth(l, r, 9, 0);

    return 0;
}