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

vector<string> PM(vector<int> minterm, vector<pair<string, vector<int>>> pi)
{
    // pm으로 골라낸 nepi들의 모음배열입니다.
    vector<string> ret = {};
    
    map<int, vector<string>> m = {};
    for (int mint : minterm)
    {
        for (pair<string, vector<int>> cover : pi)
        {
            if (find(cover.second.begin(), cover.second.end(), mint) != cover.second.end())
                m[mint].push_back(cover.first);
        }
    }
    for (auto it = m.begin(); it != m.end(); ++it)
    {
        std::cout << it->first << ' ';
        for (string s : it->second)
        {
            std::cout << s << ' ';
        }
        std::cout << endl;
    }

    vector<int> cover(minterm.begin(), minterm.end());
    while (cover.size() != 0)
    {
        int maxcover = 0;
        string pushret;
        vector<int> covered;
        for (pair<string, vector<int>> pipair : pi)
        {
            int count = 0;
            string pis = pipair.first;
            for (int i = 0; i < cover.size(); i++)
            {
                vector<string> coverlist = m[cover[i]];
                if (find(coverlist.begin(), coverlist.end(), pis) != coverlist.end())
                    count++;
            }
            if (count > maxcover)
            {
                pushret = pis;
                covered = pipair.second;
                maxcover = count;
            }
        }
        ret.push_back(pushret);
        int deletecount = 0;
        for (int i = 0; i < covered.size(); i++)
        {
            int target = covered[i];
            auto removeIt = find(cover.begin(), cover.end(), target);
            if (removeIt != cover.end())
            {
                cover.erase(removeIt);
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

    //여기까지 초기화

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
    // 여기까지 nepi찾아내기
    printPlane(nepiminterm, nepi);

    std::cout << endl << "Petrick's Method" << endl;
    vector<string> PMList = PM(nepiminterm, nepi);
    for (string print : PMList)
    {
        std::cout << print << ' ';
    }
    std::cout << "epi( ";
    for (pair<string, vector<int>> tmp : epi)
    {
        string print = tmp.first;
        std::cout << print << ' ';
    }
    std::cout << ')' << endl;
}

int main()
{
    vector<int> minterm = { 4, 10, 0, 1, 2, 3, 5, 7, 8, 10, 14, 15 };
    vector<int> minterm2 = { 4, 8, 0, 4, 8, 10, 11, 12, 13, 15 };
    solution(minterm2);
}