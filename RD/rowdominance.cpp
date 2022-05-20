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
    std::cout << format("{: >8} ", "minterms");
    for (int i = 0; i < minterm.size(); i++)
    {
        std::cout << format("{: >8} ", minterm[i]);
    }
    std::cout << endl;

    for (int i = 0; i < pi.size(); i++)
    {
        std::cout << format("{: >8} ", pi[i].first);
        for (int j = 0; j < minterm.size(); j++)
        {
            auto it = find(pi[i].second.begin(), pi[i].second.end(), minterm[j]);
            if (it != pi[i].second.end())
                std::cout << format("{: >8} ", 'v');
            else
                std::cout << format("{: >8} ", ' ');
        }
        std::cout << endl;
    }
}

vector<pair<string, string>> RD(vector<int> minterm, vector<pair<string, vector<int>>> pi)
{
    vector<pair<string, string>> ret = {};
    for (int i = 0; i < pi.size(); i++)
    {
        pair<string, vector<int>> dominate = pi[i];
        for (int j = 0; j < pi.size(); j++)
        {
            if (i == j) continue;
            pair<string, vector<int>> dominated = pi[j];

            bool isdominate = true;
            for (int mint : minterm)
            {
                auto dominateIt = find(dominate.second.begin(), dominate.second.end(), mint);
                auto dominatedIt = find(dominated.second.begin(), dominated.second.end(), mint);

                if (dominateIt == dominate.second.end() &&
                    dominatedIt != dominated.second.end()) // dominate에 없는게 dominated에 있으면
                    isdominate = false;
            }
            if (isdominate)
            {
                ret.push_back(make_pair(dominate.first, dominated.first));
            }
        }
    }

    return ret;
}

void solution(vector<int> minterm) {
    vector<pair<string, vector<int>>> pipair = Init(minterm);
    vector<pair<string, vector<int>>> prev;
    do
    {
        prev = pipair;
        pipair = findpi(pipair);
    } while (prev != pipair);
    sort(pipair.begin(), pipair.end());

    unordered_map<int, int> m;
    for (int i = 2; i < minterm[1] + 2; i++)
    {
        m[minterm[i]] = 0;
    }
    for (int i = 0; i < pipair.size(); i++)
    {
        vector<int> combined = pipair[i].second;
        for (int mint : combined)
        {
            m[mint]++;
        }
    }
    // 여기까지 초기화 부분

    vector<pair<string, vector<int>>> epi = {};
    set<int> epiminterm = {};
    vector<pair<string, vector<int>>> nepi = {};
    vector<int> nepiminterm = {};
    for (pair<string, vector<int>> pi : pipair)
    {
        bool isepi = false;
        for (int mint : pi.second)
        {
            if (m[mint] == 1)
            {
                isepi = true;
            }
        }
        if (isepi)
        {
            epi.push_back(pi);
        }
        else
        {
            nepi.push_back(pi);
        }
    }
    for (pair<string, vector<int>> epipair : epi)
    {
        vector<int> epivec = epipair.second;
        epiminterm.merge(set(epivec.begin(), epivec.end()));
    }
    for (int i = 2; i < minterm.size(); i++)
    {
        if (epiminterm.find(minterm[i]) == epiminterm.end())
            nepiminterm.push_back(minterm[i]);
    }
    printPlane(nepiminterm, nepi);

    std::cout << endl << "Row dominance" << endl;
    vector<pair<string, string>> RDList = RD(nepiminterm, nepi);
    for (pair<string, string> rd : RDList)
    {
        std::cout << rd.first << " dominate " << rd.second << endl;
    }
}

int main()
{
    vector<int> minterm = { 4, 10, 0, 1, 2, 3, 5, 7, 8, 10, 14, 15 };
    solution(minterm);
}