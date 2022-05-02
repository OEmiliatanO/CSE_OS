#include <iostream>
#include <set>
#include <utility>
#include <cassert>
using namespace std;

typedef pair<int, int> pii;

int require[] = {1,2,3,4, 2,1,5,6, 2,1,2,3, 7,6,3,2, 1,2,3,6};

int main()
{
	for (int frameN = 1; frameN <= 7; ++frameN)
	{
		set<int> S;
		int t = 0, pgfault = 0;

		while (S.size() < frameN && t < 20)
		{
			if (S.find(require[t]) == S.end())
			{
				++pgfault;
				S.emplace(require[t]);
			}
			++t;
		}
		/*
		puts("set:");
		for (int x : S) printf("%d ", x);
		puts("");
		*/

		for (; t < 20; ++t)
		{
			if (S.find(require[t]) == S.end())
			{
				if (S.size() < frameN)
				{
					++pgfault;
				}
				else
				{
					++pgfault;
					int table[10]{};
					pii ans{-1, -1};
					for (auto x : S)
						for (int i = t + 1; i < 20; ++i)
							if (require[i] == x)
							{
								table[x] = i;
								break;
							}

					for (auto x : S)
					{
						ans = max(ans, pii{table[x] ? table[x] : 100, x});
					}

					S.erase(ans.second);
					assert(ans.first != -1 and ans.second != -1);
				}
				S.emplace(require[t]);
			}
			/*
			puts("set:");
			for (int x : S) printf("%d ", x);
			puts("");
			*/
		}
		printf("%d\n", pgfault);
	}
	return 0;
}
