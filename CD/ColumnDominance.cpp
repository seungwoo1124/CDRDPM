#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <unordered_map>
#include <map>
#include <algorithm>
#include <format>
using namespace std;

vector<pair<string, vector<int>>> findpi(vector<pair<string, vector<int>>> mintermpair)
{
    if (mintermpair.size() == 1)
        return mintermpair;
    vector<pair<string, vector<int>>> ret;
    vector<pair<string, vector<int>>> removeAfterret = {};
    set<int> combine;
    for (int i = 0; i < mintermpair.size() - 1; i++)
    {
        for (int j = i + 1; j < mintermpair.size(); j++)
        {
            int count = 0;
            string tmp = "";
            string lterm = mintermpair[i].first;
            string rterm = mintermpair[j].first;
            vector<int> ltermnum = mintermpair[i].second;
            vector<int> rtermnum = mintermpair[j].second;

            for (int k = 0; k < lterm.size(); k++)
            {
                char l = lterm[k]; char r = rterm[k];
                if (l != r)
                {
                    count++;
                    tmp += '2';
                }
                else
                    tmp += l;
            }
            if (count != 1) continue;
            ltermnum.insert(ltermnum.end(), rtermnum.begin(), rtermnum.end());
            ret.push_back(make_pair(tmp, ltermnum));
            combine.insert(i);
            combine.insert(j);
        }
    }

    for (int i = 0; i < ret.size(); i++)
    {
        string s = ret[i].first;
        bool isSame = false;

        for (int j = 0; j < removeAfterret.size(); j++)
        {
            string removes = removeAfterret[j].first;
            if (s == removes)
            {
                isSame = true;
                break;
            }
        }

        if (isSame) continue;
        removeAfterret.push_back(ret[i]);
    }

    for (int i = 0; i < mintermpair.size(); i++)
    {
        bool isCombine = false;
        for (auto it = combine.begin(); it != combine.end(); ++it)
        {
            int idx = *it;
            if (i == idx)
            {
                isCombine = true;
                break;
            }
        }
        if (isCombine) continue;
        removeAfterret.push_back(mintermpair[i]);
    }

    return removeAfterret;
}

vector<pair<string, vector<int>>> Init(vector<int> minterm)
{
    vector<pair<string, vector<int>>> mintermpair;

    for (int i = 2; i < minterm[1] + 2; i++)
    {
        string bit = "";
        vector<int> mintermnum;
        int number = minterm[i];
        for (int j = 0; j < minterm[0]; j++)
        {
            bit = to_string(number % 2) + bit;
            number /= 2;
        }
        mintermnum.push_back(minterm[i]);
        pair<string, vector<int>> p = make_pair(bit, mintermnum);
        mintermpair.push_back(p);
    }

    return mintermpair;
}

vector<string> convert(vector<string> pi)
{
    vector<string> ret;
    for (int i = 0; i < pi.size(); i++)
    {
        string bin = pi[i];
        for (int j = 0; j < bin.size(); j++)
        {
            if (bin[j] == '2')
                bin[j] = '-';
        }
        ret.push_back(bin);
    }

    return ret;
}

void printPlane(vector<int> minterm, vector<pair<string, vector<int>>> pi)
{
    cout << format("{: >8} ", "minterms");
    for (int i = 2; i < minterm.size(); i++)
    {
        cout << format("{: >8} ", minterm[i]);
    }
    cout << endl;

    for (int i = 0; i < pi.size(); i++)
    {
        cout << format("{: >8} ", pi[i].first);
        for (int j = 2; j < minterm.size(); j++)
        {
            auto it = find(pi[i].second.begin(), pi[i].second.end(), minterm[j]);
            if (it != pi[i].second.end())
                cout << format("{: >8} ", 'v');
            else
                cout << format("{: >8} ", ' ');
        }
        cout << endl;
    }
}

vector<pair<int, int>> CD(vector<int> minterm, vector<pair<string, vector<int>>> pi)
{
    vector<pair<int, int>> ret = {};
    map<int, vector<pair<string, vector<int>>>> m = {};
    for (int i = 2; i < minterm.size(); i++)
    {
        m[minterm[i]] = {};
    }

    // minterm번호에 cover 하는 모든 pi넣기
    for (pair<string, vector<int>> tmp : pi)
    {
        vector<int> v = tmp.second;
        for (int i : v)
        {
            m[i].push_back(tmp);
        }
    }

    // map자료구조의 m변수가 핵심입니다.
    // key는 cover되는 minterm의 넘버이고
    // value는 key에 해당하는 minterm을 cover하는 pi의 배열입니다.
    // pi배열의 요소는 string(0200과같은 pi숫자)과 해당하는 pi가 커버하고있는 minterm들의 배열의 쌍입니다.

    for (int i = 2; i < minterm.size(); i++)
    {
        int dominate = minterm[i]; // 지배하는 minterm번호
        vector<string> dominateminterms = {}; // 지배하는 minterm을 커버하는 pi들
        for (pair<string, vector<int>> pi : m[dominate])
        {
            dominateminterms.push_back(pi.first);
        }

        for (int j = 2; j < minterm.size(); j++)
        {
            int dominated = minterm[j]; // 지배받는 minterm번호
            if (i == j) continue;
            vector<string> dominatedminterms = {}; // 지배받는 minterm을 커버하는 pi들
            for (pair<string, vector<int>> pi : m[dominated])
            {
                dominatedminterms.push_back(pi.first);
            }

            if (dominateminterms.size() < dominatedminterms.size())
                continue;

            bool isdominate = true;
            for (string cmp : dominatedminterms)
            {
                auto it = find(dominateminterms.begin(), dominateminterms.end(), cmp);
                if (it == dominateminterms.end())
                {
                    isdominate = false;
                    break;
                }
            }

            if (isdominate)
                ret.push_back(make_pair(dominate, dominated));
        }
    }
    return ret;
}

void solution(vector<int> minterm) {
    vector<pair<string, vector<int>>> pipiar = Init(minterm);
    vector<pair<string, vector<int>>> prev;
    do
    {
        prev = pipiar;
        pipiar = findpi(pipiar);
    } while (prev != pipiar);
    sort(pipiar.begin(), pipiar.end());
    printPlane(minterm, pipiar);

    unordered_map<int, int> m;
    for (int i = 2; i < minterm[1] + 2; i++)
    {
        m[minterm[i]] = 0;
    }
    for (int i = 0; i < pipiar.size(); i++)
    {
        vector<int> combined = pipiar[i].second;
        for (int mint : combined)
        {
            m[mint]++;
        }
    }

    vector<pair<string, vector<int>>> epi = {};
    vector<pair<string, vector<int>>> nepi = {};
    for (pair<string, vector<int>> pi : pipiar)
    {
        for (int mint : pi.second)
        {
            if (m[mint] == 1)
                epi.push_back(pi);
            else
                nepi.push_back(pi);
        }
    }

    cout << endl << "Column dominance" << endl;
    vector<pair<int, int>> CDList = CD(minterm, pipiar);
    for (pair<int, int> cd : CDList)
    {
        cout << cd.first << " dominate " << cd.second << endl;
    }
}

int main()
{
    vector<int> minterm = { 4, 10, 0, 1, 2, 3, 5, 7, 8, 10, 14, 15 };
    solution(minterm);
}