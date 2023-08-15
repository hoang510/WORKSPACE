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
        this->data.resize(inputArray.size());
        partial_sum(inputArray.begin(), inputArray.end(), data.begin());
    }

    long long sum(int l, int r)
    {
        if (r >= this->size())
            r = this->size() - 1;
        if (l > r)
            return 0;
#ifdef DEBUG_MODE
        if (!(l >= 0))
            throw("Invalid parameters");
#endif
        return this->data[r] - (l != 0 ? this->data[l - 1] : 0);
    }

    long long at(int i)
    {
        return this->sum(i, i);
    }
    inline int size()
    {
        return data.size();
    }

#ifdef DEBUG_MODE
    void print_out()
    {
        cerr << "DATA: ";
        for (auto i : data)
        {
            cerr << i << ' ';
        }
        cerr << '\n';
    }
#endif

    vector<long long> data;
};

auto buildFrequencyTable(int n)
{
    vector<InstantSum> data(n);

    vector<long long> a, b;
    a = vector<long long>(10, 1);
    data[0] = InstantSum(a);

    for (int i = 1; i < n; i++)
    {
        b = move(a);
        a.assign(b.size() + 9, 0);
        for (int i = 0; i < 10; i++)
        {
            for (int j = 0; j < b.size(); j++)
            {
                a[i + j] += b[j];
            }
        }
        data[i] = InstantSum(a);
    }
    return data;
}

auto splitNumber(int n)
{
    vector<int> data(10);
    int i = 0;
    while (n != 0)
    {
        data[i] = n % 10;
        n /= 10;
        i++;
    }
    return data;
}

#ifdef DEBUG_MODE
template <typename T>
void printArr(T *beg, T *en)
{
    for (auto i = beg; i != en; i++)
    {
        cerr << *i << ' ';
    }
}

string toNumber(const vector<int> &a, int i)
{
    string result = "";
    for (int j = i; j >= 0; j--)
    {
        result += to_string(a[j]);
    }

    return result;
}
#endif

long long count(const vector<int> &a, const vector<int> &b, int i, int k)
{
    static const vector<int> temp9(10, 9);
    static const vector<int> temp0(10, 0);
    static auto table = buildFrequencyTable(10);
#ifdef DEBUG_MODE
    static int isFirstTime = true;
    if (isFirstTime)
    {
        for (auto i : table)
        {
            i.print_out();
        }
        isFirstTime = false;
    }
#endif

    long long result;
    if (k < 0)
        return 0;
    else if (i == 0)
        return ((a[i] <= k && k <= b[i]) ? 1 : 0);
    else if (a[i] == b[i])
        result = count(a, b, i - 1, k - a[i]);
    else
    {
        long long left = count(a, temp9, i - 1, k - a[i]);
        long long middle =
            table[i - 1].sum(max(k - (b[i] - 1), 0), k - (a[i] + 1));
        long long right = count(temp0, b, i - 1, k - b[i]);
#ifdef DEBUG_MODE
        cerr << '\n'
             << "Left: " << left << "\nMiddle: " << middle << "\nRight: " << right;
#endif
        result = left + middle + right;
    }

#ifdef DEBUG_MODE
    cerr << '\n'
         << "Data (i = " << i << ", k = " << k << ", [" << toNumber(a, i) << ' ' << toNumber(b, i) << "]):  " << result << "    END!\n";
#endif
    return result;
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

#ifdef DEBUG_MODE
    printArr(l.data(), l.data() + 10);
    cerr << '\n';
    printArr(r.data(), r.data() + 10);
    cerr << '\n';
#endif
    long long result = 0;
    for (int n_k = k; n_k <= 81; n_k += k)
    {
        result += count(l, r, 9, n_k);
    }

    cout << result;

    return 0;
}