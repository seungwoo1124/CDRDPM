#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <unordered_map>
#include <algorithm>
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

vector<string> solution(vector<int> minterm) {
    vector<pair<string, vector<int>>> mintermpair = Init(minterm);

    vector<pair<string, vector<int>>> prev;

    do
    {
        prev = mintermpair;
        mintermpair = findpi(mintermpair);
    } while (prev != mintermpair);

    vector<string> pi;
    vector<string> epi;

    unordered_map<int, int> m;
    for (int i = 2; i < minterm[1] + 2; i++)
    {
        m[minterm[i]] = 0;
    }

    for (int i = 0; i < mintermpair.size(); i++)
    {
        vector<int> combined = mintermpair[i].second;
        for (int mint : combined)
        {
            m[mint]++;
        }
        pi.push_back(mintermpair[i].first);
    }
    sort(pi.begin(), pi.end());

    for (int i = 0; i < mintermpair.size(); i++)
    {
        vector<int> combined = mintermpair[i].second;
        bool isEpi = false;
        for (int mint : combined)
        {
            if (m[mint] == 1)
            {
                isEpi = true;
                break;
            }
        }
        if (!isEpi) continue;
        epi.push_back(mintermpair[i].first);
    }
    sort(epi.begin(), epi.end());

    vector<string> answer = convert(pi);

    answer.push_back("EPI");

    if (epi.size() > 0)
    {
        vector<string> tmp = convert(epi);
        answer.insert(answer.end(), tmp.begin(), tmp.end());
    }
    return answer;
}

int main()
{
    vector<int> minterm = { 4, 8, 0, 4, 8, 10, 11, 12, 13, 15 };
    vector<string> answer = solution(minterm);
    for (string s : answer)
    {
        cout << s << endl;
    }
}